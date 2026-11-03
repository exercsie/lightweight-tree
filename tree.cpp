#include <iostream>
#include <filesystem>

#define BLUE "\033[34m"
#define WHITE "\033[37m"
#define GREEN "\033[32m"

void print_tree(std::filesystem::path currentPath) {
    for(const std::filesystem::directory_entry& dir : std::filesystem::recursive_directory_iterator(currentPath)) {
        if(dir.is_directory()) {
            std::cout << dir << " is dirc\n";
        }
    }
}

int main(int argc, char* argv[]) {
    std::filesystem::path currentPath(".");
    std::cout << argv[0] << std::endl;
    print_tree(currentPath);
}