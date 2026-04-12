#include "../includes/TrackedFile.h"

using namespace std;

//functions
void TrackedFile::setFilePath(string filePath) {
    this->filePath = filePath;
}

void TrackedFile::setFileName(string fileName) {
    this->fileName = fileName;
}

void TrackedFile::setStatus(status newStatus) {
    this->currentStatus = newStatus;
}

std::string TrackedFile::getStatusAsString() const {
    switch(currentStatus) {
        case status::Added:
            return "Added";
        case status::Modified:
            return "Modified";
        case status::Staged:
            return "Staged";
        case status::Committed:
            return "Committed";
        default:
            return "???";
    }
}

void TrackedFile::setStatusAsString(std::string newStatusString) {
    if(newStatusString == "Added") {
        this->currentStatus = status::Added;
    }
    else if(newStatusString == "Modified") {
        this->currentStatus = status::Modified;
    }
    else if(newStatusString == "Staged") {
        this->currentStatus = status::Staged;
    }
    else if(newStatusString == "Committed") {
        this->currentStatus = status::Committed;
    }
}

void TrackedFile::setContent(std::string newContent) {
    this->content = newContent;
}

string TrackedFile::getFileName() const {
    return fileName;
}
string TrackedFile::getFilePath() const {
    return filePath;
}

TrackedFile::status TrackedFile::getFileStatus() const {
    return currentStatus;
}

std::string TrackedFile::getContent() const {
    return content;
}

void TrackedFile::setCommitMessage(string commitMsg) {
    this->commitMessage = commitMsg;
}

int TrackedFile::getEditCount() const {
    return editCount; 
}
int TrackedFile::incrementEditCount() {
    return editCount++;
}