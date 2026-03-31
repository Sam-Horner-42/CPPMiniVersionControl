
#include "../includes/TrackedFile.h"


//attributes
string filePath;
string fileName;
string content;
string status; //The status of a file will be Modified, Staged, or Committed, and i guess NULL when undefined
int editCount;
//functions
void TrackedFile::updateContent(string filePath) {

    //class filePath variable is set to the passed in filePath parameter
    this->filePath = filePath;

    //Sets filename to the name of the file with file extension
    //This works by finding the last slash in the filepath and then going past that
    this->fileName = filePath.substr(filePath.find_last_of('/') + 1);

    //TODO: using iostream to apply file content from the filePath into the content string variable
    ifstream file(filePath);
    if (file.is_open()) {
        vector<string> newContent;
        string line;
        while (getline(file, line)) {
            newContent.push_back(line);
        }
        file.close();
    }
    //counter for my compute most modified files 
    if (newContent != this->content) {
        this->editCount++;
        this->content = newContent;
    }
    else {
        this->content.clear();
    //TODO: status
    this->status = status;
}

void TrackedFile::displayFileInfo() {
    vector<string> info;
    info.push_back("File: " + fileName);
    info.push_back("Path: " + filePath);
    info.push_back("Status: " + status);
    info.push_back("Total Edits: " + to_string(editCount));
    return info;
}

string TrackedFile::getFileName() {
    return fileName;
}
string TrackedFile::getFilePath() {
    return filePath;
}

vector<string> const TrackedFile::getFileContent() {
    return content;
}

string const TrackedFile::getFileStatus() {
    return status;
}
int TrackedFile::getEditCount() const { return editCount; }