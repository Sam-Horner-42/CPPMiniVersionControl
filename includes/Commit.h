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
//#include "Repository.h"

class Commit {

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
    : commitId(commitId),parentId(parentId),message(message),
    author(author), timestamp(timestamp) {}

    // class getters
    std::string getParentId() const { return parentId; }
    std::string getAuthor() const { return author; }
    std::string getTimestamp() const { return timestamp; }
    std::string getId() const { return commitId; }
    std::string getMessage() const { return message; }

    

    std::vector<TrackedFile> const getFiles();
    
    // virtual functions
    virtual bool hasFile(const std::string& filename) = 0;
    virtual std::vector<std::string> getAllAttributes() = 0;
    virtual std::vector<std::string> displayCommit() = 0;
    virtual std::string getSummary() = 0;
};

#endif
