#include <iostream>
#include <memory>
#include <string>
#include "grid.h"
#include "state.h"
using namespace std;

int main(int argc, char *argv[]) {
    cin.exceptions(ios::eofbit | ios::failbit);
    string cmd;
    Grid g;
    Colour whoseMove = Colour::Black;

    try {
        while (true) {
            cin >> cmd;
            if (cmd == "new") {
                int n;
                cin >> n;
                // n must be at least 4 and even
                if (n < 4 || n % 2)
                    continue;
                g.init(n);
                g.setPiece(n / 2 - 1, n / 2 - 1, Colour::Black);
                g.setPiece(n / 2 - 1, n / 2, Colour::White);
                g.setPiece(n / 2, n / 2 - 1, Colour::White);
                g.setPiece(n / 2, n / 2, Colour::Black);
                whoseMove = Colour::Black; // Black starts
                cout << g;
            }
            else if (cmd == "play") {
                int r = 0, c = 0;
                cin >> r >> c;
                try {
                    g.setPiece(r, c, whoseMove);
                    cout << g;
                    if (g.isFull()) {
                        switch (g.whoWon()) {
                        case Colour::Black:
                            cout << "Black wins!" << endl;
                            break;
                        case Colour::White:
                            cout << "White wins!" << endl;
                            break;
                        default:
                            cout << "Tie!" << endl;
                        }
                        break;
                    }
                    whoseMove = whoseMove == Colour::Black ? Colour::White
                                                           : Colour::Black;
                }
                catch (InvalidMove move) {
                    whoseMove = move.whosTurn();
                } // Invalid move; let the loop go around again
            }
        }
    }
    catch (ios::failure &) {
    } // Any I/O failure quits
}
