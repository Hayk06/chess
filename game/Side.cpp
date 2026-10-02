//
// Created by Hayk Derdzyan on 12.09.26.
//

#include "Side.h"
#include <iostream>
#include <random>

Side::Side() {
    this->color = Color::NONE;
    this->captured_points = 0;
}

void Side::randomSide() {
    static std::mt19937 engine{ std::random_device{}() };
    static std::bernoulli_distribution coin(0.5);
    color = coin(engine) ? Color::WHITE : Color::BLACK;
}

void Side::setColor(Color side) {
    color = side;
}

Color Side::getColor() const {
    return color;
}

void Side::set_captured_points(int points) {
    captured_points = points;
}

int Side::get_captured_points() const {
    return captured_points;
}