#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "al_texture_packer.h"

struct packed_image {
    int x;
    int y;
    int w;
    int h;
};
struct packed_image_file {
    char *file_name;
    ALLEGRO_BITMAP *bitmap;
};

static ALLEGRO_CONFIG *pack_config;
static int has_init = 0;
static int total_packed_image_files;
static struct packed_image *packed_image_array;
static struct packed_image_file *packed_image_file_array;

void al_init_texture_packer(const char *pack_file_path, const char *pack_file_name) {

    // Do not reinitialize
    if (has_init) {
        return;
    }

    // Load the config file
    char abs_path[1024];
    sprintf(abs_path, "%s/%s", pack_file_path, pack_file_name);
    pack_config = al_load_config_file(abs_path);

    // Load all the packed image files into bitmaps
    const char *total_files_str = al_get_config_value(pack_config, "file_metadata", "total_files");
    total_packed_image_files = atoi(total_files_str);
    packed_image_file_array = (struct packed_image_file*)calloc(total_packed_image_files, sizeof(struct packed_image_file));

    const char *c_all_files_str = al_get_config_value(pack_config, "file_metadata", "files");
    char *all_files_str = strdup(c_all_files_str);

    char *img_file_name;
    const char delim[2] = ",";
    img_file_name = strtok(all_files_str, delim);
    int i = 0;
    
    while( img_file_name != NULL ) {
        
        packed_image_file_array[i].file_name = (char*)calloc(strlen(img_file_name), sizeof(char));
        strcpy(packed_image_file_array[i].file_name, img_file_name);
        sprintf(abs_path, "%s/%s", pack_file_path, img_file_name);
        packed_image_file_array[i].bitmap = al_load_bitmap(abs_path);
        
        i++;
        img_file_name = strtok(all_files_str, delim);
    }

    free(all_files_str);

    // Create an array large enough to hold information for every packed image
    const char *total_images_str = al_get_config_value(pack_config, "file_metadata", "total_images");
    int total_images = atoi(total_images_str);
    packed_image_array = (struct packed_image*)calloc(total_images, sizeof(struct packed_image));

    has_init = 1;
}

void al_destroy_texture_packer() {
    if (!has_init) {
        return;
    }

    free(packed_image_array);

    int i;
    for (i = 0; i < total_packed_image_files; i++) {
        free(packed_image_file_array[i].file_name);
        al_destroy_bitmap(packed_image_file_array[i].bitmap);
    }
    free(packed_image_file_array);

    has_init = 0;
}