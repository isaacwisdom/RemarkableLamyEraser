#ifndef SCREENLOCATIONS_H
#define SCREENLOCATIONS_H

//erase modes
#define PRESS_MODE      1 //press and hold mode
#define TOGGLE_MODE     2 //toggle mode

//actions:
//buttons do not support press and holds, but support clicks.
#define UNDO                  1 //undo button
#define REDO                  2 //redo button

//Touch Screen Coordinates:
//Right Hand, Portrait locations
#define RHtoolX 60 //define one x coordinate for all panel tools
int RHPpanel[2]   = {RHtoolX, 1820};
int RHPmarker[2]  = {RHtoolX, 1680};
int RHPeraser[2]  = {RHtoolX, 1450};
int RHPselect[2]  = {RHtoolX, 1340};
int RHPundo[2]    = {RHtoolX, 1220};
int RHPredo[2]    = {RHtoolX, 1090};

//Right Hand, Landscape locations
#define RHtoolY 50 //define one y coordinate for panel tools
int RHLpanel[2]     = {  40,  RHtoolY};
int RHLmarker[2]    = {  80,  RHtoolY};
int RHLeraser[2]    = { 430,  RHtoolY};
int RHLselect[2]    = { 550,  RHtoolY};
int RHLundo[2]      = { 670,  RHtoolY};
int RHLredo[2]      = { 800,  RHtoolY};

//Left Hand, Portrait locations
#define LHtoolX 1360 //define one y coordinate for panel tools
int LHPpanel[2]     = {LHtoolX,  1820};
int LHPmarker[2]    = {LHtoolX,  1680};
int LHPeraser[2]    = {LHtoolX,  1450};
int LHPselect[2]    = {LHtoolX,  1340};
int LHPundo[2]      = {LHtoolX,  1220};
int LHPredo[2]      = {LHtoolX,  1090};

//Left Hand, Landscape locations
#define LHtoolY 1820 //define one y coordinate for panel tools
int LHLpanel[2]     = {  40,  LHtoolY};
int LHLmarker[2]    = {  80,  LHtoolY};
int LHLeraser[2]    = { 430,  LHtoolY};
int LHLselect[2]    = { 550,  LHtoolY};
int LHLundo[2]      = { 670,  LHtoolY};
int LHLredo[2]      = { 800,  LHtoolY};

//Pen Coordiantes: (currently not needed)

#endif // SCREENLOCATIONS_H
