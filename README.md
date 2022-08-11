# RemarkableLamyEraser
Standalone tool that turns the button on the Lamy Pen into an eraser on the reMarkable.

Also confirmed to work with these other styli:
 * Samsung S6 S Pen
 * Wacom One Pen CP91300B2Z

The tool will definitely break when the reMarkable updates. When that happens, just reinstall!
# Install Instructions
```shell
cd; wget https://github.com/isaacwisdom/RemarkableLamyEraser/raw/v1/install.sh; chmod +x install.sh; ./install.sh
```
# Uninstall Instrucions
```shell
cd; wget https://github.com/isaacwisdom/RemarkableLamyEraser/raw/v1/uninstall.sh; chmod +x uninstall.sh; ./uninstall.sh
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
- [ ] Nice install script
- [ ] toltec package
- [ ] config file (as opposed to current command line argument system)
- [ ] expand "How it works" section.
- [ ] flexible triggers (such as "click", "press and hold", "double click", "double click and hold", etc.)
- [ ] freely assignable actions (as listed below, able to assign to any trigger above) *(these last two will require
      some significant code restructuring)*

