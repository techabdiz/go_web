@echo off
"C:\Program Files\Oracle\VirtualBox\VBoxManage.exe" startvm ubuntu-server --type headless
timeout 60
ssh deadspider@192.168.1.168