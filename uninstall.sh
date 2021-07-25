systemctl stop LamyEraser.service
systemctl disable LamyEraser.service
echo "Deleting RemarkableLamyEraser executable"
rm -f ~/RemarkableLamyEraser
echo "Deleting service file..."
rm -f /lib/systemd/system/LamyEraser.service
echo "Deleting Configuration folder..."
rm -rf ~/.config/LamyEraser
systemctl daemon-reload
systemctl reset-failed
echo "RemarkableLamyEraser uninstalled."
