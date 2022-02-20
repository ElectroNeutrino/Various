# Set top level search folder


#$current_folders = "$HOME\Pictures", "$HOME\Videos" #"$HOME\Downloads\Other" "$HOME\Desktop", "$HOME\Downloads", "$HOME\Pictures", "$HOME\Videos"
$current_folders = "E:\"
$Save_log_folder = "$HOME\Desktop"


# Get all files in all subdirectories
Write-Host "Getting list of all files in directory(s) $current_folders"
$files = $null
ForEach ($folder in $current_folders)
{
    $files += Get-ChildItem -Path $folder -Recurse -File
}

$file_count = $files.Count

Write-Host "$file_count files loaded"

# Initialize variable
$duplicates = @()
$duplicate_names = @()


# Search files for 
Write-Host "Searching for duplicate file names."
ForEach ($file in $files)
{
    $dup = (Get-ChildItem -Path $current_folders -Recurse -File -Filter $file.Name)
    if ($dup.Count -gt 1)
    {
        if (-not ($duplicate_names -match $file.Name))
        {
            Write-Host "Found duplicate:" $file.Name
            $duplicate_names += $file.Name
        }

        ForEach ($dup_match in $dup)
        {
            if (-not ($duplicates -like "*" + $dup_match.FullName))
            {
                #Write-Host "`tAdding:" $dup_match.FullName
                $duplicates += $dup_match.FullName
            }
        }

    }
}

Write-Host "Duplicates found:`n"

ForEach($name in $duplicate_names)
{
    $matched = $duplicates -match $name
    $matched | Write-Host
    $matched | Out-File -FilePath $Save_log_folder\duplicates.txt -Append
    Write-Host "`n"
    "`n" | Out-File -FilePath $Save_log_folder\duplicates.txt -Append
}