#include "TurbineStateMachine.h"

TurbineStateMachine::TurbineStateMachine()
    : _currentState(STATE_IDLE),
      _previousState(STATE_IDLE),
      _stateEntryTime(0)
{
}

void TurbineStateMachine::setState(TurbineState newState)
{
    if (newState != _currentState)
    {
        onStateExit();

        _previousState = _currentState;
        _currentState = newState;
        _stateEntryTime = millis();

        onStateEntry();

        Serial.printf("[STATE] %s -> %s\n",
                      getStateName(),
                      getStateName());
    }
}

TurbineState TurbineStateMachine::getState() const
{
    return _currentState;
}

const char *TurbineStateMachine::getStateName() const
{
    switch (_currentState)
    {
    case STATE_IDLE:
        return "IDLE";
    case STATE_STANDBY:
        return "STANDBY";
    case STATE_STARTUP:
        return "STARTUP";
    case STATE_MPPT:
        return "MPPT";
    case STATE_POWER_REGULATION:
        return "POWER_REG";
    case STATE_STALL:
        return "STALL";
    case STATE_FAULT:
        return "FAULT";
    default:
        return "UNKNOWN";
    }
}

unsigned long TurbineStateMachine::getTimeInState() const
{
    return millis() - _stateEntryTime;
}

void TurbineStateMachine::logTransition(const char *reason)
{
    _lastTransition.timestamp = millis();
    _lastTransition.fromState = _previousState;
    _lastTransition.toState = _currentState;
    strncpy(_lastTransition.reason, reason, sizeof(_lastTransition.reason) - 1);
    _lastTransition.reason[sizeof(_lastTransition.reason) - 1] = '\0';
}

TurbineStateMachine::StateTransition TurbineStateMachine::getLastTransition() const
{
    return _lastTransition;
}

void TurbineStateMachine::onStateEntry()
{
    // Perform state-specific initialization
    switch (_currentState)
    {
    case STATE_IDLE:
        Serial.println("[STATE] Entering IDLE - System check");
        break;
    case STATE_STANDBY:
        Serial.println("[STATE] Entering STANDBY - Waiting for wind");
        break;
    case STATE_MPPT:
        Serial.println("[STATE] Entering MPPT - Optimizing power");
        break;
    case STATE_POWER_REGULATION:
        Serial.println("[STATE] Entering POWER_REG - Limiting output");
        break;
    case STATE_STALL:
        Serial.println("[STATE] Entering STALL - High wind protection");
        break;
    case STATE_FAULT:
        Serial.println("[STATE] Entering FAULT - Safe shutdown");
        break;
    default:
        break;
    }
}

void TurbineStateMachine::onStateExit()
{
    // Cleanup before leaving state
    switch (_currentState)
    {
    case STATE_IDLE:
        Serial.println("[STATE] Exiting IDLE");
        break;
    case STATE_STANDBY:
        Serial.println("[STATE] Exiting STANDBY");
        break;
    case STATE_MPPT:
        Serial.println("[STATE] Exiting MPPT");
        break;
    case STATE_POWER_REGULATION:
        Serial.println("[STATE] Exiting POWER_REG");
        break;
    case STATE_STALL:
        Serial.println("[STATE] Exiting STALL");
        break;
    case STATE_FAULT:
        Serial.println("[STATE] Exiting FAULT");
        break;
    default:
        break;
    }
}