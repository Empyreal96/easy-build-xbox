# Easy-Build for XBOX Original



**What is it?**

A tool in testing for compiling xbox code. Easy to Update script.

- Windows XP - 10 x86

- Fixed issues with mstools/idw folders incorrectly being set

- Basic Options for compiling

- 'Unofficial' binplace script 'xcopybins.cmd' to place some built files

**How to use**
You will need access to the Xbox trunk and Team Complex's patch.

- Have easy-build-xinit.cmd on the ROOT of a drive e.g: D:\easy-build-xinit.cmd
- Open easy-build-xinit.cmd
- Setup the Xbox trunk in your Drive root as specified in easy-build-xinit (VHD users are already set up)
- Let the script apply Team Complex patches (These allow the xbox trunk to be built)
- Once that's done, follow onscreen instructions until Razzle loads
- Type into razzle: easybuild
- You can now build from the Easy-Build mainmenu
- Built binaries will be in %DriveRoot%\xbox\xboxbuilds\fre\{dump}

*Any issues or suggestions please open an issue ticket so I can look into it*

**What's Updated?**

- Small update to some 'dirs' files that adds folders to the build process that can build successfully
- Separated easybuild.cmd (mainmenu) to public/tools/ 
- Added xcopybins.cmd (basic, needing love, has pauses to tell us what's going down)
- Actual easybuild.cmd shows correct Razzle Tool Path now, starts xcopybins.cmd as postbuild
  
**If you want to help the development of Easy-Build, have some issues join the Matrix chat!**

  https://matrix.to/#/!febkSwamiedCsfevDe:matrix.org?via=matrix.org
  
**For a VHD image with the SRC set up ready go here:**

  https://gofile.io/d/PZngu0

**NOTE** The version of Easy-Build in the VHD is the initial version, **UPDATE EASY-BUILD** simply by dropping the updated files here in place.


  **NOTE: This is the first release, It is lacking in many features that Easy-Build-NT5 has.. If I get time I will eventually figure out more of the build system**
  
  ![Menu](https://github.com/Empyreal96/easy-build-xbox/raw/main/menu.png)
