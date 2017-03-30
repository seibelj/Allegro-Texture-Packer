#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "al_texture_packer.h"

struct packed_image {
    int x;
    int y;
    int w;
    int h;
    int file_idx;
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
        img_file_name = strtok(NULL, delim);
    }

    free(all_files_str);

    // Create an array large enough to hold information for every packed image
    const char *total_images_str = al_get_config_value(pack_config, "file_metadata", "total_images");
    int total_images = atoi(total_images_str);
    packed_image_array = (struct packed_image*)calloc(total_images, sizeof(struct packed_image));

    has_init = 1;
}

AL_PACKED_IMAGE_ID al_load_packed_image(const char *file_name) {
    const char *id_str = al_get_config_value(pack_config, file_name, "id");
    const char *x_str = al_get_config_value(pack_config, file_name, "x");
    const char *y_str = al_get_config_value(pack_config, file_name, "y");
    const char *w_str = al_get_config_value(pack_config, file_name, "w");
    const char *h_str = al_get_config_value(pack_config, file_name, "h");
    const char *file_str = al_get_config_value(pack_config, file_name, "file");

    int id = atoi(id_str);

    // Iterate through packed_image_file_array to find the file_idx
    int i;
    int found = 0;
    for (i = 0; i < total_packed_image_files; i++) {
        if (strcmp(packed_image_file_array[i].file_name, file_str) == 0) {
            found = 1;
            packed_image_array[id].file_idx = i;
            break;
        }
    }

    if (!found) {
        return (AL_PACKED_IMAGE_ID)-1;
    }

    packed_image_array[id].x = atoi(x_str);
    packed_image_array[id].y = atoi(y_str);
    packed_image_array[id].w = atoi(w_str);
    packed_image_array[id].h = atoi(h_str);

    return (AL_PACKED_IMAGE_ID)id;
}

void al_draw_packed_bitmap(AL_PACKED_IMAGE_ID p_img_id, float dx, float dy, int flags) {
    int p_img_int = (int)p_img_id;

    al_draw_bitmap_region(
        packed_image_file_array[packed_image_array[p_img_int].file_idx].bitmap,
        packed_image_array[p_img_int].x,
        packed_image_array[p_img_int].y,
        packed_image_array[p_img_int].w,
        packed_image_array[p_img_int].h,
        dx, dy, flags
    );
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

    al_destroy_config(pack_config);

    has_init = 0;
}