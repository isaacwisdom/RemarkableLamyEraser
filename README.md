*This is the beta branch to fix the changes introduced in the 3.0.0.0 software update. No guarantees on things working as expected!*
*If you notice any bugs, feel free to create an issue!*

# RemarkableLamyEraser
Standalone tool that turns the button on the Lamy Pen into an eraser on the reMarkable.

Also confirmed to work with these other styli:
 * Samsung S6 S Pen
 * Wacom One Pen CP91300B2Z

*As an alternative, consider using [this](https://github.com/ddvk/remarkable-stylus). (If you're already using ddvk-hacks, I'd defintely reccomend this route. This tool is for people who are looking for a less invasive option, and prefer the unaltered look of the reMarkable interface.)*

The tool will definitely break when the reMarkable updates. When that happens, just reinstall!
# Install Instructions
```shell
cd; wget https://github.com/isaacwisdom/RemarkableLamyEraser/raw/v2-3.0.0-fix/scripts/LamyInstall.sh; chmod +x LamyInstall.sh; ./LamyInstall.sh; rm ~/LamyInstall.sh
```
# Uninstall Instrucions
```shell
sh -c "$(wget https://github.com/isaacwisdom/RemarkableLamyEraser/raw/v2-3.0.0-fix/LamyUninstall.sh -O-)"
```



# Usage 
The default configuration has the trigger "press and hold" mapped to the effect "erase", and the trigger "double click"
mapped to the effect "undo".
However, you configuration can be customized by changing the configuration file at ~/.config/LamyEraser/LamyEraser.conf
(for example, by running nano ~/.config/LamyEraser/LamyEraser.conf).
In this file, you can freely assign effects to different triggers.  
  
The recognized triggers are:  
* click 
* double-click  
* triple-click
* quadruple-click  
* quintuple-click
* press&hold
* double-press&hold 
* triple-press&hold
* quadruple-press&hold
* quintuple-press&hold    
  
Effects are divided into tools and actions :  
Actions are compatible with click-type actions. The available actions are:  
*  toolbar : Presses the toolbar panel button  
*  writing : Presses the writing utensil button  
*  undo : Presses the undo button  
*  redo : Presses the redo button  
  
Tools are compatible with press and hold type triggers, or with click type triggers as toggles. The available tools are:  
*  eraser          : Changes to eraser tool. on deactivation, changes back to writing utensil  
                    note that on the RM2, this mode uses special features available from the marker plus.
*  select          : Changes to select tool. On deactivation, changes back to writing utensil  



After making changes to the config, run this command to restart the program:
``` Shell
systemctl restart LamyEraser.service
```

You can also check your current config by running the program directly 
``` Shell
systemctl stop LamyEraser.service
RemarkableLamyEraser
```

# TODO:
- [ ] toltec package
