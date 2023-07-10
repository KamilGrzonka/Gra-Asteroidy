#include "MainHeader.h"
Animacja::Animacja() {};

Animacja::Animacja(Texture& t, int x, int y, int w, int h, int licznik, float speed) {
    klatka = 0;
    predkosc = speed;

    for (int i = 0; i < licznik; i++) frames.push_back(IntRect(x + i * w, y, w, h));
    sprite.setTexture(t);
    sprite.setOrigin(w / 2, h / 2);
    sprite.setTextureRect(frames[0]);

}

void Animacja::update() {
    klatka += predkosc;
    int n = frames.size();
    if (klatka >= n) klatka -= n;
    if (n > 0)sprite.setTextureRect(frames[int(klatka)]);
}

bool Animacja::koniec() {
    return klatka + predkosc >= frames.size();
}