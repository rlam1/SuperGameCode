#pragma once
#include "stdafx.h"
#include "tileset.h"

// http://www.cplusplus.com/forum/general/18315/
// https://en.wikipedia.org/wiki/Rule_of_three_(C%2B%2B_programming)

/*
TODO:

Tilemap holds a map of tiles that makes a world.
Each map is made of a specific tileset which holds each individual
tile and its position on the tileset.

The tileset calculates its data from the size value tilemap gives, and
the internal allegro routines to determine width and height. In the future it
will also check for properties of each tile (see tileset.cpp - LoadData().

Next step is connecting tilemap with tileset to load a sample and try to draw
it to a backbuffer.
*/

class Tilemap {
public:
    Tilemap(std::string filename);
    ~Tilemap();

    ALLEGRO_BITMAP* getImage();
private:
    TileSet *set;
    ALLEGRO_BITMAP *map;
    int mapHeight, mapWidth;
    int *mapData;

    void drawMap();
    void initTestMap();
};