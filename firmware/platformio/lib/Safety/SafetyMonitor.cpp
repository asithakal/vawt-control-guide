#include "SafetyMonitor.h"

SafetyMonitor::SafetyMonitor(float overspeedRPM, float overvoltage)
    : _overspeedThreshold(overspeedRPM),
      _overvoltageThreshold(overvoltage),
      _overcurrentThreshold(30.0), // 30A default
      _overspeedFlag(false),
      _overvoltageFlag(false),
      _overcurrentFlag(false),
      _lastCheckTime(0)
{
}

bool SafetyMonitor::check(float rpm, float voltage, float current)
{
    _lastCheckTime = millis();

    // Check overspeed
    _overspeedFlag = (rpm > _overspeedThreshold);
    if (_overspeedFlag)
    {
        Serial.printf("[SAFETY] OVERSPEED: %.0f RPM (limit: %.0f)\n",
                      rpm, _overspeedThreshold);
    }

    // Check overvoltage
    _overvoltageFlag = (voltage > _overvoltageThreshold);
    if (_overvoltageFlag)
    {
        Serial.printf("[SAFETY] OVERVOLTAGE: %.1f V (limit: %.1f)\n",
                      voltage, _overvoltageThreshold);
    }

    // Check overcurrent
    _overcurrentFlag = (current > _overcurrentThreshold);
    if (_overcurrentFlag)
    {
        Serial.printf("[SAFETY] OVERCURRENT: %.1f A (limit: %.1f)\n",
                      current, _overcurrentThreshold);
    }

    return !(_overspeedFlag || _overvoltageFlag || _overcurrentFlag);
}

void SafetyMonitor::reset()
{
    _overspeedFlag = false;
    _overvoltageFlag = false;
    _overcurrentFlag = false;
}
