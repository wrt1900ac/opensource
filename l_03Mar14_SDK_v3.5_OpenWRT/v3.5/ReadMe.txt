Content
  Packages
  How to update from Belkin/Linksys UI


Packages
  mamba-openwrt-v3.5.tar.gz - this folder contains OpenWrt Development Environment including WRT1900AC source code
  mamba-openwrt-v3.5.bin.tar.gz - this folder contains the binary artifacts after building
  dl_v3.5.tar.gz - this folder contains all the Opensource packages needed to build the WRT1900AC OpenWrt image
  openwrt-armadaxp--jffs2-128k.img - this is a pre-build/runnable firmware image

How to update from Belkin/Linksys UI
  Login to WRT1900AC local UI
  Navigate to the Connectivity tab
  Select Manual firmware update
  Select image to load (e.g., openwrt-armadaxp--jffs2-128k.img)
  Select Update firmware
  After the firmware is updated, the unit will reboot, and the default ip address will be 192.168.200.1
  The default SSID's will be MAMBA_2G4, and MAMBA_5G2.
  The default username for OpwnWrt is 'root', and there is no default password set.  
  The unit will ask you to set a default password after you login to the UI.
  
How to ipdate from OpenWrt to Belkin/Linksys
  Login to the WRT1900AC web UI
  Select the 'System' Tab, and then 'Backup / Flash Firmware' tab
  In the 'Flash new firmware image' section click the 'choose file' button and select your firmware
  Click 'flash image'
