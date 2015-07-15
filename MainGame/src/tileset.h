#pragma once
#include "stdafx.h"

struct Tile {
    int gfxX;
    int gfxY;
};

struct TileSet {
    TileSet(std::string filename, int size);
    ~TileSet();
    void LoadData(std::string filename);

    Tile *tileset;
    ALLEGRO_BITMAP *graphic;
    int tileSize, setHeight, setWidth;
};