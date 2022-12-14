/* Prevent multiple inclusions */
#ifndef drenasQueOProfFezHeader
#define drenasQueOProfFezHeader

#include <gd.h>
#include "image-lib.h"
#include <pthread.h> 
#include <string.h>

/* the directories wher output files will be placed */
#define RESIZE_DIR "/serial-Resize/"
#define THUMB_DIR "/serial-Thumbnail/"
#define WATER_DIR "/serial-Watermark/"


	/* array containg the names of files to be processed	 */
extern char **files;
extern char* resize_dir;
extern char* thumb_dir;
extern char* water_dir;


void whatermark_routine(char* file, char* out_image, gdImagePtr watermark_img);
void resize_routine(char* file, char* out_image);
void thumb_routine(char* file, char* out_image);

/* End of: Protect multiple inclusions                              */
#endif