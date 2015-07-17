#pragma once
#include "stdafx.h"
#include "tileset.h"

#include <sstream>

// http://www.cplusplus.com/forum/general/18315/
// https://en.wikipedia.org/wiki/Rule_of_three_(C%2B%2B_programming)

/* Tilemap System
Tilemap holds a map of tiles that makes a world.
Each map is made of a specific tileset which holds each individual
tile and its position on the tileset.

The tileset calculates its data from the size value tilemap gives, and
the internal allegro routines to determine width and height. In the future it
will also check for properties of each tile (see tileset.cpp - LoadData().
*/

/*
TODO:
 ... Blank right now
*/

struct conf {
    int tSize;
    int mHeight, mWidth;
    int special; // special Tile that is not valid that prevents one cell draw.
    std::string tilesetName;
    std::string tiles;
};

class Tilemap {
public:
    Tilemap(); // For generating a test map.
    Tilemap(std::string filename);
    ~Tilemap();

    ALLEGRO_BITMAP* getImage();
    Tile* getTile(int x, int y);

private:
    TileSet *set;
    ALLEGRO_BITMAP *map;
    int mapHeight, mapWidth, special;
    int *mapData;

    void drawMap();
    void initTestMap();
    void zeroConfData();

    conf confData;
};