# Easy-Build for XBOX Original



**What is it?**

A tool in testing for compiling xbox code. Easy to Update script.

- Buildable on Windows XP - 10 x86.

- Fixed issues with mstools/idw folders incorrectly being set.

- Basic Options for compiling.

- 'Unofficial' binplace scripts for built files, samples and tools.

- Buildable XDK Samples and other isos (needs testing in xemu/xqemu/xbox).

*More info at the Wiki:*
https://github.com/Empyreal96/easy-build-xbox/wiki


**How to use**
You will need access to the Xbox trunk and Team Complex's patch. (Xbox tree and CPXXUPD) you can grab the .VHD at the bottom of the page if you want a 'set up' environment

- Have easy-build-xinit.cmd on the ROOT of a drive e.g: D:\easy-build-xinit.cmd
- Open easy-build-xinit.cmd
- Setup the Xbox trunk in your Drive root as specified in easy-build-xinit (VHD users are already set up)
- Let the script apply Team Complex patches (These allow the xbox trunk to be built)
- Once that's done, follow onscreen instructions until Razzle loads
- Type into razzle: easybuild
- (To build chk type "easybuild chk" without quotes)
- You can now build from the Easy-Build mainmenu
- Built binaries will be in %DriveRoot%\xbox\xboxbuilds\fre\{dump}

*Any issues or suggestions please open an issue ticket so I can look into it*

BVT Machine ReadMe: https://github.com/Empyreal96/easy-build-xbox/blob/main/BVT/README.md

**What's Updated?**

```
- Added first test version of BVTMonitor, A tool to automatically Deploy your build Kernel to a Virtual BVT (XEMU) for testing.
- Added a premade 'skeleton' VHD for BVTMonitor.
- 2bl.img and remainder.img replaced with iND BIOS 5001 versions as this had better success in booting in XEMU when packed with our Kernel
```

(*Previous updated at bottom of ReadMe*)

**XDK Building**
**InstallShield Professional 6.2 is required for this**
I have included my currently in progress script to try and build the Xbox SDK. Currently it fails compiling the InstallShield specific XDK Setup scripts.. 

- To run the script, load Easy-Build, drop to Razzle prompt and type **"private\SDK\setup\xsdkbuild.cmd"**

You may encounter issues with **"language Independent Intel 32 Files"** during build, I haven't gotten round to looking at this so if anyone does make progress let me know!

**If you want to help the development of Easy-Build, have some issues join the Matrix chat!**

  https://matrix.to/#/!febkSwamiedCsfevDe:matrix.org?via=matrix.org

**For a VHD image with the SRC set up ready go here (Let me know if link dies):**

  https://mega.nz/file/DlQ1CChY#3C0kPSwfUTCRgFiLS2qYpN0Y8c5n6UMCvLf-u82w8sw

**NOTE** The version of Easy-Build in the VHD is the initial version, **UPDATE EASY-BUILD** simply by dropping the updated files here in place.


  **NOTE: Some Features may not work as expected, whether it be from Source or implemented options, either way please let me know** 

  ![Menu](https://github.com/Empyreal96/easy-build-xbox/raw/main/menu.png)



**Previous Updates**

```
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

