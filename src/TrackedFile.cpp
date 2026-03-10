
#include "../includes/TrackedFile.h"


//attributes
string filePath;
string fileName;
string content;
string status; //The status of a file will be Modified, Staged, or Committed, and i guess NULL when undefined

//functions
void TrackedFile::updateContent(string filePath) {

    //class filePath variable is set to the passed in filePath parameter
    this->filePath = filePath; 

    //Sets filename to the name of the file with file extension
    //This works by finding the last slash in the filepath and then going past that
    this->fileName = filePath.substr(filePath.find_last_of('/') + 1);

    //TODO: using iostream to apply file content from the filePath into the content string variable

    //TODO: status

}

void TrackedFile::displayFileInfo() {
    cout << content;
}

string TrackedFile::getFileName() {
    return fileName;
}