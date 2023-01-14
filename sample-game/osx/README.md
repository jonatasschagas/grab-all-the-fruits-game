## Installation
Xcode Command Line Tools ( if not already installed )

 - xcode-select --install

Install cmake - download .dmg from official website and install

Dependencies to install

 - brew install sdl2
 - brew install sdl2_mixer
 - brew install sdl2_image --build-from-source // in order to match the libjpeg version.. meehhh...
 - brew install glm
 - brew install rapidjson
 - brew install freetype2

## Command Line Build

```
cd sample-game/osx
mkdir out
cd out
cmake ../
make
```

## On Visual Studio Code

Install "c++" and "cmake tools" extensions. Instructions here: https://code.visualstudio.com/docs/cpp/cmake-linux

Builds can be found in the root folder "build".

I've included the .vscode folder that has all the configurations to debug the sample-game on Mac Os (lldb, cmake, include paths, etc).

## Valgrind

In order to detect memory leaks use valgrind for macos: https://github.com/LouisBrunner/valgrind-macos
```
valgrind ./game.out
```

## GDB

To detect segfaults use GDB: 
```
brew install gdb
sudo gdb ./game.out
(gdb) run
(gdb) bt
```
https://stackoverflow.com/questions/2876357/determine-the-line-of-code-that-causes-a-segmentation-fault

## Art

The art of the sample-game can be found in the folder ```sample-game/assets```. The art
of the sample-game comes from here: https://pixelfrog-assets.itch.io/pixel-adventure-1.
Special thanks to "Pixel Frog" from itch.io for making this awesome art available.