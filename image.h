#ifndef IMAGE_H
#define IMAGE_H

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <malloc.h>
#include <mem.h>
//! Struct (class) for Image manipulation
/*!
    Struct use stb_image.h, stb_image_write.h for opening and writing images.
    Look these files to check type support and limitations.

    Use image_load to load image from file.
    Use image_init to allocate image with specified w, h, comp
    !IMPORTANT use image_destruct to destruct loaded and initialised image,
    otherwise there will be memory leak

    Properties:
        data - image data as array
        w - image width
        h - image height
        comp - number of image components:
            1 - grayscale
            2 - grayscale, alpha
            3 - RGB
            4 - RGBalpha

*/
struct Image
{
    unsigned char *data;
    int w, h, comp;
};

/**
    Load image from file.
    If successful function allocate memory. Use image_destruct to deallocate.

    Use req_comp to request number of components:
        0 - default
        1 - grayscale
        2 - grayscale, alpha
        3 - RGB
        4 - RGBalpha

    Example if image is RGB and you request grayscale, image will be
    loaded and converted to grayscale,

    If image fail to load, Image data will be NULL.
    Use const char* image_load_err() to get error;
*/
void image_load(struct Image *im, const char* file_path, int req_comp){
     im->data = stbi_load(file_path, &im->w, &im->h, &im->comp, req_comp);
}

const char* image_load_err(){
    return stbi_failure_reason();
}

/**
    Initialize image.
    If successful function allocate memory. Use image_destruct to deallocate.

    Functions override image data, so image should be empty or destructed.

    If function fail to allocate memory, image data will be NULL
*/
void image_init(struct Image *im, int w, int h, int comp){
    im->data = (unsigned char*) malloc(w * h * comp * sizeof(unsigned char));
    im->w = w;
    im->h = h;
    im->comp = comp;
}

/**
    Destruct image.
    Deallocate data and set it as NULL.
*/
void image_destruct(struct Image *m){
    if(m->data == NULL)
        return;

    free(m->data);

    m->data = NULL;
}

unsigned char image_get(struct Image *im, int i, int j, int n){
    return im->data[i*im->h*im->comp + j*im->comp + n];
}

void image_set(struct Image *im, int i, int j, int n, unsigned char el){
    im->data[i*im->h*im->comp + j*im->comp + n] = el;
}

void image_set_array(struct Image *dest, int i, int j, unsigned char *source, int size){
    memcpy(dest->data + i*dest->h*dest->comp + j*dest->comp, source, size);
}
/**
    Write functions.
    Use stb_image_write.h

    Each function returns 0 on failure and non-0 on success.
*/
int image_write_png(struct Image *im, const char *file_path){
    return stbi_write_png(file_path, im->w, im->h, im->comp, im->data, 0);
}

int image_write_bmp(struct Image *im, const char *file_path){
    return stbi_write_bmp(file_path, im->w, im->h, im->comp, im->data);
}

int image_write_tga(struct Image *im, const char *file_path){
    return stbi_write_tga(file_path, im->w, im->h, im->comp, im->data);
}

int image_write_hdr(struct Image *im, const char *file_path){
    return stbi_write_hdr(file_path, im->w, im->h, im->comp, im->data);
}

int image_main(){
    struct Image b_w; //black_and_white

    //load as grayscale
    image_load(&b_w ,"E:\\temp\\l.png", 1);
    if(b_w.data == NULL){
        printf(image_load_err());
        exit(-1);
    }
    int result = image_write_png(&b_w, "E:\\temp\\l_black_white.png");
    if(result == 0){
        printf("There was writing problem with writing file");
        exit(-1);
    }

    image_destruct(&b_w);
/*    printf("h:%d w:%d n:%d\n", h, w, n);
    for(i = 0; i < h; i++){
        for(j = 0; j < w; j++){
            //printf("(");
            for(k = 0; k < n; k++)
                printf("%4d", im[i*w*n + j*n + k]);
            //printf(")");
        }
        printf("\n");
    }*/

}
#endif // IMAGE_H

