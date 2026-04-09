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
    int editCount;
    status currentStatus;
    std::string commitMessage;

    public:
		TrackedFile() {};
    TrackedFile(const std::string& filePath, status status);
	~TrackedFile() {};

    enum class status {Added, Modified, Staged, Committed};

    void setNamePath(std::string filePath);
    void setStatus(status newStatus);

    std::vector<std::string> displayFileInfo();
    std::string getFileName() const;
    std::string getFilePath() const;
    status getFileStatus() const;
    std::string getCommitMessage() const;
    void setCommitMessage(std::string commitMsg);

    bool operator==(TrackedFile& other) {
	    return this->getFilePath() == other.getFilePath();
    }

    int getEditCount() const;
};

#endif