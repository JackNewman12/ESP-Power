#include <Arduino.h>
#include <DNSServer.h>
#include <ESPUI.h>
#include "Ticker.h"
#include "tenma.h"
#include "WiFiSettings.h"

#if defined(ESP32)
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif

// ___  ____ ____ _ _  _ ____ ____
// |  \ |___ |___ | |\ | |___ [__
// |__/ |___ |    | | \| |___ ___]
#define VOLTAGE_LABEL "Voltage (V):"
#define CURRENT_LABEL "Current (A):"
#define OUTPUT_LABEL "Power Output"
#define SET_VOLTAGE_LABEL "Set Voltage (V)"
#define SET_CURRENT_LABEL "Set Current (A)"
#define MISC_UPDATE_LABEL "Update Input Boxes"

// _    ____ ____ ____ _
// |    |  | |    |__| |
// |___ |__| |___ |  | |___
void UpdateMiscUI(void);
void UpdateUI(void);
void SetVoltageCallback(Control sender, int type);
void SetCurrentCallback(Control sender, int type);
void OutputSwitchCallback(Control sender, int value);
void UpdateMiscUICallback(Control sender, int value);

// ____ _    ____ ___  ____ _    ____
// | __ |    |  | |__] |__| |    [__
// |__] |___ |__| |__] |  | |___ ___]

// Update for the input text boxes
bool gUpdateMiscNeeded = false;

// Update for the Voltage input box
bool gTargetVoltageFlag = false;
float gTargetVoltage = 0.0;

// Update for the Current input box
bool gTargetCurrentFlag = false;
float gTargetCurrent = 0.0;

// Update for the power switch
bool gTargetOutputFlag = false;
bool gTargetOutput = false;

// Ticker to update UI every 400ms
Ticker UIUpdater(UpdateUI, 400);

// _  _ _    _  _ ___  ___  ____ ___ ____ ____
// |  | |    |  | |__] |  \ |__|  |  |___ [__
// |__| |    |__| |    |__/ |  |  |  |___ ___]

// Updates the test input boxes. We only update these when the user asks (so we don't overwrite what they are typing)
void UpdateMiscUI(void)
{
  ESPUI.updateText(SET_VOLTAGE_LABEL, String(Tenma_GetVoltage()));
  ESPUI.updateText(SET_CURRENT_LABEL, String(Tenma_GetCurrent()));
}

// Updates the Live Voltage / Current / Power button cards
void UpdateUI(void)
{
  // Update switches / voltages / currents / etc on all the connect browers.
  ESPUI.print(VOLTAGE_LABEL, String(Tenma_GetActualVoltage(), 2));
  ESPUI.print(CURRENT_LABEL, String(Tenma_GetActualCurrent(), 3));
  ESPUI.updateSwitcher(OUTPUT_LABEL, Tenma_GetOutput());
}

// ____ ____ _    _    ___  ____ ____ _  _ ____
// |    |__| |    |    |__] |__| |    |_/  [__
// |___ |  | |___ |___ |__] |  | |___ | \_ ___]

// Callback for when the user puts something in the Set Voltage box
void SetVoltageCallback(Control sender, int type)
{
  // Set some target values for the next mainloop
  gTargetVoltageFlag = true;
  gTargetVoltage = sender.value.toFloat();
}

// Callback for when the user puts something in the Set Current box
void SetCurrentCallback(Control sender, int type)
{
  // Set some target values for the next mainloop
  gTargetCurrentFlag = true;
  gTargetCurrent = sender.value.toFloat();
}

// Callback for when the user presses the Power Output switch
void OutputSwitchCallback(Control sender, int value)
{
  gTargetOutputFlag = true;

  switch (value)
  {
  case S_ACTIVE:
    gTargetOutput = true;
    break;
  case S_INACTIVE:
    gTargetOutput = false;
    break;
  }
}

// Callback for when the user presses the Update Inputs button
void UpdateMiscUICallback(Control sender, int value)
{
  // Set some target values for the next mainloop
  gUpdateMiscNeeded = true;
}

// ____ ____ ___ _  _ ___
// [__  |___  |  |  | |__]
// ___] |___  |  |__| |

// Define this if you want the ESP to act as a hotspot
// Otherwise you can connect to an existing network using the settings below
#define USE_HOTSPOT
#ifdef USE_HOTSPOT
DNSServer dnsServer;
#endif

void setup(void)
{
  // Setup the PSU driver
  TenmaSetup();

  // Use Serial1 for debugging since it only has a TX pin
  Serial1.begin(115200);

  // Modify the WIFI settings below
  WiFi.hostname("ESP-Power");
#ifdef USE_HOTSPOT
  WiFi.mode(WIFI_AP);
  IPAddress LocalIP = IPAddress(192, 168, 1, 1);
  WiFi.softAPConfig(LocalIP, LocalIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("ESP-Power");
  // WiFi.softAP("ESP-Power", "PASSWORD");
  // Any request to "*" (all domains) will re-direct to this device
  dnsServer.start(53, "*", LocalIP);
#else
  WiFi.begin(WIFISETTINGS_SSID, WIFISETTINGS_PASSWORD);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    // digitalWrite(LED_PIN, LOW);
    delay(500);
    Serial1.print(".");
    // digitalWrite(LED_PIN, HIGH);
    delay(500);
  }
#endif
  Serial1.println("");
  Serial1.print("IP address: ");
  Serial1.println(WiFi.localIP());

  // UI Setup
  ESPUI.switcher(OUTPUT_LABEL, false, &OutputSwitchCallback, COLOR_ALIZARIN);
  ESPUI.label(VOLTAGE_LABEL, COLOR_TURQUOISE, "NO POWER SUPPLY");
  ESPUI.label(CURRENT_LABEL, COLOR_EMERALD, "NO POWER SUPPLY");
  ESPUI.text(SET_VOLTAGE_LABEL, &SetVoltageCallback, COLOR_ALIZARIN, "Set Voltage:");
  ESPUI.text(SET_CURRENT_LABEL, &SetCurrentCallback, COLOR_ALIZARIN, "Set Current Limit");
  ESPUI.button(MISC_UPDATE_LABEL, UpdateMiscUICallback, COLOR_EMERALD, "Update");

  // Start server
  ESPUI.begin("ESP-Power");
  // Start server with password
  // ESPUI.begin("ESP-Power", "admin", "mypassword");

  // For an update on boot + start the ticker for the UI
  UpdateMiscUI();
  UpdateUI();
  UIUpdater.start();
}

// _  _ ____ _ _  _ _    ____ ____ ___
// |\/| |__| | |\ | |    |  | |  | |__]
// |  | |  | | | \| |___ |__| |__| |
void loop(void)
{
  // If the user has toggled the power button
  if (gTargetOutputFlag)
  {
    gTargetOutputFlag = false;
    Tenma_SetOutput(gTargetOutput);
    // A small delay so the UI does not update. (Stops the toggle button freaking out)
    // This is because it takes some amount of time before the PSU actually starts to return the correct 'output' status.
    delay(300);
  }

  // If the user has changed the target voltage
  if (gTargetVoltageFlag)
  {
    gTargetVoltageFlag = false;
    Tenma_SetOutput(false);
    Tenma_SetVoltage(gTargetVoltage);
  }

  // If the user has changed the target current
  if (gTargetCurrentFlag)
  {
    gTargetCurrentFlag = false;
    Tenma_SetOutput(false);
    Tenma_SetCurrent(gTargetCurrent);
  }

  // If the user requested a refresh of the input boxes
  if (gUpdateMiscNeeded)
  {
    gUpdateMiscNeeded = false;
    UpdateMiscUI();
  }

  // Ticker to update voltage / current / power switch UI
  UIUpdater.update();

#ifdef USE_HOTSPOT
  dnsServer.processNextRequest();
#endif
}
