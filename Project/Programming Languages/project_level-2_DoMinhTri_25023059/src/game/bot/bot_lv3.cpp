/**
 * Bot LV3 cpp implementation
 *
 */

#include "bot_lv3.h"

/* ---------- Importing ---------- */

#include <sstream>

#include "../../utils/logger.h"
#include "../logic.h"

/* ---------- Definitions ---------- */

/**
 * Mô tả:
 *   Constructor của BotLevel3.
 */
BotLevel3::BotLevel3(const BotLevel &_level, const char &_symbol)
    : BotLevel2(_level, _symbol)
{
}

/**
 * Mô tả:
 *   Destructor của BotLevel3.
 */
BotLevel3::~BotLevel3()
{
}

/**
 * Mô tả:
 *   Trả về nước đi của bot HARD.
 *
 * TODO:
 *   - Bước 1: Gọi hard_level để tìm nước đi tối ưu
 *   - Bước 2: Nếu không tìm được → fallback về logic của BotLevel2
 */
pII BotLevel3::getMove(char board[][BOARD_N_MAX], const int size, const int goal)
{
    pII move = hard_level(board, size, goal);
    if (move.first == -1)
        return BotLevel2::getMove(board, size, goal);
    return move;
}

pII BotLevel3::hard_level(char board[][BOARD_N_MAX], const int size, const int goal)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (!Logic::isValidMove(board, size, i, j))
                continue;
            board[i][j] = symbol;
            bool win = Logic::checkWin(board, size, symbol, goal, EndRule::OPEN_TWO);
            board[i][j] = '-';
            if (win)
                return {i, j};
        }
    }
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (!Logic::isValidMove(board, size, i, j))
                continue;
            board[i][j] = op_symbol;
            bool win = Logic::checkWin(board, size, op_symbol, goal, EndRule::OPEN_TWO);
            board[i][j] = '-';
            if (win)
                return {i, j};
        }
    }
    pII bestMove = {-1, -1};
    int bestScore = -1;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (!Logic::isValidMove(board, size, i, j))
                continue;
            int attackScore = scoreCell(board, size, goal, i, j, symbol);
            int defendScore = scoreCell(board, size, goal, i, j, op_symbol);
            int total = (int)(attackScore * 1.2) + defendScore;
            if (total > bestScore)
            {
                bestScore = total;
                bestMove = {i, j};
            }
        }
    }
    return bestMove;
}

int BotLevel3::countInDirection(char board[][BOARD_N_MAX], int size,
                                int r, int c, int dr, int dc, char sym)
{
    int count = 0;
    int nr = r + dr, nc = c + dc;
    while (nr >= 0 && nr < size && nc >= 0 && nc < size && board[nr][nc] == sym)
    {
        count++;
        nr += dr;
        nc += dc;
    }
    return count;
}

int BotLevel3::scoreCell(char board[][BOARD_N_MAX], int size, int goal,
                         int row, int col, char sym)
{
    int dr[] = {0, 1, 1, 1};
    int dc[] = {1, 0, 1, -1};
    int totalScore = 0;

    for (int dir = 0; dir < 4; dir++)
    {
        int forward = countInDirection(board, size, row, col, dr[dir], dc[dir], sym);
        int backward = countInDirection(board, size, row, col, -dr[dir], -dc[dir], sym);
        int chainLen = 1 + forward + backward;

        int headR = row + (forward + 1) * dr[dir];
        int headC = col + (forward + 1) * dc[dir];
        int tailR = row - (backward + 1) * dr[dir];
        int tailC = col - (backward + 1) * dc[dir];

        bool headOpen = Logic::isValidMove(board, size, headR, headC);
        bool tailOpen = Logic::isValidMove(board, size, tailR, tailC);
        int openEnds = (headOpen ? 1 : 0) + (tailOpen ? 1 : 0);
        if (openEnds == 0)
            continue;

        int lineScore = 1;
        if (chainLen >= goal - 1 && openEnds == 2)
            lineScore = 10000;
        else if (chainLen >= goal - 1 && openEnds == 1)
            lineScore = 1000;
        else if (chainLen >= goal - 2 && openEnds == 2)
            lineScore = 100;
        else if (chainLen >= goal - 2 && openEnds == 1)
            lineScore = 10;

        totalScore += lineScore;
    }
    return totalScore;
}