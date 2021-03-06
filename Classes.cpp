#include <iostream>
#include "Classes.h"


GamePieceType type_from_char(char c) {
    switch (c) {
        case ROCK_CHAR:
            return Rock;
        case PAPER_CHAR:
            return Paper;
        case SCISSORS_CHAR:
            return Scissors;
        case BOMB_CHAR:
            return Bomb;
        case JOKER_CHAR:
            return None;
        case FLAG_CHAR:
            return Flag;
        default:
            std::cout << "ERROR 95087216986345: char is " << c << std::endl;
            return None;
    }
}

const Cell &PlannedMove::getOrigin() const {
    return origin;
}

const Cell &PlannedMove::getDestination() const {
    return destination;
}

bool PlannedMove::isHas_joker_changed() const {
    return has_joker_changed;
}

const Cell &PlannedMove::getJoker_position() const {
    return joker_position;
}

GamePieceType PlannedMove::getNew_joker_type() const {
    return new_joker_type;
}

char GamePiece::to_char() {
    char ch;
    if (isJoker)
        ch = 'J';
    else
        switch (type) {
            case Rock:
                ch = ROCK_CHAR;
                break;
            case Paper:
                ch = PAPER_CHAR;
                break;
            case Scissors:
                ch = SCISSORS_CHAR;
                break;
            case Bomb:
                ch = BOMB_CHAR;
                break;
            case None:
                ch = JOKER_CHAR;
                break; // Notice
            case Flag:
                ch = FLAG_CHAR;
                break;
            default:;
                std::cerr << "Unknown type in store_game... cmon guys" << std::endl;
                ch = '@';
                break;
        }

    if (!player) // player 2
        ch = std::tolower(ch, std::locale()); // to lowercase

    return ch;
}

bool GamePiece::canMove() {
    switch (type) {
        case None:
            std::cerr << "BUG 16239817247" << std::endl;
            return false;
        case Rock:
        case Paper:
        case Scissors:
            return true;
        case Bomb:
        case Flag:
            return false;
        default:
            std::cerr << "BUG 7952234678123" << std::endl;
            break;
    }
    return false;
}
