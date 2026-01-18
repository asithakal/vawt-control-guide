#include "MPPTController.h"

MPPTController::MPPTController(float lambdaOpt)
    : _lambdaOpt(lambdaOpt),
      _dutyCycle(0.3),
      _lastPower(0),
      _stepSize(0.02),
      _direction(1),
      _bufferIndex(0)
{
    for (int i = 0; i < 10; i++)
        _windSpeedBuffer[i] = 0;
}

float MPPTController::update(float power, float windSpeed)
{
    // Store wind speed for turbulence calculation
    _windSpeedBuffer[_bufferIndex++] = windSpeed;
    if (_bufferIndex >= 10)
        _bufferIndex = 0;

    // Calculate adaptive step based on wind variability
    float stepSize = calculateAdaptiveStep(windSpeed);

    // Hill-Climb Search (HCS) logic
    if (power > _lastPower)
    {
        // Power increased - continue in same direction
        _dutyCycle += _direction * stepSize;
    }
    else
    {
        // Power decreased - reverse direction
        _direction *= -1;
        _dutyCycle += _direction * stepSize;
    }

    // Constrain duty cycle
    _dutyCycle = constrain(_dutyCycle, 0.1, 0.9);

    _lastPower = power;
    return _dutyCycle;
}

float MPPTController::calculateAdaptiveStep(float windSpeed)
{
    // Calculate wind speed standard deviation (turbulence indicator)
    float mean = 0;
    for (int i = 0; i < 10; i++)
        mean += _windSpeedBuffer[i];
    mean /= 10.0;

    float variance = 0;
    for (int i = 0; i < 10; i++)
    {
        float diff = _windSpeedBuffer[i] - mean;
        variance += diff * diff;
    }
    float sigma = sqrt(variance / 10.0);

    // Adaptive step: reduce step size in turbulent conditions
    float baseStep = 0.02;
    float k_turb = 0.5; // Tuning parameter
    return baseStep * (1.0 / (1.0 + k_turb * sigma));
}

void MPPTController::reset()
{
    _dutyCycle = 0.3;
    _lastPower = 0;
    _direction = 1;
}