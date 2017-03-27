# Allegro Texture Packer
This project enables the seamless use of libGDX's texture packer with the Allegro (version 5) game programming library.

[Allegro](http://liballeg.org/) is an amazing cross-platform C game programming library that supports all major desktop and mobile operating systems. [libGDX](https://libgdx.badlogicgames.com/) is also an amazing cross-platform game programming library, but it's in Java and I would rather code in C.

libGDX has a simple to use, powerful [texture packing library ](https://github.com/libgdx/libgdx/wiki/Texture-packer) that integrates nicely with libGDX API's. The purpose of a [texture packer](https://en.wikipedia.org/wiki/Texture_atlas) is to combine multiple graphic assets into one file to improve the performance of OpenGL, as swapping bitmaps causes excessive overhead that reduces performance, especially on mobile devices. This is unacceptable for the efficient game programmer.

Allegro has no such texture packer. The libGDX devs provided such a feature for their users, but Allegro did not. There are other texture packers out there, but I prefer to use libGDX's.

Therefore, I wrote this library to make using the libGDX texture packer easy for Allegro programmers.

# Usage

This project has 2 parts. The first part is a Python script that converts libGDX atlas files into [Allegro config files](http://liballeg.org/a5docs/trunk/config.html), so that Allegro can easily access all the metadata necessary to use packed images.

The second part is a C library (header / implementation) that makes interacting with the packed image and the atlas seamless.

## Using the libGDX texture packer

Please read [this well written guide](https://github.com/libgdx/libgdx/wiki/Texture-packer) on how to use the libGDX texture packer. It is very simple to use. [Download](https://libgdx.badlogicgames.com/tools.html) the standalone build and run it like so:

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
