#include "MainHeader.h"
Entity::Entity() {
    zycie = 1;
}
void Entity::settings(Animacja& a, int X, int Y, float Angle, int promien) {
    x = X;
    y = Y;
    anim = a;
    angle = Angle;
    R = promien;
};
void Entity::draw(RenderWindow& app) {
    anim.sprite.setPosition(x, y);
    anim.sprite.setRotation(angle + 90);
    app.draw(anim.sprite);
}

bool kolizja(Entity* a, Entity* b) {
    return(b->x - a->x) * (b->x - a->x) + (b->y - a->y) * (b->y - a->y) < (a->R + b->R) * (a->R + b->R);
}


Asteroida::Asteroida() {
    dx = rand() % 8 - 4;
    dy = rand() % 8 - 4;
    nazwa = "asteroida"
        ;
}
void Asteroida::update(){
    x += dx;
    y += dy;
    if (x > szerokosc) x = 0;
    if (x < 0) x = szerokosc;
    if (y > wysokosc) y = 0;
    if (y < 0) y = wysokosc;
}

Pocisk::Pocisk() {
    nazwa = "pocisk";
}
void Pocisk::update(){
    dx = cos(angle * degtorad) * 6;
    dy = sin(angle * degtorad) * 6;
    //angle += rand() % 7 - 3;                  gibanie sie pociskow
    x += dx;
    y += dy;
    if (x > szerokosc || x<0 || y>wysokosc || y < 0) zycie = 0;
}

Gracz::Gracz() {
    nazwa = "gracz";
}

void Gracz::update(){
    //ruch
    if (naped) {
        dx += cos(angle * degtorad) * 0.2;
        dy += sin(angle * degtorad) * 0.2;
    }
    else if (hamowanie) { // Je¿eli hamowanie jest aktywne
        dx *= 0.975;
        dy *= 0.975;
    }
    else {
        dx *= 0.995; // Zmniejszamy prêdkoœæ zwyk³ego poruszania
        dy *= 0.995;
    }

    int maxSpeed = 15;
    float speed = sqrt(dx * dx + dy * dy);
    if (speed > maxSpeed) {
        dx *= maxSpeed / speed;
        dy *= maxSpeed / speed;
    }
    x += dx;
    y += dy;
    if (x > szerokosc) x = 0;
    if (x < 0) x = szerokosc;
    if (y > wysokosc) y = 0;
    if (y < 0) y = wysokosc;
}

