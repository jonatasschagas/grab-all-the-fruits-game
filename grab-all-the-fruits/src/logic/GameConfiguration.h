#pragma once

#include <string>

using namespace std;

const float PLAYER_RUNNING_SPEED = 5.f;
const float PLAYER_JUMPING_FORCE = 6.f;
const float GRAVITY = -10.0f;
const float PIXELS_PER_METER = 5.f; // 1 meter is 5 pixels
const float METERS_PER_PIXEL = 1.f/PIXELS_PER_METER;

const string LEVELS_FILE = "assets/levels/levels.json";
const string OBJECTS_PATH = "assets/objects";