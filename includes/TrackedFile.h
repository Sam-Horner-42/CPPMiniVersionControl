#ifndef TRACKED_FILE_H
#define TRACKED_FILE_H

#include <string>
#include <iostream>

using namespace std;

class TrackedFile {

    public:
    void updateContent(string filePath);
    void displayFileInfo();
    string getFileName();

    private:
    string filePath;
    string fileName;
    string content;
    string status;

};

#endif