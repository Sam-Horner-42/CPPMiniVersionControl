
#include "../includes/TrackedFile.h"


//attributes
string filePath;
string fileName;
string content;
<<<<<<< HEAD
string status; //The status of a file will be Modified, Staged, or Committed, and i guess NULL when undefined
int editCount;
=======
string status; 
//The status of a file will be Modified, Staged, or Committed, and i guess NULL when undefined

>>>>>>> da4c519a7f5ece25b0c3812917dd3bebfb6608a0
//functions
void TrackedFile::updateContent(string filePath) {

    //class filePath variable is set to the passed in filePath parameter
    this->filePath = filePath;

    //Sets filename to the name of the file with file extension
    //This works by finding the last slash in the filepath and then going past that
    this->fileName = filePath.substr(filePath.find_last_of('/') + 1);

    //opens the file using the filePath parameter
    ifstream file(filePath);
<<<<<<< HEAD
    if (file.is_open()) {
        vector<string> newContent;
        string line;
        while (getline(file, line)) {
            newContent.push_back(line);
=======
    if (file.is_open()) { //checks if file is opened properly, if so then clear the content in the object
        content.clear();
        string line; 
        while (getline(file, line)) { //use "line" string to get the whole file line by line passing it into the content vector
            content.push_back(line);
>>>>>>> da4c519a7f5ece25b0c3812917dd3bebfb6608a0
        }
        file.close(); //close the file after the contents have been put into the content vector line by line
    }
    else {
        content.push_back("UpdateContent() Could not read file content"); //if unable 
    }
    //counter for my compute most modified files 
    if (newContent != this->content) {
        this->editCount++;
        this->content = newContent;
    }
<<<<<<< HEAD
    else {
        this->content.clear();
    //TODO: status
=======


>>>>>>> da4c519a7f5ece25b0c3812917dd3bebfb6608a0
    this->status = status;
}

/**
 * Mano-TODO: please fix to not use couts
 * we are going to be passing this function to the GUI so rather than void or cout it should have a return with the string of all the file info
 * do the exact same thing but use a single string rather than multiple cout lines and it will be good
 */
void TrackedFile::displayFileInfo() {
    vector<string> info;
    info.push_back("File: " + fileName);
    info.push_back("Path: " + filePath);
    info.push_back("Status: " + status);
    info.push_back("Total Edits: " + to_string(editCount));
    return info;
}

string const TrackedFile::getFileName() {
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