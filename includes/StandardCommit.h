
#pragma once

#include <vector>
#include <string>
#include <map>

#include "../includes/Commit.h"

class StandardCommit : public Commit::Commit {
private:
    std::map<std::string, std::string> fileSnapshot;
public:
    StandardCommit(
        const std::string& commitId,
        const std::string& message,
        const std::string& author,
        const std::string& timestamp
    );
    ~StandardCommit();

    // commented out the overrides because they dont override anything from commit.cpp or commit.h
    std::vector<std::string> getAllAttributes(); //override;
    std::vector<std::string> displayCommit(); //override;
    std::string getSummary(); //override;
};