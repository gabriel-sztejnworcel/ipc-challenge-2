
# Create files
Remove-Item .\files\ -ErrorAction Ignore
New-Item files -ItemType Directory
"user data" | Out-File -FilePath .\files\user.txt -Encoding ascii
"admin data" | Out-File -FilePath .\files\admin.txt -Encoding ascii

# Set admin file permissions
$acl = Get-Acl
$AccessRule = New-Object System.Security.AccessControl.FileSystemAccessRule("BUILTIN\Administrators","FullControl","Allow")
$acl.SetAccessRule($AccessRule)
$acl | Set-Acl .\files\admin.txt

# Set user file permissions
$acl = Get-Acl
$AccessRule = New-Object System.Security.AccessControl.FileSystemAccessRule("BUILTIN\Users","FullControl","Allow")
$acl.SetAccessRule($AccessRule)
$acl | Set-Acl .\files\user.txt

# Create weak user to run the client
net user challenge2 /del
net user challenge2 User123456 /add
net localgroup "Remote Desktop Users" challenge2 /add
