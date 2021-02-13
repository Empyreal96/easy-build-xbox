         Applied Microsystems Corporation - Game Technology Division
                   DVD Emulation - Xbox Development Kit
                           
                               Release Notes
                               Version: 1.2.3

            Copyright 2000, 2001 Applied Microsystems Corporation
-------------------------------------------------------------------------------

This file describes supplemental information that will help you use this
release of the DVD Emulator for the Xbox Development Kit.

TABLE OF CONTENTS
-----------------
  1. Requirements
  2. Installation
  3. New Capabilities/Changes
  4. Known Defects/Issues 
  5. Support
  6. Feedback


1. REQUIREMENTS
---------------

      Intel Pentium III 500 MHz with 128 MB RAM
      Windows 2000 Professional Service Pack 1


2. INSTALLATION
---------------

2.1  New Installation

Please see DVDEmulation-Installation.doc for installing the DVD Emulation 
hardware for the first time.  This document is located in the 
DVDEmulation directory in the root of the Xbox SDK CD or in the 
DVDEmulation directory in the Xbox SDK installation directory after installing
the Xbox SDK.


2.2  Upgrading a Current Installation

Perform the following steps only if you have installed the DVD Emulation
hardware previously:

Step 1: Install the latest Xbox SDK from the Xbox SDK CD.
Step 2: Copy <Microsoft Xbox SDK>\DVDEmulation\Driver\AmcPci.sys to 
        \WINNT\system32\drivers.  Overwrite the old AmcPci.sys file located 
		there.
Step 3: Run FPGALoader.exe as outlined at the end of the 
        DVDEmulation-Installation document.  _SHUTDOWN_ (do not reboot) your
		computer when prompted.



3. NEW CAPABILITIES/CHANGES
------------------------------------

- Modified to Emulate for XSS builds 3812 and later
- Layer switching metrics provided in emulation drive configuration INI's
- Setup.exe installation program provided
- Sector & file access logging during emulation.  This feature can be enabled 
  by right clicking on the DVD Emulation panel and selecting "Enable Log".  Log
  results are placed in a file called DvdLog.txt in the bin directory.  
  DvdLog.txt will automatically be rotated if it reaches over 2 Megabytes in size
  or the ECP is enabled.  (DvdLog.txt becomes DvdLog.1.txt, DvdLog.1.txt becomes
  DvdLog.2.txt, etc. up to DvdLog.5.txt.)  Microsoft will provide a GUI for 
  interpreting the results of these log files.


4. KNOWN DEFECTS/ISSUES
-----------------------
  
- The IDE cable that shipped with XDK for the emulation hard drive was a
  ATA/33 cable.  Note that an ATA/66 cable will work without any problems with
  the emulation hard drive.

- The "Reboot" button in the Emulation Control Panel calls xbreboot.exe.
  Therefore, it will not work unless the target machine IP address has been
  stored via a previous call to xbreboot.exe, xbdir.exe, etc.

- Each time a new layout session is started, the placement of all security 
  placeholders will be randomized.  A future update will allow you to specify 
  the seed number used in generating the random placement of the placeholders.
  
- In the Disc Geography Editor window in the Disc Layout Tool, if you drag a 
  file off the window, release the mouse button, and then move the mouse back 
  to the window, the red line will move with the mouse, even though nothing
  is selected.  Right click to get out of this state.


5. SUPPORT
----------
All technical support issues are being handled by Microsoft.  Please send all
technical support inquires to xboxds@xbox.com.


6. FEEDBACK
-----------
If you have any comments or feedback on the DVD Emulator for Xbox, send email
to xdk@xbox.com.
