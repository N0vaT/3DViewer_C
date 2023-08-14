/*****************************************************************************

 GIF construction tools

****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gif_lib.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

/******************************************************************************
 Miscellaneous utility functions
******************************************************************************/

/* return smallest bitfield size n will fit in */
int GifBitSize(int n) {
  register int i;

  for (i = 1; i <= 8; i++)
    if ((1 << i) >= n) break;
  return (i);
}

/******************************************************************************
  Color map object functions
******************************************************************************/

/*
 * Allocate a color map of given size; initialize with contents of
 * ColorMap if that pointer is non-NULL.
 */
ColorMapObject *GifMakeMapObject(int ColorCount, const GifColorType *ColorMap) {
  ColorMapObject *Object;

  /*** FIXME: Our ColorCount has to be a power of two.  Is it necessary to
   * make the user know that or should we automatically round up instead? */
  if (ColorCount != (1 << GifBitSize(ColorCount))) {
    return ((ColorMapObject *)NULL);
  }

  Object = (ColorMapObject *)malloc(sizeof(ColorMapObject));
  if (Object == (ColorMapObject *)NULL) {
    return ((ColorMapObject *)NULL);
  }

  Object->Colors = (GifColorType *)calloc(ColorCount, sizeof(GifColorType));
  if (Object->Colors == (GifColorType *)NULL) {
    free(Object);
    return ((ColorMapObject *)NULL);
  }

  Object->ColorCount = ColorCount;
  Object->BitsPerPixel = GifBitSize(ColorCount);

  if (ColorMap != NULL) {
    memcpy((char *)Object->Colors, (char *)ColorMap,
           ColorCount * sizeof(GifColorType));
  }

  return (Object);
}

/*******************************************************************************
Free a color map object
*******************************************************************************/
void GifFreeMapObject(ColorMapObject *Object) {
  if (Object != NULL) {
    (void)free(Object->Colors);
    (void)free(Object);
  }
}

/******************************************************************************
 Extension record functions
******************************************************************************/
int GifAddExtensionBlock(int *ExtensionBlockCount,
                         ExtensionBlock **ExtensionBlocks, int Function,
                         unsigned int Len, unsigned char ExtData[]) {
  ExtensionBlock *ep;

  if (*ExtensionBlocks == NULL)
    *ExtensionBlocks = (ExtensionBlock *)malloc(sizeof(ExtensionBlock));
  else
    *ExtensionBlocks = (ExtensionBlock *)realloc(
        *ExtensionBlocks, sizeof(ExtensionBlock) * (*ExtensionBlockCount + 1));

  if (*ExtensionBlocks == NULL) return (GIF_ERROR);

  ep = &(*ExtensionBlocks)[(*ExtensionBlockCount)++];

  ep->Function = Function;
  ep->ByteCount = Len;
  ep->Bytes = (GifByteType *)malloc(ep->ByteCount);
  if (ep->Bytes == NULL) return (GIF_ERROR);

  if (ExtData != NULL) {
    memcpy(ep->Bytes, ExtData, Len);
  }

  return (GIF_OK);
}

void GifFreeExtensions(int *ExtensionBlockCount,
                       ExtensionBlock **ExtensionBlocks) {
  ExtensionBlock *ep;

  if (*ExtensionBlocks == NULL) return;

  for (ep = *ExtensionBlocks; ep < (*ExtensionBlocks + *ExtensionBlockCount);
       ep++)
    (void)free((char *)ep->Bytes);
  (void)free((char *)*ExtensionBlocks);
  *ExtensionBlocks = NULL;
  *ExtensionBlockCount = 0;
}

/******************************************************************************
 Image block allocation functions
******************************************************************************/

void GifFreeSavedImages(GifFileType *GifFile) {
  SavedImage *sp;

  if ((GifFile == NULL) || (GifFile->SavedImages == NULL)) {
    return;
  }
  for (sp = GifFile->SavedImages;
       sp < GifFile->SavedImages + GifFile->ImageCount; sp++) {
    if (sp->ImageDesc.ColorMap != NULL) {
      GifFreeMapObject(sp->ImageDesc.ColorMap);
      sp->ImageDesc.ColorMap = NULL;
    }

    if (sp->RasterBits != NULL) free((char *)sp->RasterBits);

    GifFreeExtensions(&sp->ExtensionBlockCount, &sp->ExtensionBlocks);
  }
  free((char *)GifFile->SavedImages);
  GifFile->SavedImages = NULL;
}

/* end */
