// Globals.h
//

// Global Variables



extern unsigned char gBitMask1[8];
extern unsigned short gBitMask1_2[8];
extern unsigned short gBitMask2_2[8];

extern char *gMsgBuffer;
extern unsigned int gMsgSize;


// Command Line Global Variables
extern char gCoverPathFileName[MAX_PATH], *gCoverFileName;
extern char gMsgPathFileName[MAX_PATH], *gMsgFileName;
extern char gStegoPathFileName[MAX_PATH], *gStegoFileName;
extern char gInputPathFileName[MAX_PATH], *gInputFileName;
extern char gOutputPathFileName[MAX_PATH], *gOutputFileName;
extern char gAction;						// typically hide (1), extract (2), wipe (3), randomize (4), but also specifies custom actions for specific programs
extern char gPassword[MAX_PATH];
extern char gKeyPathFileName[MAX_PATH];
extern char gInputFileType;				// jpeg or bitmap
extern char gOutputFileType;			// jpeg (when hiding)


extern double gAlpha;
extern double gUniform;
extern unsigned int gImageQuality;
extern unsigned int gBitCapacity;

extern bool gExtractMsg;
extern bool gDestroyMsg;
extern bool gWipeMsg;
extern bool gHideMsg;
extern bool gDCT;
extern bool gConvert;

extern bool gGray;
extern bool gProgressive;
extern bool gVerbose;

extern double lg2; // lg2 = natural log 2
extern double lgQ; // lg2 of quality factor

// encoding/decoding objects
extern JpegEncoder gJpgEncoder;
extern JpegDecoder gJpgDecoder;
extern BmpEncoder gBitmapEncoder;
extern BmpDecoder gBitmapDecoder;
extern BitmapImage gImage;

// global variable for DCT histogram
extern short int gDCT_HistogramBefore[65536];
extern short int gDCT_HistogramAfter[65536];

