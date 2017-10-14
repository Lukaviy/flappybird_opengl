# Flappy Cube

Simple clone of popular flappy bird game.

Based on [SFML](https://www.sfml-dev.org/) framework 

***

## Build

Make sure you have installed SFML 2.4.2 and all it's dependencies (such as FreeType, libjpeg, etc.).

If not, you can download builded library with dependencies.
I use this [nightly build](https://nightlybuilds.ch/project/show/1/SFML) for VS2017 x32.

After downloading and unpacking, just copy this line to CMakeLists.txt:

```
set(CMAKE_PREFIX_PATH "<path-to-SFML>" ${CMAKE_PREFIX_PATH})
```

After this line.
```
# Detect and add SFML
```
And replace <path-to-SFML> to absolute path to unpacked SFML archive.
If you have higher version, update "cmake/FindSFML.cmake" file.
