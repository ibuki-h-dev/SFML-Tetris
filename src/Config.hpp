#pragma once
#include<string>
#include <SFML/Graphics.hpp>

#ifndef ASSET_DIR
#define ASSET_DIR "assets/"
#endif

namespace Config{
    inline constexpr unsigned int ScreenWidth = 1600u;
    inline constexpr unsigned int ScreenHeight = 800u;
    inline constexpr unsigned int nextWindowWidth = 120u;
    inline constexpr unsigned int nextiWindowHeight = 500u;
    inline constexpr unsigned int holdWindowWidth = 120u;
    inline constexpr unsigned int holdWindowHeight = 120u;
    inline constexpr unsigned int Width = 10u;
    inline constexpr unsigned int Height = 20u;
    inline constexpr unsigned int blank = 3u;


    inline constexpr unsigned int Board_X = 650u;
    inline constexpr unsigned int Board_Y = 100u;
    inline constexpr float CellSize = 30.0f;
    inline constexpr float Board_X_Extra = 160.0f;
    inline constexpr float Board_Y_Extra = 30.0f;
    inline constexpr float nextWindow_X = Board_X+CellSize*Width+(Board_X_Extra-nextWindowWidth)/2;
    inline constexpr float nextWindow_Y = Board_Y+(CellSize*(Height-blank)+2*Board_Y_Extra-nextiWindowHeight)/2;
    inline constexpr float holdWindow_X = Board_X-Board_X_Extra/2-holdWindowWidth/2;
    inline constexpr float holdWindow_Y = Board_Y+Board_Y_Extra;

    inline constexpr float defaultDelay = 0.5f;
    inline constexpr float dasDelay = 0.2f;
    inline constexpr float arrDelay = 0.1f;
    inline constexpr float lockDelay = 0.5f;

    inline constexpr unsigned int pointPerCorner = 20u;
    inline constexpr unsigned int pointPerCorner_NW = 4u;

    const sf::Color plateColor = sf::Color(170,255,255,220);
    const sf::Color nextWindowColor = sf::Color::Black;
    const sf::Color outLineColor= sf::Color::White;

    const sf::Color BG_COLOR = sf::Color(255, 255, 245);

    const std::string font = std::string(ASSET_DIR)+"X14Y24PXHEADUPDAISY.TTF";
    const sf::Color fontColor = sf::Color::Black;

}