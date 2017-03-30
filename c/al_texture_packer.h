#ifndef TEXTURE_PACKER_ALLEGRO_H
#define TEXTURE_PACKER_ALLEGRO_H

#include <allegro5/allegro.h>

typedef int AL_PACKED_IMAGE_ID;

/*
Initialize the texture packer. pack_file_name and all referenced
packed image files must be in the root of pack_file_path
*/
void al_init_texture_packer(const char *pack_file_path, const char *pack_file_name);

/*
All images must be loaded before using them. Use the returned
AL_PACKED_IMAGE_ID value in subsequent drawing and bitmap calls.

Pass in the image name without extension. Look in the .allegro.atlas file
to see the names of images in brackets.

Return value of -1 means it couldn't find the image.
*/
AL_PACKED_IMAGE_ID al_load_packed_image(const char *file_name);

/*
Like al_draw_bitmap
http://liballeg.org/a5docs/trunk/graphics.html#al_draw_bitmap
*/
void al_draw_packed_bitmap(AL_PACKED_IMAGE_ID p_img_id, float dx, float dy, int flags);

/* 
Like al_draw_tinted_bitmap
http://liballeg.org/a5docs/trunk/graphics.html#al_draw_tinted_bitmap
*/
void al_draw_tinted_packed_bitmap(AL_PACKED_IMAGE_ID p_img_id, ALLEGRO_COLOR tint,
    float dx, float dy, int flags);

/*
Like al_draw_tinted_scaled_rotated_bitmap
http://liballeg.org/a5docs/trunk/graphics.html#al_draw_tinted_scaled_rotated_bitmap
*/
void al_draw_tinted_scaled_rotated_packed_bitmap(AL_PACKED_IMAGE_ID p_img_id,
    ALLEGRO_COLOR tint,
    float cx, float cy, float dx, float dy, float xscale, float yscale,
    float angle, int flags);

/*
Calls al_create_sub_bitmap() properly and returns the bitmap of the
packed image. You can then draw and manipulate the bitmap yourself.

Be aware that you need to manually destroy the created bitmap
when you are finished with al_destroy_bitmap()

http://liballeg.org/a5docs/trunk/graphics.html#al_create_sub_bitmap
*/
ALLEGRO_BITMAP *al_create_packed_bitmap(AL_PACKED_IMAGE_ID p_img_id);

/*
Frees all memory used by the texture packer. Call this when you are finished
*/
void al_destroy_texture_packer();

#endif /* TEXTURE_PACKER_ALLEGRO_H */
