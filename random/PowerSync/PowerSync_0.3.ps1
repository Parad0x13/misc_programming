# It should be noted that currently only single character delimiters will work due to
# how I can't figure out how to properly check delimiters with regex. Lots of '$' related issues
$DELIMITER_SECTION = "|"
$DELIMITER_SECTIONID = ":"
$DELIMITER_ARRAY = "~"

$BLOCKTAG_COMMENT = "#"
$BLOCKTAG_ACTION =  "A"

$GLOBAL_PATH = "I:\CSN\DOO\MDT 501\tmp\"
$GLOBAL_FILE = "input.txt"
$filePath = $GLOBAL_PATH + $GLOBAL_FILE

$PowerSyncMessage = "PowerSync V0.3"

function createSection($sectionTitle, $sectionData) {
    return $sectionTitle + $DELIMITER_SECTIONID + $sectionData + $DELIMITER_SECTION
}

function truncateDelimiters($data, $delimiter) {
    $retVal = $data

    # Assure string does not begin in delimiter
    if($retVal[0] -eq $delimiter) {
        $retVal = $retVal -replace "^."
    }

    # Assure string does not end in delimiter
    if($retVal[-1] -eq $delimiter) {
        $retVal = $retVal -replace ".$"
    }

    return $retVal
}

$STATE = @("A and B", "B and C")
function sendState {
    # [TODO] Pause update

    # [BEG] Generate Meta Section
    $full += createSection "Meta" $PowerSyncMessage
    # [END] Generate Meta Section

    # [BEG] Generate Data Section
    $arrayData = ""
    forEach ($STATEObject in $STATE) {
        $arrayData += $STATEObject + $DELIMITER_ARRAY
    }
    $arrayData = truncateDelimiters $arrayData $DELIMITER_ARRAY
    $full += createSection "Data" $arrayData
    # [END] Generate Data Section

    $full = truncateDelimiters $full $DELIMITER_SECTION
    Add-Content $filePath $full

    # [TODO] Resume Update
}

function processInput {
    $input = $args[0]
    Write-Host $input
}

function Register-Watcher($folder, $filter) {
    $watcher = New-Object IO.FileSystemWatcher $folder, $filter -Property @{
        IncludeSubdirectories = $false
        EnableRaisingEvents = $true
    }

    $changeAction = [scriptblock]::Create('
        $path = $Event.SourceEventArgs.FullPath
        $name = $Event.SourceEventArgs.Name
        $changeType = $Event.SourceEventArgs.ChangeType
        $timeStamp = $Event.TimeGenerated

        $input = Get-Content $filePath
        processInput $input
    ')

    Register-ObjectEvent $Watcher "Changed" -Action $changeAction
}

function run {
    if(Test-Path $filePath) {
        Clear-Content $filePath
    }

    Register-Watcher $GLOBAL_PATH $GLOBAL_FILE

    sendState
}

run
