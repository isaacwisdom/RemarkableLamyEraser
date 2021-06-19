# RemarkableLamyEraser
Turns the button on the Lamy Pen into an eraser on the reMarkable.

# Install Instructions
SSH into your reMarkable and make a directory to store our files
```Shell
cd
mkdir RemarkableLamyEraser
```
Download the binary from the release page, and the .service file from the project page
```Shell
cd ~/RemarkableLamyEraser
wget https://github.com/isaacwisdom/RemarkableLamyEraser/releases/latest/download/RemarkableLamyEraser
wget https://github.com/isaacwisdom/RemarkableLamyEraser/raw/main/RemarkableLamyEraser/LamyEraser.service
```
Make the binary exectuable, copy the .service file to systemd file, enable, and start it. This means the tool will automatically start on boot.
```Shell
chmod +x RemarkableLamyEraser
cp LamyEraser.service /lib/systemd/system/
systemctl daemon-reload
systemctl enable LamyEraser.service
systemctl start LamyEraser.service
```
