/*
 * VAWT Control System - Main Loop
 * ESP32-DevKitC compatible
 */

#include <Arduino.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <WiFi.h>
#include <time.h>
#include "TurbineStateMachine.h"
#include "MPPTController.h"
#include "SafetyMonitor.h"
#include "DataLogger.h"
#include <Adafruit_INA219.h>
Adafruit_INA219 ina219;

// Pin definitions
#define RPM_SENSOR_PIN 34
#define DUMP_LOAD_PWM_PIN 25
#define BRAKE_RELAY_PIN 26
#define SD_CS_PIN 5
#define WIND_SPEED_PIN 35 // Analog input for anemometer

// Turbine configuration (500W Helical Gorlov VAWT)
const float ROTOR_RADIUS = 0.6;
const float ROTOR_HEIGHT = 1.5;
const float SWEPT_AREA = 1.8;
const float LAMBDA_OPT = 2.0;
const float CP_MAX = 0.35;
const float RATED_POWER = 500.0;
const float RATED_RPM = 180.0;
const float OVERSPEED_RPM = 250.0;

// Global objects
TurbineStateMachine stateMachine;
MPPTController mppt(LAMBDA_OPT);
SafetyMonitor safety(OVERSPEED_RPM, 60.0);
DataLogger dataLogger;

// ISR variables for RPM
volatile unsigned long lastPulseTime = 0;
volatile unsigned long pulsePeriod = 0;

// Forward declarations
void IRAM_ATTR rpmISR();
float readWindSpeed();
float calculateRPM();
float readBusVoltage();
float readBusCurrent();
float calculateLambda(float rpm, float windSpeed);
float calculateCp(float power, float windSpeed);
void engageDumpLoad();
void engageBrake();
float calculateSoftStall(float power, float rpm);

void IRAM_ATTR rpmISR()
{
    unsigned long now = micros();
    pulsePeriod = now - lastPulseTime;
    lastPulseTime = now;
}

void setup()
{
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n=== VAWT Control System Starting ===");

    // Initialize GPIO
    pinMode(RPM_SENSOR_PIN, INPUT_PULLUP);
    pinMode(DUMP_LOAD_PWM_PIN, OUTPUT);
    pinMode(BRAKE_RELAY_PIN, OUTPUT);
    pinMode(WIND_SPEED_PIN, INPUT);

    configTime(19800, 0, "pool.ntp.org"); // UTC+5:30 = 19800 seconds

    // Setup PWM
    ledcSetup(0, 20000, 8); // 20kHz, 8-bit
    ledcAttachPin(DUMP_LOAD_PWM_PIN, 0);

    // Attach RPM interrupt
    attachInterrupt(digitalPinToInterrupt(RPM_SENSOR_PIN), rpmISR, FALLING);

    // Initialize I2C
    Wire.begin();

    if (!ina219.begin())
    {
        Serial.println("Failed to find INA219 chip");
    }

    // Initialize SD card
    if (!dataLogger.begin(SD_CS_PIN))
    {
        Serial.println("WARNING: SD Card not available, continuing without logging");
    }

    Serial.println("Initialization complete.");
    stateMachine.setState(STATE_STANDBY);
}

void loop()
{
    static unsigned long lastSampleTime = 0;
    unsigned long now = millis();

    // 1 Hz sampling
    if (now - lastSampleTime >= 1000)
    {
        lastSampleTime = now;

        // Read sensors
        float windSpeed = readWindSpeed();
        float rpm = calculateRPM();
        float voltage = readBusVoltage();
        float current = readBusCurrent();
        float power = voltage * current;

        // Compute derived quantities
        float lambda = calculateLambda(rpm, windSpeed);
        float cp = calculateCp(power, windSpeed);

        // Safety checks
        bool safe = safety.check(rpm, voltage, current);
        if (!safe)
        {
            stateMachine.setState(STATE_FAULT);
            engageBrake();
        }

        // State machine logic
        TurbineState currentState = stateMachine.getState();

        switch (currentState)
        {
        case STATE_STANDBY:
            if (windSpeed > 3.0 && safe)
            {
                stateMachine.setState(STATE_MPPT);
            }
            break;

        case STATE_MPPT:
        {
            float dutyCycle = mppt.update(power, windSpeed);
            ledcWrite(0, dutyCycle * 255);

            if (power > RATED_POWER * 0.95)
            {
                stateMachine.setState(STATE_POWER_REGULATION);
            }
            if (windSpeed > 12.0)
            {
                stateMachine.setState(STATE_STALL);
            }
        }
        break;

        case STATE_POWER_REGULATION:
        {
            float softStallDuty = calculateSoftStall(power, rpm);
            ledcWrite(0, softStallDuty * 255);

            if (power < RATED_POWER * 0.8)
            {
                stateMachine.setState(STATE_MPPT);
            }
        }
        break;

        case STATE_STALL:
            engageDumpLoad();
            if (rpm < RATED_RPM)
            {
                stateMachine.setState(STATE_STANDBY);
            }
            break;

        case STATE_FAULT:
            engageBrake();
            ledcWrite(0, 0);
            break;

        case STATE_IDLE:
        case STATE_STARTUP:
            // Not implemented in this basic version
            break;
        }

        // Data logging
        dataLogger.log(now, currentState, windSpeed, rpm,
                       voltage, current, power, lambda, cp);

        // Debug output
        Serial.printf("State: %s | λ=%.2f | Cp=%.2f | P=%.1fW | RPM=%.0f\n",
                      stateMachine.getStateName(), lambda, cp, power, rpm);
    }

    // Background tasks
    if (now % 10000 == 0)
    {
        dataLogger.flush();
    }

    delay(10);
}

// === Sensor Reading Functions ===

float readWindSpeed()
{
    // Analog anemometer: 0-5V = 0-25 m/s
    int raw = analogRead(WIND_SPEED_PIN);
    float voltage = (raw / 4095.0) * 3.3; // ESP32 ADC is 0-3.3V
    return voltage * (25.0 / 3.3);        // Scale to wind speed
}

float calculateRPM()
{
    if (pulsePeriod == 0)
        return 0;
    unsigned long period = pulsePeriod;
    float freq = 1000000.0 / period;
    return freq * 60.0;
}

float readBusVoltage()
{
    return ina219.getBusVoltage_V();
}

float readBusCurrent()
{
    return ina219.getCurrent_mA() / 1000.0;
}

float calculateLambda(float rpm, float windSpeed)
{
    if (windSpeed < 0.5)
        return 0;
    float omega = rpm * (2 * PI / 60.0);
    return (omega * ROTOR_RADIUS) / windSpeed;
}

float calculateCp(float power, float windSpeed)
{
    if (windSpeed < 0.5)
        return 0;
    float rho = 1.15;
    float windPower = 0.5 * rho * SWEPT_AREA * pow(windSpeed, 3);
    return power / windPower;
}

void engageDumpLoad()
{
    ledcWrite(0, 255);
    digitalWrite(BRAKE_RELAY_PIN, LOW);
}

void engageBrake()
{
    digitalWrite(BRAKE_RELAY_PIN, HIGH);
    ledcWrite(0, 0);
}

float calculateSoftStall(float power, float rpm)
{
    float targetPower = RATED_POWER;
    float error = targetPower - power;
    static float integrator = 0;
    integrator += error * 0.001;
    float duty = 0.5 + 0.01 * error + integrator;
    return constrain(duty, 0.1, 0.9);
}
