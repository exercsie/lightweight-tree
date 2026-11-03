#include <iostream>
#include <filesystem>
#include <vector>

namespace Colours {
    char BLUE[] = "\033[34m";
    char WHITE[] = "\033[37m";
    char GREEN[] = "\033[32m";
}

namespace Counters {
    int directoryFileCounter{};
    int executableFileCounter{};
    int fileCounter{};
}

bool is_file_executable(const std::filesystem::directory_entry& dir) {
    const auto perms = std::filesystem::status(dir).permissions();
    return (perms & std::filesystem::perms::owner_exec) != std::filesystem::perms::none;
}

void print_tree(std::filesystem::path currentPath) {
    std::cout << currentPath.string() << std::endl;
    std::cout << "|" << std::endl;
    for(const std::filesystem::directory_entry& dir : std::filesystem::recursive_directory_iterator(currentPath)) {
        std::cout << Colours::WHITE << "|";
        if(dir.is_directory()) {
            ++Counters::directoryFileCounter;
            std::cout << "-";
            std::cout << Colours::BLUE << dir.path().lexically_relative(currentPath).string() << std::endl;
        } else if(is_file_executable(dir)) {
            ++Counters::executableFileCounter;
            std::cout << "-";
            std::cout << Colours::GREEN << dir.path().lexically_relative(currentPath).string() << std::endl;
        } else {
            std::cout << "-";
            std::cout << Colours::WHITE << dir.path().lexically_relative(currentPath).string() << std::endl;
        }

        ++Counters::fileCounter;
    }

    std::cout << Colours::WHITE << std::endl;
    std::cout << Counters::directoryFileCounter << " directories, " << Counters::executableFileCounter << " executables, and " << Counters::fileCounter << " total files.\n";
}

int main(int argc, char* argv[]) {
    std::filesystem::path currentPath(".");
    print_tree(currentPath);
}