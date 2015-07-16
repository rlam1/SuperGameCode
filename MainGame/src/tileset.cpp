#include "stdafx.h"
#include "tileset.h"
#include <fstream>

// Loads a tileset imga to video memory
TileSet::TileSet(std::string filename, int size)
{
    std::string res = "data/tilesets/";
    res += filename;
    //res += ".png\0";
    graphic = al_load_bitmap(res.c_str());
    if (graphic == nullptr)
    {
        tileset = nullptr;
        tileSize = 0;
        setHeight = 0;
        setWidth = 0;
        return;
    }

    tileSize = size;
    setHeight = al_get_bitmap_height(graphic) / tileSize;
    setWidth = al_get_bitmap_width(graphic) / tileSize;

    tileset = new Tile[setWidth * setHeight];
    LoadData(filename);
}

TileSet::~TileSet()
{
    al_destroy_bitmap(graphic);
    delete [] tileset;
}

void TileSet::LoadData(std::string filename)
{
    // filename will be used in the future to store Tile properties
    // in a separate filename.til file. (Wall, water, object, etc)
    // For the time bein this just calculates individual tile coords.

    int i = 0;

    for (int y = 0; y < setHeight; y++)
    {
        for (int x = 0; x < setWidth; x++)
        {
            int coordX, coordY;
            coordX = x * tileSize;
            coordY = y * tileSize;
            tileset[i] = { coordX, coordY };
            i++;
        }
    }
}