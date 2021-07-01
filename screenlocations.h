#ifndef SCREENLOCATIONS_H
#define SCREENLOCATIONS_H

//portrait locations
#define toolX 63 //define one x coordinate for all panel tools
int panelTouch[2]   = {toolX, 1820};
int markerTouch[2]  = {toolX, 1890};
int eraserTouch[2]  = {toolX, 1450};
int undoTouch[2]    = {toolX, 1220};
int redoTouch[2]    = {toolX, 1090};

//landscape locations
#define toolY 60 //define one y coordinate for panel tools
int LpanelTouch[2]     = {  65, toolY};
int LnotebookTouch[2]  = {1355, toolY};
int LnotebookLower[2]  = {1250, 360};
int LnotebookUpper[2]  = {1130, 360};

#endif // SCREENLOCATIONS_H
