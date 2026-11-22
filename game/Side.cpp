//
// Created by Hayk Derdzyan on 12.09.26.
//

#include "Side.h"
#include <random>
#include <utility>

Side::Side() {
    this->color = Color::NONE;
    this->remaining = std::chrono::milliseconds{0};
}

Side::Side(const Color color, std::string name, const std::chrono::milliseconds remaining)
    : color(color), name(std::move(name)), remaining(remaining) {
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

void Side::setName(std::string value) {
    name = std::move(value);
}

const std::string& Side::getName() const {
    return name;
}

void Side::addCapture(const PieceType type) {
    if (type != PieceType::NONE) {
        captured.push_back(type);
    }
}

const std::vector<PieceType>& Side::getCaptured() const {
    return captured;
}

int Side::get_captured_points() const {
    int total = 0;
    for (const PieceType type : captured) {
        const auto found = points.find(type);
        if (found != points.end()) {
            total += found->second;
        }
    }
    return total;
}

void Side::setRemaining(const std::chrono::milliseconds value) {
    remaining = value;
}

std::chrono::milliseconds Side::getRemaining() const {
    return remaining;
}
