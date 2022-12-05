/******************************************************************************
 * Programacao Concorrente
 * MEEC 21/22
 *
 * Projecto - Parte1
 *                           serial-complexo.c
 * 
 * Compilacao: gcc serial-complexo -o serial-complex -lgd
 *           
 *****************************************************************************/

#include "gd.h"
#include "image-lib.h"
#include <math.h>
#include <pthread.h> 
/* the directories wher output files will be placed */
#define RESIZE_DIR "./serial-Resize/"
#define THUMB_DIR "./serial-Thumbnail/"
#define WATER_DIR "./serial-Watermark/"

typedef struct twint{
	int n1;
	int n2;
}twint;

	/* array containg the names of files to be processed	 */
char * files [] =  {"Lisboa-1.png", "IST-1.png", "IST-2.png", "IST-3.png", "00841.png", "00844.png", "00846.png", "00849.png" }; 
int fraq;

void* thread_function(void*ni)
{ 
	int start = abs((int) ni);
	int range = ( (int) ni < 0 ? fraq : fraq+1);

	/* input images */
	gdImagePtr in_img,  watermark_img;
	/* output images */
	gdImagePtr out_thumb_img, out_resized_img, out_watermark_img; 

	/* file name of the image created and to be saved on disk	 */
	char out_file_name[100];	

	watermark_img = read_png_file("watermark.png");
	
	if(watermark_img == NULL){
		fprintf(stderr, "Impossible to read %s image\n", "watermark.png");
		exit(-1);
	}



	for (int i = 0; i < range; i++){	

	    printf("watermark  %s\n", files[start + i]);
		/* load of the input file */
	    in_img = read_png_file(files[start + i]);
		if (in_img == NULL){
			fprintf(stderr, "Impossible to read %s image\n", files[start + i]);
			continue;
		}

		/* add watermark */
		out_watermark_img = add_watermark(in_img, watermark_img);
  		if (out_watermark_img == NULL) {
            fprintf(stderr, "Impossible to creat thumbnail of %s image\n", files[start + i]);
        }else{
			/* save watermark */
			sprintf(out_file_name, "%s%s", WATER_DIR, files[start + i]);
			if(write_png_file(out_watermark_img, out_file_name) == 0){
	            fprintf(stderr, "Impossible to write %s image\n", out_file_name);
			}
			gdImageDestroy(out_watermark_img);
		}
		gdImageDestroy(in_img);

	}


	/* 
	 * To resize images and add watermark
	 */
	for (int i = 0; i < range; i++){	

		printf("resize %s\n", files[start + i]);
		/* load of the input file */
	    in_img = read_png_file(files[start + i]);
		if (in_img == NULL){
			fprintf(stderr, "Impossible to read %s image\n", files[start + i]);
			continue;
		}
		/* resizes of each image */
		out_watermark_img = add_watermark(in_img, watermark_img);
		if (out_watermark_img == NULL) {
			fprintf(stderr, "Impossible to add watermark to %s image\n", files[start + i]);
		}else{
			out_resized_img = resize_image(out_watermark_img, 800);
			if (out_resized_img == NULL) {
				fprintf(stderr, "Impossible to resize %s image\n", files[start + i]);
			}else{
				/* save resized */
				sprintf(out_file_name, "%s%s", RESIZE_DIR, files[start + i]);
				if(write_png_file(out_resized_img, out_file_name) == 0){
					fprintf(stderr, "Impossible to write %s image\n", out_file_name);
				}
				gdImageDestroy(out_resized_img);
			}
			gdImageDestroy(out_watermark_img);
		}
		gdImageDestroy(in_img);
	}

	/* 
	 * Add watermark and create thumbnails
	 */
	for (int i = 0; i < range; i++){	

	   	printf("thumbnail %s\n", files[start + i]);
		/* load of the input file */
	    in_img = read_png_file(files[start + i]);
		if (in_img == NULL){
			fprintf(stderr, "Impossible to read %s image\n", files[start + i]);
			continue;
		}

		/* creation of thumbnail image */
		out_watermark_img = add_watermark(in_img, watermark_img);
		if (out_watermark_img == NULL) {
			fprintf(stderr, "Impossible to creat thumbnail of %s image\n", files[start + i]);
		}else{
			out_thumb_img = make_thumb(out_watermark_img, 150);
			if (out_thumb_img == NULL) {
				fprintf(stderr, "Impossible to creat thumbnail of %s image\n", files[start + i]);
			}else{
				/* save thumbnail image */
				sprintf(out_file_name, "%s%s", THUMB_DIR, files[start + i]);
				if(write_png_file(out_thumb_img, out_file_name) == 0){
					fprintf(stderr, "Impossible to write %s image\n", out_file_name);
				}
				gdImageDestroy(out_thumb_img);
			}
			gdImageDestroy(out_watermark_img);
		}
		gdImageDestroy(in_img);

	}

	gdImageDestroy(watermark_img);

	return 0;
}


/******************************************************************************
 * main()
 *
 * Arguments: (none)
 * Returns: 0 in case of sucess, positive number in case of failure
 * Side-Effects: creates thumbnail, resized copy and watermarked copies
 *               of images
 *
 * Description: implementation of the complex serial version 
 *              This application only works for a fixed pre-defined set of files
 *
 *****************************************************************************/
int main(int argc, char* argv[]){

	
	/* length of the files array (number of files to be processed	 */
	int nn_files = 8;
	int n_threads = atoi(argv[2]);
	fraq = nn_files / n_threads;
	int start = 0;

	pthread_t *thread_id = (pthread_t*) malloc(sizeof(pthread_t) * n_threads);
	if (thread_id == NULL){
		fprintf(stderr, "cry");
		exit(-1);
	}

	/* creation of output directories */
	if (create_directory(RESIZE_DIR) == 0){
		fprintf(stderr, "Impossible to create %s directory\n", RESIZE_DIR);
		exit(-1);
	}
	if (create_directory(THUMB_DIR) == 0){
		fprintf(stderr, "Impossible to create %s directory\n", THUMB_DIR);
		exit(-1);
	}
	if (create_directory(WATER_DIR) == 0){
		fprintf(stderr, "Impossible to create %s directory\n", WATER_DIR);
		exit(-1);
	}

    
	
	for (int i = 0; i < n_threads; i++){
		if (i < (nn_files % n_threads)){
			if(pthread_create(&thread_id[i], NULL,thread_function, (void *) start) != 0){
				fprintf(stderr, "Failed to create thread number %d", i);
				exit(-1);
			}
			start += fraq +1;
		}
		else{
			if(pthread_create(&thread_id[i], NULL,thread_function, (void *) (-1*start)) != 0){
				fprintf(stderr, "Failed to create thread number %d", i);
				exit(-1);
			}
			start += fraq;
		}
	}

	for (int i = 0; i < n_threads; i++)
		pthread_join(thread_id[i],NULL);

	free(thread_id);
	exit(0);
}

