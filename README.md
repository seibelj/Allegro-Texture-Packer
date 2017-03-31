# Allegro Texture Packer
This project enables the seamless use of libGDX's texture packer with the Allegro (version 5) game programming library.

[Allegro](http://liballeg.org/) is an amazing cross-platform C game programming library that supports all major desktop and mobile operating systems. [libGDX](https://libgdx.badlogicgames.com/) is also an amazing cross-platform game programming library, but it's in Java and most of us would rather code in C.

libGDX has a simple to use, powerful [texture packing library ](https://github.com/libgdx/libgdx/wiki/Texture-packer) that integrates nicely with libGDX API's. The purpose of a [texture packer](https://en.wikipedia.org/wiki/Texture_atlas) is to combine multiple graphic assets into one file to improve the performance of OpenGL, as swapping bitmaps causes excessive overhead that reduces performance, especially on mobile devices. This is unacceptable for the efficient game programmer.

Allegro has no such texture packer. The libGDX devs provided a texture packing tool, but the Allegro devs did not. There are other texture packers out there, but I prefer to use libGDX's.

Therefore, I wrote this library to make using the libGDX texture packer easy for Allegro programmers.

## Features

- Extremely efficient - nearly zero overhead - all packed drawing calls are O(1) constant time
- Only dependency is `allegro_image` addon
- Written in the Allegro style (first call init, then draw functions, and when finished call destroy)
- All Allegro drawing functions are supported (tinted, scaled, rotated, etc.)
- Simple to use - make your life easier!

## Usage

This project has 2 parts. The first part is a Python script that converts libGDX atlas files into [Allegro config files](http://liballeg.org/a5docs/trunk/config.html), so that Allegro can easily access all the metadata necessary to use packed images.

The second part is a C library (header / implementation) that makes interacting with the packed images and the atlas file seamless.

## Using the libGDX texture packer

Please read [this well written guide](https://github.com/libgdx/libgdx/wiki/Texture-packer) on how to use the libGDX texture packer. It is very simple to use. The binary `runnable-texturepacker.jar` is included in the root of this repo, or [download](https://libgdx.badlogicgames.com/tools.html) the latest standalone build version and run it like so:

```
// *NIX (OS X/Linux)
java -cp runnable-texturepacker.jar com.badlogic.gdx.tools.texturepacker.TexturePacker inputDir outputDir packFileName

// WINDOWS
java -cp runnable-texturepacker.jar com.badlogic.gdx.tools.texturepacker.TexturePacker inputDir outputDir packFileName
```

Just put all the images you want to pack in the inputDir, point it to the outputDir, and give it a packFileName of your choice. The outputDir will be filled with packed images and a packFileName.atlas text file containing libGDX-compatible metadata about the packed images.

There are also many options you can customize using a settings file. Read the libGDX docs to learn more. This project picks up where libGDX texture packing ends.

## Converting a libGDX texture atlas to Allegro config file

Once you have packed your images and made your atlas file, you can run the Python script that converts the libGDX atlas to an Allegro atlas:

`python convert_atlas.py outputDir`

Replace `outputDir` with the folder name you used above. The script will detect the .atlas file, and produce `packFileName.allegro.atlas` in the same `outputDir`.

## Using the Packed Images in Allegro

Allegro Texture Packer operates similarly to other Allegro addons. Please read the comments in `al_texture_packer.h` to fully understand the library.

When you compile your project, you must link with allegro_image (`-lallegro_image`) and add `al_texture_packer.c` as a source code file. Make sure you init the Allegro image addon first with `al_init_image_addon()`.

Next, you must call the texture packer init method, passing it the path where you store the `.texture.atlas` file and the packed images:

`al_init_texture_packer("[packed_image_folder_path]", "myPackFile.allegro.atlas");`

Make sure that the packed image files are in the root of the `packed_image_folder_path` or the library will not work.

Next, load the image you want to draw (do not include the image extension when loading the file):

`AL_PACKED_IMAGE_ID img_id = al_load_packed_image("my_image_file");`

Use the image ID in drawing functions. All normal drawing functions are supported, such as:

`al_draw_packed_bitmap(AL_PACKED_IMAGE_ID p_img_id, float dx, float dy, int flags);`

Please read the header file for more information. Use `ALLEGRO_BITMAP *al_create_packed_bitmap(AL_PACKED_IMAGE_ID p_img_id)` to get the raw bitmap to draw yourself.

When you are closing Allegro, free memory and clean up the library with `al_destroy_texture_packer()`.

Enjoy!
