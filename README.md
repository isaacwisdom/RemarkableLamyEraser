# RemarkableLamyEraser

Standalone tool that turns the button on the Lamy Pen into an eraser on the reMarkable.

This branch is updated to work with the reMarkable 2 version 3.5.x and above.
Further, ReMarkable 1 support has been removed.
If you object to this,
and are willing to maintain the respective functionality,
feel free to open a pull request.

# Installation

The tool will definitely break when the reMarkable updates.
When that happens, just reinstall!

``` console
$ sh -c "$(wget https://github.com/isaacwisdom/RemarkableLamyEraser/raw/main/scripts/LamyInstall.sh -O-)"
```

## Uninstall

``` console
 $ sh -c "$(wget https://github.com/isaacwisdom/RemarkableLamyEraser/raw/main/scripts/LamyUninstall.sh -O-)"
```

# Usage

The default configuration has the trigger "press and hold" mapped to the effect "erase",
and the trigger "double click" mapped to the effect "undo".
However, your configuration can be customised by changing the configuration file at
`~/.config/LamyEraser/LamyEraser.conf`
(for example, by running `nano ~/.config/LamyEraser/LamyEraser.conf`).
In this file, you can freely assign effects to different triggers.

The recognised triggers are:

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
  * long-click

Additionally, you can set the following flags (with no arguments);
  * assume-toolbar-open (*)

(*) NOTE: Since the release of reMarkable OS 3.9,
the UI has become less responsive to the the screen taps that this plugin simulates.
As a result, we have had to increase some of the delays when switching tools,
which unfortunately results in toolbar flickering and a slower response time.
To avoid this, you can also set the field `assume-toolbar-open`,
which will tell the program to not attempt to open and close the toolbar
(which it otherwise does to access the buttons and preserve the current toolbar state).
The downside of this, of course, is that you must always keep the toolbar open while using this tool.
Unfortunately, this is not a complete solution either and you may still find that some effects do not fully execute.

The "long click" trigger only activates if you press, hold, and release the button without writing.
Note that if you map this trigger, you will have to be careful about not accidentally activating it:
if you press the button intending to use the press&hold trigger, but later change you mind and release it,
this will send a long-click!
To abort in this situation, simply move your pen away from the screen before releasing the button.

Effects are divided into *tools* and *actions*;
actions are compatible with click-type actions,
while tools work with press-and-hold-type triggers,
or with click-type triggers as toggles.

The available actions are:

  * `toolbar`: Presses the toolbar panel button.
  * `writing`: Presses the writing utensil button.
  * `undo`: Presses the undo button.
  * `redo`: Presses the redo button.

The available tools are:

  * `eraser-erase`: Changes to eraser tool.
    On deactivation, changes back to writing utensil;
    note this mode uses special features available from the marker plus.
  * `select`: Changes to select tool.
    On deactivation, changes back to writing utensil
  * `one-off-eraser`/`one-off-eraser-selection`/`one-off-hl`: Changes to eraser/eraser select/highlighter tool,
    and back to writing tool after you're done erasing.
    Abort by activating again.

You can also toggle between the fineliner and the highlighter using writing-hl.

After making changes to the configuration file,
run this command to restart the program:

``` console
$ systemctl restart LamyEraser.service
```

You can also check your current config by running the program directly

``` console
$ systemctl stop LamyEraser.service
$ RemarkableLamyEraser
```

This will furthermore print some additional debugging information—great when opening an issue!

# Developing

There is [Dockerfile](./Dockerfile) available,
which builds the toolchain and comes equipped with `qtcreator`.
To build, run

``` console
$ docker build -t remarkable .
```

Running this is a bit awkward, as it requires teaching docker about X forwarding:

``` console
$ docker run --rm -ti --net=host --ipc=host -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix --env="QT_X11_NO_MITSHM=1" remarkable
```

# Incompatibilities

Support for the reMarkable 1 was removed from this branch;
see the `v2-3.0.0-fix` and `v1` branches for that.

If you object to this,
and are willing to maintain this functionality,
feel free to open a pull request.

# Other Styli

The project was—at one point in time—also confirmed to work with:

 * Kindle Scribe Pen
 * Samsung S6 S Pen
 * Wacom One Pen CP91300B2Z
 * iFlytec T1 4096

[ghub:isaac-wisdom]: https://github.com/isaacwisdom
[ghub:lamy:orig]: https://github.com/isaacwisdom/RemarkableLamyEraser
[ghub:lamy:orig:maintenance]: https://github.com/isaacwisdom/RemarkableLamyEraser/issues/70
