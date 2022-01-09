#include "cell.h"

// see cell.h for more documentation
Cell::Cell(size_t r, size_t c) : r{r}, c{c} {}

void Cell::setPiece(Colour colour) {
    this->colour = colour;
    this->setState(State{StateType::NewPiece, colour, Direction::N});
    this->notifyObservers();
}


// Toggles my colour.
void Cell::toggle() {
    if (colour == Colour::Black) {
        colour = Colour::White;
    } else if (colour == Colour::White) {
        colour = Colour::Black;
    }
}

// My neighbours will call this when they've changed state
void Cell::notify(Subject<Info, State> &whoFrom) {

    // if the notifier is a new piece 
    if (whoFrom.getState().type == StateType::NewPiece) {
        // if my color is not the same as the new piece and I am currently occupied 
        // by a colour B/W, then set my state to Relay and notify my neighbours
        if (whoFrom.getState().colour != colour && colour != Colour::NoColour) {
            this->setState(State{StateType::Relay, colour, getDirection(whoFrom)});
            this->notifyObservers();
        }

        // if the notifier is in Relay State      
    } else if (whoFrom.getState().type == StateType::Relay) {
        // if I have the same colour and in a straight line with the notifier and new piece
        if (whoFrom.getState().colour == colour) {
            if (getDirection(whoFrom) == whoFrom.getState().direction) {
                // set my state to Relay and direction accoring to the notifier and 
                // notify my neighbours
                this->setState(State{StateType::Relay, colour, getDirection(whoFrom)});
                this->notifyObservers(); 
            }

            // if I am occupied and does not have the same colour with the notifier
        } else if (whoFrom.getState().colour != colour && colour != Colour::NoColour) {
            // if I am in a straight line with the notifier and the new piece
            if (getDirection(whoFrom) == whoFrom.getState().direction) {
                // set my state to Reply but reverse my direction cause I should go back
                // and look to my notifer for a potential change of colour and notify my neighbours
                this->setState(State{StateType::Reply, colour, reverse(getDirection(whoFrom))});
                this->notifyObservers();
            }
        }
        // if the notifier has state Reply
    } else if (whoFrom.getState().type == StateType::Reply) {
        // if I am occupied and do not have the same colour amd in a staright line 
        // with the notifier and new piece 
        if (whoFrom.getState().colour != colour && colour != Colour::NoColour) {
            if (getDirection(whoFrom) == whoFrom.getState().direction) {
                // also set my state to reply and change my colour because I am a piece 
                // in between two pieces with different colours
                // notify my neighbours
                this->setState(State{StateType::Reply, whoFrom.getInfo().colour, getDirection(whoFrom)});
                this->toggle();
                this->notifyObservers();    
            }
        }      
    }
}

Info Cell::getInfo() const {
    return Info{r, c, colour};
}

