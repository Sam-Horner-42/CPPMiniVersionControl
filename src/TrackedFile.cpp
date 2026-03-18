
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
    ifstream file(filePath);
    if (file.is_open()) {
        content.clear();
        string line;
        while (getline(file, line)) {
            content += line + "\n";
        }
        file.close();
    }
    else {
        content = "[Error: Could not read file content]";
    }


    //TODO: status
    this->status = status;
}

void TrackedFile::displayFileInfo() {
    cout << "\n==========================================" << endl;
    cout << "           TRACKED FILE STATUS            " << endl;
    cout << "==========================================" << endl;

    cout << left << setw(18) << "File Name:" << fileName << endl;
    cout << left << setw(18) << "System Path:" << filePath << endl;
    cout << left << setw(18) << "VCS Status:" << "[" << status << "]" << endl;

    cout << left << setw(18) << "Content Size:" << content.length() << " bytes" << endl;

    cout << "==========================================\n" << endl;
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