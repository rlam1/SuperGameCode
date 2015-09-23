// Copyright(c) 2015, Rodolfo Lam
// All rights reserved.

#include "stdafx.h"
#include "Init.h"

bool LoadConfigurationData(InitData& init, StyleData& style)
{
    ALLEGRO_CONFIG *configFile = nullptr;

    configFile = al_load_config_file("data/config/config.ini");
    if (configFile == nullptr)
    {
        al_show_native_message_box(nullptr, "A fatal error has ocurred!",
            "The configuration file has not been found!",
            "Be sure to always have a valid conf.ini in /data/config/ for the program to work...", "Understood", ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    //Populate the Application datablock
    init.strAppName = al_get_config_value(configFile, "VERSION", "APPNAME");
    init.dFps = std::stod(al_get_config_value(configFile, "DISPLAY", "TARGETFPS"));
    init.iDispW = std::stoi(al_get_config_value(configFile, "DISPLAY", "RESX"));
    init.iDispH = std::stoi(al_get_config_value(configFile, "DISPLAY", "RESY"));
    init.bfullscreen = std::stoi(al_get_config_value(configFile, "DISPLAY", "FULLSCREEN"));

    init.dFps = 1.0 / init.dFps;

    //Populate Style datablock
    style.strFontPath = al_get_config_value(configFile, "STYLE", "FONT");
    style.colBackgroundColor = StrToCol(al_get_config_value(configFile, "STYLE", "BGCOLOR"));
    style.colTextColor = StrToCol(al_get_config_value(configFile, "STYLE", "TEXTCOLOR"));
    style.iFontSize = std::stoi(al_get_config_value(configFile, "STYLE", "FONTSIZE"));

    al_destroy_config(configFile);
    return true;
}

ALLEGRO_COLOR StrToCol(std::string strColorData)
{
    if (strColorData.empty() == true)
    {
        return al_map_rgb(255, 0, 255);
    } else
    {
        return al_color_html(strColorData.c_str());
    }
}
