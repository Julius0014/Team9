// Globals.cpp
//
// this file contains the global variables
//

#include "Main.h"

unsigned char gBitMask1[8] = { 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff };
unsigned short gBitMask1_2[8] = { 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff };
unsigned short gBitMask2_2[8] = { 0xFFFE, 0xFFFC, 0xFFF8, 0xFFF0, 0xFFE0, 0xFFC0, 0xFF80, 0xFF00 };

char *gMsgBuffer = NULL;
unsigned int gMsgSize = 0;	// size of message as it is read

// Command Line Global Variables
char gCoverPathFileName[MAX_PATH], *gCoverFileName;
char gMsgPathFileName[MAX_PATH], *gMsgFileName;
char gStegoPathFileName[MAX_PATH], *gStegoFileName;
char gInputPathFileName[MAX_PATH], *gInputFileName;
char gOutputPathFileName[MAX_PATH], *gOutputFileName;
char gAction;						// typically hide (1), extract (2), wipe (3), randomize (4), but also specifies custom actions for specific programs
char gPassword[MAX_PATH];
char gKeyPathFileName[MAX_PATH];
char gInputFileType = 0;			// jpeg or bitmap
char gOutputFileType = 0;			// jpeg (when hiding)


double gAlpha = 2.0;			// jpenquan.h
double gUniform = 2.0;
unsigned int gImageQuality = 75;
unsigned int gBitCapacity = 0;

bool gExtractMsg = false;
bool gDestroyMsg = false;
bool gWipeMsg = false;
bool gHideMsg = false;
bool gDCT = false;
bool gConvert = false;

bool gGray = false;
bool gProgressive = false;
bool gVerbose = false;

double lg2;						// lg2 = natural log 2
double lgQ;						// lg2 of quality factor

// encoding/decoding objects
JpegEncoder gJpgEncoder;
JpegDecoder gJpgDecoder;
BmpEncoder gBitmapEncoder;
BmpDecoder gBitmapDecoder;
BitmapImage gImage;

// global variable holding DCT Histogram largest coefficients are around 2000
short int gDCT_HistogramBefore[65536];
short int gDCT_HistogramAfter[65536];

