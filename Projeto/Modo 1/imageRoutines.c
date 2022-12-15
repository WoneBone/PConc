#include "imageRoutines.h"

void whatermark_routine(char* file, char* out_image, gdImagePtr watermark_img){

    /*Directory of output*/
    char out_file_name[100];
    /* input images */
	gdImagePtr in_img;
	/* output images */
	gdImagePtr out_watermark_img; 
    /* load of the input file */
    in_img = read_png_file(file);
    if (in_img == NULL){
        fprintf(stderr, "Impossible to read %s image\n", file);
        return;
    }

    /* add watermark */
    out_watermark_img = add_watermark(in_img, watermark_img);
    if (out_watermark_img == NULL) {
        fprintf(stderr, "Impossible to creat thumbnail of %s image\n", file);
    }else{
        /* save watermark */
        sprintf(out_file_name, "%s%s", water_dir, out_image);
        if(write_png_file(out_watermark_img, out_file_name) == 0){
            fprintf(stderr, "Impossible to write %s image\n", out_file_name);
        }
        gdImageDestroy(out_watermark_img);
    }
    gdImageDestroy(in_img);
    return;
}

void resize_routine(char* file, char* out_image){

    /*Directory of output*/
    char out_file_name[100], waterName[100];
    /* input images */
	gdImagePtr in_img,  watermark_img;
	/* output images */
	gdImagePtr out_resized_img, out_watermark_img; 
    /* load of the input file */
    in_img = read_png_file(file);
    if (in_img == NULL){
        fprintf(stderr, "Impossible to read %s image\n", file);
        return;
    }
    /* resizes of each image */
    sprintf(waterName, "%s%s", water_dir, out_image);
    out_watermark_img = read_png_file(waterName);

    if (out_watermark_img == NULL) {
        fprintf(stderr, "Impossible to read watermark file %s\n", out_image);
    }else{
        out_resized_img = resize_image(out_watermark_img, 800);
        if (out_resized_img == NULL) {
            fprintf(stderr, "Impossible to resize %s image\n", file);
        }else{
            /* save resized */
            sprintf(out_file_name, "%s%s", resize_dir, out_image);
            if(write_png_file(out_resized_img, out_file_name) == 0){
                fprintf(stderr, "Impossible to write %s image\n", out_file_name);
            }
            gdImageDestroy(out_resized_img);
        }
        gdImageDestroy(out_watermark_img);
    }
}

void thumb_routine(char* file, char* out_image){

    /*Directory of output*/
    char out_file_name[100], waterName[100];
    /* input images */
	gdImagePtr in_img,  watermark_img;
	/* output images */
	gdImagePtr out_thumb_img, out_watermark_img; 
    /* load of the input file */
    in_img = read_png_file(file);
    if (in_img == NULL){
        fprintf(stderr, "Impossible to read %s image\n", file);
        return;
    }
    /* resizes of each image */
    sprintf(waterName, "%s%s", water_dir, out_image);
    out_watermark_img = read_png_file(waterName);

    if (out_watermark_img == NULL) {
        fprintf(stderr, "Impossible to read watermark file %s\n", out_image);
    }else{
        out_thumb_img = make_thumb(out_watermark_img, 150);
        if (out_thumb_img == NULL) {
            fprintf(stderr, "Impossible to thumnail %s image\n", file);
        }else{
            /* save resized */
            sprintf(out_file_name, "%s%s", thumb_dir, out_image);
            if(write_png_file(out_thumb_img, out_file_name) == 0){
                fprintf(stderr, "Impossible to write %s image\n", out_file_name);
            }
            gdImageDestroy(out_thumb_img);
        }
        gdImageDestroy(out_watermark_img);
    }
}