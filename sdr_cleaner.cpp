//
// Created by Kotarou on 2022/4/28.
//

#include "sdr_cleaner.h"

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <filesystem>

struct FILE_TYPE_MASK {
    static constexpr int DEFAULT = 0;
    static constexpr int BOOK_FILE = 2;
    static constexpr int SDR_FOLDER = 4;
};

static int removeOrphanedSdr(const std::filesystem::path &, const std::map<std::string, int> &, bool);

std::map<std::string, int> searchOrphanedSdr(const std::filesystem::path &);

std::vector<std::filesystem::path> getAllSubdirectories(std::string_view);

static constexpr char SDR_EXTENSION[] = ".sdr";

static constexpr int SDR_ONLY_FLAG = FILE_TYPE_MASK::SDR_FOLDER;

void SdrCleaner::clean(std::string_view root_path, bool print_msg) {
    auto working_dir = std::filesystem::path(root_path);

    // handle invalid path
    if (!std::filesystem::exists(working_dir)) {
        throw std::runtime_error(std::string(working_dir) + " does not exist.");
    }

    // get all sub-directories
    auto directories = getAllSubdirectories(root_path);

    auto total_removed = 0;
    for (auto &path: directories) {
        auto result = searchOrphanedSdr(path);
        total_removed += removeOrphanedSdr(path, result, print_msg);
    }

    std::cout << "Deleted " << total_removed << " directories" << std::endl;
}

std::vector<std::filesystem::path> getAllSubdirectories(std::string_view base_path) {
    std::vector<std::filesystem::path> subdirectories;

    // add base path to the vector
    subdirectories.emplace_back(base_path);

    // iterate all directories
    for (auto &p: std::filesystem::directory_iterator(base_path)) {
        if (std::filesystem::is_directory(p)) {
            // directory without .sdr extension should be considered as candidate for recursive sdr cleaning
            if (p.path().extension().compare(SDR_EXTENSION) != 0) {
                subdirectories.emplace_back(p.path());
            }
        }
    }

    return subdirectories;
}

std::map<std::string, int> searchOrphanedSdr(const std::filesystem::path &path) {
    std::map<std::string, int> result;

    for (auto &p: std::filesystem::directory_iterator(path)) {
        auto key = p.path().stem().string();

        // add path to map
        result.try_emplace(key, FILE_TYPE_MASK::DEFAULT);

        if (p.is_directory()) {
            // if is sdr directory apply sdr flag
            if (p.path().extension().compare(SDR_EXTENSION) == 0) {
                result[key] |= FILE_TYPE_MASK::SDR_FOLDER;
            }
        } else {
            result[key] |= FILE_TYPE_MASK::BOOK_FILE;
        }
    }

    return result;
}

int removeOrphanedSdr(const std::filesystem::path &base_dir, const std::map<std::string, int> &search_result,
                      bool print_message) {
    int deleted = 0;
    for (const auto &[key, value]: search_result) {
        if (value == SDR_ONLY_FLAG) {
            auto directory_name = std::string(key) + SDR_EXTENSION;
            auto target = std::filesystem::path(base_dir) / directory_name;

            if (std::filesystem::exists(target)) {
                if (std::filesystem::remove_all(target)) {
                    if (print_message) {
                        std::cout << "Delete directory: " << directory_name << std::endl;
                    }
                    ++deleted;
                }
            }
        }
    }

    return deleted;
}
