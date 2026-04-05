/*
    Definition of the Standard Commit class
*/
#pragma once

#include <vector>
#include <string>
#include <map>

#include "../includes/Commit.h"

class StandardCommit : public Commit {
private:
    /* key is the file name, value is file content */
    std::map<std::string, std::string> fileSnapshot;
public:
    StandardCommit(
        const std::string& commitId,
        const std::string& parentId,
        const std::string& message,
        const std::string& author,
        const std::string& timestamp
    );
    ~StandardCommit();

    void createSnapshot(const std::string& filename, const std::vector<std::string>& content);
    void updateSnapshot(const std::string& filename, const std::vector<std::string>& content);
    std::vector<std::string> getAllAttributes() override;
    std::vector<std::string> displayCommit() override;
    std::string getSummary() override;

    bool checkStagedFiles(vector<TrackedFile> fileVector);
    bool compareHashedFiles(TrackedFile comparingStagedFile);
    void addToCommitVector(bool tf, TrackedFile addingStagedFile);
    void clearCommitVector();
    vector<string> commitToRepo(vector<TrackedFile> commitFiles);
};