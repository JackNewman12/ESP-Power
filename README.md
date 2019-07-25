# ESP-Power



[![Build Status](https://travis-ci.org/JackNewman12/ESP-Power.svg?branch=master)](https://travis-ci.org/JackNewman12/ESP-Power)
[![FOSSA Status](https://app.fossa.io/api/projects/git%2Bgithub.com%2FJackNewman12%2FESP-Power.svg?type=shield)](https://app.fossa.io/projects/git%2Bgithub.com%2FJackNewman12%2FESP-Power?ref=badge_shield)
![License: MIT](https://img.shields.io/badge/License-MIT-brightgreen.svg)


This project creates a webserver on a ESP8266 that can be used to control the Tenma / Korad / etc power supplies.
Useful to save time running up and down stairs to reboot a troublesome device.

<a href="url"><img src="./images/WebsiteUI.png" align="middle" height="600" ></a> 
<a href="url"><img src="./images/tenma.jpg" align="middle" height="200" ></a>


# Building
This project uses platform.io to automatically manage the build process / dependencies.  I recommend using the VSCode plugin.
The most basic commands needed for building: (It automatically grabs all the compilers + needed libraries)
```
pip install -U platformio
platformio update
platformio run
```

## Config
By default the device has debug printing on Serial1 (GPIO2). 

If you want the ESP to connect to a WiFi network rather than creating a hotspot. Remove the `#define USE_HOTSPOT` from the code. 
You must also create a `WiFiSettings.h`that matches the `WiFiSettings.h.template`

The RS232 adaptor uses the following pinout. (Swap the TX / RX pins based on your adaptor)
<a href="url"><img src="./images/PossiblePinout.png" align="middle" height="200" ></a>

# License

Shoutout to https://github.com/s00500/ESPUI for doing the heavy lifting for the website

[![FOSSA Status](https://app.fossa.io/api/projects/git%2Bgithub.com%2FJackNewman12%2FESP-Power.svg?type=large)](https://app.fossa.io/projects/git%2Bgithub.com%2FJackNewman12%2FESP-Power?ref=badge_large)
