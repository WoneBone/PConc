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
#include <unistd.h>
#include "imageRoutines.h"

char **files;
char* resize_dir;
char* thumb_dir;
char* water_dir;

/* typedef struct info{
	int n1;
	pthread_t id;
}info; */

void* water_thread(void* ni)
{ 

	/* input images */
	gdImagePtr  watermark_img;
	char *out_image, waterName[100];
	int range = (int) ni;

	watermark_img = read_png_file("watermark.png");
	

	if(watermark_img == NULL){
		fprintf(stderr, "Impossible to read %s image\n", "watermark.png");
		exit(-1);
	}

	for (int i = 0; i <range; i++){
		for (int j = strlen(files[i]); j >= 0; j--){
			out_image = &files[i][j];
			if (files[i][j -1] == '/') break;
		}

		sprintf(waterName, "%s%s", water_dir, out_image);
		if( access( waterName, F_OK ) != -1){
		printf("%s encontrado\n", waterName);
		}else{
		
	    printf("watermark  %s\n", files[i]);
		whatermark_routine(files[i], out_image, watermark_img);
		}
		
	}

	gdImageDestroy(watermark_img);

	return 0;
}

void* resize_thread(void* ni)
{ 

	/* input images */
	char *out_image, resizebName[100];
	int range = (int) ni;


	for (int i = 0; i < range; i++){	

		for (int j = strlen(files[i]); j >= 0; j--){
			out_image = &files[i][j];
			if (files[i][j -1] == '/') break;
		}

		sprintf(resizebName, "%s%s", resize_dir, out_image);
		if( access( resizebName, F_OK ) != -1){
		printf("%s encontrado\n", resizebName);
		}
		else{
		
		printf("resize %s\n", files[i]);
		resize_routine(files[i], out_image);
		}
	}

	return 0;
}

void* thumb_thread(void* ni)
{ 

	/* input images */
	char *out_image, thumbName[100];
	int range = (int) ni;


	for (int i = 0; i < range; i++){	

		for (int j = strlen(files[i]); j >= 0; j--){
			out_image = &files[i][j];
			if (files[i][j -1] == '/') break;
		}

		sprintf(thumbName, "%s%s", thumb_dir, out_image);
		if( access( thumbName, F_OK ) != -1){
		printf("%s encontrado\n", thumbName);
		}else{
		
		printf("Thumbnail %s\n", files[i]);
		thumb_routine(files[i], out_image);

		}
	}

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
	int n_threads = 3;

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
		printf("Impossivel abrir ficheiro %s", imageList);
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
	fclose(f);
	free(imageList);


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

    
	
	pthread_create(&thread_id[0], NULL, water_thread, (void *) nn_files);
	pthread_join(thread_id[0], NULL);


	pthread_create(&thread_id[1], NULL,resize_thread, (void *) nn_files);
	pthread_create(&thread_id[2], NULL,thumb_thread, (void *) nn_files);

	pthread_join(thread_id[1], NULL);
	pthread_join(thread_id[2], NULL);

	free(thread_id);
	for(i = 0; i < nn_files; i++) free(files[i]);
	free(files);
	free(resize_dir);
	free(thumb_dir);
	free(water_dir);
	exit(0);
}

