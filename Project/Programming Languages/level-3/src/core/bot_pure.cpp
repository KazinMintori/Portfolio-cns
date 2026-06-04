/**
 * Bot (FP-style) implementation
 */

#include "bot_pure.h"

/* ---------- Importing ---------- */

#include "logic.h"
#include "pipeline.h"

namespace bot {

/* ---------- Easy ---------- */

/**
 * Mô tả: Bot Easy -- random pick.
 *
 * TODO (mức 1 -- task 2, "Higher-order function cho Bot LV1"):
 *
 *   Yêu cầu KHÔNG dùng for-loop tích luỹ. Có 2 cách triển khai gợi ý:
 *
 *   ── Cách 1: dùng `fp::pipe(...)` từ pipeline.h ──
 *       auto moves = core::enumerateValidMoves(state.board);
 *       if (moves.empty()) return INVALID_MOVE;
 *       std::uniform_int_distribution<int> dist(0, (int)moves.size() - 1);
 *       return moves[dist(rng)];
 *
 *   ── Cách 2: full pipeline + std::ranges ──
 *       using namespace std::ranges;
 *       auto cells   = core::enumerateCells(state.board);
 *       auto valid   = cells | views::filter([&](Move m){
 *                                  return core::isValidMove(state.board, m);
 *                              });
 *       auto vec     = std::vector<Move>(valid.begin(), valid.end());
 *       ...
 *
 *   Lưu ý:
 *     - KHÔNG được dùng `generator` toàn cục -- tham số `rng` bắt buộc.
 *     - Phải xử lý trường hợp board đầy → trả về INVALID_MOVE.
 */
Move easy(const GameState& /*state*/, Rng& /*rng*/) {
    // TODO (Mức 1 -- task 2)
    throw NotImplementedException();
}

/* ---------- Medium ---------- */

/**
 * Mô tả: Bot Medium -- heuristic 3 bước.
 *
 * TODO (Mức 2 -- Bot, sinh viên tự quyết áp dụng FP vào bot nào):
 *   Khuyến khích bộc lộ rõ:
 *     - Composition: ráp 3 bước lại thành 1 pipeline.
 *     - Higher-order: viết hàm `tryFirstNonEmpty(strategies)` trả về Move
 *       đầu tiên không phải INVALID_MOVE.
 *     - Closure / currying: cố định `state` & `goal` cho các predicate.
 *
 *   Có thể fallback `easy(state, rng)` nếu chưa làm.
 */
Move medium(const GameState& /*state*/, int /*goal*/, Rng& /*rng*/) {
    // TODO (Mức 2 -- Bot)
    throw NotImplementedException();
}

/* ---------- Hard ---------- */

/**
 * Mô tả: Bot Hard -- minimax / heuristic mạnh.
 *
 * TODO:
 *   - Mức 2: ưu tiên cài bằng FP (currying, composition).
 *   - Mức 3 nhánh B: TÁCH `evaluateAllMoves` thành pure function rời
 *     trong `core/parallel.h` để chạy được song song.
 */
Move hard(const GameState& /*state*/, int /*goal*/, Rng& /*rng*/) {
    // TODO (Mức 2/3 -- Bot)
    throw NotImplementedException();
}

/* ---------- Factory ---------- */

/**
 * Mô tả: Trả về BotFn ứng với level.
 *
 * TODO (mức 1 -- task 2, đi kèm easy()):
 *   - Mỗi nhánh trả về một LAMBDA (đóng gói goal vào closure):
 *
 *     case EASY:
 *         return [](const GameState& s, Rng& r) { return easy(s, r); };
 *
 *     case MEDIUM:
 *         return [goal](const GameState& s, Rng& r) {
 *             return medium(s, goal, r);
 *         };
 *
 *     case HARD:
 *         return [goal](const GameState& s, Rng& r) {
 *             return hard(s, goal, r);
 *         };
 *
 *     default:
 *         return [](const GameState&, Rng&) { return INVALID_MOVE; };
 *
 * Đây là cũng là một ví dụ HOF (factory trả về function).
 */
BotFn makeBot(BotLevel /*level*/, int /*goal*/) {
    // TODO (Mức 1 -- task 2)
    throw NotImplementedException();
}

}  // namespace bot
