#pragma once

const float PLAYER_RUNNING_SPEED = 5.f;
const float PLAYER_JUMPING_SPEED = 6.f;
const float GRAVITY = -10.0f;
const int BOX2D_VELOCITY_ITERATIONS = 6;
const int BOX2D_POSITION_ITERATIONS = 2;
const float BOX2D_TIME_STEP = 1.0f / 60.0f;
const float PIXELS_PER_METER = 5.f; // 1 meter is 5 pixels
const float METERS_PER_PIXEL = 1.f/PIXELS_PER_METER;