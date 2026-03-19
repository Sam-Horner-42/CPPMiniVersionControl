
#pragma once

#include <vector>
#include <map>

#include "../includes/Commit.h"

class StandardCommit : public Commit {
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
    std::vector<std::string> getAllAttributes();
    std::vector<std::string> getContent() override;
    std::vector<std::string> displayCommit() override;
    void getSummary() override;
};