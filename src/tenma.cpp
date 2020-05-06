#include "tenma.h"
#include <Arduino.h>

// Delay between commands otherwise the PSU freaks out.
// This works pretty reliably at 60-70ms, so I choose 100ms
#define DELAY_TIME_MS 100

// Setup the Serial for the PSU
void TenmaSetup(void)
{
    Serial.begin(9600);
    Serial.setTimeout(DELAY_TIME_MS);
    // Swap ESP8266 serial pins from 1(TX) & 3(RX) -> 15(TX) & 13(RX)
    // We do this because the ESP8266 has built-in debug printing on the UART during boot
    // Once everything is booted and ready to go, we switch UART0 to its alternative GPIOs and use them
    Serial.swap();
}

// Request Identifier from Power supply.
// Returns a String: TENMA72-2540V2.0
String Tenma_IND(void)
{
    Serial.print("*IND?");
    return Serial.readString();
}

// Get Voltage limit from PSU
// Returns float: 12.34
float Tenma_GetVoltage(void)
{
    Serial.print("VSET1?");
    return Serial.parseFloat();
}

// Get current / live Voltage from PSU
// Returns float: 12.34
float Tenma_GetActualVoltage(void)
{
    Serial.print("VOUT1?");
    return Serial.parseFloat();
}

// Set Voltage limit on PSU
void Tenma_SetVoltage(float Voltage)
{
    if (Voltage > 0.1)
        Serial.printf("VSET1:%.2f", Voltage);
    // Since no response. Force delay (otherwise PSU can freak out)
    delay(DELAY_TIME_MS);
}

// Get Current limit from PSU
// Returns float: 0.125
float Tenma_GetCurrent(void)
{
    Serial.print("ISET1?");
    return Serial.parseFloat();
}

// Get current / live Current from PSU
// Returns float: 0.125
float Tenma_GetActualCurrent(void)
{
    Serial.print("IOUT1?");
    return Serial.parseFloat();
}

// Set Current limit on PSU
void Tenma_SetCurrent(float Current)
{
    if (Current > 0.1)
        Serial.printf("ISET1:%0.3f", Current);
    // Since no response. Force delay (otherwise PSU can freak out)
    delay(DELAY_TIME_MS);
}

// Gets status byte from PSU.
// Converted using Status struct into bitfields
Status Tenma_GetStatus(void)
{
    Serial.print("STATUS?");
    // Grab a single byte
    uint8_t byte;
    Serial.readBytes(&byte, 1);
    // Attempted to use bitfields but all I got was a glitchy mess of behaviour.
    // Just manually shift them for now
    Status status = {.CH1Mode = (uint8_t)((byte >> 0) & 0x1),
                     .CH2Mode = (uint8_t)((byte >> 1) & 0x1),
                     .Tracking = (uint8_t)((byte >> 2) & 0x3),
                     .Beep = (uint8_t)((byte >> 4) & 0x1),
                     .Lock = (uint8_t)((byte >> 5) & 0x1),
                     .Output = (uint8_t)((byte >> 6) & 0x1),
                     .Unused = (uint8_t)((byte >> 7) & 0x1)};
    return status;
}

// Grabs the 'Output' field from GetStatus()
bool Tenma_GetOutput(void)
{
    return (bool)(Tenma_GetStatus().Output);
}

// Enable or disable the PSU output
void Tenma_SetOutput(bool Enabled)
{
    // True - Output On
    // False - Output Off
    Serial.printf("OUT%d", Enabled);
    // Since no response. Force delay (otherwise PSU can freak out)
    delay(DELAY_TIME_MS);
}

// Enable or disable OverVoltage protection
// Note there is no way to request if this is currently enabled or not..
void Tenma_SetOVP(bool Enabled)
{
    // True - Output On
    // False - Output Off
    Serial.printf("OVP%d", Enabled);
    // Since no response. Force delay (otherwise PSU can freak out)
    delay(DELAY_TIME_MS);
}

// Enable or disable the PSU output
// Note there is no way to request if this is currently enabled or not..
void Tenma_SetOCP(bool Enabled)
{
    // True - Output On
    // False - Output Off
    Serial.printf("OCP%d", Enabled);
    // Since no response. Force delay (otherwise PSU can freak out)
    delay(DELAY_TIME_MS);
}
