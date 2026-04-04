#ifndef TRACKED_FILE_H
#define TRACKED_FILE_H

#pragma once;

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

class TrackedFile {

    public:
    TrackedFile();
    TrackedFile(const std::string& filePath, const std::string& status);
    ~TrackedFile();

    void updateContent(std::string filePath, std::string status);
    std::vector<std::string> displayFileInfo();
    std::string const getFileName();
    std::string const getFilePath();
    std::vector<std::string> const getFileContent();
    std::string const TrackedFile::getFileStatus();
    int const getEditCount();
    private:
    std::string filePath;
    std::string fileName;
    std::vector<std::string> content;
    enum status; //The status of a file will be Modified, Staged, or Committed, and i guess NULL when undefined
    int editCount;
};

#endif