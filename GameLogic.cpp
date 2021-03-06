#include <iostream>
#include "GameLogic.h"

/**
 * simulate a fight between two pieces and return the result
 * @param attacker attacker of fight
 * @param defender piece attacked
 * @return FightResult case matching the simulation
 */
FightResult simulate_fight(const GamePiece &attacker,
                           const GamePiece &defender) {
    //Bombs kill everyone
    if (attacker.type == Bomb || defender.type == Bomb)
        return BOTH_PIECES_LOST;
    //Ties are a lose-lose scenario
    if (attacker.type == defender.type)
        return BOTH_PIECES_LOST;
    //Flags are useless in combat
    if (attacker.type == Flag)
        return DEFENDER_WON;
    if (defender.type == Flag)
        return ATTACKER_WON;
    // Rock > Scissors > Paper > Rock
    if (attacker.type == Rock && defender.type == Scissors)
        return ATTACKER_WON;
    if (attacker.type == Paper && defender.type == Rock)
        return ATTACKER_WON;
    if (attacker.type == Scissors && defender.type == Paper)
        return ATTACKER_WON;
    if (attacker.type == Rock && defender.type == Paper)
        return ATTACKER_WON;
    if (attacker.type == Paper && defender.type == Scissors)
        return ATTACKER_WON;
    if (attacker.type == Scissors && defender.type == Rock)
        return ATTACKER_WON;

    std::cout << "BUG in simulate_fight !!!";
    return FightResult::BOTH_PIECES_LOST;
}


MoveResult _make_move_part_of_planned_move(Game &game, PlannedMove &plannedMove) {
    Cell source = plannedMove.getOrigin(), destination = plannedMove.getDestination();
    int sourceRow = source.row, sourceColumn = source.column,
            destinationRow = destination.row, destinationColumn = destination.column;
//    std::cout << "attempting to move " << sourceRow << sourceColumn << " to "
//              << destinationRow << destinationColumn << std::endl;
    if (game.board[sourceRow][sourceColumn] == nullptr) {
        return TriedToMoveEmptySpace;
    }
    if (!game.board[sourceRow][sourceColumn]->canMove()) {
        return TriedToMoveUnmovablePiece;
    }
    if (game.board[sourceRow][sourceColumn]->player != game.currentPlayer) {
        return TriedToMoveEnemy;
    }
    if (sourceRow == destinationRow && sourceColumn == destinationColumn) {
        return TriedToMoveIntoAlly;
    }
    if (game.board[destinationRow][destinationColumn] == nullptr) {
        game.board[destinationRow][destinationColumn] =
                game.board[sourceRow][sourceColumn];
    } else {
        actually_fight(
                game,
                game.board[sourceRow][sourceColumn],
                game.board[destinationRow][destinationColumn],
                Cell(destinationRow, destinationColumn));
    }

    game.board[sourceRow][sourceColumn] = nullptr;

    return SuccessfulMove;
}


MoveResult make_planned_move(Game &game, PlannedMove &plannedMove) {
    MoveResult firstResult = _make_move_part_of_planned_move(game, plannedMove);
    if (firstResult != SuccessfulMove || !plannedMove.isHas_joker_changed())
        return firstResult;

    Cell jokerPosition = plannedMove.getJoker_position();
    int jr = jokerPosition.row;
    int jc = jokerPosition.column;
    auto joker = game.board[jr][jc];
    if (joker == nullptr || !joker->isJoker || joker->player != game.currentPlayer)
        return TriedIllegalJokerChange;
    switch (plannedMove.getNew_joker_type()) {
        // Only R P S B are allowed
        case Rock:
        case Paper:
        case Scissors:
        case Bomb:
            return SuccessfulMove;
        case None:
        case Flag:
            return TriedIllegalJokerChange;
        default:
            std::cerr << "BUG 7984132547" << std::endl;
            break;
    }
    return SuccessfulMove;
}

MoveResult actually_fight(Game &game, GamePiece *attacker,
                          GamePiece *defender, Cell position) {
    int r = position.row;
    int c = position.column;
    auto fightResult = simulate_fight(*attacker, *defender);
    switch (fightResult) {
        case ATTACKER_WON:
            delete defender;
            game.board[r][c] = attacker;
            return MoveResult::SuccessfulMove;
        case DEFENDER_WON:
            delete attacker;
            game.board[r][c] = defender;
            return MoveResult::SuccessfulMove;
        case BOTH_PIECES_LOST:
            delete attacker;
            delete defender;
            game.board[r][c] = nullptr;
            return MoveResult::SuccessfulMove;
        default:
            print_line("ERROR in actually_fight because of weird fight result");
            return MoveResult::SuccessfulMove;
    }
}
