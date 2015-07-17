#include "stdafx.h"
#include "tilemap.h"

Tilemap::Tilemap(std::string filename)
{
    ALLEGRO_CONFIG *configFile = nullptr;
    configFile = al_load_config_file(filename.c_str());
    if (configFile == nullptr)
    {
        std::cerr << "Invalid configuration state! " << this << " will be invalid!" << std::endl;
        return;
    }

    zeroConfData(); // put default values in all fields.
    confData.tSize = std::stoi(al_get_config_value(configFile, NULL, "TILE_SIZE"));
    confData.mHeight = std::stoi(al_get_config_value(configFile, NULL, "MAP_HEIGHT"));
    confData.mWidth = std::stoi(al_get_config_value(configFile, NULL, "MAP_WIDTH"));
    confData.special = std::stoi(al_get_config_value(configFile, NULL, "SPECIAL"));
    confData.tilesetName = al_get_config_value(configFile, NULL, "TILESET_RES_LOC");
    confData.tiles = al_get_config_value(configFile, NULL, "TILE_DATA");
    al_destroy_config(configFile);
    // Write values to memory now:

    set = new TileSet(confData.tilesetName, confData.tSize);
    mapHeight = confData.mHeight;
    mapWidth = confData.mWidth;
    special = confData.special;

    int length = mapWidth * mapHeight;
    mapData = new int[length];
    map = al_create_bitmap(mapWidth * confData.tSize, mapHeight * confData.tSize);

    std::stringstream dataStream(confData.tiles);
    int value = 0;
    int i = 0;
    while (dataStream >> value)
    {
        if (i < length)
        {
            mapData[i] = value;
            i++;
        }
    }

    /*
    In case the mapData is corrupt, at least fill with a valid value.
    map data will be corrupted and possibly render game unplayable, but
    at least it will not crash hard on the player with an invalid pointer
    ttrying to read other memory regions.
    */
    while (i < length)
    {
        mapData[i] = special;
        i++;
    }

    drawMap();
}

Tilemap::Tilemap()
{
    int tileSize = 16;
    set = new TileSet("bricks", tileSize);

    mapHeight = 5;
    mapWidth = 5;

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
               // if tileID != special
               if (tileID != special)
               {
                   int sx = set->tileset[tileID].gfxX;
                   int sy = set->tileset[tileID].gfxY;

                   al_draw_bitmap_region(set->graphic, sx, sy, size, size, x, y, NULL);
               }
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

void Tilemap::zeroConfData()
{
    confData = { 0, 0, 0, 0, "", "" };
}