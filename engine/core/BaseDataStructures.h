#pragma once
#ifndef BASEDATASTRUCTURES_H
#define BASEDATASTRUCTURES_H

class GamePoint
{
public:
    GamePoint() {};
    GamePoint(float _x, float _y) : x(_x), y(_y) {};
	float x;
	float y;
};

class GameSize
{
public:
    GameSize(){}
    GameSize(float _w, float _h) : w(_w), h(_h) {};
	float w;
	float h;
};

class Quad
{
public:

	Quad(float _x, float _y, float _w, float _h) : x(_x), y(_y), w(_w), h(_h) {};

	float x;
	float y;
	float w;
	float h;
};

struct Tile
{
	Tile(int _x, int _y) : x(_x), y(_y) {};
	Tile() : x(-1), y(-1) {};

	int x;
	int y;
};

#endif //BASEDATASTRUCTURES_H
