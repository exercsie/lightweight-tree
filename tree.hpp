#pragma once

#include <string>
#include <cstdint>
#include <filesystem>

namespace Console {
    constexpr std::string_view console = "[CONSOLE]";
    constexpr std::string_view error = "[ERROR]";
}

namespace Colours {
    constexpr std::string_view BLUE = { "\033[34m" };
    constexpr std::string_view WHITE = { "\033[37m" };
    constexpr std::string_view GREEN = { "\033[32m" };
    constexpr std::string_view ORANGE = { "\033[38;5;208m" };
}

class Tree {
public:
    // Checkers
    static bool is_file_png(const std::filesystem::directory_entry& dir);
    static bool is_file_executable(const std::filesystem::directory_entry& dir);
    static void print_tree(const std::filesystem::path& currentPath = ".", const std::filesystem::path& pathToIgnore = "");
    static int flag_handling(std::filesystem::path& currentPath, int argc, const std::string& inputtedFlag, std::string_view inputtedDirectory);
    static void directory_checker_handling(std::string_view directory);

    // Structs
    struct Counters {
    std::uint32_t directoryFileCounter{};
    std::uint32_t executableFileCounter{};
    std::uint32_t pngFileCounter{};
    std::uint32_t fileCounter{};
    };

    struct Flags {
        bool ignore_dot_files_{ false };
        bool ignore_specfiic_directory_{ false };
    };

    struct StringData {
        std::string inputtedFlag;
        std::string_view inputtedDirectory;
    };

    struct States {
        Counters counter;
        Flags flag;
        StringData data;
    };

    static States state;
};
