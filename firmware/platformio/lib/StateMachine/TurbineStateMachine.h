#ifndef TURBINE_STATE_MACHINE_H
#define TURBINE_STATE_MACHINE_H

#include <Arduino.h>

// Turbine operational states
enum TurbineState
{
    STATE_IDLE,             // System initialized, waiting
    STATE_STANDBY,          // Ready, waiting for wind
    STATE_STARTUP,          // Accelerating to minimum RPM
    STATE_MPPT,             // Maximum power point tracking
    STATE_POWER_REGULATION, // At rated power, soft-stall
    STATE_STALL,            // High wind, dump load active
    STATE_FAULT             // Error condition, safe shutdown
};

class TurbineStateMachine
{
public:
    TurbineStateMachine();

    void setState(TurbineState newState);
    TurbineState getState() const;
    const char *getStateName() const;
    unsigned long getTimeInState() const;

    // State transition logging
    struct StateTransition
    {
        unsigned long timestamp;
        TurbineState fromState;
        TurbineState toState;
        char reason[64];
    };

    void logTransition(const char *reason);
    StateTransition getLastTransition() const;

private:
    TurbineState _currentState;
    TurbineState _previousState;
    unsigned long _stateEntryTime;
    StateTransition _lastTransition;

    void onStateEntry();
    void onStateExit();
};

#endif