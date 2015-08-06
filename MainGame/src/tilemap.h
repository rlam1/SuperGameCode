#pragma once
#include "stdafx.h"
#include <tmx.h>

void* al_img_loader2(const char* path);

class TileMap {
public:
    TileMap(std::string path);
    ~TileMap();

    ALLEGRO_BITMAP* GetFullMap();
    //ALLEGRO_BITMAP* GetLayerMap(std::string LayerName);
    void DrawLayerMap(std::string LayerName);

    bool CanWalktoTileAt(int x, int y);
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

    /* Internal Variables */
    const float LINE_THICKNESS = 2.5;
    tmx_map *map;
    ALLEGRO_BITMAP *renderSurface; // A scratchpath for rendering specific parts of the map
    ALLEGRO_BITMAP *fullMap;       // Prerendered full map;
    int *walkTable;                // Precalculated array holding tile walkability.
};