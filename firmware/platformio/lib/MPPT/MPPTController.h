/**
 * @file MPPTController.h
 * @brief Header for turbulence-adaptive MPPT controller
 */

#ifndef MPPTCONTROLLER_H
#define MPPTCONTROLLER_H

#include <Arduino.h>

class MPPTController {
public:
    /**
     * @brief Constructor
     * @param lambdaOpt Optimal tip-speed ratio (λ_opt) for the turbine
     *                  Typical values: 1.5-2.5 for helical VAWTs
     */
    MPPTController(float lambdaOpt);
    
    /**
     * @brief Update MPPT algorithm (call at 10Hz)
     * @param power Current power output (W)
     * @param windSpeed Current wind speed (m/s)
     * @return PWM duty cycle (0.0-1.0)
     */
    float update(float power, float windSpeed);
    
    /**
     * @brief Reset MPPT state
     */
    void reset();
    
    /**
     * @brief Get current turbulence intensity for diagnostics
     * @return Turbulence intensity (σ_v / v_mean)
     */
    float getTurbulenceIntensity();

private:
    // Configuration
    float lambdaOpt;                    // Optimal tip-speed ratio
    static const int WIND_BUFFER_SIZE = 100;  // 10s @ 10Hz
    static constexpr float BASE_STEP_SIZE = 0.02;  // 2% base step
    
    // State variables
    float dutyCycle;                    // Current PWM duty cycle
    float lastPower;                    // Previous power sample
    float stepSize;                     // Adaptive step size
    int8_t direction;                   // Search direction (+1 or -1)
    
    // Turbulence adaptation
    float windSpeedBuffer[100];         // Circular buffer for σ_v calculation
    int bufferIndex;                    // Current buffer write position
    int sampleCount;                    // Number of samples collected
    
    /**
     * @brief Calculate adaptive step size based on wind variance
     * @param windSpeed Current wind speed (m/s)
     * @return Adaptive step size (0.005-0.02)
     */
    float calculateAdaptiveStep(float windSpeed);
};

#endif