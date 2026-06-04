/**
 * Terminal Interaction cpp implementation
 *
 */

#include "interaction.h"
#include <chrono>
#include <format>
#include <iostream>
#include <limits>
#include <thread>

#include "../utils/logger.h"

TerminalInteraction::TerminalInteraction()
{
}

/**
 * Mô tả: Destructor của TerminalInteraction.
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ: Không.
 */
TerminalInteraction::~TerminalInteraction()
{
}

/**
 * Mô tả: Khởi tạo interaction (thiết lập nguồn input).
 * Đầu vào: config - cấu hình runtime.
 * Đầu ra: Không.
 * Tác dụng phụ: Có thể redirect std::cin sang file input.
 */
void TerminalInteraction::init(const RunConfig &config)
{
    initInteraction(config);
}

/**
 * Mô tả: Thiết lập nguồn input cho chương trình (console hoặc file).
 * Đầu vào: config - cấu hình runtime.
 * Đầu ra: Không.
 * Tác dụng phụ:
 *   - Redirect std::cin sang file nếu chạy non-interactive.
 *   - Ghi log trạng thái.
 * NOTE: Nếu file không mở được, sẽ fallback về console.
 */
void TerminalInteraction::initInteraction(const RunConfig &config)
{
    cin_backup = nullptr;

    if (!config.interactive && !config.input_file.empty())
    {
        global_file_in.open(config.input_file);
        if (global_file_in.is_open())
        {
            cin_backup = std::cin.rdbuf();
            std::cin.rdbuf(global_file_in.rdbuf());

            Logger::log(std::format("redirected cin to: {}", config.input_file));
        }
        else
        {
            Logger::log("failed to open input file, using console.", Logger::Level::ERROR);
        }
    }
}

/**
 * Mô tả: Khôi phục lại nguồn input ban đầu và đóng file nếu có.
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ:
 *   - Reset std::cin về buffer gốc.
 *   - Đóng file input.
 */
void TerminalInteraction::closeInteraction()
{
    if (cin_backup)
    {
        std::cin.rdbuf(cin_backup);
        Logger::log("fallback using 'std::cin' input stream.");
    }

    if (global_file_in.is_open())
    {
        global_file_in.close();
    }
}

/**
 * Mô tả: Kiểm tra chuỗi input có hợp lệ (chỉ chứa ký tự số) hay không.
 * Đầu vào: input - chuỗi nhập từ người dùng.
 * Đầu ra: true nếu hợp lệ, false nếu chứa ký tự không phải số hoặc rỗng.
 * Tác dụng phụ: Không.
 * TODO:
 *   - Bước 1: Kiểm tra chuỗi rỗng.
 *   - Bước 2: Duyệt từng ký tự và kiểm tra có phải số.
 *   - Trường hợp biên: input rỗng hoặc chứa ký tự đặc biệt.
 */
bool TerminalInteraction::validateInput(const std::string &input)
{
    if (input.empty())
        return false;
    for (char c : input)
        if (!std::isdigit(c))
            return false;
    return true;
}

bool TerminalInteraction::getInput(int *val)
{
    if (std::cin >> *val)
        return true;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return false;
}

void TerminalInteraction::pause(int timeout)
{
    if (timeout > 0)
        std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
}

bool TerminalInteraction::selectSize(int *size)
{
    int val;
    if (!getInput(&val))
        return false;
    if (val >= BOARD_N_MIN && val <= BOARD_N_MAX)
    {
        *size = val;
        return true;
    }
    return false;
}

bool TerminalInteraction::selectGoal(int *goal, const int size)
{
    int val;
    if (!getInput(&val))
        return false;
    if (val >= 3 && val <= size)
    {
        *goal = val;
        return true;
    }
    return false;
}

bool TerminalInteraction::selectGameMode(GameMode *mode)
{
    int val;
    if (!getInput(&val))
        return false;
    switch (val)
    {
    case 1:
        *mode = GameMode::PVP;
        return true;
    case 2:
        *mode = GameMode::PVE;
        return true;
    case 3:
        *mode = GameMode::EVE;
        return true;
    }
    return false;
}

bool TerminalInteraction::selectBotLevel(BotLevel *levels, const int index)
{
    int val;
    if (!getInput(&val))
        return false;
    switch (val)
    {
    case 1:
        levels[index] = BotLevel::EASY;
        return true;
    case 2:
        levels[index] = BotLevel::MEDIUM;
        return true;
    case 3:
        levels[index] = BotLevel::HARD;
        return true;
    }
    return false;
}

bool TerminalInteraction::getPlayerMove(int *row, int *col)
{
    if (!(std::cin >> *row >> *col))
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    return true;
}

/**
 * Mô tả: Đóng interaction và giải phóng tài nguyên liên quan.
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ:
 *   - Khôi phục std::cin nếu đã bị redirect.
 *   - Đóng file input nếu đang mở.
 */
void TerminalInteraction::close()
{
    closeInteraction();
}