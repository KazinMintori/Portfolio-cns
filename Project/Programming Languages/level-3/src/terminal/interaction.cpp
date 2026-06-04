/**
 * Terminal Interaction cpp implementation
 *
 * Mô tả:
 *   Sinh viên có thể CHÉP NGUYÊN từ level 2 sang. Đây là tầng shell,
 *   được phép có cin/cout.
 */

#include "interaction.h"

/* ---------- Importing ---------- */

// #include <iostream>
// #include <string>
// #include <fstream>

/* ---------- Definitions ---------- */

TerminalInteraction::TerminalInteraction() : I_Interaction() {}
TerminalInteraction::~TerminalInteraction() = default;

void TerminalInteraction::init(const RunConfig& /*config*/) {
    // TODO (chép từ level 2)
    throw NotImplementedException();
}

void TerminalInteraction::pause(int /*timeout*/) {
    // TODO (chép từ level 2)
    throw NotImplementedException();
}

bool TerminalInteraction::selectSize(int* /*size*/) {
    // TODO (chép từ level 2)
    throw NotImplementedException();
}

bool TerminalInteraction::selectGoal(int* /*goal*/, int /*size*/) {
    // TODO (chép từ level 2)
    throw NotImplementedException();
}

bool TerminalInteraction::selectGameMode(GameMode* /*mode*/) {
    // TODO (chép từ level 2)
    throw NotImplementedException();
}

bool TerminalInteraction::selectBotLevel(BotLevel* /*levels*/, int /*index*/) {
    // TODO (chép từ level 2)
    throw NotImplementedException();
}

bool TerminalInteraction::getPlayerMove(int* /*row*/, int* /*col*/) {
    // TODO (chép từ level 2)
    throw NotImplementedException();
}

void TerminalInteraction::close() {
    // (no-op)
}
