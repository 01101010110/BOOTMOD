# BOOTMOD
![BOOTMOD.EXE](https://github.com/01101010110/BOOTMOD/blob/main/PICTURES/0-bootmod.exe.png?raw=true)

A tool that is able to help users to easily root their Nvidia Shield (2019 or 2017). 

This tool is a single executable that contains everything you need to gain a stock root and more. (Adb, fastboot, the usb drivers). Note that you do not need to be rooted to make use of BOOTMOD'S adb features like the no-code automatic apk installation, non-system apk uninstallation, file pushing, file pulling, and opening an adb shell.

## Features
The first time you run BOOTMOD, it will automatically build everything it needs in C:\BOOTMOD. It will then open an adb server for you, load the main menu, then display your connected device. When you close BOOTMOD, it will automatically kill the adb server for us, ensuring no servers are left running. The guide below will show you the rest of the features.

## WARNINGS!!
`Rooting your Shield will break AI Upscaling and Dolby Vision.` --There is hope to get beyond this with build.prop edits, but these haven't been tested by me yet. [Check out this post for more details.](https://xdaforums.com/posts/89105385/)

Also please be warned that `you will lose data any time you go to unlock and relock the bootloader.` Basically an auto factory reset will occur when you lock/ unlock, but you can flash the boot images /without/ data loss.

## Dependencies
Before you proceed, you will need to connect your Shield to your PC using a usb cable, and have usb debugging enabled on your Shield. [Here is a video of this step if you need it.](https://youtu.be/wUrdKRNbLpw)

You will also need to know what firmware version you are running. [Here is a video of this step if you need it.](https://youtu.be/GCcEDhXdS5g)

# Main Menu
![MAIN MENU](https://github.com/01101010110/BOOTMOD/blob/main/PICTURES/1-main-menu.png?raw=true)

## Option 0: Download the latest Magisk &/or TWRP
Start with option 0 to download the latest Magisk apk and / or TWRP.
![DOWNLOAD MAGISK](https://github.com/01101010110/BOOTMOD/blob/main/PICTURES/2-dl-magisk.png?raw=true)

If you want to download TRWP, you are able to press ctrl + c and click on the link to immediately open it in a browser.
![DOWNLOAD TWRP](https://github.com/01101010110/BOOTMOD/blob/main/PICTURES/3-dl-twrp.png?raw=true)

## Option 1: Enter Bootloader Menu for Root and Stock Flashing
![BOOTLOADER MENU](https://github.com/01101010110/BOOTMOD/blob/main/PICTURES/4-bootloader-menu.PNG?raw=true)
When you select option 1, the Shield will reboot into the bootloader menu for us. It is at this point that you may unlock your bootloader if you haven't done so already. Unlocking or relocking your bootloader /will/ result in data loss. Last warning, if you do have to unlock your bootloader, you will need to close BOOTMOD after starting the unlock, make sure the shield reboots afterwards,, then sign back into the Shield, and setup usb debugging again. Then you can proceed as normal by running BOOTMOD and selecting this option again.


Once your bootloader is unlocked, you may select the firmware version for your Shield. 
![SELECT FIRMWARE](https://github.com/01101010110/BOOTMOD/blob/main/PICTURES/5-root-stock-flash.png?raw=true)

Then select if you are flashing root or going back to stock.
![CHOICE](https://github.com/01101010110/BOOTMOD/blob/main/PICTURES/6-choice.png?raw=true)

If your device is not showing at this part, and you have not installed the fastboot drivers, run option 3 for help. Then you may run option 2 again and your device should be showing in the list.

Once your device shows in the list and you run option 1, BOOTMOD will erase the stock boot and flash the rooted boot image for us. (Please note there is no data loss during this process. Only data loss is with locking/unlocking the bootloader). Wait for your Shield to fully reboot then press any key to install the magisk apk to the shield. (When the autoplay message appears, your device is ready.)
![FLASH](https://github.com/01101010110/BOOTMOD/blob/main/PICTURES/8-flashing-root.png?raw=true)

Then you will need to proceed to open Magisk on the Shield in Settings, under apps. Here is a video of this step if you need it.

## Option 2: Enter Bootloader Menu and boot TWRP
![BOOTLOADER MENU](https://github.com/01101010110/BOOTMOD/blob/main/PICTURES/4-bootloader-menu.PNG?raw=true)
When you select option 2, the Shield will reboot into the bootloader menu for us. It is at this point that you may unlock your bootloader if you haven't done so already. Unlocking or relocking your bootloader /will/ result in data loss. Last warning, if you do have to unlock your bootloader, you will need to close BOOTMOD after starting the unlock, make sure the shield reboots afterwards,, then sign back into the Shield, and setup usb debugging again. Then you can proceed as normal by running BOOTMOD and selecting this option again.

![TWRP](https://github.com/01101010110/BOOTMOD/blob/main/PICTURES/9-twrp-menu.png?raw=true)
Your Shield's device id will not be displayed the first time in the TWRP menu of BOOTMOD, because the Shield has to boot into the bootloader first. Wait for the Shield to boot into the bootloader (you will hear Windows make a chime at this point) then run option 2 to verify your Shield is posting. If it isn't posting and you haven't installed your fastboot drivers, run option 3 then 2 again when complete.

![TWRP BOOT](https://github.com/01101010110/BOOTMOD/blob/main/PICTURES/10-twrp-boot.png?raw=true)
When your device is showing, and you run option 1, the Shield will immediately boot into TWRP. You will then have access to the Recovery adb shell in BOOTMOD (using option 8).

## Option 3: Reboot to System
This is just an escape in case one accidentally gets stuck in the bootloader menu. It sends a simple fastboot reboot to push the Shield back to system.

## Option 4: Install APKs Menu
Option 4 will immediately and automatically install any apk file you have placed inside of C:\BOOTMOD\APKS.
![APK INSTALL](https://github.com/01101010110/BOOTMOD/blob/main/PICTURES/11-apk-menu.png?raw=true)

## Option 5: Remove APKs Menu
When you run option 5, you will be presented with a list of all of the apks installed on your Shield. You are able to remove any non system app by double left clicking on the item to highlight the line, right click once to copy the line, then right click once again to paste. As long as the package is not a system app, it will immediately remove the selected app for you. Type the word back to exit the menu.
![REMOVE APK](https://github.com/01101010110/BOOTMOD/blob/main/PICTURES/12-apk-removal.png?raw=true)

## Option 6: Push Files Menu
Running option 6 gives you the ability to push any files in C:\BOOTMOD\PUSH to your Shield. Either immediately to the sdcard/Download folder, or the user's choice.
![PUSH FILE](https://github.com/01101010110/BOOTMOD/blob/main/PICTURES/13-push-menu.png?raw=true)

## Option 7: Pull Files Menu
Running option 7 allows you to pull all files individually from sdcard/Download, not just the whole folder. The user may choose to download any file or folder as well. The pulled items will be located in C:\BOOTMOD\PULL
![PULL FILES](https://github.com/01101010110/BOOTMOD/blob/main/PICTURES/14-pull-menu.png?raw=true)

## Option 8: Open an ADB Shell
Running option 8 will open an adb shell in a new window. You are able to use su after you have installed Magisk. Remember to grant the shell su in Magisk.There will be a popup you need to accept on the Shield the first time you run su.
![ADB SHELL](https://github.com/01101010110/BOOTMOD/blob/main/PICTURES/15-adb-shell.png?raw=true)

## Option 9: Credits
SPECIAL THANKS TO OUR FRIENDS IN THE XDA COMMUNITY!!

Thank you to @topjohnwu for making Magisk
@nooted1 for teaching us about the magisk variant "delta" that has magiskhide still configured inside the apk
@ajolly for their efforts in educating us about safetynet modules that can be used within magisk
@louforgiveno for their efforts in reverse engineering apks, determining that it would be better to clear data in the google playstore instead of cache, and providing excellent feedback on pretty much every step of the way.
@abc1054 for testing the ai upscaler and teaching me how to even use the silly thing.
@Zer0_rulz for testing the upscaler and teaching us about link2sd and providing a useful idea for studies, to "freeze apps" as opposed to straight deletion in tests. I will use both methods in the future!
@pinvok3 for their script they made to teach us how to more efficiently locate the apps tied to the ai upscaler and determining the "tvsettings.apk" to potentially be culpable in jailing our upscaler. They also taught me about the dolby vision feature on the shield
@Renate for allowing me to bother her in the AVB thread while I try to learn how to talk to people like her. haha
@Manzing for stepping up and being the hero we needed for the 2017 shield community! They were able to locate the correct pathing for the OTA Firmware as well as provide us the stock 9.1 boot and complete OTA!!
@I_did_it_just_tmrrow For taking the time to verify magisk is able to indeed patch the 2015 version of the Shield HERE and more so they explained the boot images provided on gameworks for the 2015 version have the same hash, therefore a patched boot will work on both the 16gb and 500gb models. Thank you!
@Sleenie for asking about potential data loss HERE so that tests could be done to determine at what points we actually lose our data. (Only at locking/ unlocking bootloader, in this context.)
