# Easy-Build XBOX: Build Verification Testing Monitor (BVTMonitor)

***This tool is VERY experimental, you may encounter errors and/or bugs***

- [Home](#easy-build-xbox--build-verification-testing-monitor--bvtmonitor-)
  * [**What does it contain?**](#--what-does-it-contain---)
  * [**Essential Requirements**](#--essential-requirements--)
  * [**How To Use?**](#--how-to-use---)
    + [**How does it work?**](#--how-does-it-work---)
  * [**Notes:**](#--notes---)

<img src="https://github.com/Empyreal96/easy-build-xbox/raw/main/BVTmenu.png" alt="Menu" style="zoom: 67%;" />

BVTMonitor is a tool to simulate a BVT XBOX, to allow building and deploying a compiled Kernel and other files. This tool is aimed at people who are making code edits and want an almost automated process for deploying the files to the BVT.

I Will be adding features as time goes on, the first is adding a bit more functionality, and allowing xemu's IP to be exposed to the Build VM for debugging, better deployments and to be able to use more internal source tools


## **What does it contain?**

- BVTMonitor.cmd - This is the main script used to connect to Easy-Build.
- Tools\replace.vbs - A simple VBS Script that allows BVTMonitor to modify lines in 'XEMU.ini' to point to binplaced files.
  

## **Essential Requirements**

**NOTE**: This has **ONLY BEEN TESTED ON WINDOWS 10 X64**, if this works on older OSs, let me know!

- An empty folder to be used as a Network Share for Host and Build VM ([I recommend using the `.VHDX` file](https://github.com/Empyreal96/easy-build-xbox/raw/main/BVT/BVT_HDD.7z) or creating your own  `BVT VHD`).

- [Your preferred XEMU Emulator fork](https://github.com/mborgerson/xemu) extracted to the `BVT1_XEMU` folder.

- [MCPX ROM](https://github.com/mborgerson/xemu/wiki#mcpx-boot-rom-image) (As far as I know MCPX 1.1 Images not supported*).

- [Xbox Formatted HDD Image for XEMU](https://github.com/mborgerson/xemu/wiki#hard-disk-drive-image).

- BVTMonitor.cmd at the ROOT of the BVT Drive/Folder.

  *(Items with a* * *need conformation)*



## **How To Use?**

[**Please Visit the Wiki**]() for more details on using BVTMonitor, Tips on editing the Script and other info

- Setup the BVT Folder/Drive, and make sure it has Sharing Enabled and a Network Address assigned.
- Load BVTMonitor and follow onscreen instructions until you see `Waiting for Connection.`
- On the Build VM, Load Easy-Build and type `BVT` on the Main Menu.
- Input the name of the Network Share e.g: `\\tsclient\D` 
- The build will be automatic from here, the Tree will build, Kernel packed into ROM and copied to BVT Drive. XEMU will auto-start with the newly compiled BIOS/Kernel

### **How does it work?**

Easy-Build and BVTMonitor will create .txt files in `xboxbins\NEEDED_BY_BVTMONITOR ` at each stage of the build, coupled with some `if exist "xboxbins\NEEDED_BY_BVTMONITOR\StageText.txt "` in both to tell each script what part we are at.

Built files (xboxkrnl.exe, xboxbios.bin and eventually more) are placed into `xboxbins\release` by Easy-Build after building, then the bios is copied to `Bldr_files` and xemu.ini is backed up and configured.

Xemu will automatically open when it has all the files it needs and *hopefully* boot your modified Kernel! When you are done make sure to close BVTMonitor as this will place the just tested BIOS and kernel into `xboxbins\tested`and clean up the .txt files created throughout the BVT Build.

## **Notes:** 

- You will need a compatible build of XEMU that supports the Visor patches in the Complex Xbox Tree, otherwise it wont boot the 4400 Kernel.
- You will need to experiment with what version MCPX you use