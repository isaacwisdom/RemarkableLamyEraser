systemctl stop LamyEraser.service
systemctl disable LamyEraser.service
echo "Deleting RemarkableLamyEraser executable"
rm -f /usr/sbin/RemarkableLamyEraser
echo "Deleting service file..."
rm -f /lib/systemd/system/LamyEraser.service
systemctl daemon-reload
systemctl reset-failed
echo "RemarkableLamyEraser uninstalled."