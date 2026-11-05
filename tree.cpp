#include <cstdlib>
#include <cstring>
#include <print>
#include <filesystem>
#include <cstdint>
#include <string_view>

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

struct Counters {
    std::uint32_t directoryFileCounter{};
    std::uint32_t executableFileCounter{};
    std::uint32_t pngFileCounter{};
    std::uint32_t fileCounter{};
};

struct Flags {
    bool ignore_dot_file_{ true };
};

bool is_file_png(const std::filesystem::directory_entry& dir) {
    const std::filesystem::path& pngFile(dir);
    return (pngFile.extension() == ".png");
}

bool is_file_executable(const std::filesystem::directory_entry& dir) {
    const auto perms = std::filesystem::status(dir).permissions();
    return (perms & std::filesystem::perms::owner_exec) != std::filesystem::perms::none;
}

void print_tree(Counters& counter, const std::filesystem::path& currentPath, const std::filesystem::path& pathToIgnore = "", const bool ignore_dot_file_ = false) {
    std::println("{}", currentPath.string());
    std::println("|");

    const std::string ignoreDotFiles = pathToIgnore.string();
    for(const std::filesystem::directory_entry& dir : std::filesystem::recursive_directory_iterator(currentPath)) {
        std::string dir_entry = dir.path().lexically_relative(currentPath).string();
        if(ignore_dot_file_) {
            if(dir_entry.at(0) == ignoreDotFiles.at(0)) {
                continue;
            }
        }

        std::print("{}├", Colours::WHITE);
        if(dir.is_directory()) {
            ++counter.directoryFileCounter;
            std::println("{}{}", Colours::BLUE, dir_entry);
        } else if(is_file_executable(dir)) {
            ++counter.executableFileCounter;
            std::println("{}{}", Colours::GREEN, dir_entry);
        } else if(is_file_png(dir)) {
            ++counter.pngFileCounter;
            std::println("{}{}", Colours::ORANGE, dir_entry);
        } else {
            std::println("{}{}", Colours::WHITE, dir_entry);
        }

        ++counter.fileCounter;
    }

    std::println("{}", Colours::WHITE);
    std::println("{} directory(ies), {} executable(s), {} png(s), and {} total file(s).", counter.directoryFileCounter, counter.executableFileCounter, counter.pngFileCounter, counter.fileCounter);
}

void input_handling(Counters& counter, std::filesystem::path& currentPath, int argc, char* argv[]) {
    const Flags flag;
    // print tree in current directory
    if(!(argc > 1)) {
        print_tree(counter, currentPath);
        exit(0);
    }

    // Ignore every directory starting with argv[2]
    if(std::strcmp(argv[1], "-I") == 0) {
        // Make sure the user entered a directory after the -I flag
        if(argc != 3) {
            std::println("{} Please enter a directory to ignore!", Console::error);
            exit(1);
        }

        std::string_view checkArg2HasOneVal(argv[2]);
        if(checkArg2HasOneVal.length() != 1) {
            std::println("{} Please only enter a single character!", Console::error);
            exit(1);
        }

        if(!std::filesystem::is_directory(argv[2])) {
            std::println("{} {} is not a directory!", Console::error, argv[2]);
            exit(1);
        }

        currentPath = argv[2];
        print_tree(counter, ".", argv[2], flag.ignore_dot_file_);
        exit(0);
    }

    if(!std::filesystem::is_directory(argv[1])) {
        std::println("{} {} is not a directory!", Console::error, argv[1]);
        exit(1);
    }

    currentPath = argv[1];
    print_tree(counter, currentPath);
}

int main(int argc, char* argv[]) {
    Counters counter;
    std::filesystem::path currentPath(".");

    input_handling(counter, currentPath, argc, argv);
}
