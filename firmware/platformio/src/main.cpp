/*
 * VAWT Control System - Main Loop
 * Compatible with DAQ firmware architecture
 * States: IDLE, MPPT, POWER_REGULATION, STALL, FAULT
 * 
 * Hardware: ESP32-DevKitC, INA226, Hall effect RPM sensor
 * Author: Dr. Asitha Kulasekera, University of Moratuwa
 * License: MIT
 */

#include <Arduino.h>
#include <Wire.h>
#include <SD.h>
#include <WiFi.h>
#include <time.h>
#include "TurbineStateMachine.h"
#include "MPPTController.h"
#include "SafetyMonitor.h"
#include "DataLogger.h"

// Pin definitions (adjust for your hardware)
#define RPM_SENSOR_PIN 34
#define DUMP_LOAD_PWM_PIN 25
#define BRAKE_RELAY_PIN 26
#define SD_CS_PIN 5

// Turbine configuration (500W Helical Gorlov VAWT)
const float ROTOR_RADIUS = 0.6;      // meters
const float ROTOR_HEIGHT = 1.5;      // meters
const float SWEPT_AREA = 1.8;        // m² (2*R*H for helical VAWT)
const float LAMBDA_OPT = 2.0;
const float CP_MAX = 0.35;
const float RATED_POWER = 500.0;     // Watts
const float RATED_RPM = 180.0;
const float OVERSPEED_RPM = 250.0;   // 1.4× rated

// Global objects
TurbineStateMachine stateMachine;
MPPTController mppt(LAMBDA_OPT);
SafetyMonitor safety(OVERSPEED_RPM, 60.0); // 60V overvoltage threshold
DataLogger dataLogger;

// ISR for RPM counting
volatile unsigned long lastPulseTime = 0;
volatile unsigned long pulsePeriod = 0;
void IRAM_ATTR rpmISR() {
    unsigned long now = micros();
    pulsePeriod = now - lastPulseTime;
    lastPulseTime = now;
}

void setup() {
    Serial.begin(115200);
    Serial.println("\n=== VAWT Control System Starting ===");
    
    // Initialize GPIO
    pinMode(RPM_SENSOR_PIN, INPUT_PULLUP);
    pinMode(DUMP_LOAD_PWM_PIN, OUTPUT);
    pinMode(BRAKE_RELAY_PIN, OUTPUT);
    ledcSetup(0, 20000, 8); // 20kHz PWM, 8-bit resolution
    ledcAttachPin(DUMP_LOAD_PWM_PIN, 0);
    
    // Attach RPM interrupt
    attachInterrupt(digitalPinToInterrupt(RPM_SENSOR_PIN), rpmISR, FALLING);
    
    // Initialize I2C for INA226
    Wire.begin();
    
    // Initialize SD card
    if (!SD.begin(SD_CS_PIN)) {
        Serial.println("ERROR: SD Card initialization failed!");
        stateMachine.setState(STATE_FAULT);
    }
    
    // Initialize WiFi for NTP (optional, can run standalone)
    WiFi.begin("YOUR_SSID", "YOUR_PASSWORD");
    configTime(5.5*3600, 0, "pool.ntp.org"); // UTC+5:30 Sri Lanka
    
    Serial.println("Initialization complete. Entering STANDBY state.");
    stateMachine.setState(STATE_STANDBY);
}

void loop() {
    static unsigned long lastSampleTime = 0;
    unsigned long now = millis();
    
    // 1 Hz sampling (DAQ-compatible)
    if (now - lastSampleTime >= 1000) {
        lastSampleTime = now;
        
        // Read sensors
        float windSpeed = readWindSpeed();        // From anemometer (analog or I2C)
        float rpm = calculateRPM();
        float voltage = readBusVoltage();         // INA226
        float current = readBusCurrent();         // INA226
        float power = voltage * current;
        
        // Compute derived quantities
        float lambda = calculateLambda(rpm, windSpeed);
        float cp = calculateCp(power, windSpeed);
        
        // Safety checks
        bool safe = safety.check(rpm, voltage, current);
        if (!safe) {
            stateMachine.setState(STATE_FAULT);
            engageBrake();
        }
        
        // State machine logic
        TurbineState currentState = stateMachine.getState();
        
        switch (currentState) {
            case STATE_STANDBY:
                if (windSpeed > 3.0 && safe) {
                    stateMachine.setState(STATE_MPPT);
                }
                break;
                
            case STATE_MPPT:
                {
                    float dutyCycle = mppt.update(power, windSpeed);
                    ledcWrite(0, dutyCycle * 255);
                    
                    if (power > RATED_POWER * 0.95) {
                        stateMachine.setState(STATE_POWER_REGULATION);
                    }
                    if (windSpeed > 12.0) {
                        stateMachine.setState(STATE_STALL);
                    }
                }
                break;
                
            case STATE_POWER_REGULATION:
                {
                    // Soft-stall via increased electrical loading
                    float softStallDuty = calculateSoftStall(power, rpm);
                    ledcWrite(0, softStallDuty * 255);
                    
                    if (power < RATED_POWER * 0.8) {
                        stateMachine.setState(STATE_MPPT);
                    }
                }
                break;
                
            case STATE_STALL:
                engageDumpLoad();
                if (rpm < RATED_RPM) {
                    stateMachine.setState(STATE_STANDBY);
                }
                break;
                
            case STATE_FAULT:
                engageBrake();
                ledcWrite(0, 0); // Disable converter
                break;
        }
        
        // Data logging (CSV format compatible with DAQ guide)
        dataLogger.log(now, currentState, windSpeed, rpm, 
                      voltage, current, power, lambda, cp);
                      
        // Debug output
        Serial.printf("State: %s | λ=%.2f | Cp=%.2f | P=%.1fW | RPM=%.0f\n",
                     stateMachine.getStateName(), lambda, cp, power, rpm);
    }
    
    // Background tasks
    dataLogger.flush(); // Write buffered data to SD
    
    delay(10); // Prevent WDT reset
}

// === Sensor Reading Functions ===

float readWindSpeed() {
    // Example: Analog anemometer 0-5V = 0-25 m/s
    int raw = analogRead(35);
    return (raw / 4095.0) * 25.0;
}

float calculateRPM() {
    if (pulsePeriod == 0) return 0;
    unsigned long period = pulsePeriod; // Thread-safe copy
    float freq = 1000000.0 / period; // Hz
    return freq * 60.0; // RPM
}

float readBusVoltage() {
    // INA226 I2C read (placeholder - use library)
    return 48.5; // TODO: Implement actual I2C read
}

float readBusCurrent() {
    return 8.2; // TODO: Implement actual I2C read
}

float calculateLambda(float rpm, float windSpeed) {
    if (windSpeed < 0.5) return 0;
    float omega = rpm * (2 * PI / 60.0); // rad/s
    return (omega * ROTOR_RADIUS) / windSpeed;
}

float calculateCp(float power, float windSpeed) {
    if (windSpeed < 0.5) return 0;
    float rho = 1.15; // kg/m³ (tropical sea level)
    float windPower = 0.5 * rho * SWEPT_AREA * pow(windSpeed, 3);
    return power / windPower;
}

// === Control Functions ===

void engageDumpLoad() {
    ledcWrite(0, 255); // 100% duty on dump load
    digitalWrite(BRAKE_RELAY_PIN, LOW);
}

void engageBrake() {
    digitalWrite(BRAKE_RELAY_PIN, HIGH);
    ledcWrite(0, 0);
}

float calculateSoftStall(float power, float rpm) {
    // Increase loading to maintain rated power
    float targetPower = RATED_POWER;
    float error = targetPower - power;
    static float integrator = 0;
    integrator += error * 0.001; // Simple PI controller
    float duty = 0.5 + 0.01 * error + integrator;
    return constrain(duty, 0.1, 0.9);
}