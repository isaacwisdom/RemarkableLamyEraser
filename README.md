# RemarkableLamyEraser
Standalone tool that turns the button on the Lamy Pen into an eraser on the reMarkable.

Also confirmed to work with these other styli:
 * Samsung S6 S Pen

The tool will definitely break when the reMarkable updates. When that happens, just reinstall!
# Install Instructions
SSH into your reMarkable and make a directory to store our files:
```Shell
cd
mkdir RemarkableLamyEraser
```
Download the binary from the release page, and the .service file from the project page:
```Shell
cd ~/RemarkableLamyEraser
wget https://github.com/isaacwisdom/RemarkableLamyEraser/releases/latest/download/RemarkableLamyEraser
wget https://github.com/isaacwisdom/RemarkableLamyEraser/raw/main/RemarkableLamyEraser/LamyEraser.service
```
Make the binary exectuable, copy the .service file to systemd file, enable, and start it. This means the tool will automatically start on boot:
```Shell
chmod +x RemarkableLamyEraser
cp LamyEraser.service /lib/systemd/system/
systemctl daemon-reload
systemctl enable LamyEraser.service
systemctl start LamyEraser.service
```

# Uninstall Instrucions
```Shell
systemctl stop LamyEraser.service
systemctl disable LamyEraser.service
rm -rf ~/RemarkableLamyEraser
rm /lib/systemd/system/LamyEraser.service
systemctl daemon-reload
systemctl reset-failed
```


# Usage 
Press and hold to erase, release to use as a normal pen. Double click the button to undo. Note that at the moment, double pressing to undo only works for portrait
documents in right handed orientation.

Further customization can be done by adding arguments to ExecStart line of the LamyEraser.service file. This can be opened with `nano ~/RemarkableLamyEraser/LamyEraser.service`.
The supported arguments are:  
`--press`   Press and hold to erase, release to use as a normal pen. *This is the default behavior.*  
`--toggle`  Press the button to erase, press the button again to swtich back to a normal pen.  
`--double-press undo` Double click the button to undo. *This is the default behavior.*  
`--double-press redo` Double click the button to redo.
`--force-RM1` Debug option that forces the code to use RM1 style erase events so I can test on mr RM2.
For example, this line would use the toggle mode and redo on a double click:  
`ExecStart=/home/root/RemarkableLamyEraser/RemarkableLamyEraser --toggle --double-press redo`


To apply your config, run these commands:
``` Shell
cd ~/RemarkableLamyEraser
cp LamyEraser.service /lib/systemd/system/
systemctl stop LamyEraser.service
systemctl daemon-reload
systemctl start LamyEraser.service
```

# How it works
When you press the button on the Lamy Pen, an input event with code BTN_TOOL_RUBBER is sent into dev/input/event1. Essentially, this tricks the reMarkable into
thinking you are using the eraser side of the Marker Plus.

# TODO:
- [ ] RM1 support (testers needed)
- [ ] Left handed and landscape support for actions
- [ ] Nice install script
- [ ] toltec package
- [ ] config file (as opposed to current command line argument system)
- [ ] expand "How it works" section.
- [ ] flexible triggers (such as "click", "press and hold", "double click", "double click and hold", etc.)
- [ ] freely assignable actions (as listed below, able to assign to any trigger above) *(these last two will require
      some significant code restructuring)*

# RM1 Branch
The RM1 branch is being used to test RM1 support. Currently, the RM1 code runs, **but not on an actual RM1 device.**
While the screens on the RM1 and the RM2 are physically the same size, they have different ranges of outputs. While the code
will run on an RM1, it will have unexpected effects until I can add a screenlocations.h file that is specific to the RM1's
screen.

Currently, the RM1-style erase doesn't always work. It's the same issue as #7, but more noticeable with the RM1-style erase.

The actions currently being implemented are:
- [X] undo
- [X] redo
- [ ] erase (needed for RM1 support, since it can't use the Marker Plus style rubber events)
- [ ] erase selection
- [ ] select
