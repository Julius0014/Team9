#ifndef __IMAGETYP_H
#define __IMAGETYP_H
//
// Copyright (c) 1997,1998 Colosseum Builders, Inc.
// All rights reserved.
//
// Colosseum Builders, Inc. makes no warranty, expressed or implied
// with regards to this software. It is provided as is.
//
// See the README.TXT file that came with this software for restrictions
// on the use and redistribution of this file or send E-mail to
// info@colosseumbuilders.com
//

//
//  Title:  Graphics Type Definitions
//
//  Author:  John M. Miano  miano@colosseumbuilders.com
//

#include "bitimage.h"

enum ImageType
{
  UnknownImage=0,
  BmpImage = 2,
  JpegImage = 3,
  PngImage = 7,
} ;


extern ImageType GetStreamImageType (std::istream &) ;
extern ImageType ReadImage (std::istream &strm,
                            BitmapImage &image,
                            PROGRESSFUNCTION function,
                            void *data) ;

#endif
