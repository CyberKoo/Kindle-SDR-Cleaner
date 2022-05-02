#include <iostream>

#include "sdr_cleaner.h"

int main(int argc, char *argv[]) {
    try {
        std::string cleaning_path;
        if (argc < 2) {
            // used on jailbroken kindle
            cleaning_path = "/mnt/us/documents";
            std::cout << "No cleaning given, use default path." << std::endl;
        } else {
            cleaning_path = argv[1];
        }

        SdrCleaner::clean(cleaning_path);
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        exit(-1);
    }

    return 0;
}
