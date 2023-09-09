# RemarkableLamyEraser

Standalone tool that turns the button on the Lamy Pen into an eraser on the reMarkable.

This is a continuation of
[Isaac Wisdom's][ghub:isaac-wisdom]
[repository][ghub:lamy:orig]
(also see [here][ghub:lamy:orig:maintenance]),
updated to work with the reMarkable 2 version 3.5.x.

**NOTE:** This repository is in a very early stage.
Fully expect the executable to misbehave if
(i) you are not running in right handed portrait mode,
and (ii) you are editing a PDF instead of a notebook.
Working on it.

Further, the original repository was altered in the following major ways:

  - ReMarkable 1 support has been removed.

If you object to any of these,
and are willing to maintain the respective functionality,
feel free to open a pull request.

# Installation

The tool will definitely break when the reMarkable updates.
When that happens, just reinstall!

``` console
$ sh -c "$(wget https://raw.githubusercontent.com/slotThe/RemarkableLamyEraser/main/scripts/LamyInstall.sh -O-)"
```

## Uninstall

``` console
 $ sh -c "$(wget https://raw.githubusercontent.com/slotThe/RemarkableLamyEraser/main/scripts/LamyUninstall.sh -O-)"
```

# Usage

The default configuration has the trigger "press and hold" mapped to the effect "erase",
and the trigger "double click" mapped to the effect "undo".
However, your configuration can be customised by changing the configuration file at
`~/.config/LamyEraser/LamyEraser.conf`
(for example, by running `nano ~/.config/LamyEraser/LamyEraser.conf`).
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

Effects are divided into *tools* and *actions*;
actions are compatible with click-type actions,
while tools work with press-and-hold-type triggers,
or with click-type triggers as toggles.

The available actions are:

  * toolbar: Presses the toolbar panel button.
  * writing: Presses the writing utensil button.
  * undo: Presses the undo button.
  * redo: Presses the redo button.

The available tools are:

  * eraser-erase: Changes to eraser tool.
    On deactivation, changes back to writing utensil;
    note this mode uses special features available from the marker plus.
  * select: Changes to select tool.
    On deactivation, changes back to writing utensil

After making changes to the configuration file,
run this command to restart the program:

``` console
$ systemctl restart LamyEraser.service
```

You can also check your current config by running the program directly

``` Shell
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

# Other Styli

The original project was also confirmed to work with:

 * Kindle Scribe Pen
 * Samsung S6 S Pen
 * Wacom One Pen CP91300B2Z

[ghub:isaac-wisdom]: https://github.com/isaacwisdom
[ghub:lamy:orig]: https://github.com/isaacwisdom/RemarkableLamyEraser
[ghub:lamy:orig:maintenance]: https://github.com/isaacwisdom/RemarkableLamyEraser/issues/70
