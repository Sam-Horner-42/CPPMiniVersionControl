#ifndef COMMIT_H
#define COMMIT_H
#pragma once;

/*
 * class definitions for the commit class
 * Author: Jacob Dawes - 041169788, Spencer Scarlett 041151395, Ethan Geary 041032340
 */


#include <string>
#include <map>
#include <vector>

#include "TrackedFile.h"
#include <Repository.h>

class Commit {

    public:
    string const getId();
    string const getDate();
    string const getMessages();
    vector<TrackedFile> const getFiles();
    void displayCommit();
    void getSummary();

    bool checkStagedFiles(vector<TrackedFile> filesVector);
    bool compareHashedFiles(TrackedFile comparingStagedFile);
    void addToCommitVector(bool tf, TrackedFile addingStagedFile);
    void clearCommitVector();
    vector<TrackedFile> commitToRepo();

    private:
    string commitID;
    string message;
    string author;
    string timestamp;
    vector<TrackedFile> files;
    vector<TrackedFile> commitVector;
};

#endif