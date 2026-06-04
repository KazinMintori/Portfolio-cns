/**
 * Terminal Renderer cpp implementation
 *
 * Mô tả:
 *   Cài đặt renderer cho terminal. Gần như giống level 2; sinh viên đã làm
 *   ở level 1/2 có thể CHÉP NGUYÊN từ level 2 sang, chỉ cần đổi chữ ký
 *   `displayBoard` để nhận `const Board&` thay vì `char[][]`.
 *
 * NOTE:
 *   Đây là tầng "imperative shell" -- được phép có cout, side-effect.
 *   Nhưng KHÔNG được tự log: nếu cần log, log ở Engine sau khi gọi xong.
 */

#include "renderer.h"

/* ---------- Importing ---------- */

// #include <iostream>
// #include <format>

/* ---------- Definitions ---------- */

TerminalRenderer::TerminalRenderer() : I_Renderer() {}
TerminalRenderer::~TerminalRenderer() = default;

void TerminalRenderer::init(const RunConfig& /*config*/) {
    // TODO (chép từ level 2)
    throw NotImplementedException();
}

void TerminalRenderer::clearScreen() {
    // TODO (chép từ level 2)
    throw NotImplementedException();
}

void TerminalRenderer::showSelectMenu(SelectType /*selectType*/, int /*context*/) {
    // TODO (chép từ level 2)
    throw NotImplementedException();
}

void TerminalRenderer::showInvalidSelect(SelectType /*selectType*/, int /*context*/) {
    // TODO (chép từ level 2)
    throw NotImplementedException();
}

void TerminalRenderer::showValidSelect(SelectType /*selectType*/, int /*context*/) {
    // TODO (chép từ level 2)
    throw NotImplementedException();
}

/**
 * Mô tả: Hiển thị bàn cờ.
 *
 * NOTE chuyển từ level 2:
 *   level 2:  void displayBoard(const char board[][BOARD_N_MAX], int size);
 *   level 3:  void displayBoard(const Board& board);
 *
 *   Trong cài đặt, dùng:
 *     for (int r = 0; r < board.size; ++r)
 *       for (int c = 0; c < board.size; ++c)
 *         std::cout << board.at(r, c) << ' ';
 */
void TerminalRenderer::displayBoard(const Board& /*board*/) {
    // TODO (chép & chỉnh từ level 2)
    throw NotImplementedException();
}

void TerminalRenderer::showPlayer(int /*player*/, bool /*is_bot*/) {
    // TODO (chép từ level 2)
    throw NotImplementedException();
}

void TerminalRenderer::showMove(int /*row*/, int /*col*/) {
    // TODO (chép từ level 2)
    throw NotImplementedException();
}

void TerminalRenderer::showInvalidMove() {
    // TODO (chép từ level 2)
    throw NotImplementedException();
}

void TerminalRenderer::showResult(int /*winner*/, bool /*is_bot*/, const WinLine* /*winLine*/) {
    // TODO (chép từ level 2)
    throw NotImplementedException();
}

void TerminalRenderer::printResult(const GameResult& /*gameResult*/) {
    // TODO (chép từ level 2)
    throw NotImplementedException();
}

void TerminalRenderer::close() {
    // (no-op)
}
