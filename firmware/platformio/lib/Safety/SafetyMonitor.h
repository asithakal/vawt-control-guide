#ifndef SAFETY_MONITOR_H
#define SAFETY_MONITOR_H

#include <Arduino.h>

class SafetyMonitor
{
public:
    SafetyMonitor(float overspeedRPM, float overvoltage);

    bool check(float rpm, float voltage, float current);

    bool isOverspeed() const { return _overspeedFlag; }
    bool isOvervoltage() const { return _overvoltageFlag; }
    bool isOvercurrent() const { return _overcurrentFlag; }

    void reset();

private:
    float _overspeedThreshold;
    float _overvoltageThreshold;
    float _overcurrentThreshold;

    bool _overspeedFlag;
    bool _overvoltageFlag;
    bool _overcurrentFlag;

    unsigned long _lastCheckTime;
};

#endif
