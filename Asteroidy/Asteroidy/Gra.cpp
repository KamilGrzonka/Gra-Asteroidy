#include "MainHeader.h"

Gra::Gra(): szerokosc(1200), wysokosc(800){}

void Gra::start()
{
    app.create(VideoMode(szerokosc, wysokosc), "Asteroidy");
    app.setFramerateLimit(60);

    // Inicjalizacja tekstur i czcionek
    Texture t1, t2, t3, t4, t5, t6, t7;
    t1.loadFromFile("images/statek.png");
    t2.loadFromFile("images/tlo.png");
    t3.loadFromFile("images/eksplozja.png");
    t4.loadFromFile("images/asteroida.png");
    t5.loadFromFile("images/pocisk.png");
    t6.loadFromFile("images/eksplozja.png");
    t7.loadFromFile("images/mini_asteroida.png");

    font.loadFromFile("images/Minecraft.ttf");

    // Utworzenie obiektów gracza, asteroid, pocisków itp.
    Sprite sBackground(t2);
    /*sPlayer.setOrigin(20, 20);
    sPlayer.setPosition(app.getSize().x / 2, app.getSize().y / 2);*/
    //animacja statku
    Animacja sPlayer(t1, 132, 45, 40, 40, 1, 0);
    //animacja ruchu statku
    Animacja sPlayer_ruch(t1, 132, 45, 40, 40, 1, 0);
    //animacja eksplozji
    Animacja sExplosion(t3, 0, 0, 188, 188, 7, 0.2);
    Animacja sExplosion_Ship(t6, 0, 0, 188, 188, 7, 0.2);
    //animacja kamienia
    Animacja sRock(t4, 0, 0, 128, 128, 12, 0.2);
    Animacja sRock_mini(t7, 0, 0, 64, 64, 12, 0.2);
    //Animacja Pocisku
    Animacja sBullet(t5, 0, 0, 48, 48, 20, 0.8);


    //generowanie kamienia
    std::list<Entity*> byty;
    for (int i = 0; i < 5; i++) {
        Asteroida* a = new Asteroida();
        a->settings(sRock, rand() % szerokosc, rand() % wysokosc, rand() % 360, 25);
        byty.push_back(a);
    }

    Gracz* g = new Gracz();
    g->settings(sPlayer, szerokosc / 2, wysokosc / 2, 0, 20);
    byty.push_back(g);

    while (app.isOpen())
    {
        Event event;
        while (app.pollEvent(event))
        {
            if (event.type == Event::Closed)
                app.close();
            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Space)
                {
                    Pocisk* p = new Pocisk();
                    p->settings(sBullet, g->x, g->y, g->angle, 10);
                    byty.push_back(p);
                }
            }
        }
        //sterowanie
        if (Keyboard::isKeyPressed(Keyboard::D)) g->angle += 3;
        if (Keyboard::isKeyPressed(Keyboard::A)) g->angle -= 3;
        if (Keyboard::isKeyPressed(Keyboard::W)) {
            g->naped = true;
            g->hamowanie = false; // Wy³¹czamy hamowanie przy wciœniêciu klawisza W
        }
        else if (Keyboard::isKeyPressed(Keyboard::S)) {
            g->hamowanie = true; // W³¹czamy hamowanie przy wciœniêciu klawisza S
            g->naped = false; // Wy³¹czamy napêd
        }
        else {
            g->naped = false;
            g->hamowanie = false;
        }
        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            app.close();
        }

        for (auto a : byty) {
            for (auto b : byty) {
                if (a->nazwa == "asteroida" && b->nazwa == "pocisk")
                    if (kolizja(a, b)) {
                        a->zycie = false;
                        b->zycie = false;
                        score++;
                        Asteroida* e = new Asteroida();
                        e->settings(sExplosion, a->x, b->y);
                        e->nazwa = "eksplozja";
                        byty.push_back(e);

                        for (int i = 0; i < 2; i++) {
                            if (a->R == 15) continue;
                            Entity* e = new Asteroida();
                            e->settings(sRock_mini, a->x, a->y, rand() % 360, 15);
                            byty.push_back(e);
                        }
                    }
                if (a->nazwa == "gracz" && b->nazwa == "asteroida") {
                    if (kolizja(a, b)) {
                        b->zycie = false;
                        Gracz* e = new Gracz();
                        e->settings(sExplosion_Ship, a->x, b->y);
                        e->nazwa = "eksplozja";
                        byty.push_back(e);

                        showGameOverScreen(app, font, score);

                        g->settings(sPlayer, szerokosc / 2, wysokosc / 2, 0, 20);
                        g->dx = 0;
                        g->dx = 0;

                    }
                }
            }
        }

        if (g->naped) g->anim = sPlayer_ruch;
        else g->anim = sPlayer;

        for (auto e : byty) {
            if (e->nazwa == "eksplozja") {
                if (e->anim.koniec()) e->zycie = false;
            }
        }

        if (rand() % 150 == 0) {
            Asteroida* a = new Asteroida();
            a->settings(sRock, 0, rand() % wysokosc, rand() % 360, 25);
            byty.push_back(a);
        }


        for (auto i = byty.begin(); i != byty.end();) {
            Entity* e = *i;
            e->update();
            e->anim.update();

            if (e->zycie == false)
            {
                i = byty.erase(i);
                delete e;
            }
            else i++;
        }
        sf::Text punktyText;
        punktyText.setFont(font);
        punktyText.setCharacterSize(30);
        punktyText.setFillColor(Color::White);
        punktyText.setString("Punkty: " + std::to_string(getPunkty()));
        punktyText.setPosition(10, 10);
        app.clear();
        app.draw(sBackground);
        app.draw(punktyText);
        for (auto i : byty)
            i->draw(app);
        app.display();
    }
}


void Gra::showGameOverScreen(RenderWindow& app, Font& font, int score) {
    Text text;
    text.setFont(font);
    text.setString("Game Over");
    text.setCharacterSize(60);
    FloatRect textBounds = text.getLocalBounds();
    text.setPosition(600 - textBounds.width / 2, 100);

    Text wynik;
    wynik.setFont(font);
    wynik.setString("Twoj wynik: " + std::to_string(score));
    wynik.setCharacterSize(60);
    FloatRect wynikBounds = wynik.getLocalBounds();
    wynik.setPosition(600 - wynikBounds.width / 2, 200);

    Text backButton;
    backButton.setFont(font);
    backButton.setString("Wyjdz z gry");
    backButton.setCharacterSize(40);
    FloatRect backBounds = backButton.getLocalBounds();
    backButton.setPosition(600 - backBounds.width / 2, 400);
    while (app.isOpen()) {
        Event event;
        while (app.pollEvent(event)) {
            if (event.type == Event::Closed) {
                app.close();
                return;
            }
            else if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left) {
                    Vector2i mousePosition = Mouse::getPosition(app);
                    if (backButton.getGlobalBounds().contains(Vector2f(mousePosition))) {
                        zapisz_wynik();
                        app.close();
                    }
                }
            }
        }

        app.clear();
        app.draw(text);
        app.draw(wynik);
        app.draw(backButton);
        app.display();
    }
}

void Gra::zapisz_wynik() {
    std::filesystem::path scoresFile("images/scores.txt");
    std::ofstream file(scoresFile, std::ios::app);

    if (file.is_open()) {
        std::string playerName;

        std::cout << "Podaj swoje imie: ";
        std::cin >> playerName;

        file << playerName << ": " << score << "\n";

        file.close();
    }
    else {
        std::cout << "Nie mozna otworzyc pliku z wynikami.\n";
    }
}