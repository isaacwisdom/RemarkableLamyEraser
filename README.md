# RemarkableLamyEraser
Standalone tool that turns the button on the Lamy Pen into an eraser on the reMarkable.

Also confirmed to work with these other styli:
 * Samsung S6 S Pen

The tool will definitely break when the reMarkable updates. When that happens, just reinstall!
# Install Instructions
```shell
cd; wget https://github.com/isaacwisdom/RemarkableLamyEraser/raw/v2-dev/install.sh; chmod +x install.sh; ./install.sh`
```
# Uninstall Instrucions
```shell
cd; wget https://github.com/isaacwisdom/RemarkableLamyEraser/raw/v2-dev/uninstall.sh; chmod +x uninstall.sh; ./uninstall.sh`
```


# Usage 
The default configuration has the trigger "press and hold" mapped to the effect "erase", and the trigger "double click"
mapped to the effect "undo".
However, you configuration can be customized by changing the configuration file at ~/.config/LamyEraser/LamyEraser.conf
(for example, by running nano ~.config/LamyEraser/LamyEraser.conf).
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
*  writing : Presses the writing utensil button  
*  undo : Presses the undo button  
*  redo : Presses the redo button  
  
Tools are compatible with press and hold type triggers, or with click type triggers as toggles. The available tools are:  
*  eraser          : Changes to eraser tool. on deactivation, changes back to writing utensil  
                    note that on the RM2, this mode uses special features available from the marker plus.  
*  erase-selection : Changes to erase selection tool. On deactivation, changes back to writing utensil  
*  select          : Changes to select tool. On deactivation, changes back to writing utensil  



After making changes to the config, run these command to restart the program:
``` Shell
systemctl restart LamyEraser.service
```

# TODO:
- [ ] toltec package
