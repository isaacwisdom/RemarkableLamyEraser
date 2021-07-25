#!/bin/sh
cd ~

echo "Downloading RemarkableLamyEraser Executable..."
wget https://github.com/isaacwisdom/RemarkableLamyEraser/raw/v2-dev/RemarkableLamyEraser
echo "Adding execute privleges..."
chmod +x RemarkableLamyEraser

echo "Downloading configuration file.."
wget https://github.com/isaacwisdom/RemarkableLamyEraser/raw/v2-dev/LamyEraser.conf

echo "Creating directory for configuration file..."
mkdir ~/.config/LamyEraser
echo "Placing configuration file... ~/.config/LamyEraser/LamyEraser.conf"
mv LamyEraser.conf ~/.config/LamyEraser/

read -p "Would you like to edit you configuration now? [y/n]" -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]
 then
   nano ~/.config/LamyEraser.conf
fi

read -p "Would you like RemarkableLamyEraser to start automatically on boot? [y/n]" -n 1 -r
if [[ $REPLY =~ ^[Yy]$ ]]
 then
  echo "Downloading service file..."
  wget https://github.com/isaacwisdom/RemarkableLamyEraser/raw/v2-dev/LamyEraser.service
  echo "Placing service file in /lib/systemd/system/"
  mv LamyEraser.service /lib/systemd/system/
  echo "Setting up RemarkableLamyEraser to start on boot..."
  systemctl daemon-reload
  systemctl enable LamyEraser.service
  echo "Starting RemarkableLamyEraser. Enjoy!"
  systemctl start LamyEraser.service
else
  echo "You can start RemarkableLamyEraser at any time by running /home/root/RemarkableLamyEraser

