
#include "../includes/TrackedFile.h"

using namespace std;

//functions
void TrackedFile::setNamePath(string filePath) {

    //class filePath variable is set to the passed in filePath parameter
    this->filePath = filePath;

    //Sets filename to the name of the file with file extension
    //This works by finding the last slash in the filepath and then going past that
    this->fileName = filePath.substr(filePath.find_last_of('/') + 1);
}

void TrackedFile::setStatus(status newStatus) {
    this->currentStatus = newStatus;
}

vector<string> TrackedFile::displayFileInfo() {
    vector<string> info;
    info.push_back("File: " + fileName);
    info.push_back("Path: " + filePath);

    string statusStr;
    switch (currentStatus) {
        case status::Added:
            statusStr = "Added";
            break;
        case status::Modified:
            statusStr = "Modified";
            break;
        case status::Staged:
            statusStr = "Staged";
            break;
        case status::Committed:
            statusStr = "Committed";
            break;
    }

    info.push_back("Status: " + statusStr);
    info.push_back("Total Edits: " + to_string(editCount));
    return info;
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

void TrackedFile::setCommitMessage(string commitMsg) {
    this->commitMessage = commitMsg;
}

int TrackedFile::getEditCount() const {
    return editCount; 
}