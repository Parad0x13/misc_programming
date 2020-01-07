$DELIMITER_INDEX_START = "["
$DELIMITER_INDEX_END =   "]"
$DELIMITER_BLOCK_START = "{"
$DELIMITER_BLOCK_END =   "}"

$BLOCKTAG_COMMENT = "#"
$BLOCKTAG_ACTION =  "A"

$GLOBAL_PATH = "I:\CSN\DOO\MDT 501\tmp\"
$GLOBAL_FILE = "input.txt"
$filePath = $GLOBAL_PATH + $GLOBAL_FILE

$PowerSyncMessage = "PowerSync V0.2"

$global:currentIndex = -1

$STATE = @("A", "B");
function sendState {
    $STATE[0] = "C";
    Add-Content $filePath $STATE
}

function sendLine {
    $global:currentIndex++
    $index = [String]$global:currentIndex
    $indexString = $DELIMITER_INDEX_START + $index + $DELIMITER_INDEX_END
    $blockString = $DELIMITER_BLOCK_START + $args[0] + $DELIMITER_BLOCK_END
    $full = $indexString + $blockString + " " + $args[1]
    Add-Content $filePath $full
}

function processLine {
    $line = $args[0]
    Write-Host $line

    #$r = [regex] "\[([^\[]*)\]"
    #$r = [regex] "\[(.*?)\]";

    $r = [regex] ("\" + $DELIMITER_INDEX_START + "(.*?)\" + $DELIMITER_INDEX_END);
    $match = $r.match($line)
    $index = $match.groups[1].value

    $r = [regex] ("\" + $DELIMITER_BLOCK_START + "(.*?)\" + $DELIMITER_BLOCK_END);
    $match = $r.match($line)
    $block = $match.groups[1].value

    # [TODO] Okay so I need to grab the data after the Index and Block identifier and do something with it

    if([int]$index -gt $global:currentIndex) {
        # [WARNING] This might accidentally cause the index to jump forward if two additions to the file happen and one doesn't process right away
        $global:currentIndex = [int]$index
    }
}

function Register-Watcher {
    param ($folder, $filter)

    $watcher = New-Object IO.FileSystemWatcher $folder, $filter -Property @{
        IncludeSubdirectories = $false
        EnableRaisingEvents = $true
    }

    $changeAction = [scriptblock]::Create('
        $path = $Event.SourceEventArgs.FullPath
        $name = $Event.SourceEventArgs.Name
        $changeType = $Event.SourceEventArgs.ChangeType
        $timeStamp = $Event.TimeGenerated

        $line = Get-Content $filePath | Select-Object -Last 1
        processLine $line
    ')

    Register-ObjectEvent $Watcher "Changed" -Action $changeAction
}

function run {
    if(Test-Path $filePath) {
        Clear-Content $filePath
    }

    #sendLine $BLOCKTAG_COMMENT $PowerSyncMessage

    #Register-Watcher $GLOBAL_PATH $GLOBAL_FILE
    sendState;
}

run




#Add-Content $path $input

#function doStuff {
#    Write-Host $args[0]
#}

#doStuff "abc"

#Add-Type -typedef @"
#public class MyComputer
#{
#    public string thingy() {
#        return "bbb";
#    }
#}
#"@

#$computer = New-Object MyComputer
#Write-Host $computer.thingy()
