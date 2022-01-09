#include "grid.h"
#include "textdisplay.h"
#include <stdexcept>
#include <vector>
#include <iostream>

// cstr for InvalidMove
InvalidMove::InvalidMove(Colour colour) : colour{colour} {}

// when encounter an InvalidMove, the current player need to re-play the piece
Colour InvalidMove::whosTurn() {
    return colour;
}

// clear grid and delete points in destructor
Grid::~Grid() {
    theGrid.clear();
    delete td;
    delete ob;
}

// helper function for setNeighbours(). isValid(x,y,n) checks if a Cell at position x,y is valid
    // in a n by n Grid
bool Grid::isValid(size_t x, size_t y, size_t n) {
    return ((x >= 0) && (x < n) && (y >= 0) && (y < n));
}

// sets the neignbour/observer for each cell in the grid
void Grid::setNeighbours(size_t n)
{
    // for every Cell, we have 8 possible neighbours
    for (size_t r = 0; r < n; r++)
    {
        for (size_t col = 0; col < n; col++)
        {
            // for each potential neighbour, we check if it is a valid Cell.
            // if so, we call Subject::attach to make the neighbour an Observer of
            // the current Cell (r,col)
            // NORTH NEIGHBOUR
            if (isValid(r, col + 1, n))
            {
                theGrid[r][col].attach(&theGrid[r][col + 1]);
            }
            // EAST NEIGHBOUR
            if (isValid(r + 1, col, n))
            {
                theGrid[r][col].attach(&theGrid[r + 1][col]);
            }
            // WEST NEIGHBOUR
            if (isValid(r - 1, col, n))
            {
                theGrid[r][col].attach(&theGrid[r - 1][col]);
            }
            // SOUTH NEIGHBOUR
            if (isValid(r, col - 1, n))
            {
                theGrid[r][col].attach(&theGrid[r][col - 1]);
            }
            // NORTHWEST NEIGHBOUR
            if (isValid(r - 1, col + 1, n))
            {
                theGrid[r][col].attach(&theGrid[r - 1][col + 1]);
            }
            // SOUTHWEST NEIGHBOUR
            if (isValid(r - 1, col - 1, n))
            {
                theGrid[r][col].attach(&theGrid[r - 1][col - 1]);
            }
            // NORTHEAST NEIGHBOUR
            if (isValid(r + 1, col + 1, n))
            {
                theGrid[r][col].attach(&theGrid[r + 1][col + 1]);
            }
            // SOUTHEAST NEIGHBOUR
            if (isValid(r + 1, col - 1, n))
            {
                theGrid[r][col].attach(&theGrid[r + 1][col - 1]);
            }
            // for each cell, attach textDisplay as an observer
            theGrid[r][col].attach(td);
        }
    }
}


void Grid::setObserver(Observer<Info, State> *ob) {
    this->ob = ob;
}


// go through all cells in grid to check if all of them are populated 
bool Grid::isFull() const {
    for (std::vector<Cell> i : theGrid) {
        for (Cell j : i) {
            if (j.getInfo().colour == Colour::NoColour) {
                // if one cell is not populated, return false and stop looping 
                return false;
                break;
            }
        }
    }
    return true;
}

Colour Grid::whoWon() const {
    int bCount = 0;
    int wCount = 0;

    // go thru every cell and increment the count fro B/W accordingly
    for (std::vector<Cell> i : theGrid) {
        for (Cell j : i) {
            if (j.getInfo().colour == Colour::Black) {
                bCount++;
            }
            else if (j.getInfo().colour == Colour::White) {
                wCount++;
            }
        }
    }

    // compare and see who wins
    if (bCount > wCount) {
        return Colour::Black;
    }
    else if (wCount > bCount) {
        return Colour::White;
    }
    else {
        return Colour::NoColour;
    }
}

void Grid::init(size_t n) {
    // check n is at least and even
    if ((n >= 4) && (n % 2 == 0)) {
        // clear out previous fields and start over
        if (td != nullptr) {
            delete td;
            delete ob;
            theGrid.clear();   
        }

        // initialize a new textdisplay
        td = new TextDisplay(n);

        // use temp to re-populate the grid with default cells
        std::vector<Cell> temp;
        for (size_t rows = 0; rows < n; rows++) {
            for (size_t cols = 0; cols < n; cols++) {   
                Cell tempCell = Cell{rows, cols};
                tempCell.setPiece(Colour::NoColour);
                temp.emplace_back(tempCell);
            }
            // add temp to the Grid and row by row and clear tmp every time
            theGrid.push_back(temp);
            temp.clear();
        }
        // set the observers for all cells in the grid
        setNeighbours(n);
    }
    // handles invalid grid size
    else {
        throw std::invalid_argument("Error: Grid size must be at least 4 and even");
    }
}

// Plays piece at row r, col c.
void Grid::setPiece(size_t r, size_t c, Colour colour) {
    size_t n = theGrid.size();
    // if the destination is on the board and its not occupied, then set the piece 
    // otherwise handle the InvalidMove
    if ((r >= 0) && (r < n) && (c >= 0) && (c < n)) {
        if (theGrid[r][c].getInfo().colour == Colour::NoColour) {
            theGrid[r][c].setPiece(colour);
        } else {
            throw InvalidMove{colour};
        }
    } else {
        throw InvalidMove{colour};
    }
}

// Flips piece at row r, col c.
void Grid::toggle(size_t r, size_t c) {
    theGrid[r][c].toggle();
}

// print the grid
std::ostream &operator<<(std::ostream &out, const Grid &g) {
    if (g.theGrid.size() != 0) {
        out << *g.td;
    }
    return out;
}
