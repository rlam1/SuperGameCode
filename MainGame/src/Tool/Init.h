// Copyright(c) 2015, Rodolfo Lam
// All rights reserved.

// Helper functions to read the configuration file
// to memory.

#pragma once
#include "../../stdafx.h"

struct InitData {
    InitData()
    {
        strAppName = "Application";
        dFps = 0.0;
        iDispH = 0;
        iDispW = 0;
        bfullscreen = false;
    }

    std::string strAppName;
    double dFps;
    int  iDispW;
    int  iDispH;
    bool bfullscreen;
};

struct StyleData {
    StyleData()
    {
        iFontSize = 0;
        // I trust std::string ;)
    }

    std::string strFontPath;
    ALLEGRO_COLOR colBackgroundColor;
    ALLEGRO_COLOR colTextColor;
    int iFontSize;
};

bool LoadConfigurationData(InitData& init, StyleData& style);
ALLEGRO_COLOR StrToCol(std::string strColorData);
