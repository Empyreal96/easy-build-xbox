// NOTE This is for August 2001 XDK and used to satisfy the XDK Build
// This doc is prior to the 4400 Complex Tree
// There was no documentation like this in the tree


August 2001 Preview Release  XDK


1. Summary

This release updates the target system to a beta version of the Xbox console,
the software on the Xbox console, and the Software Development Kit (SDK) on
your development system. To update:

    1.  Install the updated SDK on your development system per section 4. 
    2.  Set up your Xbox console per section 5. 
    3.  If necessary, establish network communication between the two 
        systems per section 6. 


2. XDK Contents

This update to the Xbox Development Kit (XDK) for the Xbox™ video game 
system from Microsoft consists of the following components:

    1.  Xbox SDK disc—contains the latest Xbox SDK and other tools to be 
        installed on your development system. 
    2.  Xbox console recovery disc—use the recovery disc to upgrade the 
    3.  Xbox console to the latest version of the Xbox System Software. 


3. Support

For support and to give us feedback, please contact us at:

xboxds@xbox.com — for hardware problems, XDK setup support, replacement 
    parts, and developer support. 

xdk@xbox.com — our XDK feedback alias. 

https://xds.xbox.com — the beta support Web site, which you should use for 
   other customer support issues, such as questions about shipping and billing 
   (see the Support section). You should also refer to the beta support Web site 
   for FAQs, the most up-to-date release notes, and white papers. You must log 
   in with your Microsoft beta program ID and password. If you need to obtain an 
   ID and password, contact us at xboxds@xbox.com. 



4.   Xbox Development System Setup

The development system is the desktop computer that you provide that 
will run the Microsoft® Visual C++® compiler to develop and compile 
Xbox executables, and is where the Xbox SDK must be installed.

If you have a prior release of the Xbox SDK installed on your 
development system, updating to the latest version of the Xbox SDK is 
the only step required on the development system for this release, so 
you can skip to section 4.2 below.

Otherwise, perform the following steps to prepare your development 
system for the Xbox SDK installation.

4.1   No Prior Xbox SDK Installed

If you do not have a prior release of the Xbox SDK installed, follow 
these instructions before installing the Xbox SDK:

    1.  Install Visual C++ 6.0 Professional. For installation 
        instructions, see the Visual C++ documentation. This step is not 
        necessary if a licensed copy of Visual C++ 6.0 is already 
        installed on the development system. 

    2.  Install Visual C++ 6.0 Service Pack 5. Service Pack 5 can be downloaded
        from the location given below.  To install, follow the instructions on
        the screen. It is not necessary to install Visual C++ Service Pack 4.

        Note   Service Pack 5 does not include an uninstall utility. 
        If you want to uninstall Service Pack 5, you must uninstall 
        and reinstall Visual C++ 6.0.

    3.  Install the final version of the Microsoft Visual C++ Processor 
        Pack from the Xbox SDK disc. To install, insert the disc into 
        your development system CD drive, select Install Visual C++ 
        Processor Pack from the installation screen, and follow the 
        instructions (the Processor Pack is located in the VCPP directory 
        on the disc).
        
        After you install Visual C++ 6.0 Service Pack 5, you must install the
        lastest Processor Pack, as the service pack overwrites changes made by
        any previously installed processor pack. Previous versions of the
        processor pack will not install on top of Service Pack 5.
        
    Note   If you have downloaded the XDK from the beta support Web 
    site, Visual C++ 6.0 Service Pack 5 and the final version of the 
    Visual C++ Processor Pack should be downloaded and installed from the
    following locations:

    Visual C++ 6.0 Service Pack 5: 
    http://msdn.microsoft.com/vstudio/sp/vs6sp5/vcfixes.asp

    Microsoft Visual C++ Processor Pack: 
    http://msdn.microsoft.com/vstudio/downloads/ppack/default.asp

4.2   SDK Installation

Before you can install the XDK software on Windows 2000, you must have 
permissions at the level of Administrator. Refer to the Windows help 
system for further information.

It is not necessary to uninstall a prior release of the Xbox SDK before 
installing this update. If you do not have a prior release of the Xbox 
SDK installed, first perform the steps in section 4.1 above.

Install the Xbox SDK from the Xbox SDK disc. To install, insert the 
disc into your development system CD drive, select Install Xbox SDK - August 2001 Preview Release 
from the installation screen, and follow the instructions.

    Note   When the installation program copies an updated file to the 
    Xbox SDK directory on your hard disk, it will overwrite the existing 
    file even if that file has been modified by you since the last Xbox 
    SDK install. For example, if you have made changes to the sample or 
    tutorial code installed with a previous Xbox SDK, but left those 
    modified files in the same location and with the same filename, the 
    update will overwrite those files with new versions and you will 
    lose any changes you might have made. Copy any modified files you 
    wish to retain to a new location prior to installing the Xbox SDK.


5.   Xbox Console Setup

The Xbox console runs the Xbox System Software and the game titles that 
you develop. Code written for the Xbox System Software must be run only 
on the Xbox console; do not run code written for Xbox on a development 
system or any system other than the Xbox console. The following steps 
will prepare your Xbox console for use.

5.1   Connect a TV to the Xbox Console

To create the most accurate run-time environment for testing Xbox 
games, it is recommended that a TV be used for the Xbox console 
monitor. Connect the TV using the supplied A/V pack. The console is by 
default set to support a standard NTSC TV. If you are using a PAL TV, 
you can change the console display settings by pressing the X 
controller button from the Title Launcher screen. In the settings 
screen that appears, you can change the type of television from NTSC to 
PAL. The display settings will not be updated until the console is 
rebooted.

Connecting a VGA monitor to the Xbox console is currently not 
supported.

    Note   If you have changed the display settings such that you can no 
    longer see video from the console, you can restore the settings in 
    either of two ways: 

        1.  While running recovery or the Title Launcher, use the following 
            key combinations to reset the video settings:  

            Left trigger + right trigger + D-pad right = NTSC-M (North America)  
            Left trigger + right trigger + D-pad up = PAL-I  
            Left trigger + right trigger + D-pad left = NTSC-J (Japan) 

        2.  Once you have established a network connection with the Xbox 
            console (see section 6 below), you can use the remote 
            configuration tool. Open a command window on the development 
            system and use the xbsetcfg command with the appropriate switch 
            to reset the display settings. For more information on the 
            command line switches, see the xbsetcfg topic in the Xbox SDK 
            help documentation. You will need to reboot the console after 
            running the xbsetcfg command before the display settings change 
            will take effect.

5.2   Connect the Controller

Connect the Xbox Controller to the console by inserting the plug into 
one of the four controller ports.

5.3   Attach Network Cable

The console may be connected to the development system in either of two 
ways:

    1.  Using a LAN connection. Attach one end of the provided network 
        cable to the Xbox console and the other end to the network hub.
    2.  Using a peer-to-peer connection. Connect a network cross-link 
        cable (not provided) between the Xbox console and the development 
        system.

    Note   Although a serial cable was provided with previous releases of the
    XDK, the Visual C++ debugger cannot communicate with the console via a
    serial connection; use the included network cable to connect the Xbox 
    console to your network hub.

5.4   Installing the Xbox System Software on the Xbox Console

This release of the XDK includes an Xbox console recovery disc that 
contains a new version of the Xbox System Software. Update your console 
by booting with this recovery disc. You will also use this same process 
if you need to restore the console hard disk due to corruption. 

To install or recover the Xbox System Software:

    1.  Power up the console and insert the console recovery disc into 
        the console disc drive. 

    2.  You will be prompted to press any controller button. 

    3.  After the console is updated, the disc drive tray will open and 
        you will be prompted to remove the disc and press any controller 
        button to reboot. 

    4.  The Title Launcher screen will be displayed in the Settings: 
        Machine Name mode. Enter a name for your console for network use. 
        The name may be up to 255 characters long; all characters on the 
        Xbox virtual keyboard are valid.  Select OK when you are 
        finished. 

WARNINGS:

    1.  Booting from the console recovery disc will erase the data on the 
        console hard disk. Copy any data that you wish to retain to 
        another system or storage device before booting from the recovery 
        disc.

    2.  Since the recovery process updates the ROM on the console, it is 
        critical that you do not power off or reboot the console during 
        the recovery process.

    Note   If you downloaded the XDK Update from the beta support Web 
    site (http://www.betaplace.com) rather than receiving an actual disc 
    from Microsoft, you must burn your own Xbox console recovery disc 
    image using the .iso file that is available for download from the 
    beta support Web site.


6.   Establish a Network Connection Between the Xbox Console and the 
Development System

If you are installing the Xbox SDK for the first time on this 
development system, or have changed the machine name of the Xbox 
console, you must set up the network connection between the console and 
the development system. After completing all the steps in Xbox 
Development System Setup and Xbox Console Setup, and with both systems 
running:

    1.  From the development system, click the Start button, click Run, 
        and type "cmd" in the Open: window that appears. 

    2.  From the C:\ prompt in the cmd.exe window, type "xbdir -x name 
        xe:\", where name is the name of your Xbox console entered during 
        the Installing the Updated Xbox System Software on the Xbox 
        Console step. In place of a name that you assign, you may enter 
        the IP address of the Xbox console (for example, 123.45.67.89). 
        The IP address is displayed at the bottom of the Title Launcher.

    Note   If the Xbox console and the development system are not on the 
    same subnet, you must enter the IP address instead of the Xbox 
    console name.

    Note   If steps 1 and 2 are not accomplished and you attempt to 
    build an Xbox executable, a No address for Xbox has been set 
    compiler error message will result.

    Note   You can use the "xbdir xe:\" command at any time to verify 
    that the two systems are communicating.


7.   To Uninstall the Xbox SDK

You do not need to perform this step unless you want to uninstall the 
Xbox SDK for some reason.

    1.  Launch the Add/Remove Programs dialog from the Windows Control 
        Panel, select Microsoft Xbox SDK, and then click Change/Remove. 

    2.  Setup will detect the previous installation of the Xbox SDK. 
        Follow the instructions on the screen. 

        Note   Xbox SDK uninstallation may not delete all files installed 
        with the Xbox SDK. To complete uninstallation in this case, manually 
        delete the Xbox SDK root folder (located at C:\Program 
        Files\Microsoft Xbox SDK, by default).

For uninstallation of Visual C++, see the Visual C++ documentation. You 
cannot uninstall Visual C++ Service Pack 4 or the Processor Pack 
separately.
