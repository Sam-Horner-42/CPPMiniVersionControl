
#include "../includes/TrackedFile.h"

//functions
void TrackedFile::updateContent(std::string filePath, status fileStatus) {

    //class filePath variable is set to the passed in filePath parameter
    this->filePath = filePath;

    //Sets filename to the name of the file with file extension
    //This works by finding the last slash in the filepath and then going past that
    this->fileName = filePath.substr(filePath.find_last_of('/') + 1);

    //opens the file using the filePath parameter
    std::ifstream file(filePath);
    if (file.is_open()) { //checks if file is opened properly, if so then clear the content in the object to get a blank slate
        content.clear();
        std::string line;
        while (getline(file, line)) { //use "line" string to get the whole file line by line passing it into the content vector
            content.push_back(line);
        }
        file.close(); //close the file after the contents have been put into the content vector line by line
    }
    else {
        content.push_back("UpdateContent() Could not read file content"); //if unable 
    }
    //counter for my compute most modified files 
    if (content != this->content) {
        this->editCount++;
        this->content = content;
    }
    else {
        this->content.clear();
    }

    this->currentStatus = fileStatus;
}

/**
 * Mano-TODO: please fix to not use couts
 * we are going to be passing this function to the GUI so rather than void or cout it should have a return with the string of all the file info
 * do the exact same thing but use a single string rather than multiple cout lines and it will be good
 */
std::vector<std::string> TrackedFile::displayFileInfo() {  //fixed to be a vector<string> return since it was set to void which cant return anything
    std::vector<std::string> info;
    info.push_back("File: " + fileName);
    info.push_back("Path: " + filePath);

    std::string statusStr;
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
    info.push_back("Total Edits: " + std::to_string(editCount));
    return info;
}

std::string TrackedFile::getFileName() const {
    return fileName;
}
std::string TrackedFile::getFilePath() const { // readded const to have it work
    return filePath;
}

std::vector<std::string> TrackedFile::getFileContent() const {
    return content;
}

TrackedFile::status TrackedFile::getFileStatus() const {
    return currentStatus;
}

std::string TrackedFile::getCommitMessage() const {
    return commitMessage;
}

void TrackedFile::setCommitMessage(std::string commitMsg) {
    this->commitMessage = commitMsg;
}

int const TrackedFile::getEditCount() { //fixed to have const in the correct spot
    return editCount; 
}