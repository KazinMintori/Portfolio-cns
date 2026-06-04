/**
 * Core Logic implementation (PURE)
 *
 * Mô tả:
 *   Các stub TODO cho sinh viên mức 1 -- yêu cầu task 1 ("FP-hoá Logic::*").
 *
 * Quy tắc khi cài đặt:
 *   - KHÔNG include <iostream>, KHÔNG dùng Logger (đây là tầng pure).
 *   - KHÔNG dùng biến toàn cục, KHÔNG random ở đây.
 *   - Trả về giá trị mới thay vì sửa tham số.
 *   - Khuyến khích std::ranges (C++20) và đệ quy.
 *
 * NOTE:
 *   File này không include "../shell/logger.h" hay tương tự -- nếu cần log,
 *   phải log ở tầng `shell/` SAU KHI gọi pure function.
 */

#include "logic.h"

/* ---------- Importing ---------- */

// (chỉ import standard, KHÔNG import shell/)
// #include <algorithm>
// #include <ranges>
// #include <numeric>
// #include "pipeline.h"   // fp::pipe, fp::filter, fp::map, fp::reduce

namespace core {

/* ---------- Construction ---------- */

/**
 * Mô tả: Tạo board rỗng.
 *
 * Gợi ý FP: đây là phép "khởi tạo" -- không có dữ liệu cũ để bất biến hoá,
 * nên dùng style mệnh lệnh để init local rồi return cũng OK.
 */
Board initBoard(int /*size*/) {
    // TODO (Mức 1 -- task 1)
    //
    // Bước 1: Tạo Board mới (Board b;) -- mặc định grid đã zero-init.
    //
    // Bước 2: Set b.size = size.
    //
    // Bước 3: Điền EMPTY_CELL vào các ô [0..size) x [0..size).
    //         (các ô ngoài phạm vi không cần care.)
    //
    // Bước 4: return b;

    throw NotImplementedException();
}

/* ---------- Queries ---------- */

/**
 * Mô tả: Nước đi có hợp lệ?
 *
 * Hàm này phải PURE -- không log, không side-effect.
 */
bool isValidMove(const Board& /*board*/, Move /*move*/) {
    // TODO (Mức 1 -- task 1)
    //
    // Trả về true nếu:
    //   - move.row, move.col nằm trong [0..board.size)  (gợi ý: board.inRange)
    //   - board.at(move.row, move.col) == EMPTY_CELL

    throw NotImplementedException();
}

/**
 * Mô tả: Liệt kê tất cả các Move (r, c) trên board.
 *
 * Đây là "nguồn" cho các pipeline filter/map/reduce phía sau.
 * Vì input là Board (không phải sẵn 1 container), enumerateCells là phép
 * "khởi tạo container" -- không cần dùng pipeline.h ở chính nó.
 */
std::vector<Move> enumerateCells(const Board& /*board*/) {
    // TODO (Mức 1 -- task 1d)
    //
    // Bước 1: Tạo vector<Move> rỗng (out).
    //
    // Bước 2: Đẩy mọi cặp (r, c) với r, c trong [0..board.size) vào out.
    //         Có thể chọn 1 trong các cách (yêu cầu PURE):
    //           (i) Đệ quy đuôi:
    //                  enumHelper(r, c, acc) -> acc when r == size
    //                                        -> enumHelper(r, c+1, acc++move) when c+1 < size
    //                                        -> enumHelper(r+1, 0,  acc++move) ngược lại
    //          (ii) 2 vòng for + push_back (mệnh lệnh, đơn giản).
    //         (iii) C++23: std::ranges::cartesian_product(...) | std::ranges::to<...>.
    //
    // Bước 3: return out.

    throw NotImplementedException();
}

/**
 * Mô tả: Liệt kê các nước đi hợp lệ (= các ô trống).
 *
 * Đây là chỗ rất phù hợp để DÙNG `pipeline.h` -- input là 1 vector<Move>
 * (từ enumerateCells), output cũng là vector<Move>. Đúng dạng filter HOF.
 */
std::vector<Move> enumerateValidMoves(const Board& /*board*/) {
    // TODO (Mức 1 -- task 1c, ví dụ pipeline filter -> map -> reduce)
    //
    // ƯU TIÊN cách (a) -- dùng pipeline.h đã cung cấp sẵn:
    //
    //    auto pickValid = fp::pipe(
    //        ...,
    //        fp::filter([&](...) {
    //            return ...;
    //        })
    //    );
    //
    //    return ...(board);
    //
    // Ý tưởng:
    //   - enumerate toàn bộ ô / nước đi
    //   - filter lại chỉ giữ move hợp lệ
    //   - pipeline sẽ tự nối các bước với nhau
    //
    // Nếu chưa quen pipeline, có thể tự cài bằng C++ chuẩn:
    //
    //   (b) C++20 ranges:
    //
    //       auto valid = ...(board)
    //                  | std::views::filter([&](...) {
    //                        return ...;
    //                    });
    //
    //       return {...};
    //
    //   Gợi ý:
    //     - filter view chỉ "nhìn" dữ liệu, chưa tạo vector mới
    //     - cần convert / copy sang container phù hợp
    //
    //   (c) C++23 ranges::to:
    //
    //       return ...(board)
    //            | std::views::filter(...)
    //            | std::ranges::to<...>();
    //
    //   Gợi ý:
    //     - đọc thử type của expression sau từng bước pipeline
    //
    //   (d) Cổ điển:
    //
    //       std::vector<Move> result;
    //
    //       std::copy_if(
    //           ...,
    //           ...,
    //           std::back_inserter(result),
    //           [&](...) {
    //               return ...;
    //           }
    //       );
    //
    //   Gợi ý:
    //     - nghĩ xem begin/end lấy từ đâu
    //     - predicate nên kiểm tra điều gì

    throw NotImplementedException();
}

/**
 * Mô tả: Ô (x, y) có phải đầu mở của chuỗi `symbol` không?
 *
 * Pure: chỉ đọc board, không log, không global.
 */
bool isEmptyHead(const Board& /*board*/, int /*x*/, int /*y*/, char /*symbol*/) {
    // TODO (Mức 1 -- task 1)
    //
    // Cài đặt theo luật Gomoku đã làm ở Level 2 -- chỉ cần BỎ phần log.

    throw NotImplementedException();
}

/* ---------- Transitions ---------- */

/**
 * Mô tả: Tạo Board MỚI sau khi đặt symbol vào ô move.
 *
 * Đây là viên gạch nền của tính BẤT BIẾN -- nhớ KHÔNG sửa `board` đầu vào.
 */
Board applyMove(const Board& /*board*/, Move /*move*/, char /*symbol*/) {
    // TODO (Mức 1 -- task 1, LÕI CỦA TÍNH BẤT BIẾN)
    //
    // Bước 1: Tạo một bản sao local từ board hiện tại.
    //
    //   Gợi ý:
    //     - có thể copy trực tiếp cả struct
    //     - mục tiêu là mọi thay đổi đều diễn ra trên bản mới
    //
    // Bước 2: Ghi symbol vào đúng vị trí của move trên BẢN COPY.
    //
    //   Gợi ý:
    //     - truy cập ô bằng row / col
    //     - chỉ sửa đúng 1 cell cần thiết
    //
    // Bước 3: Trả về board mới.
    //
    // GHI NHỚ:
    //   - tuyệt đối KHÔNG động vào `board` (nó là const tham chiếu)
    //   - hãy tự kiểm tra xem hàm của bạn có còn side effect không
    //   - nếu xóa biến local copy đi thì điều gì xảy ra?

    throw NotImplementedException();
}

/**
 * Mô tả: Tính GameState MỚI sau 1 lượt đi.
 *
 * Đây là hàm "fold step" -- Engine ở Mức 2 chỉ cần GỌI hàm này, KHÔNG mutate.
 * Mỗi lượt sinh ra một GameState mới, state cũ vẫn còn nguyên.
 */
GameState applyTurn(const GameState& /*state*/, Move /*move*/, int /*goal*/) {
    // TODO (Mức 2 -- Engine không mutate board)
    //
    // Mục tiêu:
    //   - mỗi lượt đi tạo ra "state mới"
    //   - state cũ phải giữ nguyên hoàn toàn
    //
    // Bước 1: Xác định symbol tương ứng với currentPlayer.
    //
    //   Gợi ý:
    //     - player id != symbol hiển thị
    //     - nên có hàm mapping riêng thay vì hard-code
    //
    //   Ví dụ: char sym = symbolOf(state.currentPlayer);
    //
    // Bước 2: Tạo board mới sau khi đánh move.
    //
    //   Gợi ý:
    //     - dùng lại hàm immutable đã viết ở task trước
    //     - board cũ không được thay đổi
    //
    // Bước 3: Tạo next state từ state hiện tại.
    //
    //   Gợi ý:
    //     - copy toàn bộ state trước
    //     - sau đó chỉ cập nhật các field cần đổi
    //
    // Bước 4: Kiểm tra game đã kết thúc chưa.
    //
    //   Gợi ý:
    //     - có 2 trường hợp kết thúc phổ biến
    //     - nghĩ xem kiểm tra thắng hay hòa trước hợp lý hơn
    //     - nếu game kết thúc, cần cập nhật thêm field nào?
    //
    // Bước 5: Nếu game chưa kết thúc:
    //   - đổi lượt player
    //   - tăng turn
    //
    //   Gợi ý:
    //     - currentPlayer thường luân phiên giữa 2 giá trị
    //     - tránh magic number nếu có thể
    //
    // Bước 6: return state mới.
    //
    // Tự kiểm tra:
    //   - state truyền vào có bị thay đổi không?
    //   - next.board có dùng board mới không?
    //   - winner / isFinished có nhất quán không?

    throw NotImplementedException();
}

/* ---------- End-of-game ---------- */

/**
 * Mô tả: Có người thắng?
 *
 * Có thể là ứng viên cho chứng minh ở nhánh A (mức 3).
 */
bool checkWin(const Board& /*board*/, char /*symbol*/, int /*goal*/, EndRule /*rule*/) {
    // TODO (Mức 1 -- task 1)
    //
    // Cài lại checkWin của Level 2 nhưng PURE
    // -> chỉ đọc board, KHÔNG log debug, KHÔNG gọi Logger.

    throw NotImplementedException();
}

/**
 * Mô tả: Hoà? (không còn ô trống và chưa có người thắng)
 *
 * PURE.
 */
bool checkDraw(const Board& /*board*/) {
    // TODO (Mức 1 -- task 1)
    //
    // Hoà khi không còn ô trống. Có thể tái dùng countSymbol:
    //
    //    return countSymbol(board, EMPTY_CELL) == 0;
    //
    // (Lưu ý: hàm này KHÔNG kiểm tra "đã có ai thắng chưa" -- gọi sau checkWin.)

    throw NotImplementedException();
}

/**
 * Mô tả: Lấy đường thắng (nếu có).
 */
std::optional<WinLine> getWinLine(const Board& /*board*/, char /*symbol*/,
                                  int /*goal*/, EndRule /*rule*/) {
    // TODO (Mức 1 -- task 1)
    return std::nullopt;
}

/* ---------- Helpers ---------- */

/**
 * Mô tả: Đếm số ô = symbol.
 *
 * Đây là hàm rất phù hợp để CHỨNG MINH ở nhánh A (mức 3):
 *   countSymbol(applyMove(b, m, s), s) = countSymbol(b, s) + 1
 *   với điều kiện isValidMove(b, m) == true.
 */
int countSymbol(const Board& /*board*/, char /*symbol*/) {
    // TODO (Mức 1 -- task 1d)
    //
    // Có nhiều hướng cài đặt.
    // ƯU TIÊN thử cách functional hơn so với cách imperative.
    //
    // (a) ƯU TIÊN -- dùng pipeline.h (filter -> map -> reduce):
    //
    //     Ý tưởng 1: enumerate toàn bộ cell
    //         -> giữ lại cell thỏa điều kiện
    //         -> reduce để đếm số lượng
    //
    //     Khung:
    //
    //         auto result = fp::pipe(
    //             ...,
    //             fp::filter([&](...) {
    //                 return ...;
    //             }),
    //             fp::reduce(..., [](int acc, ...) {
    //                 return ...;
    //             })
    //         );
    //
    //         return ...(board);
    //
    //     Ý tưởng 2: dùng map thay vì filter:
    //         -> map mỗi cell -> 0 hoặc 1
    //         -> rồi cộng tổng lại
    //
    //     Gợi ý:
    //         - map dùng để "biến đổi"
    //         - reduce dùng để "gộp kết quả"
    //         - suy nghĩ xem accumulator nên bắt đầu từ đâu
    //
    //     Khung tương tự ý tưởng 1.
    //
    // (b) Đệ quy đuôi (rất phù hợp để chứng minh quy nạp ở mức 3A):
    //
    //     Ý tưởng: viết helper(idx, acc):
    //         -> nếu đã duyệt hết -> trả acc
    //         -> ngược lại, cập nhật acc và gọi tiếp helper(...)
    //
    //     Gợi ý:
    //         - idx tuyến tính có thể đổi sang (row, col)
    //         - thử nghĩ:
    //             row = ?
    //             col = ?
    //         - acc đóng vai trò gì trong tail recursion?
    //
    //     Khung:
    //
    //         helper(idx, acc):
    //             if idx == ...: return acc
    //             else: return helper(idx + 1, acc + ...)
    //
    // (c) 2 vòng for + biến đếm (mệnh lệnh, đơn giản nhất).
    //
    //     Gợi ý:
    //         - duyệt toàn bộ board
    //         - gặp symbol cần tìm thì tăng biến đếm

    throw NotImplementedException();
}

}  // namespace core
