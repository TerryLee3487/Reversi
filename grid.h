#ifndef GRID_H
#define GRID_H
#include <cstddef>
#include <iostream>
#include <vector>
#include "cell.h"
#include "info.h"
#include "state.h"

class TextDisplay;
template <typename InfoType, typename StateType>
class Observer;

class InvalidMove {
    Colour colour;
  public:
    InvalidMove(Colour colour);
    Colour whosTurn();
};

class Grid
{
    std::vector<std::vector<Cell>> theGrid; // The actual grid.
    TextDisplay *td = nullptr;              // The text display.
    Observer<Info, State> *ob = nullptr;    // Another observer (intent:  graphics)
    // Add private members, if necessary.

    bool isValid(size_t x, size_t y, size_t n);
    void setNeighbours(size_t n);

public:
    ~Grid();

    void setObserver(Observer<Info, State> *ob);
    bool isFull() const;                              // Is the game over, i.e., is the grid full?
    Colour whoWon() const;                            // Who has more pieces when the board is full?
    void init(size_t n);                              // Sets up an n x n grid.  Clears old grid, if necessary.
    void setPiece(size_t r, size_t c, Colour colour); // Plays piece at row r, col c.
    void toggle(size_t r, size_t c);                  // Flips piece at row r, col c.

    friend std::ostream &operator<<(std::ostream &out, const Grid &g);
};

#endif
