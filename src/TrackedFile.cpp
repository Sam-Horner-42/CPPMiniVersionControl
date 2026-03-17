
#include "../includes/TrackedFile.h"


//functions
void TrackedFile::updateContent(string filePath, string status) {

    //class filePath variable is set to the passed in filePath parameter
    this->filePath = filePath; 

    //Sets filename to the name of the file with file extension
    //This works by finding the last slash in the filepath and then going past that
    this->fileName = filePath.substr(filePath.find_last_of('/') + 1);

    //TODO: using fstream to apply file content from the filePath into the content string variable
    ifstream file(filePath);
    if (file.is_open()) {
        content.clear();

        string line;
        while (getline(file, line)) {
            content.push_back(line);
        }

        file.close();
    }

    //TODO: status
    this->status = status;

}

void TrackedFile::displayFileInfo() {
    //TODO: this function
}

string const TrackedFile::getFileName() {
    return fileName;
}

string const TrackedFile::getFilePath() {
    return filePath;
}

vector<string> const TrackedFile::getFileContent() {
    return content;
}

string const TrackedFile::getFileStatus() {
    return status;
}