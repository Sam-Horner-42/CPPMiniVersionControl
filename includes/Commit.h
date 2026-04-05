#ifndef COMMIT_H
#define COMMIT_H
#pragma once

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
    std::string getParentId() const { return parentId; }
    std::string getAuthor() const { return author; }
    std::string getTimestamp() const { return timestamp; }

    std::string getId() const;
    std::string getDate() const;
    std::string getMessage() const;

    std::vector<std::string> getAllAttributes();
    Commit* getParentCommit();

    vector<TrackedFile> const getFiles();
    void displayCommit();
    void getSummary();
};

#endif
