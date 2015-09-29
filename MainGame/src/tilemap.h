// Copyright(c) 2015, Rodolfo Lam
// All rights reserved.

#pragma once
#include "stdafx.h"
#include <tmx.h>

void* al_img_loader2(const char* path);

class TileMap {
class TileMap
{
public:
    TileMap(std::string path);
    ~TileMap();

    ALLEGRO_BITMAP* GetFullMap();
    void DrawLayerMap(std::string LayerName);

    /*
        pixCoord = Player's position in pixels
        pixSize  = Player's hitbox size
        offset   = Used when only a specific part of the player has a hitbox.
        */
    bool CanWalktoTileAt(Vec2D pixCoord, Vec2D pixSize, Vec2D offset = {0.0, 0.0, });
private:
    ALLEGRO_COLOR int_to_al_color(int color);

    /* Object Drawing routines */
    void draw_polyline(double **points, double x, double y, int pointsc, ALLEGRO_COLOR color);
    void draw_polygone(double **points, double x, double y, int pointsc, ALLEGRO_COLOR color);
    void draw_objects(tmx_object *head, ALLEGRO_COLOR color);

    /* Tile Layer drawing routines */
    int gid_extract_flags(unsigned int gid);
    int gid_clear_flags(unsigned int gid);
    void draw_layer(tmx_map *map, tmx_layer *layer);

    /* Map Rendering functions */
    void renderFullMap();

    /* Map traversal utilities */
    tmx_layer *getLayerByName(std::string name);
    void readWalkProperty(int arrLength);
    void identLayer(tmx_layer *layer);

    /* Internal Variables */
    const float LINE_THICKNESS = 2.5;
    tmx_map *map;
    ALLEGRO_BITMAP *fullMap;       // Prerendered full map;
    int *walkTable;                // Precalculated array holding tile walkability.
};