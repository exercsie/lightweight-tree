#include "tree.hpp"

#include <cstdlib>
#include <cstring>
#include <print>
#include <filesystem>
#include <string_view>
#include <string>

Tree::States Tree::state{};

bool Tree::is_file_png(const std::filesystem::directory_entry& dir) {
    const std::filesystem::path& pngFile(dir);
    return (pngFile.extension() == ".png");
}

bool Tree::is_file_executable(const std::filesystem::directory_entry& dir) {
    const auto perms = std::filesystem::status(dir).permissions();
    return (perms & std::filesystem::perms::owner_exec) != std::filesystem::perms::none;
}

void Tree::print_tree(const std::filesystem::path& currentPath, const std::filesystem::path& pathToIgnore) {
    std::println("{}", currentPath.string());
    std::println("|");

    const std::string ignore = pathToIgnore.string();
    for(const std::filesystem::directory_entry& dir : std::filesystem::recursive_directory_iterator(currentPath)) {
        std::string dir_entry = dir.path().lexically_relative(currentPath).string();
        if(Tree::state.flag.ignore_dot_files_) {
            if(dir_entry.at(0) == ignore.at(0)) {
                continue;
            }
        }

        if(Tree::state.flag.ignore_specfiic_directory_) {
            if(dir_entry == ignore) {
                continue;
            }
        }

        std::print("{}├", Colours::WHITE);
        if(dir.is_directory()) {
            ++Tree::state.counter.directoryFileCounter;
            std::println("{}{}", Colours::BLUE, dir_entry);
        } else if(is_file_executable(dir)) {
            ++Tree::state.counter.executableFileCounter;
            std::println("{}{}", Colours::GREEN, dir_entry);
        } else if(is_file_png(dir)) {
            ++Tree::state.counter.pngFileCounter;
            std::println("{}{}", Colours::ORANGE, dir_entry);
        } else {
            std::println("{}{}", Colours::WHITE, dir_entry);
        }

        ++Tree::state.counter.fileCounter;
    }

    std::println("{}", Colours::WHITE);
    std::println("{} directory(ies), {} executable(s), {} png(s), and {} total file(s).", Tree::state.counter.directoryFileCounter, Tree::state.counter.executableFileCounter, state.counter.pngFileCounter, state.counter.fileCounter);
}

void Tree::directory_checker_handling(const std::string_view directory) {
    if(!std::filesystem::is_directory(directory)) {
        std::println("{} {} is not a directory!", Console::error, directory);
        exit(1);
    }
}

int Tree::flag_handling(std::filesystem::path& currentPath, const int argc, const std::string& inputtedFlag, const std::string_view inputtedDirectory) {
    // Ignore every directory starting with argv[2]
    if(std::strcmp(inputtedFlag.c_str(), "-I") == 0) {
        // Make sure the user entered a directory after the -I flag
        if(argc != 3) {
            std::println("{} Must have three arguments! tree [flag] [dir]", Console::error);
            return 1;
        }

        const std::string_view checkArg2HasOneVal(inputtedDirectory);
        if(checkArg2HasOneVal.length() != 1) {
            std::println("{} Please only enter a single character!", Console::error);
            return 1;
        }

        directory_checker_handling(inputtedDirectory);
        state.flag.ignore_dot_files_ = true;
        print_tree(".", inputtedDirectory);
        return 0;
    }

    // Ignore a specific directory
    if(std::strcmp(inputtedFlag.c_str(), "-Id") == 0) {
        if(argc != 3) {
            std::println("{} Please enter a directory to ignore!", Console::error);
            return 1;
        }

        directory_checker_handling(inputtedDirectory);
        state.flag.ignore_specfiic_directory_ = true;
        print_tree(".", inputtedDirectory);
        return 0;
    }

    print_tree(inputtedFlag);
    return 1;
}

int main(int argc, char* argv[]) {
    std::filesystem::path currentPath(".");

    // Print current directory tree
    if(!(argc > 1)) {
        Tree::print_tree();
        return 1;
    }

    // Print argv[1] directory specified tree
    if(argc == 2) {
        Tree::directory_checker_handling(argv[1]);
        Tree::print_tree(argv[1]);
        return 1;
    }

    // Print current directory tree with specified flags
    Tree::state.data.inputtedFlag = argv[1];
    Tree::state.data.inputtedDirectory = argv[2];
    return Tree::flag_handling(currentPath, argc, Tree::state.data.inputtedFlag, Tree::state.data.inputtedDirectory);
}
