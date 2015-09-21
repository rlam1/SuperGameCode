// MainGame.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "src\Tool\Init.h"
#include "src\tilemap.h"
#include "src\Tool\Sprite.h"

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
    al_set_new_display_option(ALLEGRO_VSYNC, 1, ALLEGRO_REQUIRE);
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
    bool done = false;
    bool redraw = false;

    TileMap map("data/maps/test1.tmx");
    Sprite sprt("data/sprites/demo.adf");
    sprt.SendNewState(AnimState::IDLE, AnimDir::DOWN);
    
    ALLEGRO_TRANSFORM camera;

    int x_offset = 0, y_offset = 0;
    int x_delta, y_delta;
    int key_state[2] = { 0, 0 };

    x_delta = applicationConfig.iDispW - al_get_bitmap_width(map.GetFullMap());
    y_delta = applicationConfig.iDispH - al_get_bitmap_height(map.GetFullMap());

    Vec2D playerPos{ 0, 0 };
    Vec2D playerVel{ 0, 0 };

    al_start_timer(timer);
    while (!done)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);

        switch (ev.type)
        {
            case ALLEGRO_EVENT_TIMER:
                al_identity_transform(&camera);

                x_offset += key_state[0];
                y_offset += key_state[1];

                if (x_delta > 0)
                {
                    x_offset = x_delta / 2;
                } else
                {
                    if (x_offset < x_delta) x_offset = x_delta;
                    if (x_offset > 0) x_offset = 0;
                }
                if (y_delta > 0)
                {
                    y_offset = y_delta / 2;
                } else
                {
                    if (y_offset < y_delta) y_offset = y_delta;
                    if (y_offset > 0) y_offset = 0;
                }

                int playTileX, playTileY;
                playTileX = playerPos.x / 32;  // 32 is the current tile size (32x32)
                playTileY = (playerPos.y + 32) / 32;

                if (map.CanWalktoTileAt(playTileX, playTileY) == true)
                {
                    playerPos = playerPos + playerVel;
                } else
                {
					Vec2D backtracing = playerVel;
					playerVel = { 0, 0 }; // we delete player velocitity to stop oscillating in place during collision.

					while (map.CanWalktoTileAt(playTileX, playTileY) == false)
					{
						playerPos = playerPos - backtracing;
						playTileX = playerPos.x / 32;  // 32 is the current tile size (32x32)
						playTileY = (playerPos.y + 32) / 32;
					}
                }

                sprt.Update();
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

                switch (ev.keyboard.keycode)
                {
                    case ALLEGRO_KEY_LEFT:  key_state[0] = 4; break;
                    case ALLEGRO_KEY_RIGHT: key_state[0] = -4; break;
                    case ALLEGRO_KEY_UP:    key_state[1] = 4; break;
                    case ALLEGRO_KEY_DOWN:  key_state[1] = -4; break;

                    case ALLEGRO_KEY_A:     
                        playerVel.x = -2;
                        sprt.SendNewState(AnimState::MOVE, AnimDir::LEFT);
                        break;
                    case ALLEGRO_KEY_D:     
                        playerVel.x = 2;
                        sprt.SendNewState(AnimState::MOVE, AnimDir::RIGHT);
                        break;
                    case ALLEGRO_KEY_W:     
                        playerVel.y = -2;
                        sprt.SendNewState(AnimState::MOVE, AnimDir::UP);
                        break;
                    case ALLEGRO_KEY_S:     
                        playerVel.y = 2;
                        sprt.SendNewState(AnimState::MOVE, AnimDir::DOWN);
                        break;
                }

                break;
            case ALLEGRO_EVENT_KEY_UP:
                switch (ev.keyboard.keycode)
                {
                    case ALLEGRO_KEY_LEFT:
                    case ALLEGRO_KEY_RIGHT: key_state[0] = 0; break;
                    case ALLEGRO_KEY_UP:
                    case ALLEGRO_KEY_DOWN:  key_state[1] = 0; break;

                    case ALLEGRO_KEY_A:     
                        playerVel.x = 0;
                        sprt.SendNewState(AnimState::IDLE, AnimDir::LEFT);
                        break;
                    case ALLEGRO_KEY_D:  
                        playerVel.x = 0;
                        sprt.SendNewState(AnimState::IDLE, AnimDir::RIGHT);
                        break;
                    case ALLEGRO_KEY_W: 
                        playerVel.y = 0;
                        sprt.SendNewState(AnimState::IDLE, AnimDir::UP);
                        break;
                    case ALLEGRO_KEY_S:     
                        playerVel.y = 0;
                        sprt.SendNewState(AnimState::IDLE, AnimDir::DOWN);
                        break;
                }
                break;
            default:
                break;
        }

        if (redraw && al_is_event_queue_empty(queue))
        {
            al_translate_transform(&camera, x_offset, y_offset);
            al_use_transform(&camera);

            redraw = false;
            al_clear_to_color(styleConfig.colBackgroundColor);

            map.DrawLayerMap("Background");
            map.DrawLayerMap("Foreground");
            /*
            Playable characters should draw on this position
            */
            sprt.Render(playerPos.x, playerPos.y);
            map.DrawLayerMap("Top");

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

