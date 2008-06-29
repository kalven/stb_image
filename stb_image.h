/* stbi-0.56 - public domain JPEG/PNG reader - http://nothings.org/stb_image.c
                      when you control the images you're loading

   TODO:
      stbi_info_*

   history:
      0.57   fix bug: jpg last huffman symbol before marker was >9 bits but less
                      than 16 available
      0.56   fix bug: zlib uncompressed mode len vs. nlen
      0.55   fix bug: restart_interval not initialized to 0
      0.54   allow NULL for 'int *comp'
      0.53   fix bug in png 3->4; speedup png decoding
      0.52   png handles req_comp=3,4 directly; minor cleanup; jpeg comments
      0.51   obey req_comp requests, 1-component jpegs return as 1-component,
             on 'test' only check type, not whether we support this variant
*/


////   begin header file  ////////////////////////////////////////////////////
//
// Limitations:
//    - no progressive/interlaced support (jpeg, png)
//    - 8-bit samples only (jpeg, png)
//    - not threadsafe
//    - channel subsampling of at most 2 in each dimension (jpeg)
//    - no delayed line count (jpeg) -- image height must be in header
//    - unsophisticated error handling
//
// Basic usage:
//    int x,y,n;
//    unsigned char *data = stbi_load(filename, &x, &y, &n, 0);
//    // ... process data if not NULL ...
//    // ... x = width, y = height, n = # 8-bit components per pixel ...
//    // ... replace '0' with '1'..'4' to force that many components per pixel
//    stbi_free_image(data)
//
// Standard parameters:
//    int *x       -- outputs image width in pixels
//    int *y       -- outputs image height in pixels
//    int *comp    -- outputs # of image components in image file
//    int req_comp -- if non-zero, # of image components requested in result
//
// The return value from an image loader is an 'unsigned char *' which points
// to the pixel data. The pixel data consists of *y scanlines of *x pixels,
// with each pixel consisting of N interleaved 8-bit components; the first
// pixel pointed to is top-left-most in the image. There is no padding between
// image scanlines or between pixels, regardless of format. The number of
// components N is 'req_comp' if req_comp is non-zero, or *comp otherwise.
// If req_comp is non-zero, *comp has the number of components that _would_
// have been output otherwise. E.g. if you set req_comp to 4, you will always
// get RGBA output, but you can check *comp to easily see if it's opaque.
//
// An output image with N components has the following components interleaved
// in this order in each pixel:
//
//     N=#comp     components
//       1           grey
//       2           grey, alpha
//       3           red, green, blue
//       4           red, green, blue, alpha
//
// If image loading fails for any reason, the return value will be NULL,
// and *x, *y, *comp will be unchanged. The function stbi_failure_reason()
// can be queried for an extremely brief, end-user unfriendly explanation
// of why the load failed. Define STB_IMAGE_NO_FAILURE_REASON to avoid
// compiling these strings at all.
//
// Paletted PNG images are automatically depalettized.

#ifndef STB_IMAGE_H
#define STB_IMAGE_H

#include <stdio.h>

enum
{
   STBI_default = 0, // only used for req_comp

   STBI_grey       = 1,
   STBI_grey_alpha = 2,
   STBI_rgb        = 3,
   STBI_rgb_alpha  = 4,
};

typedef unsigned char stbi_uc;

#ifdef __cplusplus
extern "C" {
#endif

// PRIMARY API - works on images of any type

// load image by filename, open file, or memory buffer
extern stbi_uc *stbi_load            (char *filename,           int *x, int *y, int *comp, int req_comp);
extern stbi_uc *stbi_load_from_file  (FILE *f,                  int *x, int *y, int *comp, int req_comp);
extern stbi_uc *stbi_load_from_memory(stbi_uc *buffer, int len, int *x, int *y, int *comp, int req_comp);
// for stbi_load_from_file, file pointer is left pointing immediately after image

// get a VERY brief reason for failure
extern char    *stbi_failure_reason  (void);

// free the loaded image -- this is just free()
extern void     stbi_image_free      (stbi_uc *retval_from_stbi_load);

// get image dimensions & components without fully decoding
extern int      stbi_info            (char *filename,           int *x, int *y, int *comp);
extern int      stbi_info_from_file  (char *filename,           int *x, int *y, int *comp);
extern int      stbi_info_from_memory(stbi_uc *buffer, int len, int *x, int *y, int *comp);


// ZLIB client - used by PNG, available for other purposes

extern char *stbi_zlib_decode_malloc_guesssize(int initial_size, int *outlen);
extern char *stbi_zlib_decode_malloc(char *buffer, int len, int *outlen);
extern int   stbi_zlib_decode_buffer(char *obuffer, int olen, char *ibuffer, int ilen);


// TYPE-SPECIFIC ACCESS

// is it a jpeg?
extern int      stbi_jpeg_test_file       (FILE *f);
extern int      stbi_jpeg_test_memory     (stbi_uc *buffer, int len);

extern stbi_uc *stbi_jpeg_load            (char *filename,           int *x, int *y, int *comp, int req_comp);
extern stbi_uc *stbi_jpeg_load_from_file  (FILE *f,                  int *x, int *y, int *comp, int req_comp);
extern stbi_uc *stbi_jpeg_load_from_memory(stbi_uc *buffer, int len, int *x, int *y, int *comp, int req_comp);
extern int      stbi_jpeg_info            (char *filename,           int *x, int *y, int *comp);
extern int      stbi_jpeg_info_from_file  (char *filename,           int *x, int *y, int *comp);
extern int      stbi_jpeg_info_from_memory(stbi_uc *buffer, int len, int *x, int *y, int *comp);

// is it a png?
extern int      stbi_png_test_file        (FILE *f);
extern int      stbi_png_test_memory      (stbi_uc *buffer, int len);

extern stbi_uc *stbi_png_load             (char *filename,           int *x, int *y, int *comp, int req_comp);
extern stbi_uc *stbi_png_load_from_file   (FILE *f,                  int *x, int *y, int *comp, int req_comp);
extern stbi_uc *stbi_png_load_from_memory (stbi_uc *buffer, int len, int *x, int *y, int *comp, int req_comp);
extern int      stbi_png_info             (char *filename,           int *x, int *y, int *comp);
extern int      stbi_png_info_from_file   (char *filename,           int *x, int *y, int *comp);
extern int      stbi_png_info_from_memory (stbi_uc *buffer, int len, int *x, int *y, int *comp);

#ifdef __cplusplus
}
#endif

//
//
////   end header file   /////////////////////////////////////////////////////

#endif
