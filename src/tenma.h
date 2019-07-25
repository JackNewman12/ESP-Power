#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <Arduino.h>

// ____ ___ ____ _  _ ____ ___ ____
// [__   |  |__/ |  | |     |  [__
// ___]  |  |  \ |__| |___  |  ___]

// Convert returned byte to bit fields
struct Status
{
    uint8_t CH1Mode;
    uint8_t CH2Mode;
    uint8_t Tracking; // 00=Independent, 01=Tracking series,11=Tracking parallel
    uint8_t Beep;     // 0=Off, 1=On
    uint8_t Lock;     // 0=Lock, 1=Unlock
    uint8_t Output;   // 0=Off, 1=On
    uint8_t Unused;
};

// ___  _  _ ___  _    _ ____
// |__] |  | |__] |    | |
// |    |__| |__] |___ | |___
// Setup the Serial for the PSU
extern void TenmaSetup(void);

// Request Identifier from Power supply.
// Returns a String: TENMA72-2540V2.0
extern String Tenma_IND(void);

// Get Voltage limit from PSU
// Returns float: 12.34
extern float Tenma_GetVoltage(void);

// Get current / live Voltage from PSU
// Returns float: 12.34
extern float Tenma_GetActualVoltage(void);

// Set Voltage limit on PSU
extern void Tenma_SetVoltage(float Voltage);

// Get Current limit from PSU
// Returns float: 0.125
extern float Tenma_GetCurrent(void);

// Get current / live Current from PSU
// Returns float: 0.125
extern float Tenma_GetActualCurrent(void);

// Set Current limit on PSU
extern void Tenma_SetCurrent(float Current);

// Gets status byte from PSU.
// Converted using Status struct into bitfields
extern Status Tenma_GetStatus(void);

// Grabs the 'Output' field from GetStatus()
extern bool Tenma_GetOutput(void);

// Enable or disable the PSU output
extern void Tenma_SetOutput(bool Enabled);

// Enable or disable OverVoltage protection
extern void Tenma_SetOVP(bool Enabled);

// Enable or disable OverCurrent protection
extern void Tenma_SetOCP(bool Enabled);