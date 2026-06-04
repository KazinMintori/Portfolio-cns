/**
 * Ternimal Renderer cpp implementation
 *
 */

#include "renderer.h"

/* ---------- Importing ---------- */

#include <format>
#include <iomanip>
#include <iostream>

#include "../game/setup.h"
#include "../utils/config.h"

/* ---------- Definitions ---------- */

/**
 * Mô tả:
 *   Constructor của TerminalRenderer.
 *
 * TODO:
 *   - Khởi tạo trạng thái ban đầu nếu cần
 */
TerminalRenderer::TerminalRenderer() : I_Renderer() {}

TerminalRenderer::~TerminalRenderer() {}

void TerminalRenderer::init(const RunConfig& config) {
}

void TerminalRenderer::clearScreen() {
}

void TerminalRenderer::showSelectMenu(SelectType selectType, int context) {
    switch (selectType) {
        case SelectType::TITLE_UI:
            std::cout << std::format(">----- Tic-tac-toe [Console v{}] -----<\n\n", VERSION);
            break;
        case SelectType::SIZE_UI:
            std::cout << std::format("Enter the board size (NxN, 3 <= N <= {}): ", BOARD_N_MAX);
            break;
        case SelectType::GOAL_UI:
            std::cout << "Enter the goal (3 - 5, goal <= size): ";
            break;
        case SelectType::GAME_MODE_UI:
            std::cout << "Select Your Game Mode:\n";
            std::cout << "(1) PvP | (2) PvE | (3) EvE\n";
            break;
        case SelectType::BOT_LEVEL_UI:
            std::cout << "Select Bot Level:(1) EASY | (2) MEDIUM | (3) HARD\n";
            break;
        case SelectType::MUL_BOT_LEVEL_UI:
            if (context == 0)
                std::cout << "Bot 1 - Select Bot Level:(1) EASY | (2) MEDIUM | (3) HARD\n";
            else
                std::cout << "Bot 2 - Select Bot Level:(1) EASY | (2) MEDIUM | (3) HARD\n";
            break;
        case SelectType::PLAYER_UI:
            std::cout << "Enter Your Move (row col): ";
            break;
        default:
            break;
    }
    std::cout.flush();
}

void TerminalRenderer::showInvalidSelect(SelectType selectType, int context) {
    std::cout << "Invalid input. Please try again.\n";
}

void TerminalRenderer::showValidSelect(SelectType selectType, int context) {
}

void TerminalRenderer::displayBoard(const char board[][BOARD_N_MAX], const int size) {
    std::cout << "    ";
    for (int j = 0; j < size; j++)
        std::cout << j << (j < 10 ? " " : "");
    std::cout << "\n    ";
    for (int j = 0; j < size; j++)
        std::cout << "--";
    std::cout << "\n";
    for (int i = 0; i < size; i++) {
        std::cout << std::setw(2) << i << " | ";
        for (int j = 0; j < size; j++)
            std::cout << board[i][j] << " ";
        std::cout << "\n";
    }
    std::cout << std::endl;
}

void TerminalRenderer::showPlayer(int player, bool is_bot) {
    if (is_bot)
        std::cout << std::format("Bot (Player {}) is thinking...\n", player);
    else
        std::cout << std::format("Player {}'s turn\n", player);
}

void TerminalRenderer::showMove(const int row, const int col) {
    std::cout << std::format("Move placed at ({}, {})\n", row, col);
}

void TerminalRenderer::showInvalidMove() {
    std::cout << "This move is invalid\n";
}

void TerminalRenderer::showResult(const int winner, const bool is_bot, const WinLine* winLine) {
    if (winner == DRAW_RESULT) {
        std::cout << "It's a draw!\n";
    } else {
        int displayPlayer = winner + 1;
        if (is_bot)
            std::cout << std::format("Bot {} (Player {}) wins!\n", displayPlayer, displayPlayer);
        else
            std::cout << std::format("Player {} wins!\n", displayPlayer);
    }
}

void TerminalRenderer::printResult(const GameResult& gameResult) {
    std::cout << gameResult.winner << " " << gameResult.turns << std::endl;
}

void TerminalRenderer::close() {
}