#include "MainHeader.h"

int main() {
    RenderWindow app(VideoMode(1200, 800), "Menu Glowne");
    Font font;
    font.loadFromFile("images/Minecraft.ttf");

    Menu menu(font);
    Instrukcja instrukcja(font);
    //GameOver gameover(font);
    TablicaWynikow scoreboard(font);

    StanGry stan_gry = StanGry::Menu;

    while (app.isOpen()) {
        Event event;
        while (app.pollEvent(event)) {
            if (event.type == Event::Closed) {
                app.close();
            }
            else if (event.type == Event::MouseButtonPressed) {
                    Vector2i mousePosition = Mouse::getPosition(app);
                    if (stan_gry == StanGry::Menu) {
                        if (menu.isPlayButtonPressed(mousePosition)) {
                            app.close();
                            Gra gra;
                            gra.start();
                        
                        }
                        else if (menu.isInstructionButtonPressed(mousePosition)) {
                            stan_gry = StanGry::Instrukcja;
                        }
                        else if (menu.isScoreBoardButtonPressed(mousePosition)) {
                            stan_gry = StanGry::Scoreboard;
                        }
                        else if (menu.isQuitButtonPressed(mousePosition)) {
                            stan_gry = StanGry::Wyjscie;
                        }
                    }
                    else if (stan_gry == StanGry::Instrukcja) {
                        if (instrukcja.isBackClicked(mousePosition)) {
                            stan_gry = StanGry::Menu;
                        }
                    }

                    else if (stan_gry == StanGry::Scoreboard) {
                        if (scoreboard.isBackClicked(mousePosition)) {
                            stan_gry = StanGry::Menu;
                        }
                    }
                   
                    else if (stan_gry == StanGry::Wyjscie) {
                        app.close();
                    }
                
            }

            app.clear();

            if (stan_gry == StanGry::Menu) {
                menu.draw(app);
            }
            else if (stan_gry == StanGry::Instrukcja) {
                instrukcja.draw(app);
            }
            else if (stan_gry == StanGry::Scoreboard) {
                scoreboard.draw(app);
            }
            else if (stan_gry == StanGry::Wyjscie) {
                app.close();
            }
            

            app.display();
        }
        
    }
    return 0;
}

