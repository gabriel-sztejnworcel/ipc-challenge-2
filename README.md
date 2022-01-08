# PSM Securithon 2021
## Windows Interprocess Commnunication - Challenge #2

This application simulates a file server serving file contents from a predefined folder, with a basic access control mechanism. The application has a vulnerability and your goal is to find and exploit the vulnerability.

Steps and guildelines:
1. Download and build the source code
2. Try to understand the appliacation and the mechanisms it uses
3. Login to your target VM as an administrator and copy the executable and the script (init.ps1) to some folder
4. Run init.ps1, it will create a "files" subfolder with 2 files: user.txt and admin.txt. The file user.txt has read/write for the Users group, while admin.txt has read/write only for the Adminitrators group. The script will also create a non-admin user called challenge2
5. Build a client application that communicates with the file server, run it as user challenge2 (you can login with RDP as challenge2, or just run: "runas /user:challenge2 cmd"
6. Check that the client works, you should be able to get user.txt, but not admin.txt
7. Find the vulnerability and exploit it - your goal is to get the content of admin.txt while connected as challenge2
