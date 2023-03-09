@echo off
title [BOOTMOD]
color 0A
if "%1" neq "" ( goto %1)

:MENU
cls  
echo BOOTMOD -- Version 2
echo =========================================
adb devices
echo =========================================
echo 1. Enter Bootloader Menu for Mod Flashing
echo 2. Enter APK Menu
echo 3. ADB Connect
echo 4. Reboot to System
echo 5. HELP
echo 6. EXIT BOOTMOD
echo ========================================= 
set /p answer= Please Enter your Selection: 
if %answer%==1 goto BOOTLOADER
if %answer%==2 goto APK
if %answer%==3 goto ADB
if %answer%==4 goto REBOOT
if %answer%==5 goto HELP
if %answer%==6 goto EXIT
:ADB
cls 
set /p answer= Type in SHIELD IP Address: 
adb connect %answer%
goto MENU
 
:APK
cls
echo ===========
echo APK MENU
echo ===========
echo 1. Magisk by TopJohnWu
echo 2. Magisk Delta fork by HuskyDG
echo 3. Return to Main Menu
echo ================================
set /p answer= Please Enter your Selection: 
if %answer%==1 goto MAGISKAPK
if %answer%==2 goto DELTAAPK
if %answer%==3 goto MENU

:MAGISKAPK
adb install magisk.apk
goto MENU

:DELTAAPK
adb install delta.apk
goto MENU

:HELP
cls
echo Adb must be installed on the computer and USB debugging must be enabled on the Shield. 
echo You will need the Fastboot USB drivers installed on your PC. 
echo Check the GUIDE on XDA for more help.
echo LINK: https://forum.xda-developers.com/t/bootmod-root-your-shield-in-1-minute-2015-2017-2019.4524873/
echo =====================================================================================
set /p answer=Press 1 to return to Main Menu: 
if %answer%==1 goto MENU

:UNLOCK
cls
fastboot oem unlock
echo ==============================================================
echo Use your shield to select yes. Then press any key to continue
echo ==============================================================
pause
set /p answer=Press 1 to Enter Flash Menu or 2 to return to Main Menu: 
if %answer%==1 goto FLASH
if %answer%==2 goto MENU

:BOOTLOADER
cls
adb reboot bootloader
pause
cls
echo =====================================================
fastboot devices
echo =====================================================
echo Is your device listed above?
echo =====================================================
set /p answer=Press 1 for yes or 2 for no or 3 for Help: 
if %answer%==1 goto FLASH
if %answer%==2 goto BOOTLOADER
if %answer%==3 goto HELP

:FLASH
cls
echo How would you like to flash your boot today? Do you want to include:
echo ====================================================================
echo 1. TopJohnWu's Magisk 
echo 2. HuskyDG's Magisk Delta
echo 3. Revert Back To Stock Boot Image
echo 4. Reboot Shield to System
echo 5. Return to Main Menu
echo ====================================
set /p answer=Select a number and press enter: 
if %answer%==1 goto MAGISK
if %answer%==2 goto DELTA
if %answer%==3 goto STOCK
if %answer%==4 goto REBOOT
if %answer%==5 goto MENU                                                                                            

:MAGISK
cls
fastboot erase boot
fastboot flash boot bootmod1.img
fastboot reboot
echo =====================================
echo Wait for your Shield to fully Reboot.
echo =====================================
pause
adb install magisk.apk
goto MENU

:DELTA
cls
fastboot erase boot
fastboot flash boot bootmod2.img
fastboot reboot
echo =====================================
echo Wait for your Shield to fully Reboot.
echo =====================================
pause
adb install delta.apk
goto MENU

:STOCK
cls
fastboot erase boot
fastboot flash boot boot.img
echo ================================================================================
echo REMEMBER TO LOCK YOUR BOOTLOADER FOR AI UPSCALING AND DOLBY VISION FUNCTIONALITY
echo ================================================================================
echo 1. Reboot Shield to the System
echo 2. Return to Main Menu
echo =========================
set /p answer=Enter your selection: 
if %answer%==1 goto REBOOT
if %answer%==2 goto MENU

:REBOOT
fastboot reboot
goto MENU

:EXIT
exit /b