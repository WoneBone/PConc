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

#include <math.h>
#include "imageRoutines.h"

char **files;
int fraq;
char* resize_dir;
char* thumb_dir;
char* water_dir;

void* thread_function(void*ni)
{ 
	int start = abs((int) ni);
	int range = ( (int) ni <= 0 ? fraq : fraq+1);

	/* input images */
	gdImagePtr  watermark_img;
	char *out_image;

	watermark_img = read_png_file("watermark.png");
	

	if(watermark_img == NULL){
		fprintf(stderr, "Impossible to read %s image\n", "watermark.png");
		exit(-1);
	}



	for (int i = 0; i < range; i++){
		for (int j = strlen(files[start + i]); j >= 0; j--){
			out_image = &files[start + i][j];
			if (files[start + i][j] == '/') break;
		}

	    printf("watermark  %s\n", files[start + i]);
		whatermark_routine(files[start + i], out_image, watermark_img);
		
	}


	/* 
	 * To resize images and add watermark
	 */
	for (int i = 0; i < range; i++){	

		for (int j = strlen(files[start + i]); j >= 0; j--){
			out_image = &files[start + i][j];
			if (files[start + i][j] == '/') break;
		}

		printf("resize %s\n", files[start + i]);
		resize_routine(files[start + i], out_image);
		
	   	printf("thumbnail %s\n", files[start + i]);
		thumb_routine(files[start + i], out_image);



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
	int nn_files = 0;
	int n_threads = atoi(argv[2]);
	if (n_threads <= 0) exit(-1);
	if(argv[1][strlen(argv[1])] == '/') argv[1][strlen(argv[1])] = '\0';
	char image[100];
	char *imageList = (char *) malloc(sizeof(char) * (strlen(argv[1]) + strlen("/image-list.txt") + 1));
	if (imageList == NULL){
		exit(-1);
	}
	strcpy(imageList, argv[1]);
	strcat(imageList,"/image-list.txt");
	FILE *f = fopen(imageList, "r");
	if (f == NULL){
		exit(-1);
	}

	while(fgets(image, 100, f)){
		nn_files++;
	}
	rewind(f);
	files = (char **)malloc(sizeof(char*) * nn_files);
	if(files == NULL){
		exit(-1);
	}
	
	int i = 0;
	while(fgets(image, 100, f)){

		sscanf(image, "%s", image);
		strcpy(imageList, image);
		strcpy(image, "./");
		strcat(image,argv[1]);
		image[strlen(image) + 1] = '\0';
		image[strlen(image)] = '/';
		strcat(image, imageList);

		files[i] = (char *)malloc(sizeof(char)*(strlen(image) + 1));
		if (files[i] == NULL) exit(-1);
		strcpy(files[i], image);
		i++;
	}
	free(imageList);

	fraq = nn_files / n_threads;
	int start = 0;

	resize_dir = (char*)malloc(sizeof(char) * (strlen(argv[1]) + strlen(RESIZE_DIR) + 1));
	strcpy(resize_dir, argv[1]);
	strcat(resize_dir, RESIZE_DIR);
	thumb_dir = (char*)malloc(sizeof(char) * (strlen(argv[1]) + strlen(THUMB_DIR) + 1));
	strcpy(thumb_dir, argv[1]);
	strcat(thumb_dir, THUMB_DIR);
	water_dir = (char*)malloc(sizeof(char) * (strlen(argv[1]) + strlen(WATER_DIR) + 1));
	strcpy(water_dir, argv[1]);
	strcat(water_dir, WATER_DIR);

	pthread_t *thread_id = (pthread_t*) malloc(sizeof(pthread_t) * n_threads);
	if (thread_id == NULL){
		fprintf(stderr, "cry");
		exit(-1);
	}

	/* creation of output directories */
	if (create_directory(resize_dir) == 0){
		fprintf(stderr, "Impossible to create %s directory\n", resize_dir);
		exit(-1);
	}
	if (create_directory(thumb_dir) == 0){
		fprintf(stderr, "Impossible to create %s directory\n", thumb_dir);
		exit(-1);
	}
	if (create_directory(water_dir) == 0){
		fprintf(stderr, "Impossible to create %s directory\n", water_dir);
		exit(-1);
	}

    
	
	for (int i = 0; i < n_threads; i++){
		if (i <= (nn_files % n_threads) && i != 0){
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

