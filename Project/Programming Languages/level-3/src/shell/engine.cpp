/**
 * Engine implementation
 */

#include "engine.h"

/* ---------- Importing ---------- */

#include <chrono>
#include <sstream>
#include <thread>

#include "../core/bot_pure.h"
#include "../core/logic.h"
#include "../utils/helper.h"

/* ---------- Definitions ---------- */

Engine::Engine(const RunConfig* config,
               I_Renderer* iRenderer,
               I_Interaction* iInteraction,
               Logger* logger,
               Rng* rng)
    : config_(config),
      iRenderer_(iRenderer),
      iInteraction_(iInteraction),
      logger_(logger),
      rng_(rng) {}

Engine::~Engine() = default;

/* ---------- init ---------- */

void Engine::init() {
    if (logger_) logger_->log("Engine initializing . . .");

    if (iRenderer_)    iRenderer_->init(*config_);
    if (iInteraction_) iInteraction_->init(*config_);

    if (logger_) logger_->log("Engine initialized!");
}

bool Engine::sanity_check() {
    bool ok = true;
    if (!iRenderer_) {
        if (logger_) logger_->log("Renderer not implemented!", Logger::Level::WARNING);
        ok = false;
    }
    if (!iInteraction_) {
        if (logger_) logger_->log("Interaction not implemented!", Logger::Level::WARNING);
        ok = false;
    }
    if (!logger_) {
        // không log được, nhưng vẫn fail safety check
        ok = false;
    }
    if (!rng_) {
        if (logger_) logger_->log("RNG not provided!", Logger::Level::ERROR);
        ok = false;
    }
    return ok;
}

/* ---------- startGame ---------- */

/**
 * Mô tả: Lấy input cấu hình từ user (size, goal, mode, bot levels).
 *
 * TODO (mức 1 -- task 3, đã được Logger DI):
 *   - Cài đặt giống level 2, NHƯNG dùng `logger_->log(...)` thay vì
 *     `Logger::log(...)`.
 *   - Sau khi xong: khởi tạo `gameState_` bằng `core::initBoard(...)`.
 */
void Engine::startGame() {
    if (logger_) logger_->log("[Engine] Starting game . . .");

    if (!sanity_check()) {
        if (logger_) logger_->log("[Engine] Game stopped!", Logger::Level::ERROR);
        return;
    }

    // TODO (mức 1 -- task 3):
    //   1. Hiển thị title (showSelectMenu(SelectType::TITLE_UI)).
    //   2. Lấy size, goal, mode, bot levels -- pattern do-while giống level 2.
    //   3. Khởi tạo gameState_:
    //        gameState_.board         = core::initBoard(gameSetup_.size);
    //        gameState_.currentPlayer = 0;
    //        gameState_.turn          = 0;
    //        gameState_.winner        = -1;
    //        gameState_.isFinished    = false;
    //
    //   QUAN TRỌNG: thay mọi `Logger::log(...)` bằng `logger_->log(...)`.
    throw NotImplementedException();
}

/* ---------- playGame ---------- */

/**
 * Mô tả: Game loop CHÍNH.
 *
 * TODO:
 *   ── Mức 1 (task 3): chỉ cần thay Logger global → logger_-> ──
 *      Có thể giữ tinh thần while-loop của level 2.
 *
 *   ── Mức 2 (Engine): KHÔNG mutate board ──
 *      Thay vì:
 *          Logic::makeMove(gameSetup.board, row, col, symbols[player]);
 *      dùng:
 *          gameState_ = core::applyTurn(gameState_, {row, col}, gameSetup_.goal);
 *
 *      Loop dừng khi `gameState_.isFinished == true`.
 *
 *      Khuyến khích: viết `playGame` thành một đệ quy đuôi:
 *          GameResult loop(GameState s) {
 *              if (s.isFinished) return result_of(s);
 *              auto move  = nextMove(s);
 *              auto next  = core::applyTurn(s, move, goal);
 *              return loop(next);   // tail call
 *          }
 *      → minh hoạ rõ "game loop là một fold".
 *
 * Trả về: GameResult.
 */
GameResult Engine::playGame() {
    if (logger_) logger_->log("[Engine] Playing game . . .");

    GameResult gameResult{-1, false, 0};

    if (!sanity_check()) {
        if (logger_) logger_->log("[Engine] Game stopped!", Logger::Level::ERROR);
        return gameResult;
    }

    // TODO (mức 1+2): cài đặt game loop theo gợi ý ở trên.
    //
    // Gợi ý mức 1: cụ thể hoá bot bằng FP factory:
    //   bot::BotFn bot0 = (mode == EVE)
    //                       ? bot::makeBot(gameSetup_.levels[0], gameSetup_.goal)
    //                       : bot::BotFn{};
    //   bot::BotFn bot1 = (mode != PVP)
    //                       ? bot::makeBot(gameSetup_.levels[1], gameSetup_.goal)
    //                       : bot::BotFn{};
    //
    // Gợi ý đo thời gian (mức 1+3B):
    //   auto move = measureExecutionTime(
    //       "bot->getMove",
    //       [&]{ return bot1(gameState_, *rng_); },
    //       config_->verbose_flag,
    //       [this](const std::string& m){ logger_->log(m, Logger::Level::DEBUG); }
    //   );

    throw NotImplementedException();
    return gameResult;
}

/* ---------- endGame ---------- */

/**
 * Mô tả: Hiển thị kết quả game.
 *
 * TODO (mức 1 -- task 3):
 *   - Giống level 2 nhưng dùng logger_->log(...).
 *   - Truyền `gameState_.board` (immutable) vào renderer thay vì mảng cũ.
 */
void Engine::endGame(const GameResult& /*gameResult*/) {
    if (logger_) logger_->log("[Engine] Ending game . . .");

    if (!sanity_check()) {
        if (logger_) logger_->log("[Engine] Game stopped!", Logger::Level::ERROR);
        return;
    }

    // TODO (mức 1)
    throw NotImplementedException();
}

/* ---------- close ---------- */

void Engine::close() {
    if (logger_) logger_->log("Engine closing . . .");

    if (iRenderer_)    iRenderer_->close();
    if (iInteraction_) iInteraction_->close();

    if (logger_) logger_->log("Engine closed!");
}
