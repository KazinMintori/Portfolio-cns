/**
 * Logic cpp implementation
 *
 */

#include "logic.h"

/* ---------- Importing ---------- */

#include <format>
#include <limits>
#include <sstream>

#include "../utils/logger.h"

/* ---------- Definitions ---------- */

/**
 * Mô tả: Khởi tạo bàn cờ với ký tự mặc định.
 * Đầu vào: board, size
 * Đầu ra: Không có
 * Tác dụng phụ: Ghi dữ liệu vào board
 * TODO:
 */
void Logic::initBoard(char board[][BOARD_N_MAX], const int size) {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            board[i][j] = '-';
}

bool Logic::isValidMove(const char board[][BOARD_N_MAX], const int size, const int row, const int col) {
    return (row >= 0 && row < size && col >= 0 && col < size && board[row][col] == '-');
}

void Logic::makeMove(char board[][BOARD_N_MAX], const int row, const int col, const char symbol) {
    board[row][col] = symbol;
}

bool Logic::isEmptyHead(const char board[][BOARD_N_MAX], const int size, int x, int y, const char symbol) {
    if (x < 0 || x >= size || y < 0 || y >= size) return true;
    char opponent = (symbol == 'X') ? 'O' : 'X';
    return (board[x][y] != opponent);
}

bool Logic::checkWin(char board[][BOARD_N_MAX], const int size, const char symbol, const int goal, EndRule rule) {
    int dx[] = {0, 1, 1, 1};
    int dy[] = {1, 0, 1, -1};

    for (int dir = 0; dir < 4; dir++) {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (board[i][j] != symbol) continue;

                int count = 1;
                int ni = i + dx[dir], nj = j + dy[dir];
                while (ni >= 0 && ni < size && nj >= 0 && nj < size && board[ni][nj] == symbol) {
                    count++;
                    ni += dx[dir];
                    nj += dy[dir];
                }
                if (count < goal) continue;

                int bi = i - dx[dir], bj = j - dy[dir];
                bool headOpen = (bi < 0 || bi >= size || bj < 0 || bj >= size) ? true : (board[bi][bj] == '-');
                bool tailOpen = (ni < 0 || ni >= size || nj < 0 || nj >= size) ? true : (board[ni][nj] == '-');

                if (rule == EndRule::NONE) return true;
                if (rule == EndRule::OPEN_ONE && (headOpen || tailOpen)) return true;
                if (rule == EndRule::OPEN_TWO && headOpen && tailOpen) return true;
            }
        }
    }
    return false;
}

bool Logic::checkDraw(char board[][BOARD_N_MAX], const int size) {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (board[i][j] == '-') return false;
    return true;
}

std::optional<WinLine> Logic::getWinLine(
    const char board[][BOARD_N_MAX],
    const int size,
    const char symbol,
    const int goal,
    EndRule rule) {

    int dx[] = {0, 1, 1, 1};
    int dy[] = {1, 0, 1, -1};

    for (int dir = 0; dir < 4; dir++) {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (board[i][j] != symbol) continue;

                std::vector<std::pair<int, int>> cells;
                cells.push_back({i, j});
                int ni = i + dx[dir], nj = j + dy[dir];
                while (ni >= 0 && ni < size && nj >= 0 && nj < size && board[ni][nj] == symbol) {
                    cells.push_back({ni, nj});
                    ni += dx[dir];
                    nj += dy[dir];
                }
                if ((int)cells.size() < goal) continue;

                int bi = i - dx[dir], bj = j - dy[dir];
                bool headOpen = (bi < 0 || bi >= size || bj < 0 || bj >= size) ? true : (board[bi][bj] == '-');
                bool tailOpen = (ni < 0 || ni >= size || nj < 0 || nj >= size) ? true : (board[ni][nj] == '-');

                bool win = false;
                if (rule == EndRule::NONE) win = true;
                if (rule == EndRule::OPEN_ONE && (headOpen || tailOpen)) win = true;
                if (rule == EndRule::OPEN_TWO && headOpen && tailOpen) win = true;

                if (win) {
                    WinLine wl;
                    wl.cells = cells;
                    return wl;
                }
            }
        }
    }
    return std::nullopt;
}