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

inline States state;

bool is_file_png(const std::filesystem::directory_entry& dir) {
    const std::filesystem::path& pngFile(dir);
    return (pngFile.extension() == ".png");
}

bool is_file_executable(const std::filesystem::directory_entry& dir) {
    const auto perms = std::filesystem::status(dir).permissions();
    return (perms & std::filesystem::perms::owner_exec) != std::filesystem::perms::none;
}

void print_tree(const std::filesystem::path& currentPath = ".", const std::filesystem::path& pathToIgnore = "") {
    std::println("{}", currentPath.string());
    std::println("|");

    const std::string ignore = pathToIgnore.string();
    for(const std::filesystem::directory_entry& dir : std::filesystem::recursive_directory_iterator(currentPath)) {
        std::string dir_entry = dir.path().lexically_relative(currentPath).string();
        if(state.flag.ignore_dot_files_) {
            if(dir_entry.at(0) == ignore.at(0)) {
                continue;
            }
        }

        if(state.flag.ignore_specfiic_directory_) {
            if(dir_entry == ignore) {
                continue;
            }
        }

        std::print("{}├", Colours::WHITE);
        if(dir.is_directory()) {
            ++state.counter.directoryFileCounter;
            std::println("{}{}", Colours::BLUE, dir_entry);
        } else if(is_file_executable(dir)) {
            ++state.counter.executableFileCounter;
            std::println("{}{}", Colours::GREEN, dir_entry);
        } else if(is_file_png(dir)) {
            ++state.counter.pngFileCounter;
            std::println("{}{}", Colours::ORANGE, dir_entry);
        } else {
            std::println("{}{}", Colours::WHITE, dir_entry);
        }

        ++state.counter.fileCounter;
    }

    std::println("{}", Colours::WHITE);
    std::println("{} directory(ies), {} executable(s), {} png(s), and {} total file(s).", state.counter.directoryFileCounter, state.counter.executableFileCounter, state.counter.pngFileCounter, state.counter.fileCounter);
}

void directory_checker_handling(const std::string_view directory) {
    if(!std::filesystem::is_directory(directory)) {
        std::println("{} {} is not a directory!", Console::error, directory);
        exit(1);
    }
}

void flag_handling(std::filesystem::path& currentPath, int argc, const std::string inputtedFlag, const std::string_view inputtedDirectory) {
    // Ignore every directory starting with argv[2]
    if(std::strcmp(inputtedFlag.c_str(), "-I") == 0) {
        // Make sure the user entered a directory after the -I flag
        if(argc != 3) {
            std::println("{} Must have three arguments! tree [flag] [dir]", Console::error);
            exit(1);
        }

        const std::string_view checkArg2HasOneVal(inputtedDirectory);
        if(checkArg2HasOneVal.length() != 1) {
            std::println("{} Please only enter a single character!", Console::error);
            exit(1);
        }

        directory_checker_handling(inputtedDirectory);
        state.flag.ignore_dot_files_ = true;
        print_tree(".", inputtedDirectory);
        exit(0);
    }

    // Ignore a specific directory
    if(std::strcmp(inputtedFlag.c_str(), "-Id") == 0) {
        if(argc != 3) {
            std::println("{} Please enter a directory to ignore!", Console::error);
            exit(1);
        }

        directory_checker_handling(inputtedDirectory);
        state.flag.ignore_specfiic_directory_ = true;
        print_tree(".", inputtedDirectory);
        exit(0);
    }

    print_tree(inputtedFlag);
}

int main(int argc, char* argv[]) {
    std::filesystem::path currentPath(".");

    // Print current directory tree
    if(!(argc > 1)) {
        print_tree();
        return 1;
    }

    // Print argv[1] directory specified tree
    if(argc == 2) {
        directory_checker_handling(argv[1]);
        print_tree(argv[1]);
        return 1;
    }

    // Print current directory tree with specified flags
    state.data.inputtedFlag = argv[1];
    state.data.inputtedDirectory = argv[2];
    flag_handling(currentPath, argc, state.data.inputtedFlag, state.data.inputtedDirectory);
}
