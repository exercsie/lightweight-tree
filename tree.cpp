#include <print>
#include <filesystem>
#include <cstdint>

namespace Colours {
    constexpr char BLUE[] = { "\033[34m" };
    constexpr char WHITE[] = { "\033[37m" };
    constexpr char GREEN[] = { "\033[32m" };
}

namespace Counters {
    std::uint32_t directoryFileCounter{};
    std::uint32_t executableFileCounter{};
    std::uint32_t fileCounter{};
}

bool is_file_executable(const std::filesystem::directory_entry& dir) {
    const auto perms = std::filesystem::status(dir).permissions();
    return (perms & std::filesystem::perms::owner_exec) != std::filesystem::perms::none;
}

void print_tree(const std::filesystem::path& currentPath) {
    std::println("{}", currentPath.string());
    std::println("|");
    for(const std::filesystem::directory_entry& dir : std::filesystem::recursive_directory_iterator(currentPath)) {
        std::print("{}|", Colours::WHITE);
        if(dir.is_directory()) {
            ++Counters::directoryFileCounter;
            std::print("-");
            std::println("{}{}", Colours::BLUE, dir.path().lexically_relative(currentPath).string());
        } else if(is_file_executable(dir)) {
            ++Counters::executableFileCounter;
            std::print("-");
            std::println("{}{}", Colours::GREEN, dir.path().lexically_relative(currentPath).string());
        } else {
            std::print("-");
            std::println("{}{}", Colours::WHITE, dir.path().lexically_relative(currentPath).string());
        }

        ++Counters::fileCounter;
    }

    std::println("{}", Colours::WHITE);
    std::println("{} directories, {} executables, and {} total files.", Counters::directoryFileCounter, Counters::executableFileCounter, Counters::fileCounter);
}

int main(int argc, char* argv[]) {
    std::filesystem::path currentPath(".");
    print_tree(currentPath);
}
