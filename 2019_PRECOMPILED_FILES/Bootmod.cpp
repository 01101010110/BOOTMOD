#include <iostream> // For std::cout, std::cin, std::endl
#include <windows.h> // For SetEnvironmentVariableW
#include <sstream>  // For std::ostringstream
#include <fstream>  // For std::ofstream
#include <string> // For std::string
#include <vector>  // For std::vector
#include <filesystem> // For std::filesystem
#include <direct.h>  // For _chdir
#include <algorithm> // Required for std::find
#include <cstdlib> // Required for system()
#include <limits> // Required for std::numeric_limits

// Include the header files for the files to be unpacked to C:\BOOTMOD\BIN
#include "FastbootDrivers_png.h"
#include "jq_win64_exe.h"
#include "libcurl_x64_def.h"
#include "libcurl_x64_dll.h"
#include "curl_ca_bundle_crt.h"
#include "curl_exe.h"
#include "adb_exe.h"
#include "fastboot_exe.h"
#include "AdbWinApi_dll.h"
#include "AdbWinUsbApi_dll.h"
#include "91MOD_img.h"
#include "823MOD_img.h"
#include "900MOD_img.h"
#include "911MOD_img.h"
#include "91STOCK_IMG.h"
#include "823STOCK_img.h"
#include "900STOCK_img.h"
#include "911STOCK_img.h"
#include "android_winusb.h"
#include "androidwinusb86.h"
#include "androidwinusba64.h"
#include "source_properties.h"
#include "NOTICE.h"
#include "WdfCoInstaller01009.h"
#include "winusbcoinstaller2.h"
#include "WUDFUpdate_01009.h"
#include "i386NOTICE.h"
#include "i386WdfCoInstaller01009.h"
#include "i386winusbcoinstaller2.h"
#include "i386WUDFUpdate_01009.h"

// Function declarations
BOOL WINAPI ConsoleHandler(DWORD dwType);
void ShowMenu();
void UnpackAllFiles();
void UnpackFile(const std::string& filename, const unsigned char* data, unsigned int size);
void BootloaderMenu();
void RunCommand(const std::string& command);
void FlashBootMod(const std::string& modFileName);
void RevertToStock(const std::string& firmwareVersion);
void DisplayHelp();
void TwrpMenu();
void RebootToSystem();
void InstallApksMenu();
void RemoveApksMenu();
void PullFilesMenu();
void PushFilesMenu();
void OpenAdbShell();
void DownloadMagisk();
void InstallTWRP();
void Credits();

int main() {
    // Attempt to set a control handler to manage console events like closing
    if (!SetConsoleCtrlHandler(ConsoleHandler, TRUE)) {
        std::cerr << "ERROR: Could not set control handler." << std::endl; // If setting the handler fails, print an error message
        return 1; // Exit the program with a status code indicating an error
    }

    // Define the path for the BIN folder where binaries and other necessary files will be stored
    const std::filesystem::path binFolderPath = "C:\\BOOTMOD\\BIN";

    // Check if the BIN folder exists, and if not, attempt to create it
    if (!std::filesystem::exists(binFolderPath) && !std::filesystem::create_directories(binFolderPath)) {
        std::cerr << "Failed to create BIN folder. Unknown error." << std::endl; // Print an error message if the folder cannot be created
        return 1; // Exit the program with a status code indicating an error
    }

    // Unpack all necessary files to the BIN folder
    UnpackAllFiles();

    // Start the ADB server using the adb.exe binary located in the BIN folder
    system("C:\\BOOTMOD\\BIN\\adb.exe start-server");
    Sleep(2000); // Wait for 2 seconds to ensure the ADB server has enough time to start
    ShowMenu(); // Display the main menu
    return 0; // Exit the program
}


// Function to ensure that the ADB server is killed when the user closes the console window
BOOL WINAPI ConsoleHandler(DWORD dwType) {
    switch (dwType) {
    case CTRL_CLOSE_EVENT:
        std::cout << "Closing ADB server..." << std::endl;
        system("C:\\BOOTMOD\\BIN\\adb.exe kill-server");

        // Give it some time to execute the command
        Sleep(2000); // Sleep for 2000 milliseconds (2 seconds)

        // Signal that the handler has processed the event
        return TRUE;
    default:
        break;
    }
    return FALSE;
}

// Function to unpack a single file from embedded binary data into the filesystem
void UnpackFile(const std::string& filename, const unsigned char* data, unsigned int size) {
    std::ofstream fileStream(filename, std::ios::binary); // Create an output file stream in binary mode
    fileStream.write(reinterpret_cast<const char*>(data), size); // Write the binary data to the file
    fileStream.close(); // Close the file stream, ensuring data is flushed and file is saved
}

// Function to unpack all required files to their respective directories
void UnpackAllFiles() {
    // Define paths for the main directory and subdirectories where files will be unpacked
    const std::filesystem::path bootmodPath = "C:\\BOOTMOD";
    const std::filesystem::path bootmodBinPath = bootmodPath / "BIN";
    const std::filesystem::path appsFolderPath = bootmodPath / "APKS";
    const std::filesystem::path pushFolderPath = bootmodPath / "PUSH";
    const std::filesystem::path pullFolderPath = bootmodPath / "PULL";
    const std::filesystem::path amd64FolderPath = bootmodBinPath / "amd64";
    const std::filesystem::path i386FolderPath = bootmodBinPath / "i386";

    // Ensure all necessary directories exist, creating them if they do not
    if (!std::filesystem::exists(bootmodPath)) std::filesystem::create_directories(bootmodPath);
    if (!std::filesystem::exists(bootmodBinPath)) std::filesystem::create_directories(bootmodBinPath);
    if (!std::filesystem::exists(appsFolderPath)) std::filesystem::create_directories(appsFolderPath);
    if (!std::filesystem::exists(pushFolderPath)) std::filesystem::create_directories(pushFolderPath);
    if (!std::filesystem::exists(pullFolderPath)) std::filesystem::create_directories(pullFolderPath);
    if (!std::filesystem::exists(amd64FolderPath)) std::filesystem::create_directories(amd64FolderPath);
    if (!std::filesystem::exists(i386FolderPath)) std::filesystem::create_directories(i386FolderPath);

    // List of main files to be unpacked, each represented by its binary data and size
    std::vector<std::pair<const unsigned char*, size_t>> main_files = {
        {Fastboot_Drivers_png, Fastboot_Drivers_png_len},
        {curl_exe, curl_exe_len},
        {curl_ca_bundle_crt, curl_ca_bundle_crt_len},
        {jq_win64_exe, jq_win64_exe_len},
        {libcurl_x64_def, libcurl_x64_def_len},
        {libcurl_x64_dll, libcurl_x64_dll_len},
        {adb_exe, adb_exe_len},
        {fastboot_exe, fastboot_exe_len},
        {AdbWinApi_dll, AdbWinApi_dll_len},
        {AdbWinUsbApi_dll, AdbWinUsbApi_dll_len},
        {__91MOD_img, __91MOD_img_len},
        {__823MOD_img, __823MOD_img_len},
        {__900MOD_img, __900MOD_img_len},
        {__911MOD_img, __911MOD_img_len},
        {__91STOCK_img, __91STOCK_img_len},
        {__823STOCK_img, __823STOCK_img_len},
        {__900STOCK_img, __900STOCK_img_len},
        {__911STOCK_img, __911STOCK_img_len},
        {android_winusb_inf, android_winusb_inf_len},
        {androidwinusb86_cat, androidwinusb86_cat_len},
        {androidwinusba64_cat, androidwinusba64_cat_len},
        {source_properties, source_properties_len}
    };
    // List of filenames for the main files to be unpacked
    std::vector<std::string> main_filenames = {
        "Fastboot Drivers.png",
        "curl.exe",
        "curl-ca-bundle.crt",
        "jq-win64.exe",
        "libcurl-x64.def",
        "libcurl-x64.dll",
        "adb.exe",
        "fastboot.exe",
        "AdbWinApi.dll",
        "AdbWinUsbApi.dll",
        "91MOD.img",
        "823MOD.img",
        "900MOD.img",
        "911MOD.img",
        "91STOCK.img",
        "823STOCK.img",
        "900STOCK.img",
        "911STOCK.img",
        "android_winusb.inf",
        "androidwinusb86.cat",
        "androidwinusba64.cat",
        "source.properties"
    };

    // Unpack each main file to the BIN directory
    for (size_t i = 0; i < main_files.size(); ++i) {
        UnpackFile((bootmodBinPath / main_filenames[i]).string(), main_files[i].first, static_cast<unsigned int>(main_files[i].second));
    }

    // Lists and filenames for AMD64-specific files
    std::vector<std::pair<const unsigned char*, size_t>> amd64_files = {
        {NOTICE_txt, NOTICE_txt_len},
        {WdfCoInstaller01009_dll, WdfCoInstaller01009_dll_len},
        {winusbcoinstaller2_dll, winusbcoinstaller2_dll_len},
        {WUDFUpdate_01009_dll, WUDFUpdate_01009_dll_len}
    };
    std::vector<std::string> amd64_filenames = {
        "NOTICE.txt",
        "WdfCoInstaller01009.dll",
        "winusbcoinstaller2.dll",
        "WUDFUpdate_01009.dll"
    };

    // Unpack AMD64 files to the amd64 directory (for shield drivers)
    for (size_t i = 0; i < amd64_files.size(); ++i) {
        UnpackFile((amd64FolderPath / amd64_filenames[i]).string(), amd64_files[i].first, static_cast<unsigned int>(amd64_files[i].second));
    }

    // Unpack i386 files to the i386 directory (for shield drivers)
    std::vector<std::pair<const unsigned char*, size_t>> i386_files = {
        {i386NOTICE_txt, i386NOTICE_txt_len},
        {i386WdfCoInstaller01009_dll, i386WdfCoInstaller01009_dll_len},
        {i386winusbcoinstaller2_dll, i386winusbcoinstaller2_dll_len},
        {i386WUDFUpdate_01009_dll, i386WUDFUpdate_01009_dll_len}
    };
    std::vector<std::string> i386_filenames = {
        "NOTICE.txt",
        "WdfCoInstaller01009.dll",
        "winusbcoinstaller2.dll",
        "WUDFUpdate_01009.dll"
    };

    // Unpack i386 files to the i386 directory
    for (size_t i = 0; i < i386_files.size(); ++i) {
        UnpackFile((i386FolderPath / i386_filenames[i]).string(), i386_files[i].first, static_cast<unsigned int>(i386_files[i].second));
    }
}

// Function to display the main menu and handle user input
void ShowMenu() {
    system("cls"); // Clear the console screen
    system("C:\\BOOTMOD\\BIN\\adb.exe devices"); // List connected devices
    std::cout << "\x1b[32m"; // Set text color to green
    std::cout << R"($$$$$$$\   $$$$$$\   $$$$$$\ $$$$$$$$\ $$\      $$\  $$$$$$\  $$$$$$$\)" << std::endl;
    std::cout << R"($$  __$$\ $$  __$$\ $$  __$$\\__$$  __|$$$\    $$$ |$$  __$$\ $$  __$$\)" << std::endl;
    std::cout << R"($$ |  $$ |$$ /  $$ |$$ /  $$ |  $$ |   $$$$\  $$$$ |$$ /  $$ |$$ |  $$ |)" << std::endl;
    std::cout << R"($$$$$$$\ |$$ |  $$ |$$ |  $$ |  $$ |   $$\$$\$$ $$ |$$ |  $$ |$$ |  $$ |)" << std::endl;
    std::cout << R"($$  __$$\ $$ |  $$ |$$ |  $$ |  $$ |   $$ \$$$  $$ |$$ |  $$ |$$ |  $$ |)" << std::endl;
    std::cout << R"($$ |  $$ |$$ |  $$ |$$ |  $$ |  $$ |   $$ |\$  /$$ |$$ |  $$ |$$ |  $$ |)" << std::endl;
    std::cout << R"($$$$$$$  | $$$$$$  | $$$$$$  |  $$ |   $$ | \_/ $$ | $$$$$$  |$$$$$$$  |)" << std::endl;
    std::cout << R"(\_______/  \______/  \______/   \__|   \__|     \__| \______/ \_______/ )" << std::endl;
    std::cout << "\nVersion 1 - 2019 Pro Model\n";
    std::cout << "\x1b[0m"; // Reset text color to default
    std::cout << "\n==================\n";
    std::cout << "Main Menu Options: \n";
    std::cout << "==================\n\n";
    std::cout << "1. Enter Bootloader Menu for Root and Stock Flashing\n";
    std::cout << "2. Enter Bootloader Menu and boot TWRP\n";
    std::cout << "3. Reboot to System\n";
    std::cout << "4. Install APKs Menu\n";
    std::cout << "5. Remove APKs Menu\n";
    std::cout << "6. Push Files Menu\n";
    std::cout << "7. Pull Files Menu\n";
    std::cout << "8. Open an ADB Shell\n";
    std::cout << "9. Credits\n";
    std::cout << "0. Download the latest Magisk &/or TWRP.\n\n";

    int choice; // Variable to store the user's choice
    std::vector<int> validChoices = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };

    while (true) {
        std::cout << "Type a number above and press enter: ";
        std::cin >> choice;
        if (std::find(validChoices.begin(), validChoices.end(), choice) != validChoices.end()) {
            break;
        }
        else {
            std::cout << "Invalid option. Please try again.\n";
        }
    }

    switch (choice) {
    case 1:
        BootloaderMenu();
        break;
    case 2:
        TwrpMenu();
        break;
    case 3:
        RebootToSystem();
        break;
    case 4:
        InstallApksMenu();
        break;
    case 5:
        RemoveApksMenu();
		break;
    case 6:
        PushFilesMenu();
        break;
    case 7:
        PullFilesMenu();
        break;
    case 8:
        OpenAdbShell();
        break;
    case 9:
        Credits();
		break;
    case 0:
        DownloadMagisk();
        break;
    }
}

std::string basePath = "C:\\BOOTMOD\\BIN\\"; // Global variable

// Function to reboot the device to the bootloader menu and assist the user in flashing root or stock firmware
void BootloaderMenu() {
    system("cls");
    system("C:\\BOOTMOD\\BIN\\adb.exe reboot bootloader");
    system("cls");
    std::cout << "Select the firmware version for your device:\n\n";
    std::cout << "1 - 8.2.3\n";
    std::cout << "2 - 9.0.0\n";
    std::cout << "3 - 9.1.0\n";
    std::cout << "4 - 9.1.1\n\n";
    std::cout << "Enter the number of your firmware version: ";

    int choice;
    std::cin >> choice;

    // Mapping user choice to firmware version
    std::string firmwareVersion;
    switch (choice) {
    case 1:
        firmwareVersion = "823";
        break;
    case 2:
        firmwareVersion = "900";
        break;
    case 3:
        firmwareVersion = "91";
        break;
    case 4:
        firmwareVersion = "911";
        break;
    default:
        std::cout << "Invalid choice. Please restart the process.\n";
        return;
    }
    // Ask the user if they want to flash the rooted boot mod or revert back to stock
    system("cls");
    std::cout << "\nDo you want to:\n\n";
    std::cout << "1 - Flash the Rooted Boot Mod\n";
    std::cout << "2 - Revert Back to Stock\n\n";
    std::cout << "Enter your choice: ";
    int flashChoice;
    std::cin >> flashChoice;

    if (flashChoice == 1) {
        std::string modFileName = basePath + firmwareVersion + "MOD.img";
        FlashBootMod(modFileName);
    }
    else if (flashChoice == 2) {
        RevertToStock(firmwareVersion);
    }
    else {
        std::cout << "Invalid choice. Please restart the process.\n";
    }
}

// Conversion function
std::wstring ConvertToWideString(const std::string& str) {
    std::wstring wstr(str.begin(), str.end());
    return wstr;
}

// Function to run a command in the command prompt and display the output
void RunCommand(const std::string& command) {
    std::string fullCommand = command + " > temp.txt";
    system(fullCommand.c_str());

    std::ifstream inFile("temp.txt");
    std::string line;
    while (std::getline(inFile, line)) {
        std::cout << line << std::endl;
    }
    inFile.close();
    system("del temp.txt");
}

// Function to Flash the Boot Mod (Root)
void FlashBootMod(const std::string& modFileName) {
    system("cls");

    bool deviceConfirmed = false;
    while (!deviceConfirmed) {
        RunCommand("C:\\BOOTMOD\\BIN\\fastboot.exe devices");

        std::cout << "\nIs your device id displayed above?\n";
        std::cout << "\n1. Yes\n2. No\n3. Help\n";
        std::cout << "\nEnter your choice: ";
        int choice;
        std::cin >> choice;

        if (choice == 1) {
            deviceConfirmed = true;
        }
        else if (choice == 2) {
            system("cls");
            continue;
        }
        else if (choice == 3) {
            DisplayHelp();
            continue;
        }
        else {
            std::cout << "Invalid option. Please try again.\n";
            system("pause");
            system("cls");
            continue;
        }
    }

    // Set ANDROID_PRODUCT_OUT environment variable
    std::string androidProductOutPath = "C:\\BOOTMOD\\BIN";
    SetEnvironmentVariableW(L"ANDROID_PRODUCT_OUT", ConvertToWideString(androidProductOutPath).c_str());
    system("cls");
    system("C:\\BOOTMOD\\BIN\\fastboot.exe erase boot");
    system(("C:\\BOOTMOD\\BIN\\fastboot.exe flash boot " + modFileName).c_str());
    system("C:\\BOOTMOD\\BIN\\fastboot.exe reboot");

    std::cout << "\n=======================================================\n";
    std::cout << "Wait for your Shield to fully Reboot before continuing.\n";
    std::cout << "=======================================================\n\n";
    system("pause");

    system("C:\\BOOTMOD\\BIN\\adb.exe install C:\\BOOTMOD\\BIN\\magisk.apk");
    std::cout << "\nPress any key to return to the main menu..\n";
    system("pause>nul"); // Pause for user verification
    system("cls"); // Clear the console screen
    ShowMenu();
}

// Function to revert the device back to stock firmware
void RevertToStock(const std::string& firmwareVersion) {
    system("cls");
    bool deviceConfirmed = false;
    while (!deviceConfirmed) {
        RunCommand("C:\\BOOTMOD\\BIN\\fastboot.exe devices");
        std::cout << "\nIs your device id displayed above?\n";
        std::cout << "\n1. Yes\n2. No\n3. Help\n";
        std::cout << "\nEnter your choice: ";
        int choice;
        std::cin >> choice;
        if (choice == 1) {
            deviceConfirmed = true;
        }
        else if (choice == 2) {
            system("cls");
            continue;
        }
        else if (choice == 3) {
            DisplayHelp();
            continue;
        }
        else {
            std::cout << "Invalid option. Please try again.\n";
            system("pause");
            system("cls");
            continue;
        }
    }

    // Set ANDROID_PRODUCT_OUT environment variable
    std::string androidProductOutPath = "C:\\BOOTMOD\\BIN";
    SetEnvironmentVariableW(L"ANDROID_PRODUCT_OUT", ConvertToWideString(androidProductOutPath).c_str());
    system("cls");
    std::string stockFileName = basePath + firmwareVersion + "STOCK.img";
    std::cout << "Flashing stock firmware: " << stockFileName << "\n";
    RunCommand("C:\\BOOTMOD\\BIN\\fastboot.exe erase boot");
    RunCommand("C:\\BOOTMOD\\BIN\\fastboot.exe flash boot " + stockFileName);
    RunCommand("C:\\BOOTMOD\\BIN\\fastboot.exe reboot");
    std::cout << "\nDevice is now rebooting with stock firmware...\n";
    std::cout << "\nPress any key to return to the Main Menu..\n";
    system("pause>nul"); // Pause for user verification
    system("cls"); // Clear the console screen
    ShowMenu();
}

// Function to display help instructions for installing fastboot drivers
void DisplayHelp() {
    system("cls"); // Clear the console screen

    // Open the help image in the default viewer
    system("start \"\" \"C:\\BOOTMOD\\BIN\\Fastboot Drivers.png\"");

    // Open Device Manager
    system("start devmgmt.msc");

    // Display instructions
    std::cout << "You will need NVIDIA's fastboot drivers installed in order to proceed.\n\n";
    std::cout << "Device Manager has now been opened for you.\n\n";
    std::cout << "Picture instructions have been provided and opened for your convenience.\n\n";
    std::cout << "If you prefer video instructions, see this video: https://www.youtube.com/watch?v=KNUwgptfInE\n";
    std::cout << "(Press ctrl + c and click on the link to open it in a browser)\n\n";
    std::cout << "WRITTEN INSTRUCTIONS:\n\n";
    std::cout << "In device manager, under \"Other devices\", right-click the fastboot device.\n\n";
    std::cout << "Select update driver. Select browse my computer, then browse to C:\\BOOTMOD\\BIN.\n\n";
    std::cout << "Then Select \"let me pick from a list of available drivers on my PC\"\n\n";
    std::cout << "Select \"Show All Devices\", then select next. \n\n";
    std::cout << "Select \"have disk\", \"browse\", and open android_winusb.inf. Press \"OK\".\n\n";
    std::cout << "Lastly, select \"Android Bootloader Interface\" and say yes to the update driver warning.\n\n";
    std::cout << "When you are done you will have a new device \"Android Phone\" with the Android Bootloader Interface registered to it.\n\n";
    std::cout << "You may now close device manager and proceed with your flash.\n\n";
    system("pause"); // Wait for user to read the instructions
    system("cls"); // Clear the console screen
}

// Function to boot the device into TWRP recovery
void TwrpMenu() {
    system("cls");
    system("C:\\BOOTMOD\\BIN\\adb.exe reboot bootloader");
    system("cls");
    std::cout << "Waiting for device to enter bootloader mode...\n";
    Sleep(20000); // Wait for 20 seconds for the device to enter bootloader mode
    bool deviceConfirmed = false;
    while (!deviceConfirmed) {
        RunCommand("C:\\BOOTMOD\\BIN\\fastboot.exe devices");

        std::cout << "\nIs your device ID displayed above? (1. Yes, 2. No, 3. Help)\n";
        std::cout << "Enter your choice: ";
        int choice;
        std::cin >> choice;

        if (choice == 1) {
            deviceConfirmed = true;
        }
        else if (choice == 2) {
            system("cls");
            continue;
        }
        else if (choice == 3) {
            DisplayHelp(); 
            continue;
        }
        else {
            std::cout << "Invalid option. Please try again.\n";
            system("pause");
            system("cls");
            continue;
        }
    }

    std::cout << "Booting TWRP...\n";
    RunCommand("C:\\BOOTMOD\\BIN\\fastboot.exe boot C:\\BOOTMOD\\BIN\\twrp.img");
    std::cout << "\n\nYour Shield should now be booting into TWRP.\n";
    std::cout << "\nPress any key to return to the main menu..\n";
    system("pause"); // Wait for user acknowledgment
    ShowMenu(); // Return to the main menu
}

// Function to Reboot the device to the system from fastboot
void RebootToSystem() {
    system("cls"); // Clear the console screen
    // Run the fastboot command to reboot the system
    system("C:\\BOOTMOD\\BIN\\fastboot reboot");
    std::cout << "\nReboot to system complete.\n" << std::endl; // Notify the user
    system("pause");// Pause for user acknowledgment   
    ShowMenu(); // Return to the main menu
}

// Function to install APKs from the APKS folder
void InstallApksMenu() {
    system("cls"); // Clear the console screen
    std::cout << "=========================\n";
    std::cout << "Now Installing All Apks..\n";
    std::cout << "=========================\n\n";
    const std::filesystem::path appsFolderPath = "C:\\BOOTMOD\\APKS";

    // Check if the APKS folder exists
    if (!std::filesystem::exists(appsFolderPath) || std::filesystem::is_empty(appsFolderPath)) {
        std::cout << "No APKs found in " << appsFolderPath << std::endl;
        system("pause"); // Wait for user acknowledgment
        ShowMenu();
        return;
    }

    // Iterate over each file in the APKS folder so we can install them automatically
    for (const auto& file : std::filesystem::directory_iterator(appsFolderPath)) {
        if (file.path().extension() == ".apk") {
            std::string apkPath = file.path().string();
            std::cout << "Installing: " << apkPath << std::endl;
            RunCommand("C:\\BOOTMOD\\BIN\\adb.exe install \"" + apkPath + "\"");
        }
    }

    std::cout << "\nAPKs installed successfully.\n";
    system("pause"); // Wait for user acknowledgment
    system("cls"); // Clear the console screen
    ShowMenu();
}

// Function to remove installed APKs using package manager
void RemoveApksMenu() {
    while (true) {
        system("cls");
        std::cout << "================\n";
        std::cout << "Remove Apks Menu\n";
        std::cout << "================\n\n";

        // List all installed APKs
        std::cout << "Listing all installed APKs...\n";
        system("C:\\BOOTMOD\\BIN\\adb.exe shell \"pm list packages\"");

        // Wait for the user to enter the package name to be removed
        std::cout << "\nEnter the full package name (you may include 'package:') of the APK to uninstall, or type 'back' to return: ";

        std::string packageName;
        std::getline(std::cin, packageName);

        if (packageName.empty()) {
            std::cout << "Invalid input. Please try again.\n";
            system("pause");
            continue;
        }
        else if (packageName == "back") {
            ShowMenu(); // Return to the main menu
            return;
        }

        // Check for and remove "package:" prefix if present
        std::string prefix = "package:";
        if (packageName.find(prefix) == 0) {
            packageName.erase(0, prefix.length());
        }

        // Execute the uninstall command
        std::string uninstallCommand = "C:\\BOOTMOD\\BIN\\adb.exe shell \"pm uninstall \\\"" + packageName + "\\\"\"";
        system(uninstallCommand.c_str());
        system("pause");

        // Clear the input buffer before reading the next package name
        std::cin.clear();
        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
    }
}

// Function to Push files to the device from the PUSH folder
void PushFilesMenu() {
    system("cls");  // Clear the console screen
    std::cout << "================\n";
    std::cout << "Push Files Menu:\n";
    std::cout << "================\n";
    std::cout << "1. Push all files to sdcard/Download\n";
    std::cout << "2. Choose where to push to\n";
    std::cout << "3. Return to Main Menu\n";
    std::cout << "\nEnter your choice: ";

    int choice;
    std::cin >> choice;

    const std::string sourceFolder = "C:\\BOOTMOD\\PUSH";
    const std::string adbPath = "C:\\BOOTMOD\\BIN\\adb.exe";
    switch (choice) {
    case 1: {
        system("cls");  // Clear the console screen
        std::cout << "\nPushing all files to sdcard/Download...\n";

        if (std::filesystem::exists(sourceFolder) && !std::filesystem::is_empty(sourceFolder)) {
            for (const auto& file : std::filesystem::directory_iterator(sourceFolder)) {
                std::string filePath = file.path().string();
                std::string fileName = file.path().filename().string();
                std::string pushCommand = adbPath + " push \"" + filePath + "\" \"/sdcard/Download/" + fileName + "\""; // Corrected path
                if (system(pushCommand.c_str()) != 0) {
                    std::cout << "Failed to push " << fileName << std::endl;
                }
                else {
                    std::cout << "Pushed " << fileName << " successfully to /sdcard/Download\n";
                }
            }
        }
        else {
            std::cout << "No files to push in " << sourceFolder << std::endl;
        }
        break;
    }
    case 2: {
        system("cls");  // Clear the console screen
        std::cout << "\nPlease type in the full path of the directory you want to push to (e.g., sdcard/Documents): ";
        std::string targetPath;
        std::cin.ignore(); // Ignore any newline in the input buffer
        std::getline(std::cin, targetPath); // Use getline to allow spaces in the path

        if (std::filesystem::exists(sourceFolder) && !std::filesystem::is_empty(sourceFolder)) {
            for (const auto& file : std::filesystem::directory_iterator(sourceFolder)) {
                std::string filePath = file.path().string();
                std::string fileName = file.path().filename().string();
                std::string pushCommand = adbPath + " push \"" + filePath + "\" \"" + targetPath + "/" + fileName + "\""; // Corrected path
                if (system(pushCommand.c_str()) != 0) {
                    std::cout << "Failed to push " << fileName << " to " << targetPath << std::endl;
                }
                else {
                    std::cout << "Pushed " << fileName << " successfully to " << targetPath << "\n";
                }
            }
        }
        else {
            std::cout << "No files to push in " << sourceFolder << std::endl;
        }
        break;
    }
    case 3: {
        std::cout << "\nReturning to Main Menu...\n";
        ShowMenu();
        break;
    }
    default: {
        std::cout << "\nInvalid choice. Please try again.\n";
        PushFilesMenu(); // Recursive call to handle invalid choices
        break;
    }
    }

    system("pause"); // Wait for user acknowledgment
    system("cls");   // Clear the console screen
    ShowMenu();      // Return to the main menu
}

// Function to pull all files from the specified location on the device to C:\BOOTMOD\PULL
void PullFilesMenu() {
    system("cls");  // Clear the console screen
    std::cout << "================\n";
    std::cout << "Pull Files Menu:\n";
    std::cout << "================\n";
    std::cout << "1. Pull all files from sdcard/Download\n";
    std::cout << "2. Choose where to pull from\n";
    std::cout << "3. Return to Main Menu\n";
    std::cout << "\nEnter your choice: ";
    int choice;
    std::cin >> choice;
    const std::string destinationFolder = "C:\\BOOTMOD\\PULL";
    const std::string tempFile = "temp_file_list.txt";
    switch (choice) {
    case 1: {
        system("cls");  // Clear the console screen
        std::cout << "\nPulling all files from sdcard/Download...\n";

        // List all files in the sdcard/Download directory to a temporary file
        std::string listCommand = "C:\\BOOTMOD\\BIN\\adb.exe shell ls \"sdcard/Download\" > " + tempFile;
        system(listCommand.c_str());

        // Read the temporary file to get the list of files
        std::ifstream fileList(tempFile);
        std::string fileName;
        while (std::getline(fileList, fileName)) {
            // Remove any newline or carriage return characters
            fileName.erase(std::remove(fileName.begin(), fileName.end(), '\r'), fileName.end());
            fileName.erase(std::remove(fileName.begin(), fileName.end(), '\n'), fileName.end());

            if (!fileName.empty()) {
                // Pull each file individually
                std::string pullCommand = "C:\\BOOTMOD\\BIN\\adb.exe pull \"sdcard/Download/" + fileName + "\" \"" + destinationFolder + "\"";
                system(pullCommand.c_str());
            }
        }
        fileList.close();

        
        std::remove(tempFile.c_str()); 
        std::cout << "\nAll files have been pulled from sdcard/Download to " << destinationFolder << std::endl;
        break;
    }
    case 2: {
        system("cls");  // Clear the console screen
        std::cout << "\nPlease type in the full path of the directory you want to pull from (e.g., sdcard/Documents): ";
        std::string sourcePath;
        std::cin >> sourcePath; // Get directory path from the user

        // List all files in the user specified directory to a temporary file
        std::string listCommand = "C:\\BOOTMOD\\BIN\\adb.exe shell ls \"" + sourcePath + "\" > " + tempFile;
        system(listCommand.c_str());

        // Read the temporary file to get the list of files
        std::ifstream fileList(tempFile);
        std::string fileName;
        while (std::getline(fileList, fileName)) {
            // Remove any newline or carriage return characters
            fileName.erase(std::remove(fileName.begin(), fileName.end(), '\r'), fileName.end());
            fileName.erase(std::remove(fileName.begin(), fileName.end(), '\n'), fileName.end());

            if (!fileName.empty()) {
                // Pull each file individually
                std::string pullCommand = "C:\\BOOTMOD\\BIN\\adb.exe pull \"" + sourcePath + "/" + fileName + "\" \"" + destinationFolder + "\"";
                system(pullCommand.c_str());
            }
        }
        fileList.close();
        std::remove(tempFile.c_str()); // Delete the temporary file
        std::cout << "\nFiles have been pulled from " << sourcePath << " to " << destinationFolder << std::endl;
        break;
    }
    case 3: {
        std::cout << "\nReturning to Main Menu...\n";
        ShowMenu(); 
        break;
    }
    default: {
        std::cout << "\nInvalid choice. Please try again.\n";
        break;
    }
    }

    system("pause"); // Wait for user acknowledgment
    system("cls"); // Clear the console screen
    ShowMenu();
}

// Function to open an ADB shell in a new command prompt window
void OpenAdbShell() {
    system("cls");  // Clear the console screen
    std::cout << "Opening ADB Shell in a new window...\n\n";

    // Command to open a new command prompt window, change to the C:\BOOTMOD\BIN directory, and run adb shell
    std::string command = "start cmd.exe /K \"cd /D C:\\BOOTMOD\\BIN && adb shell\"";
    system(command.c_str());

    std::cout << "ADB Shell is now open in a new window.\n\n";
    system("pause"); // Wait for user input
    ShowMenu(); // Call the ShowMenu function to return to the main menu
}

// Function to display the credits
void Credits() {
    system("cls");  // Clear the console screen
    std::cout << "Special thanks to the following XDA community members for their contributions:\n\n";
    std::cout << "Thank you to @topjohnwu for making Magisk\n\n";
    std::cout << "@nooted1 for teaching us about the magisk variant \"delta\" that has magiskhide still configured inside the apk\n\n";
    std::cout << "@ajolly for their efforts in educating us about safetynet modules that can be used within magisk\n\n";
    std::cout << "@louforgiveno for their efforts in reverse engineering apks, determining that it would be better to clear data in the google playstore instead of cache, and providing excellent feedback on pretty much every step of the way.\n\n";
    std::cout << "@abc1054 for testing the ai upscaler and teaching me how to even use the silly thing.\n\n";
    std::cout << "@Zer0_rulz for testing the upscaler and teaching us about link2sd and providing a useful idea for studies, to \"freeze apps\" as opposed to straight deletion in tests. I will use both methods in the future!\n\n";
    std::cout << "@pinvok3 for their script they made to teach us how to more efficiently locate the apps tied to the ai upscaler and determining the \"tvsettings.apk\" to potentially be culpable in jailing our upscaler. They also taught me about the dolby vision feature on the shield\n\n";
    std::cout << "@Renate for allowing me to bother her in the AVB thread while I try to learn how to talk to people like her. haha\n\n";
    std::cout << "@Manzing for stepping up and being the hero we needed for the 2017 shield community! They were able to locate the correct pathing for the OTA Firmware as well as provide us the stock 9.1 boot and complete OTA!!\n\n";
    std::cout << "@I_did_it_just_tmrrow For taking the time to verify magisk is able to indeed patch the 2015 version of the Shield HERE and more so they explained the boot images provided on gameworks for the 2015 version have the same hash, therefore a patched boot will work on both the 16gb and 500gb models. Thank you!\n\n";
    std::cout << "The following Repositories were used in the creation of this tool:\n\n";
    std::cout << "https://github.com/curl/curl File used to download Magisk\n";
	std::cout << "https://github.com/jqlang/jq JSON processor used to help dl Magisk\n";
    std::cout << "https://developer.android.com/studio for adb and fastboot\n";
    std::cout << "https://twrp.me/ for TWRP, thanks so much to TeamWIN\n\n";
    std::cout << "Developed by Jenneh @XDA\n\n";
    system("pause"); // Wait for user input
    ShowMenu(); // Call the ShowMenu function to return to the main menu
}

// Function to download the latest Magisk 
void DownloadMagisk() {
    system("cls"); // Clear the console screen

    std::cout << "Do you want to download and install Magisk? (y/n): ";
    char userInput;
    std::cin >> userInput;

    if (userInput == 'y' || userInput == 'Y') {
        std::cout << "\nDownloading the latest Magisk...\n";

        // Change the working directory
        _chdir("C:\\BOOTMOD\\BIN");

        // Command to download the latest Magisk APK
        std::string command =
            "for /f \"delims=\" %i in ('curl -s https://api.github.com/repos/topjohnwu/Magisk/releases/latest ^| jq-win64.exe -r \".assets[] | select(.name | endswith(\\\"apk\\\")) | .browser_download_url\"') do curl -L -o \"magisk.apk\" \"%i\"";

        // Execute the command
        std::system(command.c_str());

        std::cout << "\nDownload complete.\n";
        std::cout << "Press any key to continue..\n";
        system("pause>nul"); // Pause for user verification
        system("cls"); // Clear the console screen
        InstallTWRP();
    }
    else {
        std::cout << "\nProceeding to TWRP installation.\n";
        InstallTWRP(); // Call the InstallTWRP function if user does not want to download Magisk
    }
}

// Function to help the user install TWRP
void InstallTWRP() {
    system("cls"); // Clear the console screen

    std::cout << "This process will direct you to the TWRP download page.\n\n";
    std::cout << "Please follow the instructions to download TWRP.\n\n";

    // Provide instructions for the user
    std::cout << "1. Open your web browser.\n\n";
    std::cout << "2. Go to the following URL: https://dl.twrp.me/mdarcy/twrp-3.7.0_9-0-mdarcy.img\n\n";
    std::cout << "(Press ctrl + c and click on the link to open it in a browser)\n\n";
    std::cout << "3. Click on the 'Download twrp-3.7.0_9-0-mdarcy.img' link.\n\n";
    std::cout << "4. When prompted, save the file to 'C:\\BOOTMOD\\BIN'.\n\n 8";
    std::cout << "5. Change the name of the file to 'twrp.img'.\n\n";

    std::cout << "After completing these steps, press any key to continue.\n";
    system("pause>nul"); // Pause for user verification
    system("cls"); // Clear the console screen

    ShowMenu();
}