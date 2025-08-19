# Google Photos unlimited backup module
Adds Photos features and unlimited original backup

Current version : **1.1-stable** [Download from releases](https://git.disroot.org/cuynu/gphotos-unlimited-zygisk/releases)

### [Chuyển ngôn ngữ Tiếng Việt](https://git.disroot.org/cuynu/gphotos-unlimited-zygisk/wiki/VI)

# Introduction 
Based from Pixelify GitHub. This module will spoof your device info to Pixel XL on Google apps and Google Photos to get unlimited backup storage at original quality !

# Warning !
Module will only works with Zygisk, it will not works with Riru, Shamiko, etc !!

Some modules could break and prevent this module from running, if module does not work, try disable some modules and see.

# Usage 
Download module from [release page](https://git.disroot.org/cuynu/gphotos-unlimited-zygisk/releases), enable Zygisk then install with Magisk v24.3+ and reboot. 

For KernelSU/KernelSU-Next/RKSU/APatch users, **DON'T use ZygiskNext 1.0.0+** !. Instead, use [ReZygisk](https://github.com/PerformanC/ReZygisk/releases). 
- Tested with : `Magisk v25.2+` `Magisk Delta/Kitsune` `KernelSU` `KernelSU-Next `APatch`

# Known issues 
- **Some Xiaomi devices with MIUI 14 when this module is active, its will cause Bootloop with SystemUI crash exception**
- When use with `Magisk-GApps` module, it will conflict and cause lose the ability to backup unlimited at original quality (Still able to backup unlimited at storage saver quality)

# Tested devices
- Sony Xperia XZ2 (Stock Android 10, LineageOS 18.1,19.1,20.0,21.0,22.2)
- Redmi Note 7 (MIUI 12.5 CN, Nusantara EOL A10)
- Google Pixel 2 (Android 11 stock)
- Masstel_X9 (Stock Android 9, LineageOS 18.1/19.1)
- LG G7 ThinQ (Stock Android 10, LineageOS 21.0/22.2)
- Vsmart Joy 2+ (LineageOS 20.0)

# ToDo
- Rewrite hook library in C++ and take it away from Pixelify (as long as i learn C++ :>)

# Build zygisk binary
`git clone https://git.disroot.org/cuynu/gphotos-unlimited-zygisk.git`

Import folder zygisk_build to Android Studio or Gradle and build then copy compiled zygisk library to zygisk folder (not zygisk_build)

