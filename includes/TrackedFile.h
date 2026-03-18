#pragma once;

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class TrackedFile {

    public:
    TrackedFile();
    TrackedFile(const string& filePath, const string& status);
    ~TrackedFile();

    void updateContent(string filePath, string status);
    void displayFileInfo();
    string const getFileName();
    string const getFilePath();
    vector<string> const getFileContent();
    string const TrackedFile::getFileStatus();

    private:
    string filePath;
    string fileName;
    vector<string> content;
    string status; //The status of a file will be Modified, Staged, or Committed, and i guess NULL when undefined

}; //test change lol