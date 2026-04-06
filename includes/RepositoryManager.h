/*
 * definition of repo manager
 * Author: Jacob Dawes - 041169788
 */
#pragma once

#include <string>
#include <unordered_map>
#include <algorithm>

#include "../includes/Repository.h"
#include "../includes/TrackedFile.h"
#include "../includes/Commit.h"
#include "../includes/StandardCommit.h"
#include "../includes/DataManager.h"
#include "../includes/AnalyticsEngine.h"

class RepositoryManager {
  private:
    Repository repo;
    DataManager data;
    AnalyticsEngine analyzer;
  public:
    RepositoryManager(Repository& repo);
    ~RepositoryManager();
    void createRepository(const std::string& repoName, const std::string& repoPath);
    bool loadRepostiory(const std::string& repoName);
    void saveRepository();
    Commit* searchCommits(const std::string& searchString);
    void getFileStatus(const TrackedFile& file);
    void restoreToParent(const std::string& commitId);

    std::unordered_map<std::string,std::string> callParentDifferentiation(Commit* diffCommit);
    std::unordered_map<std::string,std::string> callRegularDifferentiation(Commit* diffCommit1, Commit* diffCommit2);

    Commit* getParentCommit(const std::string& commitId);
    void restore(const std::string& commitId, const std::string& restoreCommitId);
};