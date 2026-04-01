#ifndef TRACKED_FILE_H
#define TRACKED_FILE_H
#pragma once;

#ifndef TRACKEDFILE_H
#define TRACKEDFILE_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

class TrackedFile {

    public:
<<<<<<< HEAD
    TrackedFile();
    TrackedFile(const string& filePath, const string& status);
    ~TrackedFile();

    void updateContent(string filePath, string status);
    void displayFileInfo();
    string const getFileName();
    string const getFilePath();
    vector<string> const getFileContent();
    string const TrackedFile::getFileStatus();
    int const getEditCount();
    private:
    string filePath;
    string fileName;
    vector<string> content;
    string status; //The status of a file will be Modified, Staged, or Committed, and i guess NULL when undefined
    int editCount;
};

#endif
=======
    void updateContent(std::string filePath);
    void displayFileInfo();
    std::string getFileName() const;
    std::string getFilePath() const;

    private:
    std::string filePath;
    std::string fileName;
    std::string content;
    std::string status;

};

#endif
>>>>>>> 9fd035e (changed hpp to h, worked on repo and diffEngine classes - Jacob)
