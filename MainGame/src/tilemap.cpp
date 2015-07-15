#include "stdafx.h"
#include "tilemap.h"

Tilemap::Tilemap(std::string filename)
{
    int tileSize = 16;
    set = new TileSet("bricks", tileSize);

    mapHeight = 5;
    mapWidth  = 5;

    int length = mapHeight * mapWidth;
    mapData = new int[length];
    map = al_create_bitmap(mapWidth*tileSize, mapHeight*tileSize);

    initTestMap();
    drawMap();
}

Tilemap::~Tilemap()
{
    delete set;
    delete [] mapData;
    al_destroy_bitmap(map);
}

ALLEGRO_BITMAP* Tilemap::getImage()
{
    return map;
}

void Tilemap::drawMap()
{
    al_set_target_bitmap(map);
    al_hold_bitmap_drawing(true);

    int length = mapHeight * mapWidth;
    int size = set->tileSize;
    int xcoord = 0, ycoord = 0, i = 0;

    for (int y = 0; y < mapHeight * size; y += size)
    {
        for (int x = 0; x < mapWidth * size; x += size)
        {
            if (i < length) // dont try to read beyond the array
            {
               int tileID = mapData[i];
               int sx = set->tileset[tileID].gfxX;
               int sy = set->tileset[tileID].gfxY;

               al_draw_bitmap_region(set->graphic, sx, sy, size, size, x, y, NULL);
               i++;
            }
        }
    }

    al_hold_bitmap_drawing(false);
    al_set_target_backbuffer(al_get_current_display());
}

void Tilemap::initTestMap()
{
    int length = mapHeight * mapWidth;

    for (int i = 0; i < length; i++)
    {
      mapData[i] = 0;
    }

    mapData[6] = 2;
    mapData[12] = 2;
    mapData[18] = 2;
}