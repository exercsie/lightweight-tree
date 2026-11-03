#include <iostream>
#include <filesystem>

#define BLUE "\033[34m"
#define WHITE "\033[37m"
#define GREEN "\033[32m"

int main(int argc, char* argv[]) {
    std::filesystem::path currentPath(".");
    std::cout << argv[0] << std::endl;
}