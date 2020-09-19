# Controller

TTGO T-Controller ESP Arduino Framework code, using PlatformIO.

![image](https://github.com/marf41/Controller/images/pinout.jpg)

[More details on the module.](https://p3dt.net/post/2019/07/22/ttgo-t-controller.html)

## Libraries:

 - [U8g2](https://github.com/olikraus/u8g2.git)
 - [EasyButton](https://github.com/evert-arias/EasyButton.git)
 - [NTP](https://github.com/sstaub/NTP.git)


## Building and flashing

Clone repository and enter directory:

```sh
git clone https://github.com/marf41/Controller
cd Controller
```

Set your Wi-Fi SSID and password - create file `include/wifi.hpp` with:

```hpp
const char* wifissid = "YOUR_SSID_HERE";
const char* wifipass = "YOUR_PASSWORD_HERE";
```

> WARNING: This file is in `.gitignore`, so it will NOT be commited to git.

Adjust COM port in `platformio.ini` (COMx in Windows, /dev/ttyXXX in Linux):

```conf
[env:serial]
upload_port = COM3
upload_speed = 115200
```

Plug microUSB cable into the board, and flash it:

```sh
platformio run -t upload -e serial
```

After upload and reboot, it should connect to WiFi and display assigned IP on OLED.

On subsequent uploads, You can set its IP in `platformio.ini`:


```conf
[env:ota]
upload_protocol = espota
upload_port = 192.168.0.15
```

And upload wirelessly using:

```sh
platformio run -t upload -e ota
```
