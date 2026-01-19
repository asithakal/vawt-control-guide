/**
 * @file MPPTController.cpp
 * @brief Turbulence-Adaptive Hill-Climb Search MPPT for VAWT
 * 
 * Implements adaptive step sizing based on wind speed variance (σ_v) to:
 * - Converge faster in steady winds (large Δduty)
 * - Reduce oscillation in gusty conditions (small Δduty)
 * 
 * Reference: Kulasekera (2026), VAWT Control Systems Guide, Ch 5.4
 * Compatible with: ESP32, STM32F4 (tested with 500W helical VAWT)
 * 
 * @author Dr. Asitha Kulasekera
 * @license MIT
 */

#include "MPPTController.h"
#include <Arduino.h>

// Constructor
MPPTController::MPPTController(float lambdaOpt) 
    : lambdaOpt(lambdaOpt),
      dutyCycle(0.3),           // Initial duty cycle (30% - safe startup)
      lastPower(0),
      stepSize(0.02),           // Base step size (2%)
      direction(1),
      bufferIndex(0),
      sampleCount(0)
{
    // Initialize wind speed buffer to zero
    for (int i = 0; i < WIND_BUFFER_SIZE; i++) {
        windSpeedBuffer[i] = 0;
    }
}

/**
 * @brief Main MPPT update function - call at 10Hz (100ms intervals)
 * 
 * @param power Current electrical power output (W)
 * @param windSpeed Current wind speed (m/s) - from anemometer or estimated
 * @return Updated PWM duty cycle (0.0-1.0)
 * 
 * @note Adaptive step size activates after 100 samples (10s) for stable σ_v
 */
float MPPTController::update(float power, float windSpeed) {
    // Store wind speed in circular buffer for turbulence calculation
    windSpeedBuffer[bufferIndex] = windSpeed;
    bufferIndex = (bufferIndex + 1) % WIND_BUFFER_SIZE;
    if (sampleCount < WIND_BUFFER_SIZE) sampleCount++;
    
    // Calculate adaptive step based on wind variability
    float stepSize = calculateAdaptiveStep(windSpeed);
    
    // -------------------------------------------------------------------
    // Hill-Climb Search (HCS) Core Logic
    // -------------------------------------------------------------------
    if (power > lastPower) {
        // Power increased → continue in same direction
        dutyCycle += direction * stepSize;
    } else {
        // Power decreased → reverse direction
        direction *= -1;
        dutyCycle += direction * stepSize;
    }
    
    // Constrain duty cycle to safe operating range
    // Lower limit: 10% to maintain minimum load on generator
    // Upper limit: 90% to prevent saturation/instability
    dutyCycle = constrain(dutyCycle, 0.1, 0.9);
    
    lastPower = power;
    return dutyCycle;
}

/**
 * @brief Calculate adaptive MPPT step size based on wind turbulence
 * 
 * Strategy:
 * - Low turbulence (σ_v < 0.5 m/s): Large steps for fast convergence
 * - High turbulence (σ_v > 1.0 m/s): Small steps to reduce oscillation
 * - Linear interpolation between 0.5-1.0 m/s
 * 
 * @param windSpeed Current wind speed (unused, kept for future enhancements)
 * @return Adaptive step size (0.005 to 0.02)
 * 
 * @note Requires ≥100 samples (10s) for reliable variance estimation
 */
float MPPTController::calculateAdaptiveStep(float windSpeed) {
    // Minimum samples required for stable statistics
    if (sampleCount < WIND_BUFFER_SIZE) {
        return BASE_STEP_SIZE;  // Use base step during warm-up
    }
    
    // -------------------------------------------------------------------
    // Calculate Wind Speed Standard Deviation (σ_v)
    // -------------------------------------------------------------------
    // Step 1: Compute mean wind speed over buffer window
    float mean = 0.0;
    for (int i = 0; i < WIND_BUFFER_SIZE; i++) {
        mean += windSpeedBuffer[i];
    }
    mean /= WIND_BUFFER_SIZE;
    
    // Step 2: Compute variance
    float variance = 0.0;
    for (int i = 0; i < WIND_BUFFER_SIZE; i++) {
        float diff = windSpeedBuffer[i] - mean;
        variance += diff * diff;
    }
    float sigma = sqrt(variance / WIND_BUFFER_SIZE);  // Standard deviation
    
    // -------------------------------------------------------------------
    // Adaptive Step Size Calculation
    // -------------------------------------------------------------------
    // Tuning parameter (range: 0.3-0.7; higher = more aggressive reduction)
    const float k_turb = 0.5;  
    
    // Formula: Δduty = Δbase / (1 + k_turb × σ_v)
    // - Steady wind (σ_v → 0):  Δduty → Δbase (0.02 = 2%)
    // - Gusty wind (σ_v = 2.0): Δduty → 0.01 (1%)
    float adaptiveStep = BASE_STEP_SIZE / (1.0 + k_turb * sigma);
    
    // Enforce minimum step size to prevent stalling in extreme turbulence
    const float MIN_STEP_SIZE = 0.005;  // 0.5% minimum
    return max(adaptiveStep, MIN_STEP_SIZE);
}

/**
 * @brief Reset MPPT state - call after fault recovery or mode change
 */
void MPPTController::reset() {
    dutyCycle = 0.3;      // Safe restart duty cycle
    lastPower = 0;
    direction = 1;
    sampleCount = 0;
    bufferIndex = 0;
    for (int i = 0; i < WIND_BUFFER_SIZE; i++) {
        windSpeedBuffer[i] = 0;
    }
}

/**
 * @brief Get current turbulence intensity (for logging/debugging)
 * @return σ_v / mean wind speed (dimensionless)
 */
float MPPTController::getTurbulenceIntensity() {
    if (sampleCount < WIND_BUFFER_SIZE) return 0.0;
    
    float mean = 0.0;
    for (int i = 0; i < WIND_BUFFER_SIZE; i++) {
        mean += windSpeedBuffer[i];
    }
    mean /= WIND_BUFFER_SIZE;
    
    if (mean < 0.5) return 0.0;  // Avoid division by zero at low winds
    
    float variance = 0.0;
    for (int i = 0; i < WIND_BUFFER_SIZE; i++) {
        float diff = windSpeedBuffer[i] - mean;
        variance += diff * diff;
    }
    float sigma = sqrt(variance / WIND_BUFFER_SIZE);
    
    return sigma / mean;  // Turbulence intensity (TI)
}