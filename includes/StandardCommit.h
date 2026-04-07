#pragma once

#include <vector>
#include <string>
#include <map>

#include "../includes/Commit.h"
#include "../includes/TrackedFile.h"

class StandardCommit : public Commit {
private:
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

    // commented out the overrides because they dont override anything from commit.cpp or commit.h
    //std::vector<std::string> getAllAttributes(); //override;
    //std::vector<std::string> displayCommit(); //override;
    //std::string getSummary(); //override;
    void createSnapshot(const std::string& filename, const std::vector<std::string>& content);
    void updateSnapshot(const std::string& filename, const std::vector<std::string>& content);
    std::vector<std::string> getAllAttributes() override;
    std::vector<std::string> displayCommit() override;
    std::string getSummary() override;

    bool checkStagedFiles(std::vector<TrackedFile> fileVector);
    bool compareHashedFiles(TrackedFile comparingStagedFile);
    void addToCommitVector(bool tf, TrackedFile addingStagedFile);
    void clearCommitVector();
    std::vector<std::string> commitToRepo(vector<TrackedFile> commitFiles);
};