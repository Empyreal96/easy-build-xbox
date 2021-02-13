
CMD 648/649/646 IDE Controllers Windows 2000 Drivers Installation Instructions
------------------------------------------------------------------------------

Table of Contents
-----------------

I) Windows 2000 Fresh Installation Instructions. 
II) Install Windows 2000 on Windows NT4.
III) Upgrade Windows 2000. 


I) Windows 2000 Fresh Installation Instructions. 
------------------------------------------------
Follow these instructions in this section if you are freshly installing 
Windows 2000.

You may start up Windows 2000 installation from CD. If your CD-ROM drive is
not bootable, your can start up with floppy diskettes.

1.  If you choose to install Windows 2000 from floppies, create four 
    Windows 2000 set up floppy diskettes from CD before installation.

2.  Power off the system and connect hard drives and CD-ROM drives
    to the CMD IDE controller. Insert CMD IDE controller to a PCI slot.

3.  Put the diskette #1 into drive A or put the CD into CD-ROM/DVD drive
    if you choose to boot up from CD. Turn on your computer system.  

4.  Press 'F1' key to enter IDE mode when IDE controller BIOS is scanning devices.
    Notes:   This step is required only when you are using a controller BIOS that
             default to RAID mode.

5.  Continue to insert floppy diskette #2, #3, #4 if you are installing
    from floppy diskettes.

6.  Wait while files are copied from floppy or CD in the text mode installation.
    Caution: Do not press F6 for third party SCSI or RAID driver installation
             during this time. CMD PCI0648 controller does not use SCSI or RAID
             driver under Windows 2000.

7.  Follow setup instructions to select your choices for partitions and file system.

8.  After setup examines your disks, it would copy files to Windows 2000 
    installation folders and restart the system. 
    Again, Press 'F1' key to enter IDE mode when IDE controller BIOS is scanning devices
    if you are using a controller BIOS that default to RAID mode.
    
    The setup program would continue to finish installation after restart.

9.  Wait until Windows 2000 finishes installing devices, regional settings, 
    networking settings, components, and final set of tasks, reboot the system.

10. Go to device manager and install CMD IDE controller driver for Windows 2000  by the 
    following steps

	a. Right click on my computer icon, select properties, left click on 'Hardware'
           tab, and then on 'Device Manager' button.

	b. Double click on 'IDE ATA/ATAPI controllers', then on 'Standard Dual Channel 
           PCI IDE controller', left click on 'Driver' tab, then on 'Update Driver' button
           to bring up 'Upgrade Device Driver Wizard.'

        c. Select 'search for a suitable driver for my device' and click 'next' when install
                  hardware drivers.

           Insert the floppy diskette with CMD driver.

           Select 'floppy disk drive' and click 'next' when locate driver files.

           At search results, the Driver Wizard either found the a:\win2k\cmdide.inf at the
           first time CMD driver is installed or found the c:\winnt\inf\oem0.inf if the cmdide.inf
           has been installed to the hard disk before.

           Continue to install and ignore the 'Digital signature not found' message.

           Click 'OK' to copy driver file from a:\win2k

   
    You must restart the computer for the CMD IDE driver to take effect.

    Notes: Make sure the transfer mode settings is "DMA if available" in the IDE channel properties
           under device manager to get maximal performance.



II) Install Windows 2000 on Windows NT4.0
-----------------------------------------
Follow these instructions in this section if you are upgrading from NT4 to 
Windows 2000.

1.  Boot up your system to the Windows NT 4.0 desktop.

2.  Insert Windows 2000 CD to the CD-ROM/DVD drive.

3.  Double click the SETUP icon in the CD root directory.

4.  Double click "install Windows 2000" and select "upgrade to Windows 2000",
    then follow the instructions prompted by the setup program to upgrade Windows 2000.

5.  After the setup program copies installation files to disk, the system would restart.

6.  Press 'F1' key to enter IDE mode when IDE controller BIOS is scanning devices.
    Notes:   This step is required only when you are using a controller BIOS that
             default to RAID mode.

7.  Wait while files are copied from CD in the text mode installation.
    Caution: Do not press F6 for third party SCSI or RAID driver installation
             during this time. CMD PCI0648 controller does not use SCSI or RAID
             driver under Windows 2000.

8.  Follow step 8,9,10 of section I) to finish upgrading and driver installation.
    
    Notes: a. Do not forget to press 'F1' key to enter IDE mode in step 8.
           b. install CMD IDE driver for Windows 2000 following instructions in step 10.



III) Upgrade Windows 2000.
--------------------------
Follow these instructions in this section if you are installing newer version of 
Windows 2000 to your current Windows 2000.

1.  Boot up your system to the Windows 2000 desktop.

2.  Insert the newer version of Windows 2000 CD to the CD-ROM/DVD drive.

3.  Double click the SETUP icon in the CD root directory.

4.  Click "install Windows 2000" and select "upgrade to Windows 2000", 
    then follow the instructions prompted by the setup program to upgrade Windows 2000.

5.  After the setup program copies installation files to disk, the system would restart.

6.  Press 'F1' key to enter IDE mode when IDE controller BIOS is scanning devices.
    
    Notes:   This step is required only when you are using a controller BIOS that
             default to RAID mode.

7.  Wait while files are copied from CD in the text mode installation.
    Caution: Do not press F6 for third party SCSI or RAID driver installation
             during this time. CMD PCI0648 controller does not use SCSI or RAID
             driver under Windows 2000.


8.  After setup examines your disks, it would copy files to Windows 2000 
    installation folders and restart the system. 
    Again, Press 'F1' key to enter IDE mode when IDE controller BIOS is scanning devices
    if you are using a controller BIOS that default to RAID mode.
    
    The setup program would continue to finish installation after restart.

9.  Wait until Windows 2000 finishes installing devices, regional settings, 
    networking settings, components, and final set of tasks, reboot the system.

    Notes:   No need to re-install CMD IDE driver again unless it did not exist before the
             upgrade.
