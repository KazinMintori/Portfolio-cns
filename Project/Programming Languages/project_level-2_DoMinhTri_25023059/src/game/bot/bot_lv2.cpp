/**
 * Bot LV2 cpp implementation
 *
 */

#include "bot_lv2.h"

/* ---------- Importing ---------- */

#include <sstream>

#include "../../utils/logger.h"
#include "../logic.h"

/* ---------- Definitions ---------- */

/**
 * Mô tả:
 *   Constructor của BotLevel2.
 */
BotLevel2::BotLevel2(const BotLevel &_level, const char &_symbol)
    : BotLevel1(_level, _symbol)
{
}

/**
 * Mô tả:
 *   Destructor của BotLevel2.
 */
BotLevel2::~BotLevel2()
{
}

/**
 * Mô tả:
 *   Trả về nước đi của bot MEDIUM.
 *
 * TODO:
 *   - Gọi heuristic để chọn nước đi
 */
pII BotLevel2::getMove(char board[][BOARD_N_MAX], const int size, const int goal)
{
    return simple_heuristic(board, size, goal, symbol, op_symbol);
}

pII BotLevel2::simple_heuristic(char board[][BOARD_N_MAX], const int size, const int goal, const char botSymbol, const char playerSymbol)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (board[i][j] == '-')
            {
                board[i][j] = botSymbol;
                bool win = Logic::checkWin(board, size, botSymbol, goal, EndRule::OPEN_TWO);
                board[i][j] = '-';
                if (win)
                    return {i, j};
            }
        }
    }
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (board[i][j] == '-')
            {
                board[i][j] = playerSymbol;
                bool win = Logic::checkWin(board, size, playerSymbol, goal, EndRule::OPEN_TWO);
                board[i][j] = '-';
                if (win)
                    return {i, j};
            }
        }
    }
    int center = size / 2;
    if (board[center][center] == '-')
        return {center, center};
    return random_pick(board, size);
}