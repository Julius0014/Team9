// Hide.cpp
// 
// This file contains all the routines to Embed/Extract Message Data
//

#include "Main.h"
//*
// this function writes the DCT histogram to a file
void writeDCT_Histogram(char *filename, short int DCT_Histogram[65536] )
{
	FILE *fptr;

	fptr = fopen(filename, "w+");
	if(fptr == NULL) return;

	for(int i = 0; i < 1024; i++)
	{
		fprintf(fptr, "%d\n", gDCT_HistogramBefore[i]);
	}
	fclose(fptr);
	return;
} // writeDCT_Histogram
//*/


// this function collects DCT histogram coefficients
void getDCT_Histogram(JpegEncoderCoefficientBlock *data, short int DCT_Histogram[65536] )
{
	int row, col;
	short value;
	unsigned short valueIndx;

	for(row = 0; row < JpegSampleWidth; row++)
		for(col = 0; col < JpegSampleWidth; col++)
		{
			value = (*data)[row][col];
		//	value += 32768;	// should always be positive
			valueIndx = (unsigned short) value;
			DCT_Histogram[valueIndx]++;
		}
	return;
} // getDCT_Histogram

// This function gets numBits from the message starting from the last position
// up until the end of the message (numBits range from 1 to 8)
unsigned char getBitsFromBuffer(unsigned int numBits, unsigned char *inBuffer, unsigned int inBufferLength)
{

	return(0);
} // getBitsFromBuffer

// this function gets the Uniformity Factor
double getUniformity_E(JpegEncoderCoefficientBlock data)
{

	return(0);
} // getUniformity_E

// hide the data in a block of coefficients
void hideInBlock(JpegEncoderCoefficientBlock *data, JpegEncoderQuantizationTable &qt)
{
// example of how to access the DCT coefficients

	for(row = 0; row < JpegSampleWidth; row++)
		for(col = 0; col < JpegSampleWidth; col++)
		{
			tmpData = abs((double) (*data)[row][col]);
			qt.GetDataValue(row*JpegSampleWidth+col);
		}
	return;
} // hideInBlock

// takes the buffer, extracts a filename, and writes the rest of the data to disk
void writeMsg()
{

	return;
} // writeMsg

// uses the first 4 bytes in the message buffer to set the actual size of the message
void setMsgSize()
{

	return;
} // setMsgSize

// takes some number of bits and places them in a inBufferfer
int putBitsInBuffer(unsigned int numBits, unsigned char bits, unsigned char *outBuffer, unsigned int outBufferLength)
{


	return(SUCCESS);
} // putBitsInBuffer

// this function gets the Uniformity Factor
double getUniformity_D(JpegDecoderCoefficientBlock data)
{

	return(0);
} // getUniformity_D

// this function removes the bits from a block
void extractFromBlock(JpegDecoderCoefficientBlock data, const JpegDecoderQuantizationTable &qt)
{
	static int line = 0;
	unsigned int row, col, numBits;


	for(row = 0; row < JpegSampleWidth; row++)
		for(col = 0; col < JpegSampleWidth; col++)
		{
			tmpData = abs((double) data[row][col]);
			qt.GetDataValue(row*JpegSampleWidth+col);

		} // double nested for loop
	return;
} // extractFromBlock

