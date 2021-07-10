#ifndef SCREENLOCATIONS_H
#define SCREENLOCATIONS_H

//Touch Screen Coordinates:
//Right Hand, Portrait locations
#define RHtoolX 60 //define one x coordinate for all panel tools
static const int RHPtoolbar[2]   = {RHtoolX, 1820};
static const int RHPwriting[2]  = {RHtoolX, 1680};
static const int RHPeraser[2]  = {RHtoolX, 1450};
static const int RHPselect[2]  = {RHtoolX, 1340};
static const int RHPundo[2]    = {RHtoolX, 1220};
static const int RHPredo[2]    = {RHtoolX, 1090};

//Right Hand, Landscape locations
#define RHtoolY 50 //define one y coordinate for panel tools
static const int RHLtoolbar[2]     = {  40,  RHtoolY};
static const int RHLwriting[2]    = {  80,  RHtoolY};
static const int RHLeraser[2]    = { 430,  RHtoolY};
static const int RHLselect[2]    = { 550,  RHtoolY};
static const int RHLundo[2]      = { 670,  RHtoolY};
static const int RHLredo[2]      = { 800,  RHtoolY};

//Left Hand, Portrait locations
#define LHtoolX 1360 //define one y coordinate for panel tools
static const int LHPtoolbar[2]     = {LHtoolX,  1820};
static const int LHPwriting[2]    = {LHtoolX,  1680};
static const int LHPeraser[2]    = {LHtoolX,  1450};
static const int LHPselect[2]    = {LHtoolX,  1340};
static const int LHPundo[2]      = {LHtoolX,  1220};
static const int LHPredo[2]      = {LHtoolX,  1090};

//Left Hand, Landscape locations
#define LHtoolY 1820 //define one y coordinate for panel tools
static const int LHLtoolbar[2]     = {  40,  LHtoolY};
static const int LHLwriting[2]    = {  80,  LHtoolY};
static const int LHLeraser[2]    = { 430,  LHtoolY};
static const int LHLselect[2]    = { 550,  LHtoolY};
static const int LHLundo[2]      = { 670,  LHtoolY};
static const int LHLredo[2]      = { 800,  LHtoolY};

//Pen Coordinates: (currently not needed)

#endif // SCREENLOCATIONS_H
