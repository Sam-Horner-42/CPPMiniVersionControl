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
#include "Repository.h"

class Commit {
private:
    std::string commitId;
    std::string parentId;
    std::string message;
    std::string author;
    std::string timestamp;
public:
    // default constructor
    Commit(
        const std::string& commitId,
        const std::string& parentId,
        const std::string& message,
        const std::string& author,
        const std::string& timestamp
    )
    : commitId(commitId),message(message),
    author(author), timestamp(timestamp) {}

    // class getters
    std::string getId() { return commitId; }
    std::string getParentCommit() { return parentId; }
    std::string getMessage() { return message; }
    std::string getAuthor() { return author; }
    std::string getTimestamp() { return timestamp; }

    public:
    string const getId();
    string const getDate();
    string const getMessages();
    vector<TrackedFile> const getFiles();
    void displayCommit();
    void getSummary();

    static bool checkStagedFiles(vector<TrackedFile> fileVector);
    static bool compareHashedFiles(TrackedFile comparingStagedFile);
    static void addToCommitVector(bool tf, TrackedFile addingStagedFile);
    static void clearCommitVector();
    vector<string> commitToRepo(vector<TrackedFile> commitFiles);
};

#endif
