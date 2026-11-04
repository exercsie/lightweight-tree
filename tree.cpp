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

bool is_file_png(const std::filesystem::directory_entrsy& dir) {
    const std::filesystem::path& pngFile(dir);
    return (pngFile.extension() == ".png");
}

bool is_file_executable(const std::filesystem::directory_entry& dir) {
    const auto perms = std::filesystem::status(dir).permissions();
    return (perms & std::filesystem::perms::owner_exec) != std::filesystem::perms::none;
}

void print_tree(Counters& counter, const std::filesystem::path& currentPath) {
    std::println("{}", currentPath.string());
    std::println("|");
    for(const std::filesystem::directory_entry& dir : std::filesystem::recursive_directory_iterator(currentPath)) {
        std::print("{}├", Colours::WHITE);
        if(dir.is_directory()) {
            ++counter.directoryFileCounter;
            std::println("{}{}", Colours::BLUE, dir.path().lexically_relative(currentPath).string());
        } else if(is_file_executable(dir)) {
            ++counter.executableFileCounter;
            std::println("{}{}", Colours::GREEN, dir.path().lexically_relative(currentPath).string());
        } else if(is_file_png(dir)) {
            ++counter.pngFileCounter;
            std::println("{}{}", Colours::ORANGE, dir.path().lexically_relative(currentPath).string());
        } else {
            std::println("{}{}", Colours::WHITE, dir.path().lexically_relative(currentPath).string());
        }

        ++counter.fileCounter;
    }

    std::println("{}", Colours::WHITE);
    std::println("{} directory(ies), {} executable(s), {} png(s), and {} total file(s).", counter.directoryFileCounter, counter.executableFileCounter, counter.pngFileCounter, counter.fileCounter);
}

int main(int argc, char* argv[]) {
    Counters counter;
    std::filesystem::path currentPath(".");
    if(argc > 1) {
        if(!std::filesystem::is_directory(argv[1])) {
            std::println("{} {} is not a directory!", Console::error, argv[1]);
            return 0;
        }

        currentPath = argv[1];
        print_tree(counter, currentPath);
        return 0;
    }

    print_tree(counter, currentPath);
}
