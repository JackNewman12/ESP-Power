# ESP-Power
This project creates a webserver on a ESP8266 that can be used to control a Tenma / Korad / Other clone power supplies.

<a href="url"><img src="./images/WebsiteUI.png" align="middle" height="700" ></a>
<a href="url"><img src="./images/tenma.jpg" align="middle" height="300" ></a><a href="https://app.fossa.io/projects/git%2Bgithub.com%2FJackNewman12%2FESP-Power?ref=badge_shield" alt="FOSSA Status"><img src="https://app.fossa.io/api/projects/git%2Bgithub.com%2FJackNewman12%2FESP-Power.svg?type=shield"/></a>



## Config
If you want the ESP to connect to a WiFi network rather than creating a hotspot. Remove the #define USE_HOTSPOT from the code. 
You must also create a WiFiSettings.h that matches the WiFiSettings.h.template

The RS232 adaptor uses the following pinout. (Swap the TX / RX pins based on your adaptor)
<a href="url"><img src="./images/PossiblePinout.png" align="middle" height="200" ></a>

# License

Shoutout to https://github.com/s00500/ESPUI for doing the heavy lifting for the website

[![FOSSA Status](https://app.fossa.io/api/projects/git%2Bgithub.com%2FJackNewman12%2FESP-Power.svg?type=large)](https://app.fossa.io/projects/git%2Bgithub.com%2FJackNewman12%2FESP-Power?ref=badge_large)