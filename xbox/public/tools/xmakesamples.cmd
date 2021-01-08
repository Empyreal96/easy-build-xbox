@echo off
if not exist "%_NT386TREE%\xbld_pvt" mkdir "%_NT386TREE%\xbld_pvt"
set xbld_pvt=%_NT386TREE%\xbld_pvt
if "%xbld_pvt%" == "" goto usage
if "%_buildver%" == "" goto usage
if "%_ntbindir%" == "" goto usage

echo.
echo WARNING!!!  This script will destroy the current contents
echo of your target tree structure before building the new one.
echo.
rem pause

setlocal
set DEBUG=0
if %debug% == 1 echo debug is %debug%
if %DEBUG% == 1 (
set XCOPY_PARMS=/f /y /i /s
) ELSE (
set XCOPY_PARMS=/q /y /i /s
)

set CERTIFICATION_SAMPLES=techcertgame
set GRAPHICS_SAMPLES=alphafog AntiAlias backbufferscale beginpush benchmark billboard cartoon displacementmap dolphin dolphinclassic dolphinhdtv explosion fieldrender fire fog fresnelreflect fur fuzzyteapot matrixpaletteskinning minnaert mirrorclip modifypixelshader motionblur nosortalphablend notifier painteffect patch perftest perpixellighting perpixellightingvs persistdisplay pixelshader playfield pointsprites PolynomialTextureMaps projectedtexture pushbuffer shadowbuffer strip SwapCallback swizzle tiling trees TrueTypeFont twosidedlighting userclipplane vertexblend visibilitytest volumefog volumelight volumesprites volumetexture xprviewer xray zsprite
set GRAPHICS_BUMPMAPPING_SAMPLES=bumpdemo bumpearth bumplens dotproduct3
set GRAPHICS_ENVMAPPING_SAMPLES=CubeMap SphereMap
set GRAPHICS_STENCILBUFFER_SAMPLES=ShadowVolume StencilDepth StencilMirror
set GRAPHICS_VERTEXSHADER_SAMPLES=MultiShader Ripple ShaderSplicer StateShader VSLights
set GRAPHICS_TUTORIALS_SAMPLES=createdevice vertices matrices lights textures meshes vertexshaders
set GRAPHICS_SUBDIR_SAMPLES=%GRAPHICS_BUMPMAPPING_SAMPLES% %GRAPHICS_ENVMAPPING_SAMPLES% %GRAPHICS_STENCILBUFFER_SAMPLES% %GRAPHICS_TUTORIALS_SAMPLES% %GRAPHICS_VERTEXSHADER_SAMPLES%
set INPUT_SAMPLES=debugkeyboard Gamepad Rumble
set MISC_SAMPLES=AsyncWrite DebugChannel PerformanceCounters SectionLoad
set NETWORKING_SAMPLES=Auth ContentDownload Friends MatchMaking SimpleAuth SimpleContentDownload VoiceChat VoiceLoopBack WinsockPeer
set REFERENCEUI_SAMPLES=LoadSave VirtualKeyboard
set SOUND_SAMPLES=AudioStress BackgroundMusic Dm3DScript DmAudioPath DmGrooveLevel DmMultipass DmNotifications DmScript DmTool EnumSoundTrack EnvelopeGenerator FileStream FxMultipass GlobalFX ManualPanning MultiPass Play3Dsound SetFilter WaveBank WMAInMemory WMAStream
set VIDEO_SAMPLES=WMVCutScene

set ALL_SAMPLES=%CERTIFICATION_SAMPLES% %GRAPHICS_SAMPLES% %GRAPHICS_SUBDIR_SAMPLES% %INPUT_SAMPLES% %MISC_SAMPLES% %NETWORKING_SAMPLES% %REFERENCEUI_SAMPLES% %SOUND_SAMPLES% %VIDEO_SAMPLES%
if %DEBUG% == 1 echo all_samples == %all_samples%

set PDRIVE=%_BINPLACE_ROOT%
if %DEBUG% == 1 echo pdrive == %pdrive%
set SDRIVE=%_NTBINDIR%\private
if %DEBUG% == 1 echo sdrive == %sdrive%
if "%TARGET%" == "" set TARGET=%_BINPLACE_ROOT%\XDKSamples
if %DEBUG% == 1 echo target == %target%
if %DEBUG% == 1 pause

set TARGET=%pdrive%\XDKSamples
md %TARGET%
rem rd /s /q %TARGET%
md %TARGET%\media

rem -----------------------------------------------------
rem plop down the bits for the installer
rem -----------------------------------------------------

copy %COPY_PARMS% %PDRIVE%\fre\dump\installer.xbe %TARGET%\default.xbe
if errorlevel 1 goto error
xcopy %XCOPY_PARMS% %SDRIVE%\xbetools\installer\Media %TARGET%\media
if errorlevel 1 goto error

md %TARGET%\Files
md %TARGET%\Files\Samples

rem -----------------------------------------------------

set SAMPLE_SOURCE=cert
for %%i in (%CERTIFICATION_SAMPLES%) do (
echo Copying %SAMPLE_SOURCE% sample "%%i".
md %TARGET%\Files\Samples\%%i
if %DEBUG% == 1 echo ************************ i == %%i
xcopy %XCOPY_PARMS% %PDRIVE%\fre\dump\%%i.xbe %TARGET%\Files\Samples\%%i
if errorlevel 1 (
@echo The file %%i.xbe is missing
echo.
echo %%i.xbe is missing!
echo.
rem pause
)
if exist %SDRIVE%\ATG\Samples\%SAMPLE_SOURCE%\game\Media (
xcopy %XCOPY_PARMS% %SDRIVE%\ATG\Samples\%SAMPLE_SOURCE%\game\Media %TARGET%\Files\Samples\%%i\media
if exist %TARGET%\Files\Samples\%%i\media\Textures (
rd /q /s %TARGET%\Files\Samples\%%i\media\Textures
)
if exist %TARGET%\Files\Samples\%%i\media\Fonts (
rd /q /s %TARGET%\Files\Samples\%%i\media\Fonts
)
) ELSE (
echo No MEDIA tree found for sample "%%i"!
rem pause
)
if errorlevel 1 goto error
)

rem -----------------------------------------------------

set SAMPLE_SOURCE=graphics
for %%i in (%GRAPHICS_SAMPLES%) do (
echo Copying %SAMPLE_SOURCE% sample "%%i".
md %TARGET%\Files\Samples\%%i
if %DEBUG% == 1 echo ************************ i == %%i
xcopy %XCOPY_PARMS% %PDRIVE%\fre\dump\%%i.xbe %TARGET%\Files\Samples\%%i
if errorlevel 1 (
@echo The file %%i.xbe is missing
echo.
echo %%i.xbe is missing!
echo.
rem pause
)
if exist %SDRIVE%\ATG\Samples\%SAMPLE_SOURCE%\%%i\media (
xcopy %XCOPY_PARMS% %SDRIVE%\ATG\Samples\%SAMPLE_SOURCE%\%%i\media %TARGET%\Files\Samples\%%i\media
if exist %TARGET%\Files\Samples\%%i\media\Textures (
rd /q /s %TARGET%\Files\Samples\%%i\media\Textures
)
if exist %TARGET%\Files\Samples\%%i\media\Fonts (
rd /q /s %TARGET%\Files\Samples\%%i\media\Fonts
)
) ELSE (
echo No MEDIA tree found for sample "%%i"!
rem pause
)
if errorlevel 1 goto error
)

rem -----------------------------------------------------

set SAMPLE_SOURCE=input
for %%i in (%INPUT_SAMPLES%) do (
echo Copying %SAMPLE_SOURCE% sample "%%i".
md %TARGET%\Files\Samples\%%i
if %DEBUG% == 1 echo ************************ i == %%i
xcopy %XCOPY_PARMS% %PDRIVE%\fre\dump\%%i.xbe %TARGET%\Files\Samples\%%i
if errorlevel 1 (
@echo The file %%i.xbe is missing
echo.
echo %%i.xbe is missing!
echo.
rem pause
)
if exist %SDRIVE%\ATG\Samples\%SAMPLE_SOURCE%\%%i\media (
xcopy %XCOPY_PARMS% %SDRIVE%\ATG\Samples\%SAMPLE_SOURCE%\%%i\media %TARGET%\Files\Samples\%%i\media
if exist %TARGET%\Files\Samples\%%i\media\Textures (
rd /q /s %TARGET%\Files\Samples\%%i\media\Textures
)
if exist %TARGET%\Files\Samples\%%i\media\Fonts (
rd /q /s %TARGET%\Files\Samples\%%i\media\Fonts
)
) ELSE (
echo No MEDIA tree found for sample "%%i"!
rem pause
)
if errorlevel 1 goto error
)

rem -----------------------------------------------------

set SAMPLE_SOURCE=misc
for %%i in (%MISC_SAMPLES%) do (
echo Copying %SAMPLE_SOURCE% sample "%%i".
md %TARGET%\Files\Samples\%%i
if %DEBUG% == 1 echo ************************ i == %%i
xcopy %XCOPY_PARMS% %PDRIVE%\fre\dump\%%i.xbe %TARGET%\Files\Samples\%%i
if errorlevel 1 (
@echo The file %%i.xbe is missing
echo.
echo %%i.xbe is missing!
echo.
rem pause
)
if exist %SDRIVE%\ATG\Samples\%SAMPLE_SOURCE%\%%i\media (
xcopy %XCOPY_PARMS% %SDRIVE%\ATG\Samples\%SAMPLE_SOURCE%\%%i\media %TARGET%\Files\Samples\%%i\media
if exist %TARGET%\Files\Samples\%%i\media\Textures (
rd /q /s %TARGET%\Files\Samples\%%i\media\Textures
)
if exist %TARGET%\Files\Samples\%%i\media\Fonts (
rd /q /s %TARGET%\Files\Samples\%%i\media\Fonts
)
) ELSE (
echo No MEDIA tree found for sample "%%i"!
rem pause
)
if errorlevel 1 goto error
)

rem -----------------------------------------------------

set SAMPLE_SOURCE=networking
for %%i in (%NETWORKING_SAMPLES%) do (
echo Copying %SAMPLE_SOURCE% sample "%%i".
md %TARGET%\Files\Samples\%%i
if %DEBUG% == 1 echo ************************ i == %%i
xcopy %XCOPY_PARMS% %PDRIVE%\fre\dump\%%i.xbe %TARGET%\Files\Samples\%%i
if errorlevel 1 (
@echo The file %%i.xbe is missing
echo.
echo %%i.xbe is missing!
echo.
rem pause
)
if exist %SDRIVE%\ATG\Samples\%SAMPLE_SOURCE%\%%i\media (
xcopy %XCOPY_PARMS% %SDRIVE%\ATG\Samples\%SAMPLE_SOURCE%\%%i\media %TARGET%\Files\Samples\%%i\media
if exist %TARGET%\Files\Samples\%%i\media\Textures (
rd /q /s %TARGET%\Files\Samples\%%i\media\Textures
)
if exist %TARGET%\Files\Samples\%%i\media\Fonts (
rd /q /s %TARGET%\Files\Samples\%%i\media\Fonts
)
) ELSE (
echo No MEDIA tree found for sample "%%i"!
rem pause
)
if errorlevel 1 goto error
)

rem -----------------------------------------------------

set SAMPLE_SOURCE=refui
for %%i in (%REFERENCEUI_SAMPLES%) do (
echo Copying %SAMPLE_SOURCE% sample "%%i".
md %TARGET%\Files\Samples\%%i
if %DEBUG% == 1 echo ************************ i == %%i
xcopy %XCOPY_PARMS% %PDRIVE%\fre\dump\%%i.xbe %TARGET%\Files\Samples\%%i
if errorlevel 1 (
@echo The file %%i.xbe is missing
echo.
echo %%i.xbe is missing!
echo.
rem pause
)
if exist %SDRIVE%\ATG\Samples\%SAMPLE_SOURCE%\%%i\media (
xcopy %XCOPY_PARMS% %SDRIVE%\ATG\Samples\%SAMPLE_SOURCE%\%%i\media %TARGET%\Files\Samples\%%i\media
if exist %TARGET%\Files\Samples\%%i\media\Textures (
rd /q /s %TARGET%\Files\Samples\%%i\media\Textures
)
if exist %TARGET%\Files\Samples\%%i\media\Fonts (
rd /q /s %TARGET%\Files\Samples\%%i\media\Fonts
)
) ELSE (
echo No MEDIA tree found for sample "%%i"!
rem pause
)
if errorlevel 1 goto error
)

rem -----------------------------------------------------

set SAMPLE_SOURCE=sound
for %%i in (%SOUND_SAMPLES%) do (
echo Copying %SAMPLE_SOURCE% sample "%%i".
md %TARGET%\Files\Samples\%%i
if %DEBUG% == 1 echo ************************ i == %%i
xcopy %XCOPY_PARMS% %PDRIVE%\fre\dump\%%i.xbe %TARGET%\Files\Samples\%%i
if errorlevel 1 (
@echo The file %%i.xbe is missing
echo.
echo %%i.xbe is missing!
echo.
rem pause
)
if exist %SDRIVE%\ATG\Samples\%SAMPLE_SOURCE%\%%i\media (
xcopy %XCOPY_PARMS% %SDRIVE%\ATG\Samples\%SAMPLE_SOURCE%\%%i\media %TARGET%\Files\Samples\%%i\media
if exist %TARGET%\Files\Samples\%%i\media\Textures (
rd /q /s %TARGET%\Files\Samples\%%i\media\Textures
)
if exist %TARGET%\Files\Samples\%%i\media\Fonts (
rd /q /s %TARGET%\Files\Samples\%%i\media\Fonts
)
) ELSE (
echo No MEDIA tree found for sample "%%i"!
rem pause
)
if errorlevel 1 goto error
)

rem -----------------------------------------------------

set SAMPLE_SOURCE=video
for %%i in (%VIDEO_SAMPLES%) do (
echo Copying %SAMPLE_SOURCE% sample "%%i".
md %TARGET%\Files\Samples\%%i
if %DEBUG% == 1 echo ************************ i == %%i
xcopy %XCOPY_PARMS% %PDRIVE%\fre\dump\%%i.xbe %TARGET%\Files\Samples\%%i
if errorlevel 1 (
@echo The file %%i.xbe is missing
echo.
echo %%i.xbe is missing!
echo.
rem pause
)
if exist %SDRIVE%\ATG\Samples\%SAMPLE_SOURCE%\%%i\media (
xcopy %XCOPY_PARMS% %SDRIVE%\ATG\Samples\%SAMPLE_SOURCE%\%%i\media %TARGET%\Files\Samples\%%i\media
if exist %TARGET%\Files\Samples\%%i\media\Textures (
rd /q /s %TARGET%\Files\Samples\%%i\media\Textures
)
if exist %TARGET%\Files\Samples\%%i\media\Fonts (
rd /q /s %TARGET%\Files\Samples\%%i\media\Fonts
)
) ELSE (
echo No MEDIA tree found for sample "%%i"!
rem pause
)
if errorlevel 1 goto error
)

rem -----------------------------------------------------

set SAMPLE_SOURCE=bumpmapping
for %%i in (%GRAPHICS_BUMPMAPPING_SAMPLES%) do (
echo Copying %SAMPLE_SOURCE% sample "%%i".
md %TARGET%\Files\Samples\%%i
xcopy %XCOPY_PARMS% %PDRIVE%\fre\dump\%%i.xbe %TARGET%\Files\Samples\%%i
if errorlevel 1 (
@echo The file %%i.xbe is missing
echo.
echo %%i.xbe is missing!
echo.
rem pause
)
if exist %SDRIVE%\ATG\Samples\graphics\%SAMPLE_SOURCE%\%%i\media (
xcopy %XCOPY_PARMS% %SDRIVE%\ATG\Samples\graphics\%SAMPLE_SOURCE%\%%i\Media %TARGET%\Files\Samples\%%i\media
if exist %TARGET%\Files\Samples\%%i\media\Textures (
rd /q /s %TARGET%\Files\Samples\%%i\media\Textures
)
if exist %TARGET%\Files\Samples\%%i\media\Fonts (
rd /q /s %TARGET%\Files\Samples\%%i\media\Fonts
)
) ELSE (
echo No MEDIA tree found for sample "%%i"!
rem pause
)
if errorlevel 1 goto error
)

rem -----------------------------------------------------

set SAMPLE_SOURCE=envmapping
for %%i in (%GRAPHICS_ENVMAPPING_SAMPLES%) do (
echo Copying %SAMPLE_SOURCE% sample "%%i".
md %TARGET%\Files\Samples\%%i
xcopy %XCOPY_PARMS% %PDRIVE%\fre\dump\%%i.xbe %TARGET%\Files\Samples\%%i
if errorlevel 1 (
@echo The file %%i.xbe is missing
echo.
echo %%i.xbe is missing!
echo.
rem pause
)
if exist %SDRIVE%\ATG\Samples\graphics\%SAMPLE_SOURCE%\%%i\media (
xcopy %XCOPY_PARMS% %SDRIVE%\ATG\Samples\graphics\%SAMPLE_SOURCE%\%%i\Media %TARGET%\Files\Samples\%%i\media
if exist %TARGET%\Files\Samples\%%i\media\Textures (
rd /q /s %TARGET%\Files\Samples\%%i\media\Textures
)
if exist %TARGET%\Files\Samples\%%i\media\Fonts (
rd /q /s %TARGET%\Files\Samples\%%i\media\Fonts
)
) ELSE (
echo No MEDIA tree found for sample "%%i"!
rem pause
)
if errorlevel 1 goto error
)

rem -----------------------------------------------------

set SAMPLE_SOURCE=stencilbuffer
for %%i in (%GRAPHICS_STENCILBUFFER_SAMPLES%) do (
echo Copying %SAMPLE_SOURCE% sample "%%i".
md %TARGET%\Files\Samples\%%i
xcopy %XCOPY_PARMS% %PDRIVE%\fre\dump\%%i.xbe %TARGET%\Files\Samples\%%i
if errorlevel 1 (
@echo The file %%i.xbe is missing
echo.
echo %%i.xbe is missing!
echo.
rem pause
)
if exist %SDRIVE%\ATG\Samples\graphics\%SAMPLE_SOURCE%\%%i\media (
xcopy %XCOPY_PARMS% %SDRIVE%\ATG\Samples\graphics\%SAMPLE_SOURCE%\%%i\Media %TARGET%\Files\Samples\%%i\media
if exist %TARGET%\Files\Samples\%%i\media\Textures (
rd /q /s %TARGET%\Files\Samples\%%i\media\Textures
)
if exist %TARGET%\Files\Samples\%%i\media\Fonts (
rd /q /s %TARGET%\Files\Samples\%%i\media\Fonts
)
) ELSE (
echo No MEDIA tree found for sample "%%i"!
rem pause
)
if errorlevel 1 goto error
)

rem -----------------------------------------------------

set SAMPLE_SOURCE=vertexshader
for %%i in (%GRAPHICS_VERTEXSHADER_SAMPLES%) do (
echo Copying %SAMPLE_SOURCE% sample "%%i".
md %TARGET%\Files\Samples\%%i
xcopy %XCOPY_PARMS% %PDRIVE%\fre\dump\%%i.xbe %TARGET%\Files\Samples\%%i
if errorlevel 1 (
@echo The file %%i.xbe is missing
echo.
echo %%i.xbe is missing!
echo.
rem pause
)
if exist %SDRIVE%\ATG\Samples\graphics\%SAMPLE_SOURCE%\%%i\media (
xcopy %XCOPY_PARMS% %SDRIVE%\ATG\Samples\graphics\%SAMPLE_SOURCE%\%%i\Media %TARGET%\Files\Samples\%%i\media
if exist %TARGET%\Files\Samples\%%i\media\Textures (
rd /q /s %TARGET%\Files\Samples\%%i\media\Textures
)
if exist %TARGET%\Files\Samples\%%i\media\Fonts (
rd /q /s %TARGET%\Files\Samples\%%i\media\Fonts
)
) ELSE (
echo No MEDIA tree found for sample "%%i"!
rem pause
)
if errorlevel 1 goto error
)

rem -----------------------------------------------------

for %%i in (%GRAPHICS_TUTORIALS_SAMPLES%) do (
echo Copying %SAMPLE_SOURCE% sample "%%i".
md %TARGET%\Files\Samples\%%i
xcopy %XCOPY_PARMS% %PDRIVE%\fre\dump\%%i.xbe %TARGET%\Files\Samples\%%i
if errorlevel 1 (
@echo The file %%i.xbe is missing
echo.
echo %%i.xbe is missing!
echo.
rem pause
)
)
xcopy %XCOPY_PARMS% %SDRIVE%\ATG\Samples\graphics\tutorials\tut05_textures\Media %TARGET%\Files\Samples\textures\media
if errorlevel 1 goto error
xcopy %XCOPY_PARMS% %SDRIVE%\ATG\Samples\graphics\tutorials\tut06_meshes\Media %TARGET%\Files\Samples\meshes\media
if errorlevel 1 goto error
if exist %TARGET%\Files\Samples\meshes\media\Textures (
rd /q /s %TARGET%\Files\Samples\meshes\media\Textures
)
if exist %TARGET%\Files\Samples\meshes\media\Fonts (
rd /q /s %TARGET%\Files\Samples\meshes\media\Fonts
)
xcopy %XCOPY_PARMS% %SDRIVE%\ATG\Samples\graphics\tutorials\tut07_vertexshaders\Media %TARGET%\Files\Samples\vertexshaders\media
if errorlevel 1 goto error
if exist %TARGET%\Files\Samples\vertexshaders\media\Textures (
rd /q /s %TARGET%\Files\Samples\vertexshaders\media\Textures
)
if exist %TARGET%\Files\Samples\vertexshaders\media\Fonts (
rd /q /s %TARGET%\Files\Samples\vertexshaders\media\Fonts
)

rem -----------------------------------------------------
rem Make the Iso image
rem -----------------------------------------------------

cd /d %pdrive%
gdfimage %TARGET% %pdrive%\XDKSamples%_buildver%.iso
if not errorlevel 1 goto end
echo.
echo GDFIMAGE failed!!!
echo.
rem pause

goto end

:error

echo.
echo A sample failed to copy!!!  Aborting...
echo.

goto end

:usage

echo.
echo You must have XBLD_PVT, _BUILDVER, and _NTBINDIR set in your
echo environment before running this command.
echo.

:end
endlocal
