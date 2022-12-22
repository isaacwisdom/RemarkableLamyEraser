# RemarkableLamyEraser
Standalone tool that turns the button on the Lamy Pen into an eraser on the reMarkable.

Also confirmed to work with these other styli:
 * Samsung S6 S Pen
 * Wacom One Pen CP91300B2Z

The tool will definitely break when the reMarkable updates. When that happens, just reinstall!
# Install Instructions
```shell
sh -c "$(wget https://github.com/isaacwisdom/RemarkableLamyEraser/raw/v1/LamyInstall.sh -O-)"
```
# Uninstall Instrucions
```shell
sh -c "$(wget https://github.com/isaacwisdom/RemarkableLamyEraser/raw/v1/LamyUninstall.sh -O-)"
```


# Usage 
Press and hold to erase, release to use as a normal pen. Double click the button to undo. Note that at the moment, double pressing to undo only works for portrait orientation documents.

Further customization can be done by adding arguments to ExecStart line of the LamyEraser.service file. This can be opened with `nano /lib/systemd/system/LamyEraser.service`.
The supported arguments are:  
`--press`   Press and hold to erase, release to use as a normal pen. *This is the default behavior.*  
`--toggle`  Press the button to erase, press the button again to swtich back to a normal pen.  
`--double-press undo` Double click the button to undo. *This is the default behavior.*  
`--double-press redo` Double click the button to redo.  
`--left-handed` Use this option if you are using left handed mode.  
For example, this line would use the toggle mode and redo on a double click:  
`ExecStart=RemarkableLamyEraser --toggle --double-press redo`


To apply your config, run these commands:
``` Shell
systemctl stop LamyEraser.service
systemctl daemon-reload
systemctl start LamyEraser.service
```
# V2 (beta)
The V2 branch of this project offers a ton of customization options, such as being able to select other tools, and configure exactly how you'd like to trigger erasing, undoing, among other things. It also knows what orientation your document is in, meaning that it works for portrait and landscape documents, regardless of your handedness. If you'd like to give it a try, uninstall this version, and reinstall from the V2 branch.

However, all this customization comes at a slight cost. The way that I've implemented the extra customization also means that basic actions of erasing and undoing aren't quite as "snappy" as version 1. Keep in mind that V2 is still in beta, and some things might not be as reliable.

# How it works
When you press the button on the Lamy Pen, an input event with code BTN_TOOL_RUBBER is sent into dev/input/event1. Essentially, this tricks the reMarkable into
thinking you are using the eraser side of the Marker Plus.

# How to build

* Download the latest toolchain for your device from <https://remarkablewiki.com/devel/toolchain> (e.g. `codex-x86_64-cortexa9hf-neon-rm10x-toolchain-3.1.15.sh`)
* run that file to install the toolchain (e.g. `sudo sh codex-x86_64-cortexa9hf-neon-rm10x-toolchain-3.1.15.sh`)
* source the printed environment file (e.g. `source /opt/codex/rm11x/3.1.15/environment-setup-cortexa7hf-neon-remarkable-linux-gnueabi`)
* compile `main.c` using the `CC` environment variable (e.g. `$CC -O2 main.c`)
    * if there is an error like `no such file or directory`, copy the command and execute it directly instead of using `$CC`, e.g. `arm-remarkable-linux-gnueabi-gcc  -mfpu=neon -mfloat-abi=hard -mcpu=cortex-a7 --sysroot=/opt/codex/rm11x/3.1.15/sysroots/cortexa7hf-neon-remarkable-linux-gnueabi -O2 main.c`)

# TODO:
- [ ] RM1 support (testers needed)
- [x] Nice install script
- [ ] toltec package
- [x] config file (as opposed to current command line argument system) -''V2''
- [x] flexible triggers (such as "click", "press and hold", "double click", "double click and hold", etc.) -''V2''
- [x] freely assignable actions (as listed below, able to assign to any trigger above) -''V2''

