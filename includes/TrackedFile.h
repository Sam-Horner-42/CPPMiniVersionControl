#ifndef TRACKED_FILE_H
#define TRACKED_FILE_H

#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

class TrackedFile {

    public:
    void updateContent(std::string filePath);
    void displayFileInfo();
    std::string getFileName() const;
    std::string getFilePath() const;

    void updateContent(string filePath, string status);
    void displayFileInfo();
    std::string getFileName() const;
    std::string getFileName() const;
    string getFilePath();
    vector<string> getFileContent() const;
    Status::Status getFileStatus();
    int const getEditCount();
    enum class status {Added, Modified, Staged, Committed};
    
    void updateContent(std::string filePath);
    void displayFileInfo();
    std::string getFileName() const;
    std::string getFilePath() const;

    private:
    std::string filePath;
    std::string fileName;
    std::vector<std::string> content;
    int editCount;
    status currentStatus;

    public:
    TrackedFile();
    TrackedFile(const std::string& filePath, const std::string& status);
    ~TrackedFile();

    void updateContent(std::string filePath, status fileStatus);

    std::vector<std::string> displayFileInfo();
    std::string getFileName() const;
    std::string getFilePath() const;
    std::vector<std::string> getFileContent() const;
    status getFileStatus() const;

    int const getEditCount();
};

#endif
