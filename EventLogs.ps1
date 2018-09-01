######################################################
######################################################
##                                                  ##
##  This powershell script gathers log information  ##
##  from the past 30 minutes of the selected event  ##
##  types and exports them into a CSV file.         ##
##                                                  ##
######################################################
######################################################



## Create a Folder to save the exported log events to

$savePath = "C:$env:HOMEPATH\Desktop\Logs"
MD "$savepath\" -EA SilentlyContinue


## Specify the start and end time to export

$my_start = (Get-Date).AddMinutes(-30)
$my_end = Get-Date


## Create list of event logs to pull from
## Use comments to add or a line to the list

$my_logs = @()
$my_logs += "Application"
$my_logs += "System"


## Create a list of event types to gather
## Use comments to add or a line to the list

$my_events = @()
$my_events += "Error"
#$my_events += "Information"
#$my_events += "FailureAudit"
#$my_events += "SuccessAudit"
$my_events += "Warning"

## Pull logs and export the CSV files

ForEach ($logname in $my_logs) {
    Get-EventLog -EntryType $my_events -After $my_start -Before $my_end -LogName $logname | Export-CSV "$savePath\$logname.csv" -NoTypeInformation
}

Write-Host "The logs have been exported to $savepath."
