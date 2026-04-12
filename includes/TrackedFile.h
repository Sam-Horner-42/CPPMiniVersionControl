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
    std::string content;
    int editCount =   0;
    status currentStatus;
    std::string commitMessage;

    public:
	TrackedFile() {};
    TrackedFile(const std::string& filePath, const std::string& fileName, status status) {}; //initial file creation, will have other params set to null or Added status
    TrackedFile(const std::string& filePath, const std::string& fileName, status status, const std::string& content) {}; //everything constructor
	TrackedFile(const std::string& fileName, status status) {}; //constructor for editing status
    ~TrackedFile() {};

    void setFilePath(std::string filePath);
    void setFileName(std::string fileName);
    void setStatus(status newStatus);
    void setContent(std::string newContent);

    std::string getFileContent() const { return content; }

    std::vector<std::string> displayFileInfo();
    std::string getFileName() const;
    std::string getFilePath() const;
    std::string getContent() const;
    TrackedFile::status getFileStatus() const;

    std::string getStatusAsString() const;
    void setStatusAsString(std::string newStatusString);
    
    std::string getCommitMessage() const;
    int incrementEditCount();
    void setCommitMessage(std::string commitMsg);

    bool operator==(const TrackedFile& other) {
	    return this->getFilePath() == other.getFilePath();
    }

    int getEditCount() const;
};

#endif