#ifndef CELL_H
#define CELL_H
#include <cstddef>
#include "info.h"
#include "observer.h"
#include "state.h"
#include "subject.h"

class Cell : public Subject<Info, State>, public Observer<Info, State> {
    const size_t r, c;
    Colour colour = Colour::NoColour;

    // implement a helper function to determine where is "WhoFrom" cell relative to current cell 
    Direction getDirection(Subject<Info, State> &whoFrom) {
        
        size_t otherR = whoFrom.getInfo().row;
        size_t otherC = whoFrom.getInfo().col;
        
        // deter the direction according the their location in the grid by accessing their row and col #
        if ((otherR == r) && (otherC > c)) {
            return Direction::E;
        } else if ((otherR == r) && (otherC < c)) {
            return Direction::W;
        } else if ((otherR < r) && (otherC == c)) {
            return Direction::N;
        } else if ((otherR > r) && (otherC == c)) {
            return Direction::S;
        } else if ((otherR < r) && (otherC < c)) {
            return Direction::NW;
        } else if ((otherR > r) && (otherC > c)) {
            return Direction::SE;
        } else if ((otherR < r) && (otherC > c)) {
            return Direction::NE;
        } else {
            return Direction::SW;
        }

    }

    // a helper function which is given a direction and return the opposite direction
    Direction reverse(Direction dir) {
        if (dir == Direction::N) {
            return Direction::S;
        } else if (dir == Direction::S) {
            return Direction::N;
        } else if (dir == Direction::W) {
            return Direction::E;
        } else if (dir == Direction::E) {
            return Direction::W;
        } else if (dir == Direction::NE) {
            return Direction::SW;
        } else if (dir == Direction::SW) {
            return Direction::NE;
        } else if (dir == Direction::NW) {
            return Direction::SE;
        } else {
            return Direction::NW;
        }

    }


public:
    Cell(size_t r, size_t c);

    void setPiece(Colour colour); // Place a piece of given colour here.
    void toggle(); // Toggles my colour.

    void notify(Subject<Info, State> &whoFrom)
            override; // My neighbours will call this
    // when they've changed state
    Info getInfo() const override;
};
#endif
