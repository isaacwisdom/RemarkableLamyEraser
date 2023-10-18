#!/usr/bin/env bash

# Check whether we are actually SSH'd into a reMarkable
(uname -a | grep reMarkable) || (echo "Please SSH into your remarkable" && exit)

clear
cd ~/ || exit

systemctl stop LamyEraser.service
systemctl disable LamyEraser.service > /dev/null/

echo "Downloading RemarkableLamyEraser Executable..."
cd /usr/sbin || exit
rm -f RemarkableLamyEraser
wget https://github.com/slotThe/RemarkableLamyEraser/raw/main/RemarkableLamyEraser
echo "Adding execute privleges..."
chmod +x RemarkableLamyEraser
cd ~/ || exit

echo "Downloading configuration file.."
rm -f LamyEraser.conf
wget https://github.com/slotThe/RemarkableLamyEraser/raw/main/config/LamyEraser.conf

echo "Creating directory for configuration file..."
mkdir -p ~/.config/LamyEraser

if [ -f ~/.config/LamyEraser/LamyEraser.conf ]
 then
   echo "Previous .conf file found... preserving as LamyEraser.conf.old"
   # ask before perhaps overwriting legacy config
   mv -i ~/.config/LamyEraser/LamyEraser.conf ~/.config/LamyEraser/LamyEraser.conf.old
fi

echo "Placing configuration file... ~/.config/LamyEraser/LamyEraser.conf"
mv LamyEraser.conf ~/.config/LamyEraser/

read -p "Would you like to edit your configuration now? [y/n]" -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]
 then
   nano ~/.config/LamyEraser/LamyEraser.conf
fi

read -p "Would you like RemarkableLamyEraser to start automatically on boot? [y/n]" -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]
 then
  echo "Downloading service file..."
  rm -f LamyEraser.service
  wget https://github.com/slotThe/RemarkableLamyEraser/raw/main/config/LamyEraser.service
  echo "Placing service file in /lib/systemd/system/"
  mv LamyEraser.service /lib/systemd/system/
  echo "Setting up RemarkableLamyEraser to start on boot..."
  systemctl daemon-reload
  systemctl enable LamyEraser.service
  echo "Starting RemarkableLamyEraser. Enjoy!"
  systemctl start LamyEraser.service
else
  echo "You can start RemarkableLamyEraser at any time by running /home/root/RemarkableLamyEraser"
  echo "Enjoy!"
fi
