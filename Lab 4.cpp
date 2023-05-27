#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <windows.h>

namespace fs = std::filesystem;
using namespace std;

bool hasValidAttributes(const fs::path& filePath) {
    DWORD attributes = GetFileAttributes(filePath.c_str());
    if (attributes != INVALID_FILE_ATTRIBUTES &&
        (attributes & FILE_ATTRIBUTE_HIDDEN) == 0 &&
        (attributes & FILE_ATTRIBUTE_READONLY) == 0 &&
        (attributes & FILE_ATTRIBUTE_ARCHIVE) == 0) {
        return true;
    }
    return false;
}

long long calculateDirectorySize(const fs::path& directoryPath, const string& filePattern) {
    if (!fs::exists(directoryPath) || !fs::is_directory(directoryPath)) {
        cout << "Invalid directory path." << endl;
        return 0;
    }

    long long totalSize = 0;

    for (const auto& entry : fs::recursive_directory_iterator(directoryPath)) {
        if (fs::is_regular_file(entry) && entry.path().extension() == filePattern) {
            if (hasValidAttributes(entry.path())) {
                totalSize += fs::file_size(entry);
            }
        }
    }

    return totalSize;
}

int main() {
    vector<string> directoryPaths;
    string directoryPath;
    string filePattern;

    cout << "Enter directory paths (enter 'done' to finish):" << endl;

    while (true) {
        cout << "Directory path: ";
        getline(cin, directoryPath);

        if (directoryPath == "done") {
            break;
        }

        directoryPaths.push_back(directoryPath);
    }

    cout << "Enter file pattern (For example: .txt): ";
    getline(cin, filePattern);

    if (filePattern.empty()) {
        cout << "Invalid file pattern." << endl;
        return 1;
    }

    long long totalSize = 0;

    for (const auto& path : directoryPaths) {
        totalSize += calculateDirectorySize(path, filePattern);
    }

    cout << "Total size of files matching pattern " << filePattern << " in the directories: " << totalSize << " bytes." << endl;

    return 0;
}