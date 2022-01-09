#include "textdisplay.h"
#include "subject.h"
#include "info.h"
#include "cell.h"
#include <string>
#include <vector>

// initialize the textDiaply with default "-"
TextDisplay::TextDisplay(int n) : gridSize{n} {
    std::vector<char> temp;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            temp.push_back('-');
        }
        theDisplay.push_back(temp);
        temp.clear();
    }
}

// change the text display according the notifier's colour
void TextDisplay::notify(Subject<Info, State> &whoNotified) {
    size_t r = whoNotified.getInfo().row;
    size_t c = whoNotified.getInfo().col;

    Colour colour = whoNotified.getState().colour;

    if (colour == Colour::Black) {
        theDisplay[r][c] = 'B';
    }
    else if (colour == Colour::White) {
        theDisplay[r][c] = 'W';
    }
    else {
        theDisplay[r][c] = '-';
    }
}

// print all chars in the textDisplay 
std::ostream &operator<<(std::ostream &out, const TextDisplay &td) {
    int size = td.gridSize;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            out << td.theDisplay[i][j];
        }
        out << std::endl;
    }
    return out;
}
