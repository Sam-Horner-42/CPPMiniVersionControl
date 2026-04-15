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
	TrackedFile()
		: filePath(""), fileName(""), content(""), editCount(0), currentStatus(status::Added), commitMessage("") {}

	TrackedFile(const std::string& filePath, const std::string& fileName)
		: filePath(filePath), fileName(fileName), content(""), editCount(0), currentStatus(status::Added), commitMessage("") {}

	TrackedFile(const std::string& filePath, const std::string& fileName, status newStatus, const std::string& content)
		: filePath(filePath), fileName(fileName), content(content), editCount(0), currentStatus(newStatus), commitMessage("") {}

	TrackedFile(const std::string& fileName, status newStatus)
		: filePath(""), fileName(fileName), content(""), editCount(0), currentStatus(newStatus), commitMessage("") {}

	TrackedFile(const std::string& filePath, const std::string& fileName, status newStatus, int editCount)
		: filePath(filePath), fileName(fileName), content(""), editCount(editCount), currentStatus(newStatus), commitMessage("") {}

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
	int getEditCount() const;
	
    TrackedFile::status getFileStatus() const;

    std::string getStatusAsString() const;
    void setStatusAsString(std::string newStatusString);
    
    std::string getCommitMessage() const;
    int incrementEditCount();
    void setCommitMessage(std::string commitMsg);

    bool operator==(const TrackedFile& other) {
	    return this->getFilePath() == other.getFilePath();
    }

    
};

#endif