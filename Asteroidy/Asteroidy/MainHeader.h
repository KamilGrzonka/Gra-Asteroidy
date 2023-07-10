#pragma once
#include <future>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <regex>
#include <filesystem>
#include <thread>
#include <future>
using namespace sf;

const int szerokosc = 1200;
const int wysokosc = 800;
const float degtorad = 0.017453f;

enum class StanGry {
    Menu,
    Instrukcja,
    Wyjscie,
    Gameover,
    Scoreboard
};

class Animacja {
public:
    float klatka, predkosc;
    Sprite sprite;
    std::vector<IntRect> frames;
    Animacja();
    Animacja(Texture& t, int x, int y, int w, int h, int licznik, float speed);
    void update();
    bool koniec();
};

class Entity {
public:
    float x, y, dx, dy, R, angle;
    bool zycie;
    std::string nazwa;
    Animacja anim;
    Entity();
    void settings(Animacja& a, int X, int Y, float Angle = 0, int promien = 1);
    virtual void update() = 0;
    void draw(RenderWindow& app);
};

class Asteroida : public Entity {
public:
    Asteroida();
    void update() override;
};

class Pocisk : public Entity {
public:
    Pocisk();
    void update() override;
};

class Gracz : public Entity {
public:

    bool naped = false; // Pocz¹tkowo naped jest wy³¹czony
    bool hamowanie = false; // Pocz¹tkowo hamowanie jest wy³¹czone
    Gracz();
    void update() override;

};

bool kolizja(Entity* a, Entity* b);

class Gra : public Animacja, Gracz, Pocisk, Asteroida
{
public:
    Gra();
    void start();
    void showGameOverScreen(RenderWindow& app, Font& font, int score);
   // void wczytaj_wynik();
    void zapisz_wynik();
    //void update() override;
    int getPunkty() const { return score; }
private:
    int szerokosc;
    int wysokosc;
    int score = 0;
    RenderWindow app;
    Sprite sBackground;
    std::list<Entity*> byty;
    Texture t1, t2, t3, t4, t5, t6, t7;
    Font font;
    Animacja sPlayer, sPlayer_ruch, sExplosion, sExplosion_Ship, sRock, sRock_mini, sBullet;
    Gracz* g = new Gracz();
};

class Menu : public Gra{
private:
    Font font;
    Text titleText;
    Text playButton;
    Text instructionButton;
    Text scoreboardButton;
    Text quitButton;
    Texture tm;
    Sprite sMenu;

public:
    Menu(Font& font);
    void draw(RenderWindow& app);
    bool isPlayButtonPressed(const Vector2i& mousePosition);
    bool isScoreBoardButtonPressed(const Vector2i& mousePosition);
    bool isInstructionButtonPressed(const Vector2i& mousePosition);
    bool isQuitButtonPressed(const Vector2i& mousePosition);
};

class Instrukcja {
private:
    Font font;
    Text instructionText;
    Text backText;
    Texture instructionTexture;
    Sprite instructionSprite;
    bool textureLoaded;
    std::thread textureLoadingThread;
    std::mutex textureMutex;
    void loadTextureAsync();
public:
    Instrukcja(const Font& font);
    ~Instrukcja();
    void draw(RenderWindow& app);

    bool isBackClicked(const Vector2i& mousePosition);
};

class TablicaWynikow {
private:
    Font font;
    Text scoreboardText;
    Text backText;
    Text scoresText;
    Texture tm;
    Sprite sMenu;

public:
    TablicaWynikow(const Font& font);
    void draw(RenderWindow& app);

    bool isBackClicked(const Vector2i& mousePosition);
    void WczytajWynik();
    
};