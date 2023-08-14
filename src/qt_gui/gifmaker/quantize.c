/*****************************************************************************

 quantize.c - quantize a high resolution image into lower one

 Based on: "Color Image Quantization for frame buffer Display", by
 Paul Heckbert SIGGRAPH 1982 page 297-307.

 This doesn't really belong in the core library, was undocumented,
 and was removed in 4.2.  Then it turned out some client apps were
 actually using it, so it was restored in 5.0.

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "gif_lib.h"
#include "gif_lib_private.h"

#define ABS(x) ((x) > 0 ? (x) : (-(x)))

#define COLOR_ARRAY_SIZE 32768
#define BITS_PER_PRIM_COLOR 5
#define MAX_PRIM_COLOR 0x1f

static int SortRGBAxis;

typedef struct QuantizedColorType {
  GifByteType RGB[3];
  GifByteType NewColorIndex;
  long Count;
  struct QuantizedColorType *Pnext;
} QuantizedColorType;

typedef struct NewColorMapType {
  GifByteType RGBMin[3], RGBWidth[3];
  unsigned int NumEntries; /* # of QuantizedColorType in linked list below */
  unsigned long Count;     /* Total number of pixels in all the entries */
  QuantizedColorType *QuantizedColors;
} NewColorMapType;

static int SubdivColorMap(NewColorMapType *NewColorSubdiv,
                          unsigned int ColorMapSize,
                          unsigned int *NewColorMapSize);
static int SortCmpRtn(const void *Entry1, const void *Entry2);

/* end */
