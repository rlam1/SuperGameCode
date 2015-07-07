// MainGame.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int main(int argc, char** argv)
{
    al_init();
    
    ALLEGRO_DISPLAY *display = nullptr;
    display = al_create_display(800, 600);

    if (display == nullptr)
    {
        return EXIT_FAILURE;
    }

    ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);
    al_clear_to_color(white);
    al_flip_display();
    al_rest(5.0);
    return EXIT_SUCCESS;
}

