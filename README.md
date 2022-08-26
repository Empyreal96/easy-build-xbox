# **Easy-Build for XBOX Original**

## I plan to slowly start to revise this build environment, what will be the first changes?
- BVT Monitor will be removed to 'streamline' the script and complicate it less.
- (Hopefully, no promises) some specific code changes for the trunk will be applied.
- Removing requirements for the "Complex" patches
- Add documentation of what I currently know about the tree.

I have added a [Release](https://github.com/Empyreal96/easy-build-xbox/releases/tag/1.0.22.06.2021) to archive the current state of the project for if anyone wants to retain the BVT side of the script


![menu](https://github.com/Empyreal96/easy-build-xbox/raw/main/menu.png)

## **What is it?**

A tool in testing for compiling xbox code. Easy to Update script.

- Buildable on Windows 7 - 10 x86 and x64 (x64 in testing and will have compile errors)

- Fixed issues with mstools/idw folders incorrectly being set.

- Basic Options for compiling.

- 'Unofficial' binplace scripts for built files, samples and tools.

- Buildable XDK Samples and other isos (needs testing in xemu/xqemu/xbox).

- Xbox SDK Builder

- Buildable BIOS with Biospack (From the Barnabus repack)

  

### *Wikis:*

###  [Easy-Build Wiki](https://github.com/Empyreal96/easy-build-xbox/wiki)

### [BVTMonitor](https://github.com/Empyreal96/easy-build-xbox/wiki/Build-Verification-Testing-Monitor-Wiki)

### [**Easy-Build Devkit Menu**](https://github.com/Empyreal96/easy-build-xbox/wiki/Easy-Build-Developer-Kit-Menu)




## **Overview on how to use**
You will need access to the Xbox trunk and Team Complex's patch. (Xbox tree and CPXXUPD) you can grab the .VHD at the bottom of the page if you want a 'set up' environment

- Have easy-build-xinit.cmd on the ROOT of a drive e.g: D:\easy-build-xinit.cmd

- Open easy-build-xinit.cmd

- Setup the Xbox trunk in your Drive root as specified in easy-build-xinit (VHD users are already set up)

- Let the script apply Team Complex patches (These allow the xbox trunk to be built)

- Once that's done, follow onscreen instructions until Razzle loads

- Type into razzle: easybuild

- (To build chk type "easybuild chk" without quotes)

- You can now build from the Easy-Build mainmenu

- Built binaries will be in %DriveRoot%\xbox\xboxbuilds\\{fre\\chk}\dump

  
  
  
  
  **NOTE: Some Features may not work as expected, whether it be from Source or implemented options, either way please let me know. Also any suggestions to Features or just feedback is very welcome**

​       **BUILT BIOS WILL NOT BOOT ON 1.6(b) XBOXES, SEE EASY-BUILD WIKI FOR INFO** 

## If you want to use Easy-Build with a different Source Tree, read on [Modifying Easy-Build](https://github.com/Empyreal96/easy-build-xbox/wiki/Modifying-Easy-Build)



# **What's Updated?**

```
- Sadly Windows XP is currently NOT SUPPORTED by Easy-Build.. When I tested (on a XP x86 SP3 fresh install,) I recieved errors with the cl.exe and c1.dll. Not sure why maybe I needed to install a few updates so this needs looking at.

- Unified 'easy-build-xinit.cmd' for both x86 and amd64
- Added early 'config' support, Now Settings on 'First run', 'CPXXUPD done' and DevKit IPs are stored in an 'easybuild.conf' (In theory it will automatically update any left over files, there may be issues as its in testing)
- Updated info displayed in 'easy-build-xinit.cmd'
- (Main focus for update) Added a 'Developer Kit' menu, this will allow users to interact with their Debug kits with basic activities for now.
- Added 'Yelo Neighborhood v11' as it is a feature for the Devkit menu
- 'xbConsole.cmd' is a pop-out cmd for Easy-Build DevKit Menu
- Added Work aorund fixes for SDK not building when loaded in FRE

Developer Kit Features so far:
*NOTE* This menu is VERY WIP and you may have a few issues, I will work on any that occur after this initial release. You understand the risks when modifying files on the Xbox HDD!

- Transfer files to and from the console
- Update the console with the 4400 XDK Launcher (Requires XDK Launcher of any kind already set up with xbdm.dll)
- Shortcut to install the 4400 XDK
- Pop-out console to run Xbox CLI tools (e.g xbmemdump.exe)
- Reboot the console from Easy-Build
- Launch a debugging session thanks to XBDM.dll and xbWatson.exe
- Launch Yelo Neigborhood for Functions that provides 
```

(*Previous updated at bottom of ReadMe*)

# Third-Party Tools Used

I am in NO WAY AT ALL affiliated with any persons, companies or software used in this tool.

- Yelo Neighborhood 
  -  https://www.remnantmods.com/forums/viewtopic.php?f=5&t=1771 
  - https://code.google.com/archive/p/open-sauce/downloads (v10 version)
- Biospack - Barnabus Xbox 4400 Kernel Repack
  - https://vetusware.com/download/Microsoft%20Xbox%20Source%201.00.4400/?id=13484
- 4400 Source 
  - We all know who originally owned this, then Complex added their magic..



# Building on x64 Windows

**This will have errors in set parts of the tree, ideally we need to look into a cross compiler as `amd64` wasn't a thing during the time of the compiler**

- Currently Easy-Build can load razzle on an x64 version of Windows, the Kernel (`ntos`) is fully buildable on x64.
- Many things in the tree will be have Syntax errors, issues with inline x86 targeted code and functions causing compiler errors. *(Some DirectX code, MS CRT code and more)*
- Process for building is no different

**NOTES**: I have tried but I can't see an easy way (and a way I know) that will allow a full build on windows Win64 yet.



# **Virtual Build Verification Testing (BVT) Machine**

## Brief Overview

The Virtual BVT Consists of an XEMU fork of your choice (Ideally supporting visor hacked Bioses), BVTMonitor.cmd and any Boot files XEMU Required (Just not BIOS, BVTMonitor is used to test the Source Build Kernels).

It provides *(With a teeny bit of setup)* an almost fully automated process for connecting Easy-Build to BVTMonitor, building the source, and Binplacing the files on the BVT Drive and starting the BVT (XEMU) as soon as the required bios files have been placed!

## BVTMonitor's Capabilities 

- Detect when Easy-Build has Initiated the BVT Session.

- Monitor the BVT Drive for any files placed by Easy-Build.

- Automatically configure XEMU with source-placed files

- Start as soon as everything is placed and configured

- Can work without a VM, connects to a local Network Share 

  

### Read for more information on getting setup with BVT:

[**BVTMonitor Readme**](https://github.com/Empyreal96/easy-build-xbox/blob/main/BVT/README.md)

[**Go to the Wiki Page:**](https://github.com/Empyreal96/easy-build-xbox/wiki/Build-Verification-Testing-Monitor-Wiki)



# **XDK Building** (Main Method)

A small discovery I found was that MS decided to switch from the InstallShield style Setup to a custom build tool called `xpacker.exe`, This is the tool they use to pack the XDK, XBSE and other tools.

This build script is located at **`private\setup\xdk\xdkbuild.bat`**

It uses **`xdk.ini`,`xdk.csv`** and **`private\external\sdk`** to configure and build the Setup

- Choose **Build Xbox SDK** from Easy-Build's Main Menu or run `private\setup\xdk\xdkbuild.bat` from a Razzle window



# **XDK Building** (Old Deprecated Method)
**InstallShield Professional 6.2 is required for this**
I have included my currently in progress script to try and build the Xbox SDK. Currently it fails compiling the InstallShield specific XDK Setup scripts.. 

- To run the script, load Easy-Build, drop to Razzle prompt and type **`private\SDK\setup\xsdkbuild.cmd`**

You may encounter issues with **`language Independent Intel 32 Files`** during build, I haven't gotten round to looking at this so if anyone does make progress let me know!

# **Links**



**For a VHD image with the SRC set up ready go here (Let me know if link dies):**

  https://mega.nz/file/2s5kFBDK#xyg4VEcEvqXDSqofyIOCLDmkt6_dPmtnxhjXVGP_J7A

**NOTE** The version of Easy-Build in the VHD is the initial version, **UPDATE EASY-BUILD** simply by dropping the updated files here in place.

# **Previous Updates**

```
- Added 'easy-build-xinit64' which will allow easy-build to start on x64 builds
- Added folders to some 'dirs' files as they are buildable (Mainly test tools)
- Added starter info on modifying Easy-Build
- XDKSetup4400.exe is now Buildable! (Some files were missing from the tree and have been included from XDK 3823)
- Modified Wiki
- Fixed issue with overwriting a saved BVT Address
- Fixed a few issues which would cause errors during BVT Test
- Re-Readded Updated VHD, the issue is not the VHD, notes on BVTMonitor Readme
- Added first test version of BVTMonitor, A tool to automatically Deploy your build Kernel to a Virtual BVT (XEMU) for testing.
- Added a premade 'skeleton' VHD for BVTMonitor.
- 2bl.img and remainder.img replaced with iND BIOS 5001 versions as this had better success in booting in XEMU when packed with our Kernel
- Added idw\biospack to %PATH%.
- Replaced 'rombld' sections of ntos\init\*\makefile.inc with BIOSpack to allow bios Images to be created during build.
- Allowed seperate EEPROM and BIOS images to be created for CHK and FRE ('Postbuild only')
- Added more Information to easy-build-xinit
- Removed CHK builds dependancy on building to 'objd', now builds to 'obj' to avoid errors 
- Some FREE .libs are used as wevhave no suitable CHK versions that don't cause unresolved externals
- Made Easy-Build backup old Makefile.def before applying the CHK compatible one.
- Fixed incorrect error logs being opened on CHK
- Fixed 'bldrldr.lib' error finally
- Moved rebuilding 'ntos\bootx' to Rombld section , will now attempt to rebuild if files are missing
- Fixed spelling error in EEPROM menu
- Updated link for the .vhd as it was down
- Added BIOSPack from the Barnabus Kernel leak as it allows building of a *working* BIOS ROM
- Added EEPROM Building options
- Better definitions for building Retai (XM3)
- Written from scratch a new script for copying Debugging Symbols
- Adjusted Main menu with a sub-menu for building ISOs
- Added a 'First Run' check.. so after the initial welcome screen, you won't see it again
- Allowed 'RETAIMXM3' to be defined through easybuild 'easybuild free XM3'
- Added Directory to be built for the tool KeyDump
- Added a line to ntos\bootx\bldr32\sources that seems present in barnabus but not in 4400 (ours)
- Set %COMPUTERNAME% to XBuilds as it affects part of the postbuild and possibly build.
- Tweaked Rom build section to use source built version.
- Fixed an issue with xcopybins on Windows 10 x86 *I think* 
- Added 'timeout.exe' from Windows Server 2003 because some versions of XP don't have 'timeout' (It's used in easybuild to make timed pauses)
- Fixed wrong folder being built when dealing with xpreldr32.bin error
- Linked in-testing SDK Build script to the menu. (Advanced users, see *XDK Building* below)
- Added *VERY* in-testing of building bios rom (Advanced users, has issues described in the Easy-Build main menu)
- Added checks to see if CPXXUPD has already been applied on making new profile
- Added note on Easy-Build-xinit.cmd menu about loading CHK builds
- Build chk is now supported
- Postbuild scripts now have 'basic' logs in the %_NT386TREE% folder
- Added copying 'HVS Launcher Test' files (not sure what this is)
- Added two more buildable directories in "private\sdktools\factory"
- Added 'xmakesamples.cmd' which builds the XDK Sample CD.
- Added a WIP script to set off the XDK build *see XDK below*
- Fixed %_BUILDVER% not being set on razzle-easybuild handover
- Changed menu colours (I may change back depending on feedback)
- Small update to some 'dirs' files that adds folders to the build process that can build successfully
- Separated easybuild.cmd (mainmenu) to public/tools/ 
- Added xcopybins.cmd (basic, needing love, has pauses to tell us what's going down)
- Actual easybuild.cmd shows correct Razzle Tool Path now, starts xcopybins.cmd as postbuild
```

