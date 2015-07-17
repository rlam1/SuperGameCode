// MainGame.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "src\Tool\Init.h"
#include "src\tilemap.h"

ALLEGRO_DISPLAY     *display = nullptr;
ALLEGRO_EVENT_QUEUE *queue = nullptr;
ALLEGRO_TIMER       *timer = nullptr;
ALLEGRO_FONT        *textFont = nullptr;

InitData  applicationConfig;
StyleData styleConfig;

bool Init()
{
    if (LoadConfigurationData(applicationConfig, styleConfig) == false)
        return false;

    al_init();

    if (applicationConfig.bfullscreen == true)
    {
        al_set_new_display_flags(ALLEGRO_FULLSCREEN);
    }
    al_set_app_name(applicationConfig.strAppName.c_str());
    display = al_create_display(applicationConfig.iDispW, applicationConfig.iDispH);

    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    timer = al_create_timer(applicationConfig.dFps);
    queue = al_create_event_queue();
    textFont = al_load_ttf_font(styleConfig.strFontPath.c_str(), styleConfig.iFontSize, NULL);

    if ((display == nullptr) || (timer == nullptr) || (queue == nullptr) || (textFont == nullptr))
    {
        std::cout << "ERROR: Either the display, timer, queue or font failed to initialize!" << std::endl
            << "0x" << display << " 0x" << timer << " 0x" << queue << " 0x" << textFont << std::endl;
        return false;
    }

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    return true;
}

void AppBody()
{
    al_start_timer(timer);

    bool done = false;
    bool redraw = false;

    Tilemap map("data/tilesets/bricks");
    Tile *test = map.getTile(0, 4);
    std::cout << test << " " << test->gfxX << " " << test->gfxY << std::endl;

    while (!done)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);

        switch (ev.type)
        {
            case ALLEGRO_EVENT_TIMER:
                redraw = true;
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
            case ALLEGRO_EVENT_KEY_DOWN:
                if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                {
                    done = true;
                }
                //************
                else
                {
                    // NOTHING RIGHT NOW...
                }
                //************
                break;
            default:
                break;
        }

        if (redraw && al_is_event_queue_empty(queue))
        {
            redraw = false;
            al_clear_to_color(styleConfig.colBackgroundColor);
            al_draw_bitmap(map.getImage(), 0.0, 0.0, NULL);
            al_flip_display();
        }
    }
}

void Shutdown()
{
    al_destroy_display(display);
    al_destroy_event_queue(queue);
    al_destroy_timer(timer);
    al_destroy_font(textFont);

    std::cout << "Termination successfull, now quiting..." << std::endl;
}

int main(int argc, char** argv)
{
    if (Init() == false)
    {
        std::cout << "Fatal error! Closing application..." << std::endl;
        al_rest(10.0);
        return EXIT_FAILURE;
    }

    AppBody();
    Shutdown();

    al_rest(0.5);
    return EXIT_SUCCESS;
}

