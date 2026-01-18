#include "DataLogger.h"

DataLogger::DataLogger()
    : _csPin(5),
      _sdInitialized(false),
      _bufferPos(0)
{
}

bool DataLogger::begin(uint8_t csPin)
{
    _csPin = csPin;

    // Initialize SD card
    if (!SD.begin(_csPin))
    {
        Serial.println("[LOGGER] SD Card initialization failed!");
        _sdInitialized = false;
        return false;
    }

    uint8_t cardType = SD.cardType();
    if (cardType == CARD_NONE)
    {
        Serial.println("[LOGGER] No SD card attached");
        _sdInitialized = false;
        return false;
    }

    Serial.println("[LOGGER] SD Card initialized");
    _sdInitialized = true;

    // Create filename with date
    _filename = "/vawt_data.csv";

    // Check if file exists, if not write header
    if (!SD.exists(_filename.c_str()))
    {
        _logFile = SD.open(_filename.c_str(), FILE_WRITE);
        if (_logFile)
        {
            writeHeader();
            _logFile.close();
        }
    }

    return true;
}

void DataLogger::log(unsigned long timestamp, TurbineState state,
                     float windSpeed, float rpm, float voltage,
                     float current, float power, float lambda, float cp)
{
    if (!_sdInitialized)
        return;

    // Format CSV line
    char line[256];
    snprintf(line, sizeof(line),
             "%lu,%s,%.1f,%.0f,%.2f,%.2f,%.1f,%.2f,%.3f\n",
             timestamp,
             stateToString(state).c_str(),
             windSpeed,
             rpm,
             voltage,
             current,
             power,
             lambda,
             cp);

    // Append to buffer
    size_t lineLen = strlen(line);
    if (_bufferPos + lineLen < sizeof(_buffer))
    {
        strcpy(_buffer + _bufferPos, line);
        _bufferPos += lineLen;
    }
    else
    {
        flush(); // Buffer full, write to SD
        strcpy(_buffer, line);
        _bufferPos = lineLen;
    }
}

void DataLogger::flush()
{
    if (!_sdInitialized || _bufferPos == 0)
        return;

    _logFile = SD.open(_filename.c_str(), FILE_APPEND);
    if (_logFile)
    {
        _logFile.write((uint8_t *)_buffer, _bufferPos);
        _logFile.close();
        _bufferPos = 0;
    }
    else
    {
        Serial.println("[LOGGER] Failed to open log file");
    }
}

void DataLogger::close()
{
    flush();
    _sdInitialized = false;
}

void DataLogger::writeHeader()
{
    _logFile.println("timestamp,state,wind_speed_ms,rotor_rpm,voltage_dc,current_dc,power_w,lambda,cp");
}

String DataLogger::stateToString(TurbineState state)
{
    switch (state)
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
