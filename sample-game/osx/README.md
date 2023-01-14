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