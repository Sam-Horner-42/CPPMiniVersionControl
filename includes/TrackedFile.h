#ifndef TRACKED_FILE_H
#define TRACKED_FILE_H

#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

class TrackedFile {

    public:
    enum class status {Added, Modified, Staged, Committed};
    
    private:
    std::string filePath;
    std::string fileName;
    std::vector<std::string> content;
    int editCount;
    status currentStatus;
    std::string commitMessage;

    public:
    TrackedFile(const std::string& filePath, status status);
    ~TrackedFile();

    void updateContent(std::string filePath, status fileStatus);

    std::vector<std::string> displayFileInfo();
    std::string getFileName() const;
    std::string getFilePath() const;
    std::vector<std::string> getFileContent() const;
    status getFileStatus() const;
    std::string getCommitMessage() const;
    void setCommitMessage(std::string commitMsg);

    int const getEditCount();
};

#endif