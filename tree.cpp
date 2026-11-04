#include <print>
#include <filesystem>
#include <cstdint>
#include <string>

namespace Console {
    constexpr std::string console = "[CONSOLE]";
    constexpr std::string error = "[ERROR]";
}

namespace Colours {
    constexpr std::string BLUE = { "\033[34m" };
    constexpr std::string WHITE = { "\033[37m" };
    constexpr std::string GREEN = { "\033[32m" };
    constexpr std::string ORANGE = { "\033[38;5;208m" };
}

namespace Counters {
    std::uint32_t directoryFileCounter{};
    std::uint32_t executableFileCounter{};
    std::uint32_t pngFileCounter{};
    std::uint32_t fileCounter{};
}

bool is_file_png(const std::filesystem::directory_entry& dir) {
    const std::filesystem::path& pngFile(dir);
    return (pngFile.extension() == ".png");
}

bool is_file_executable(const std::filesystem::directory_entry& dir) {
    const auto perms = std::filesystem::status(dir).permissions();
    return (perms & std::filesystem::perms::owner_exec) != std::filesystem::perms::none;
}

void print_tree(const std::filesystem::path& currentPath) {
    std::println("{}", currentPath.string());
    std::println("|");
    for(const std::filesystem::directory_entry& dir : std::filesystem::recursive_directory_iterator(currentPath)) {
        std::print("{}├", Colours::WHITE);
        if(dir.is_directory()) {
            ++Counters::directoryFileCounter;
            std::println("{}{}", Colours::BLUE, dir.path().lexically_relative(currentPath).string());
        } else if(is_file_executable(dir)) {
            ++Counters::executableFileCounter;
            std::println("{}{}", Colours::GREEN, dir.path().lexically_relative(currentPath).string());
        } else if(is_file_png(dir)) {
            ++Counters::pngFileCounter;
            std::println("{}{}", Colours::ORANGE, dir.path().lexically_relative(currentPath).string());
        } else {
            std::println("{}{}", Colours::WHITE, dir.path().lexically_relative(currentPath).string());
        }

        ++Counters::fileCounter;
    }

    std::println("{}", Colours::WHITE);
    std::println("{} directory(ies), {} executable(s), {} png(s), and {} total file(s).", Counters::directoryFileCounter, Counters::executableFileCounter, Counters::pngFileCounter, Counters::fileCounter);
}

int main(int argc, char* argv[]) {
    std::filesystem::path currentPath(".");
    if(argc > 1) {
        if(!std::filesystem::is_directory(argv[1])) {
            std::println("{} {} is not a directory!", Console::error, argv[1]);
            return 0;
        }

        currentPath = argv[1];
        print_tree(currentPath);
        return 0;
    }

    print_tree(currentPath);
}
