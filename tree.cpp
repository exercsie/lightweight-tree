#include <iostream>
#include <filesystem>

#define BLUE "\033[34m"
#define WHITE "\033[37m"
#define GREEN "\033[32m"

void print_tree(std::filesystem::path currentPath) {
    std::cout << currentPath.string() << std::endl;
    std::cout << "|" << std::endl;
    for(const std::filesystem::directory_entry& dir : std::filesystem::recursive_directory_iterator(currentPath)) {
        std::cout << WHITE << "|";
        if(dir.is_directory()) {
            std::cout << "-";
            std::cout << BLUE << dir << std::endl;
        } else {
            std::cout << "-";
            std::cout << WHITE << dir << std::endl;
        }
    }
}

int main(int argc, char* argv[]) {
    std::filesystem::path currentPath(".");
    print_tree(currentPath);
}