#ifndef MPPT_CONTROLLER_H
#define MPPT_CONTROLLER_H

#include <Arduino.h>

class MPPTController
{
public:
    MPPTController(float lambdaOpt);
    float update(float power, float windSpeed);
    void reset();

private:
    float _lambdaOpt;
    float _dutyCycle;
    float _lastPower;
    float _stepSize;
    int8_t _direction;

    // Turbulence-adaptive step size
    float calculateAdaptiveStep(float windSpeed);
    float _windSpeedBuffer[10];
    int _bufferIndex;
};

#endif
