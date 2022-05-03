#include <tuple>
#include <iostream>
#include <algorithm>

#include "sdr_cleaner.h"

bool str2bool(std::string_view str) {
    std::string lower_str;
    std::transform(str.begin(), str.end(), lower_str.begin(), ::tolower);

    return lower_str == "true" || lower_str == "on" || lower_str == "yes" || lower_str == "1";
}

void print_usage(std::ostream &out, const char *name) {
    out << "Usage: " << name << " [cleaning path] [print message]";
}

std::tuple<std::string, bool> parse_args(const int argc, char *argv[]) {
    switch (argc) {
        case 1:
            return {"/mnt/us/documents", false};
        case 2:
            return {argv[1], false};
        case 3:
            return {argv[1], str2bool(argv[2])};
        default:
            std::cerr << "Invalid number of parameters, use default value." << std::endl;
            print_usage(std::cerr, argv[0]);
            return {"/mnt/us/documents", false};
    }
}

int main(int argc, char *argv[]) {
    try {
        const auto [cleaning_path, print_message] = parse_args(argc, argv);
        SdrCleaner::clean(cleaning_path, print_message);
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        exit(-1);
    }

    return 0;
}
