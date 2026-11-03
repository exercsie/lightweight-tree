#include <iostream>
#include <filesystem>

#define BLUE "\033[34m"
#define WHITE "\033[37m"
#define GREEN "\033[32m"

bool is_file_executable(const std::filesystem::directory_entry& dir) {
    const auto perms = std::filesystem::status(dir).permissions();
    return (perms & std::filesystem::perms::owner_exec) != std::filesystem::perms::none;
}

void print_tree(std::filesystem::path currentPath) {
    int directoryCounter{};
    int fileCounter{};
    std::cout << currentPath.string() << std::endl;
    std::cout << "|" << std::endl;
    for(const std::filesystem::directory_entry& dir : std::filesystem::recursive_directory_iterator(currentPath)) {
        std::cout << WHITE << "|";
        if(dir.is_directory()) {
            ++directoryCounter;
            std::cout << "-";
            std::cout << BLUE << dir.path().lexically_relative(currentPath).string() << std::endl;
        } else if(is_file_executable(dir)) {
            std::cout << "-";
            std::cout << GREEN << dir.path().lexically_relative(currentPath).string() << std::endl;
        } else {
            std::cout << "-";
            std::cout << WHITE << dir.path().lexically_relative(currentPath).string() << std::endl;
        }

        ++fileCounter;
    }

    std::cout << WHITE << std::endl;
    std::cout << directoryCounter << " directories, " << fileCounter << " files.\n";
}

int main(int argc, char* argv[]) {
    std::filesystem::path currentPath(".");
    print_tree(currentPath);
}