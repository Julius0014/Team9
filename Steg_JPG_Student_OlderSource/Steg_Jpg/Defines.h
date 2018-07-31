// Defines.h
//
// this file contains the static definitions
//

// DEFINES
#define VERSION 1.11

// 1.10 fixes bug in converting from bmp to jpg
//		requires actions be full words i.e. -hide vs. -h
//		fixes bug in sanity check of extracted file name - zero was counted as bad

#define SUCCESS 0
#define FAILURE 1
#define DONE 255

#define ACTION_HIDE 1
#define ACTION_EXTRACT 2
#define ACTION_WIPE_MSG 3
#define ACTION_RND_MSG 4

#define ACTION_CONVERT 14
#define ACTION_DCT 15

// PROTOTYPES
int initializeHiding(char *MessageFileName);
void hideInBlock(JpegEncoderCoefficientBlock data, JpegEncoderQuantizationTable &qt);
void extractFromBlock(JpegDecoderCoefficientBlock data, const JpegDecoderQuantizationTable &qt);
void writeMsg();
void getDCT_Histogram(JpegEncoderCoefficientBlock *data, short int DCT_Histogram[65536] );
void writeDCT_Histogram(char *filename, short int DCT_Histogram[65536] );

// require initializing
unsigned char getBitsFromBuffer(unsigned int numBits, unsigned char *inBuffer, unsigned int inBufferLength);
int putBitsInBuffer(unsigned int numBits, unsigned char bits, unsigned char *outBuffer, unsigned int outBufferLength);

int readBitmap(char *inputFile);
int readJpg(char *inputFile, bool v);
int writeBitmap(char *outputFile);
int writeJpg(char *outputFile, bool g, bool p);

// STRUCTURES
