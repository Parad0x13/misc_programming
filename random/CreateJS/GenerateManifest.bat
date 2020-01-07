@ECHO OFF

REM Manifest Creator v1.0 Bryan Tremblay 20May2016
REM Will iterate through all files and folders in the data folder and
REM create a full Manifest of all the files. So don't duplicate names

ECHO Creating Manifest
SETLOCAL EnableDelayedExpansion

SET TAB=	
SET DATA_FOLDER=data
SET BUILD_FOLDER=build
SET MANIFEST_FILE=!BUILD_FOLDER!/manifest.json

IF NOT EXIST !BUILD_FOLDER! mkdir !BUILD_FOLDER!

ECHO {>						!MANIFEST_FILE!
ECHO !TAB!"path": "/",>>	!MANIFEST_FILE!
ECHO !TAB!"manifest": [>>	!MANIFEST_FILE!

SET DATA_OBJECT_COUNT=0
FOR /R %DATA_FOLDER% %%F IN (*) DO (SET /A DATA_OBJECT_COUNT=!DATA_OBJECT_COUNT!+1)

SET CURRENT_COUNT=0
SET "r=%__CD__%"
FOR /R %DATA_FOLDER% %%F IN (*) DO (
	SET p=%%F

	SET relDirAndFile=!p:%r%=!
	REM We have to replace backslashes with forward slashes...
	REM Stupid stupid stupid syntax issue with the PreloadJS library
	SET relDirAndFile=!relDirAndFile:\=/!
	SET filenameOnly=%%~nF

	SET fullLine=!TAB!!TAB!{"id": "!filenameOnly!", "src": "!relDirAndFile!"}
	SET /A CURRENT_COUNT=!CURRENT_COUNT!+1
	IF NOT !CURRENT_COUNT!==!DATA_OBJECT_COUNT! (
		SET fullLine=!fullLine!,
	) ELSE (
		SET fullLine=!fullLine!
	)
	ECHO !fullLine!>> !MANIFEST_FILE!
)

ECHO !TAB!]>>	!MANIFEST_FILE!
ECHO }>>		!MANIFEST_FILE!

ECHO Manifest Created