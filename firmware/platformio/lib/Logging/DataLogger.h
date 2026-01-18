#ifndef DATA_LOGGER_H
#define DATA_LOGGER_H

#include <Arduino.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include "TurbineStateMachine.h"

class DataLogger
{
public:
    DataLogger();

    bool begin(uint8_t csPin = 5);
    void log(unsigned long timestamp, TurbineState state,
             float windSpeed, float rpm, float voltage,
             float current, float power, float lambda, float cp);
    void flush();
    void close();

private:
    File _logFile;
    String _filename;
    uint8_t _csPin;
    bool _sdInitialized;
    char _buffer[512];
    size_t _bufferPos;

    void writeHeader();
    String stateToString(TurbineState state);
};

#endif
