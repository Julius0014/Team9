// Main.cpp
//
// This routine is used solely to pass arguments to either the encoder or decoder.
// It checks the input file type and makes a decision based on the extension
// if the extension is ".bmp" then call the encoder
// if the extension is ".jpg" then call the decoder

#include "Main.h"

using namespace std ;

int isValidJpeg(char *pathFileName)
{
	FILE *fptr;
	unsigned char buffer[8];

	fptr = fopen(pathFileName, "rb");
	if(fptr == NULL) 
	{
		fprintf(stderr, "Error - Could not access file <%s> to check jpeg validity\n\n", pathFileName);
		return FAILURE;
	}

	fread(buffer, 1, 8, fptr);
	fclose(fptr);

	if(*buffer != 0xFF || *(buffer + 1) != 0xD8 || *(buffer + 2) != 0xFF) return FAILURE;

	return SUCCESS;
} // isValidJpeg

int isValidBitmap(char *pathFileName)
{
	FILE *fptr;
	unsigned char buffer[8];

	fptr = fopen(pathFileName, "rb");
	if(fptr == NULL) 
	{
		fprintf(stderr, "Error - Could not access file <%s> to check Bitmap validity\n\n", pathFileName);
		return FAILURE;
	}

	fread(buffer, 1, 8, fptr);
	fclose(fptr);
	if(*buffer != 'B' || *(buffer + 1) != 'M') return FAILURE;

	return SUCCESS;
} // isValidBitmap

// prints help message to the screen
void Usage(char *programName)
{
	fprintf(stdout, "\n\n");
	fprintf(stdout, "To Hide Data Inside a Jpeg Image:\n");
	fprintf(stdout, "\t%s -hide -c <cover file> -m <msg file>  [-s <stego file>]\n", programName);
	fprintf(stdout, "\t  Parameters: [ -q ]  [ -a ]  [ -u ]\n\n");

	fprintf(stdout, "To Extract a Previously Hidden Image:\n");
	fprintf(stdout, "\t%s -extract -s <stego file> [-m <message file>]\n", programName);
	fprintf(stdout, "\t  Parameters: [ -q ]  [ -a ]  [ -u ]\n\n");

	fprintf(stdout, "To Wipe (zero) the Message Bits:\n");
	fprintf(stdout, "\t%s -wipe -s <stego file> [-o <output file>]\n", programName);
	fprintf(stdout, "\t  Parameters: [ -q ]  [ -a ]  [ -u ]\n\n");

	fprintf(stdout, "To Randomize the Message Bits:\n");
	fprintf(stdout, "\t%s -rnd -s <stego file> [-o <output file>]\n", programName);
	fprintf(stdout, "\t  Parameters: [ -q ]  [ -a ]  [ -u ]\n\n");

	fprintf(stdout, "To Convert from Jpeg to Bitmap or Bitmap to Jpeg:\n");
	fprintf(stdout, "\t%s -convert -i <input file> [-o <output file>]\n", programName);
	fprintf(stdout, "\t  Parameters: [ -q ] --> Bitmap to Jpeg Only\n\n");

	fprintf(stdout, "Parameters:\n");
	fprintf(stdout, "Set Jpeg Quality:			-q		( 10 to 100 def:75 )\n");
	fprintf(stdout, "Set Alpha Factor:			-a		( 0.10 to 9.99 def:2.00 )\n");
	fprintf(stdout, "Set Uniformity Threshold:	-u		( 0.10 to 9.99 def:2.00 )\n\n");

	fprintf(stdout, "Options:\n");
	fprintf(stdout, "For an 8-bit gray scale output file add: -g\n\n");
	fprintf(stdout, "For a progressive jpeg output file add:\t -p\n\n");
//	fprintf(stdout, "To collect DCT coefficients add:\t -dct\n\n");

	fprintf(stdout, "\n\tNOTES:\n\t1. Order of parameters is irrelevant.\n\t2. All selections in \"[]\" are optional.\n");
	fprintf(stdout, "\t3. Cover File may be a bitmap or a jpeg." 
		"\n\t4. When extracting data, the same -a and -u options MUST be chosen. (quality is automatic)\n\n");
	
	return;
} // Usage


DWORD getIndexOfFileNameExtension(char *fileName)
{
	DWORD x;

	x = (DWORD) strlen(fileName);
	while(x > 1)
	{
		if(fileName[x] == '.') return x;
		x--;
	}
	return 0;
} // getIndexOfFileNameExtension

void createOutputFileName(char *prefix, char *ext)
{
	DWORD index;
	char tmpFileName[MAX_PATH];
	FILE *fptr;
	int cnt;

	if(gOutputPathFileName[0] != 0) return;	// name already selected

	index = getIndexOfFileNameExtension(gInputFileName);
	if(index != 0) gInputFileName[index] = 0;	// cutoff extension for input file name

	cnt = 0;
	while(cnt < 1000)
	{
		if(gAction == ACTION_HIDE)
		{
			sprintf(tmpFileName, "hid_%s_a%3.2f_u%3.1f_%03d.jpg", gInputFileName, gAlpha, gUniform, cnt++);
		}
		else
		{
			sprintf(tmpFileName, "%s_%s_%03d.%s", prefix, gInputFileName, cnt++, ext);
		}

		GetFullPathName(tmpFileName, MAX_PATH, gOutputPathFileName, &gOutputFileName);

		// check if file exists
		fptr = fopen(gOutputPathFileName, "rb");
		if(fptr == NULL) break;
		fclose(fptr);
	}

	// realistically, this should never happen
	if(cnt == 1000)
	{
		fprintf(stderr, "Error - Could not create unused file name.\n\n");
		exit(-1);
	}

	if(index != 0) gInputFileName[index] = '.';	// restore extension for input file name
	return;
} // createOutputFileName

void readMsgFile()
{
	DWORD fileSize, bufferSize, msgFileNameLength, tmpD;
	FILE *fptr;

	fptr = fopen( gMsgPathFileName, "rb");
	if(fptr == NULL)
	{
		fprintf(stderr, "Error - Could not open message file <%s>\n\n", gMsgPathFileName);
		exit(-1);
	}


	fclose(fptr);

	return;
} // readMsgFile

// determines actions based on command line inputs
void parseCommandLine(int argc, char *argv[])
{
	int cnt, tmpInt;
	double tmpDbl;
	DWORD index;

	if(argc == 1)
	{
		Usage(argv[0]);
		exit(0);
	}

	// RESET Parameters to make error checking easier
	gAction = 0;
	gCoverPathFileName[0] = 0;
	gMsgPathFileName[0] = 0;
	gStegoPathFileName[0] = 0;
	gInputPathFileName[0] = 0;
	gOutputPathFileName[0] = 0;
	cnt = 1;
	while(cnt < argc)	// argv[0] = program name
	{
		if(stricmp(argv[cnt], "-c") == 0)	// cover file
		{
			cnt++;
			if(cnt == argc)
			{
				fprintf(stderr, "\n\nError - no file name following <%s> parameter.\n\n", argv[cnt-1]);
				exit(-1);
			}

			if(gCoverPathFileName[0] != 0)
			{
				fprintf(stderr, "\n\nError - cover file <%s> already specified.\n\n", gCoverPathFileName);
				exit(-2);
			}
			GetFullPathName(argv[cnt], MAX_PATH, gCoverPathFileName, &gCoverFileName);
		}
		else if(stricmp(argv[cnt], "-m") == 0)	// msg file
		{
			cnt++;
			if(cnt == argc)
			{
				fprintf(stderr, "\n\nError - no file name following <%s> parameter.\n\n", argv[cnt-1]);
				exit(-1);
			}

			if(gMsgPathFileName[0] != 0)
			{
				fprintf(stderr, "\n\nError - message file <%s> already specified.\n\n", gMsgPathFileName);
				exit(-2);
			}
			GetFullPathName(argv[cnt], MAX_PATH, gMsgPathFileName, &gMsgFileName);
		}
		else if(stricmp(argv[cnt], "-s") == 0) // stego file
		{
			cnt++;
			if(cnt == argc)
			{
				fprintf(stderr, "\n\nError - no file name following '%s' parameter.\n\n", argv[cnt-1]);
				exit(-1);
			}

			if(gStegoPathFileName[0] != 0)
			{
				fprintf(stderr, "\n\nError - stego file <%s> already specified.\n\n", gStegoPathFileName);
				exit(-2);
			}
			GetFullPathName(argv[cnt], MAX_PATH, gStegoPathFileName, &gStegoFileName);
		}
		else if(stricmp(argv[cnt], "-i") == 0) // input file
		{
			cnt++;
			if(cnt == argc)
			{
				fprintf(stderr, "\n\nError - no file name following '%s' parameter.\n\n", argv[cnt-1]);
				exit(-1);
			}

			if(gInputPathFileName[0] != 0)
			{
				fprintf(stderr, "\n\nError - input file <%s> already specified.\n\n", gInputPathFileName);
				exit(-2);
			}
			GetFullPathName(argv[cnt], MAX_PATH, gInputPathFileName, &gInputFileName);
		}
		else if(stricmp(argv[cnt], "-o") == 0) // output file
		{
			cnt++;
			if(cnt == argc)
			{
				fprintf(stderr, "\n\n Error - no file name following '%s' parameter.\n\n", argv[cnt-1]);
				exit(-1);
			}

			if(gOutputPathFileName[0] != 0)
			{
				fprintf(stderr, "\n\nError - output file <%s> already specified.\n\n", gOutputPathFileName);
				exit(-2);
			}
			GetFullPathName(argv[cnt], MAX_PATH, gOutputPathFileName, &gOutputFileName);
		}
		else if(stricmp(argv[cnt], "-k") == 0)	// key file
		{
			cnt++;
			if(cnt == argc)
			{
				fprintf(stderr, "\n\nInput Error - no file name following '%s' parameter.\n\n", argv[cnt-1]);
				exit(-1);
			}

			fprintf(stderr, "\n\nKey File currently not supported, ignoring....\n\n");
		}
		else if(stricmp(argv[cnt], "-p") == 0)	// stego/output file
		{
			cnt++;
			if(cnt == argc)
			{
				fprintf(stderr, "\n\nInput Error - no file name following '%s' parameter.\n\n", argv[cnt-1]);
				exit(-1);
			}

			fprintf(stderr, "\n\nPassword feature currently not supported, ignoring....\n\n");
		}
		else if(stricmp(argv[cnt], "-hide") == 0)	// hide
		{
			if(gAction)
			{
				fprintf(stderr, "\n\nError, an action has already been specified.\n\n");
				exit(-1);
			}
			gHideMsg = true;
			gAction = ACTION_HIDE;
		}
		else if(stricmp(argv[cnt], "-extract") == 0)	// extract
		{
			if(gAction)
			{
				fprintf(stderr, "\n\nError, an action has already been specified.\n\n");
				exit(-1);
			}
			gExtractMsg = true;
			gAction = ACTION_EXTRACT;
		}
		else if(stricmp(argv[cnt], "-wipe") == 0)	// wipe
		{
			if(gAction)
			{
				fprintf(stderr, "\n\nError, an action has already been specified.\n\n");
				exit(-1);
			}
			gWipeMsg = true;
			gAction = ACTION_WIPE_MSG;
		}
		else if(stricmp(argv[cnt], "-rnd") == 0)	// hide randomized data
		{
			if(gAction)
			{
				fprintf(stderr, "\n\nError, an action has already been specified.\n\n");
				exit(-1);
			}
			gDestroyMsg = true;
			gAction = ACTION_RND_MSG;
		}

		// specific, optional actions for Steg_LSB
		else if(stricmp(argv[cnt], "-convert") == 0)
		{
			if(gAction)
			{
				fprintf(stderr, "\n\nError, an action has already been specified.\n\n");
				exit(-1);
			}
			gConvert = true;
			gAction = ACTION_CONVERT;
		}
		else if(stricmp(argv[cnt], "-q") == 0)
		{
			cnt++;
			if(cnt == argc)
			{
				fprintf(stderr, "\n\nInput Error - no number following '%s' parameter.\n\n", argv[cnt-1]);
				exit(-1);
			}

			tmpInt = atoi(argv[cnt]);

			if(tmpInt < 10 || tmpInt > 100)
			{
				fprintf(stderr, "Error = quality must be between 10 and 100.\n\n");
				exit(-1);
			}

			gImageQuality = tmpInt;
		}
		else if(stricmp(argv[cnt], "-a") == 0)
		{
			cnt++;
			if(cnt == argc)
			{
				fprintf(stderr, "\n\nInput Error - no number following '%s' parameter.\n\n", argv[cnt-1]);
				exit(-1);
			}

			tmpDbl = atof(argv[cnt]);
			if(tmpDbl < 0.1 || tmpDbl > 9.99)
			{
				fprintf(stderr, "Error - Alpha value must be between 0.10 and 9.99\n\n");
				exit(-2);
			}
			gAlpha = tmpDbl;
		}
		else if(stricmp(argv[cnt], "-u") == 0)
		{
			cnt++;
			if(cnt == argc)
			{
				fprintf(stderr, "\n\nInput Error - no number following '%s' parameter.\n\n", argv[cnt-1]);
				exit(-1);
			}

			tmpDbl = atof(argv[cnt]);
			if(tmpDbl < 0.1 || tmpDbl > 9.99)
			{
				fprintf(stderr, "Error - Uniformity must be between 0.10 and 9.99.\n\n");
				exit(-3);
			}
			gUniform = tmpDbl;
		}
		else if(stricmp(argv[cnt], "-dct") == 0)
		{
			gDCT = true;
		}
		else if(stricmp(argv[cnt], "-g") == 0)
		{
			gGray = true;
		}
		else if(stricmp(argv[cnt], "-p") == 0)
		{
			gProgressive = true;
		}

		else if(stricmp(argv[cnt], "-v") == 0)
		{
			gVerbose = true;
		}


		cnt++;
	} // end while loop

	// check that parameters selected make sense
	switch(gAction)
	{
	case 0:
		fprintf(stderr, "\n\nNo Action specified.\n\n");
		Usage(argv[0]);
		exit(-1);

	case ACTION_HIDE:	// must have cover and msg  [stego OK]
		if(gCoverPathFileName[0] == 0 || gMsgPathFileName[0] == 0)
		{
			fprintf(stderr, "\n\nCan't hide data without both a cover file=(%s) and a message file=(%s).\n\n", gCoverPathFileName, gMsgPathFileName);
			exit(-1);
		}
		GetFullPathName(gCoverPathFileName, MAX_PATH, gInputPathFileName, &gInputFileName);	// cover file is input

		// input file MUST be a Bitmap OR Jpeg
		if( isValidJpeg(gInputPathFileName) == SUCCESS )
		{
			gInputFileType = 'j';
		}

		if( isValidBitmap(gInputPathFileName) == SUCCESS )
		{
			gInputFileType = 'b';
		}

		if( gInputFileType == 0)
		{
			fprintf(stderr, "Error - Input file <%s> does not appear to be a Bitmap or Jpeg file.\n\n", gInputPathFileName);
			exit(-1);
		}

		// use gOutputPathFileName as tmp holding
		strcpy(gOutputPathFileName, gMsgPathFileName);
		GetFullPathName(gOutputPathFileName, MAX_PATH, gMsgPathFileName, &gMsgFileName);		// message file is input
		gOutputPathFileName[0] = 0;	// reset

		if(gStegoPathFileName[0] != 0)
		{
			GetFullPathName(gStegoPathFileName, MAX_PATH, gOutputPathFileName, &gOutputFileName);	// stego file is output
		}
		break;

	case ACTION_EXTRACT:	// must have stego,  [msg OK] -- output is the message file
		if(gStegoPathFileName[0] == 0)
		{
			fprintf(stderr, "\n\nCan't extract data without a stego file=(%s).\n\n", gStegoPathFileName);
			exit(-1);
		}
		GetFullPathName(gStegoPathFileName, MAX_PATH, gInputPathFileName, &gInputFileName);	// stego file is input

		// input file MUST be a Bitmap OR Jpeg
		if( isValidJpeg(gInputPathFileName) == SUCCESS )
		{
			gInputFileType = 'j';
		}
		else
		{
			fprintf(stderr, "Error - Input file <%s> does not appear to be a Jpeg file.\n\n", gInputPathFileName);
			exit(-1);
		}

		if(gMsgPathFileName[0] != 0)
		{
			GetFullPathName(gMsgPathFileName, MAX_PATH, gOutputPathFileName, &gOutputFileName);	// message file is output
		}
		break;

	case ACTION_WIPE_MSG:		// must have a stego file [output OK]
		if(gStegoPathFileName[0] == 0)
		{
			fprintf(stderr, "\n\nCan't wipe data without specifying a stego file.\n\n");
			exit(-1);
		}
		GetFullPathName(gStegoPathFileName, MAX_PATH, gInputPathFileName, &gInputFileName);

		// input file MUST be a Bitmap OR Jpeg
		if( isValidJpeg(gInputPathFileName) == SUCCESS )
		{
			gInputFileType = 'j';
		}

		if( isValidBitmap(gInputPathFileName) == SUCCESS )
		{
			gInputFileType = 'b';
		}

		if( gInputFileType == 0)
		{
			fprintf(stderr, "Error - Input file <%s> does not appear to be a Bitmap or Jpeg file.\n\n", gInputPathFileName);
			exit(-1);
		}

		break;

	case ACTION_RND_MSG:		//  must have a stego file [output OK]
		if(gStegoPathFileName[0] == 0)
		{
			fprintf(stderr, "\n\nCan't randomize data without specifying a stego file.\n\n");
			exit(-1);
		}
		GetFullPathName(gStegoPathFileName, MAX_PATH, gInputPathFileName, &gInputFileName);
		// input file MUST be a Bitmap OR Jpeg
		if( isValidJpeg(gInputPathFileName) == SUCCESS )
		{
			gInputFileType = 'j';
		}

		if( isValidBitmap(gInputPathFileName) == SUCCESS )
		{
			gInputFileType = 'b';
		}

		if( gInputFileType == 0)
		{
			fprintf(stderr, "Error - Input file <%s> does not appear to be a Bitmap or Jpeg file.\n\n", gInputPathFileName);
			exit(-1);
		}

		break;

	case ACTION_CONVERT:		//  must have an input file [output OK]
		if(gInputPathFileName[0] == 0)
		{
			fprintf(stderr, "\n\nCan't convert files without an input file.\n\n");
			exit(-1);
		}

		// input file MUST be a Bitmap OR Jpeg
		if( isValidJpeg(gInputPathFileName) == SUCCESS )
		{
			gInputFileType = 'j';
			gOutputFileType ='b';
		}

		if( isValidBitmap(gInputPathFileName) == SUCCESS )
		{
			gInputFileType = 'b';
			gOutputFileType ='j';
		}

		if( gInputFileType == 0)
		{
			fprintf(stderr, "Error - Input file <%s> does not appear to be a Bitmap or Jpeg file.\n\n", gInputPathFileName);
			exit(-1);
		}

		break;

	case ACTION_DCT:		//  must have an input file [output OK]
		if(gInputPathFileName[0] == 0)
		{
			fprintf(stderr, "\n\nCan't get DCT coefficients without an input file.\n\n");
			exit(-1);
		}

		// input file MUST be a Bitmap OR Jpeg
		if( isValidJpeg(gInputPathFileName) == SUCCESS )
		{
			gInputFileType = 'j';
		}

		if( isValidBitmap(gInputPathFileName) == SUCCESS )
		{
			gInputFileType = 'b';
		}

		if( gInputFileType == 0)
		{
			fprintf(stderr, "Error - Input file <%s> does not appear to be a Bitmap or Jpeg file.\n\n", gInputPathFileName);
			exit(-1);
		}

		break;

	} // end switch on action

	return;
} // parseCommandLine

// initialize all global variables
void initGlobals()
{
	int i;

	// zero out histgram fr DCT coefficients
	for(i = 0; i < 1024; i++)
	{
		gDCT_HistogramBefore[i] = 0;
		gDCT_HistogramAfter[i] = 0;
	}

	return;
} // initGlobals

int main(int argc, char *argv[])
{
	FILE *fptr;
	int i, x, size;
	DWORD indexInputExt, indexOutputExt, fileSize;
	char inExt, outExt;

	double tmpDbl;
	unsigned int tmpInt;
	
	fprintf(stdout, "\n\n %s  Version: %4.2f\n\n", argv[0], VERSION);
	fprintf(stdout, "    by John A. Ortiz, last update Jan 2012\n\n");

	if(argc < 2) 
	{
		Usage(argv[0]);
		exit(0);
	}

	initGlobals();
	parseCommandLine(argc, argv);

	// initialize variables
	srand(1);
	lg2 = log( (double) 2); // lg2 = natural log 2

	// treat DC coefficeint a bit differently
	// this calculates lg2 of quality factor less hiding in DC coeffcients
	lgQ = log( (double) gImageQuality)/lg2; // lgQ==1 will nullify effect
	lgQ = 2/lgQ;

	// take directed action
	getBitsFromBuffer(0, NULL, 0);	// reset buffer static variables
	switch(gAction)
	{
	case ACTION_HIDE:	// output file name MUST have .jpg extension
		createOutputFileName("hid", "jpg");

		// read the message file into memory
		readMsgFile();	// sets gMessageBuffer

		// input file MUST be a Bitmap OR Jpeg
		if( gInputFileType == 'b' )
			readBitmap(gInputPathFileName);
		else
			readJpg(gInputPathFileName, gVerbose);

		// for hiding, jpeg MUST be output type
		writeJpg(gOutputPathFileName, gGray, gProgressive);

		if(gMsgBuffer != NULL)
		{
			free(gMsgBuffer);
		}
		fprintf(stdout, "\n\nStorage Capacity: %d bits (%d bytes)\nMessage Size: %u bytes\n", gBitCapacity, gBitCapacity/8, gMsgSize);
		if( (gBitCapacity/8) < gMsgSize) fprintf(stdout, "\n\nWARNING! ENTIRE MESSAGE WAS NOT HIDDEN!!!\n\n");
		break;

	case ACTION_WIPE_MSG:
	case ACTION_RND_MSG:

		if(gAction == ACTION_WIPE_MSG)
			createOutputFileName("wipe", "jpg");
		else
			createOutputFileName("rnd", "jpg");

		// set a small message buffer
		gMsgBuffer = (char *) malloc(64);
		if(gMsgBuffer == NULL)
		{
			fprintf(stderr, "Error - Could not allocate 64 bytes!!!\n\n");
			exit(-1);
		}
		gMsgSize = 0;

		// input file MUST be a Bitmap OR Jpeg
		if( gInputFileType == 'b' )
			readBitmap(gInputPathFileName);
		else
			readJpg(gInputPathFileName, gVerbose);

		// for wiping or randomizing, jpeg MUST be output type
		writeJpg(gOutputPathFileName, gGray, gProgressive);

		if(gMsgBuffer != NULL)
		{
			free(gMsgBuffer);
		}
		fprintf(stdout, "\n\nStorage Capacity: %d bits (%d bytes)\n\n", gBitCapacity, gBitCapacity/8);

		break;

	case ACTION_EXTRACT:
		putBitsInBuffer(0, 0, NULL, 0);	// initialize output buffer static variables

		// get a size for the message buffer by assuming the input file is larger than the message
		fptr = fopen(gInputPathFileName, "rb");
		if(fptr == NULL)
		{
			fprintf(stderr, "Error - Could not open input file <%s>\n\n", gInputPathFileName);
			exit(-1);
		}

		// make assumption that message file is NOT as large as input file
		fileSize = (DWORD) _filelength(_fileno(fptr)) + MAX_PATH;	// get length of input jpg for max size of message
																	// the 260 is for the possible length of the embedded filename
		gMsgBuffer = (char *) malloc(fileSize);
		if(gMsgBuffer == NULL)
		{
			fprintf(stderr, "Error - Could not allocate %u bytes for message file.\n\n", fileSize);
			exit(-1);
		}
		memset(gMsgBuffer, 0, fileSize);
		gMsgSize = fileSize;

		readJpg(gInputPathFileName, gVerbose);
		if(gMsgSize > 0) writeMsg();

		fprintf(stdout, "\n\nStorage Capacity: %d bits (%d bytes)\nMessage Size: %d bytes\n", gBitCapacity, gBitCapacity/8, gMsgSize);
		if( (gBitCapacity/8) < gMsgSize) fprintf(stdout, "\n\nWARNING! ENTIRE MESSAGE WAS NOT EXTRACTED!!!\n\n");

		break;

	case ACTION_CONVERT:	// use input file type to determine conversion (converting to same type results in copy)
		if( gInputFileType == 'b' )
		{
			readBitmap(gInputPathFileName);
			createOutputFileName("conv", "jpg");	// if specified, this will return
		}
		else
		{
			readJpg(gInputPathFileName, gVerbose);
			createOutputFileName("conv", "bmp");	// if specified, this will return
		}

		if(gOutputFileType == 'b')
			writeBitmap(gOutputPathFileName);
		else
			writeJpg(gOutputPathFileName, gGray, gProgressive);

		break;

	} // end switch on Action



	exit(0);
} // main
