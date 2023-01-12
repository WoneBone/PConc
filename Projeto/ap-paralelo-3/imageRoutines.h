/* Prevent multiple inclusions */
#ifndef drenasQueOProfFezHeader
#define drenasQueOProfFezHeader

#include <gd.h>
#include "image-lib.h"
#include <pthread.h> 
#include <string.h>

/* the directories wher output files will be placed */
#define RESIZE_DIR "/Resize-dir/"
#define THUMB_DIR "/Thumbnail-sir/"
#define WATER_DIR "/Watermark-dir/"
#define MAX_STR 100


extern char* resize_dir;
extern char* thumb_dir;
extern char* water_dir;


void whatermark_routine(char* file, char* out_image, gdImagePtr watermark_img);
void resize_routine(char* file, char* out_image);
void thumb_routine(char* file, char* out_image);

/* End of: Protect multiple inclusions                              */
#endif