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
//  Title:  Image Type Functions
//
//  Author:  John M. Miano miano@colosseumbuilders.com
//
#include <stdlib.h>

#include "imagetyp.h"

#include "jpegdeco.h"
#include "pngdecod.h"
#include "bmpdecod.h"

//
//  Description:
//
//    This function determines the type of image stored in a stream.
//
//  Parameters:
//    strm:  The image stream
//
//  Return Value:
//    An enumeration that identifies the stream type
//
ImageType GetStreamImageType (std::istream &strm)
{
  long startpos = strm.tellg () ;
  
  ImageType result = UnknownImage ;

  char buffer [10] ;
  strm.read (buffer, sizeof (buffer)) ;
  if (strm.gcount () == sizeof (buffer))
  {
    if (memcmp (buffer, "\211PNG\r\n\032\n", 8) == 0)
    {
      result = PngImage ;
    }
    else if (memcmp (buffer, "\xFF\xD8\xFF\xE0", 4) == 0
             && memcmp (&buffer [6], "JFIF", 4) == 0)
    {
      result = JpegImage ;
    }
    else if (memcmp (buffer, "BM", 2) == 0)
    {
      result = BmpImage ;
    }
  }
  strm.seekg (startpos) ;

  return result ;
}

ImageType ReadImage (std::istream &strm,
                BitmapImage &image,
                PROGRESSFUNCTION function,
                void *data)
{
  ImageType type = GetStreamImageType (strm) ;
  if (type == UnknownImage)
    return type ;

  BmpDecoder bmpdecoder ;
  JpegDecoder jpegdecoder ;
  PngDecoder pngdecoder ;
  BitmapImageDecoder *decoder ;
  switch (type)
  {
  case BmpImage:
    decoder = &bmpdecoder ;
    break ;
  case PngImage:
    decoder = &pngdecoder ;
    break ;
  case JpegImage:
    decoder = &jpegdecoder ;
    break ;
  }

  decoder->SetProgressFunction (function, data) ;
  decoder->ReadImage (strm, image) ;
  return type ;
}
