#include "stdafx.h"
#include "tilemap.h"

/*
TODO:

- Make the drawing calls here avoid using a buffer, consider drawing to the screen directly.
*/

void* al_img_loader2(const char* path)
{
    ALLEGRO_BITMAP *res = NULL;
    ALLEGRO_PATH   *alpath = NULL;

    if (!(alpath = al_create_path(path))) return NULL;

    al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_ANY_WITH_ALPHA);
    res = al_load_bitmap(al_path_cstr(alpath, ALLEGRO_NATIVE_PATH_SEP));

    al_destroy_path(alpath);

    return (void*)res;
}

TileMap::TileMap(std::string path)
{
    tmx_img_load_func = al_img_loader2;
    tmx_img_free_func = (void(*)(void*))al_destroy_bitmap;

    map = tmx_load(path.c_str());
    if (map == nullptr)
    {
        std::cerr << tmx_strerr() << std::endl;
        return;
    }

    int bmpWidth = map->tile_width * map->width;
    int bmpHeight = map->tile_height * map->height;
    fullMap = al_create_bitmap(bmpWidth, bmpHeight);
    if (fullMap == nullptr)
    {
        std::cerr << "Could not create a new bitmap resource! " << this << std::endl;
        tmx_map_free(map);
        return;
    }

    renderFullMap();

    int arrLength = map->height * map->width;
    walkTable = new int[arrLength];
    for (int i = 0; i < arrLength; i++)
    {
        walkTable[i] = 1; // Default to all spaces walkable in the tilemap
    }
    readWalkProperty(arrLength);
}

TileMap::~TileMap()
{
    tmx_map_free(map);
    al_destroy_bitmap(fullMap);
    delete[] walkTable;
}

ALLEGRO_BITMAP* TileMap::GetFullMap()
{
    return fullMap;
}

void TileMap::DrawLayerMap(std::string LayerName)
{
    tmx_layer *layer = getLayerByName(LayerName);

    if (layer == nullptr)
    {
        return; // Error message is printed by getLayerByName, so not needed here.
    }

    if (layer->visible)
    {
        identLayer(layer);
    }
}

bool TileMap::CanWalktoTileAt(Vec2D pixCoord, Vec2D pixSize, Vec2D offset)
{
	float tileW = map->tile_width;
	float tileH = map->tile_height;
	float x = (pixCoord.x + offset.x) / tileW;
	float y = (pixCoord.y + offset.y) / tileH;

    unsigned int range = (int)x * (int)y;
    if (range > ((map->height * map->width) - 1))
    {
        std::cerr << "Warning: Tile outside range: (" << (int)x << "," << (int)y << ")" <<std::endl;
        return false;
    }

    // arrayPosition = (y * numberOfColumns) + x
    int value = walkTable[((int)y * map->width) + (int)x];
	if (value > 0) // If its 0 or less (unlikely) you cannot walk, anything else is ok
	{
		return true; // can walk
	}
	else
	{

		/*public bool intersects(aabb other)
		{
			if (this.max.x < other.min.x ||
				this.max.y < other.min.y ||
				this.min.x > other.max.x ||
				this.min.y > other.max.y)
			{
				return false;
			}
			return true;
		}*/

		Vec2D tilePos = { x * tileW, y * tileH };
		Vec2D tileSize = { (x * tileW) + tileW, (y * tileH) + tileH };
		if (tileSize.x < pixCoord.x ||
			tileSize.y < pixCoord.y ||
			tilePos.x > pixSize.x ||
			tilePos.y > pixSize.y)
		{
			return false;
		}
		return true;
	}
}

ALLEGRO_COLOR TileMap::int_to_al_color(int color)
{
    unsigned char r, g, b;

    r = (color >> 16) & 0xFF;
    g = (color >> 8) & 0xFF;
    b = (color)& 0xFF;

    return al_map_rgb(r, g, b);
}

void TileMap::draw_polyline(double **points, double x, double y, int pointsC, ALLEGRO_COLOR color)
{
    int i;
    for (i = 1; i < pointsC; i++)
    {
        al_draw_line(x + points[i - 1][0], y + points[i - 1][1], x + points[i][0], y + points[i][1], color, LINE_THICKNESS);
    }
}

void TileMap::draw_polygone(double **points, double x, double y, int pointsC, ALLEGRO_COLOR color)
{
    draw_polyline(points, x, y, pointsC, color);
    if (pointsC > 2)
    {
        al_draw_line(x + points[0][0], y + points[0][1], x + points[pointsC - 1][0], y + points[pointsC - 1][1], color, LINE_THICKNESS);
    }
}

void TileMap::draw_objects(tmx_object *head, ALLEGRO_COLOR color)
{
    while (head)
    {
        if (head->visible)
        {
            switch (head->shape)
            {
                case S_SQUARE:
                    al_draw_rectangle(head->x, head->y, head->x + head->width, head->y + head->height, color, LINE_THICKNESS);
                    break;
                case S_POLYGON:
                    draw_polygone(head->points, head->x, head->y, head->points_len, color);
                    break;
                case S_POLYLINE:
                    draw_polyline(head->points, head->x, head->y, head->points_len, color);
                    break;
                case S_ELLIPSE:
                    al_draw_ellipse(head->x + head->width / 2.0, head->y + head->height / 2.0, head->width / 2.0, head->height / 2.0, color, LINE_THICKNESS);
                    break;
                default:
                    std::cerr << "Warning: An unknown object type was found: " << head->shape << std::endl;
                    break;
            }
        }

        head = head->next;
    }
}

int TileMap::gid_extract_flags(unsigned int gid)
{
    int res = 0;

    if (gid & TMX_FLIPPED_HORIZONTALLY) res |= ALLEGRO_FLIP_HORIZONTAL;
    if (gid & TMX_FLIPPED_VERTICALLY)   res |= ALLEGRO_FLIP_VERTICAL;
    /* FIXME allegro has no diagonal flip */
    return res;
}

int TileMap::gid_clear_flags(unsigned int gid)
{
    return gid & TMX_FLIP_BITS_REMOVAL;
}

void TileMap::draw_layer(tmx_map *map, tmx_layer *layer)
{
    unsigned long x, y; // The map coordinates
    unsigned int sx, sy, sw, sh, flags; // Individual tile coordinates
    float op; // Tile opacity value

    tmx_tileset *ts;
    ALLEGRO_BITMAP *tileset; /* Owned by the tmx library */

    op = layer->opacity;

    al_hold_bitmap_drawing(true);
    for (y = 0; y < map->height; y++)
    {
        for (x = 0; x < map->width; x++)
        {
            ts = tmx_get_tileset(map, layer->content.gids[(y*map->width) + x], &sx, &sy);
            if (ts)
            {
                sw = ts->tile_width;
                sh = ts->tile_height;

                tileset = (ALLEGRO_BITMAP*)ts->image->resource_image;
                flags = gid_extract_flags(layer->content.gids[(y*map->width) + x]);

                al_draw_tinted_bitmap_region(tileset, al_map_rgba_f(op, op, op, op),
                    sx, sy, sw, sh, x*ts->tile_width, y*ts->tile_height, flags);
            }
        }
    }
    al_hold_bitmap_drawing(false);
}

void TileMap::renderFullMap()
{
    tmx_layer *layers = map->ly_head;

    if (map->orient != O_ORT)
    {
        std::cerr << "Error: I only render Ortogonal maps!" << std::endl;
        return;
    }

    al_set_target_bitmap(fullMap);
    al_clear_to_color(int_to_al_color(map->backgroundcolor));

    while (layers)
    {
        identLayer(layers);
        layers = layers->next;
    }

    al_set_target_backbuffer(al_get_current_display());
}

tmx_layer *TileMap::getLayerByName(std::string name)
{
    tmx_layer *currLayer = map->ly_head;
    std::string layerName = currLayer->name;

    while (currLayer)
    {
        if (layerName == name)
        {
            return currLayer;
        } else
        {
            currLayer = currLayer->next;
            layerName = currLayer->name;
        }
    }

    std::cerr << "Error: Layer " << name << " not found." << std::endl;
    return nullptr;
}

void TileMap::readWalkProperty(int arrLength)
{
    tmx_layer *layer = map->ly_head;

    while (layer)
    {
        if (layer->type == L_LAYER)
        {
            for (int i = 0; i < arrLength; i++)
            {
                tmx_tile *tile = tmx_get_tile(map, layer->content.gids[i]);

                if (tile != nullptr)
                {
                    tmx_property *props = tile->properties;
                    while (props)
                    {
                        std::string name = props->name;
                        if (name == "canWalk")
                        {
                            walkTable[i] = atoi(props->value);
                        }
                        props = props->next;
                    }
                }
            }
        }

        layer = layer->next;
    }
}

// Decides how to render a layer. then renders it.
void TileMap::identLayer(tmx_layer *layer)
{
    switch (layer->type)
    {
        case L_OBJGR:
            draw_objects(layer->content.head, int_to_al_color(layer->color));
            break;
        case L_IMAGE:
            if (layer->opacity < 1.)
            {
                float op = layer->opacity;
                al_draw_tinted_bitmap((ALLEGRO_BITMAP*)layer->content.image->resource_image, al_map_rgba_f(op, op, op, op), 0, 0, 0); /* TODO: does not render at correct position */
            } else
            {
                al_draw_bitmap((ALLEGRO_BITMAP*)layer->content.image->resource_image, 0, 0, 0);
            }
            break;
        case L_LAYER:
            draw_layer(map, layer);
            break;
        default:
            std::cerr << "Warning: I just dropped an unknown layer: " << layer->name << std::endl;
            break;
    }
}
