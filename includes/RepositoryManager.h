/*
 * definition of repo manager
 * Author: Jacob Dawes - 041169788
 */
#pragma once

#ifndef REPOSITORYMANAGER_H
#define REPOSITORYMANAGER_H

#include <string>

#include "../includes/Repository.h"
#include "../includes/TrackedFile.h"
#include "../includes/Commit.h"
#include "../includes/StandardCommit.h"
#include "../includes/DataManager.h"

class RepositoryManager {
  private:
    Repository repo;
    DataManager data;
  public:
  struct Project{
      std::string name;
      std::string id;
      std::string path;
    };

    Project getProjectInfo();

    RepositoryManager (Repository& repo);
    ~RepositoryManager();
    Repository createRepository(const std::string& repoName);
    void loadRepository();
    bool saveRepository();
    Commit* searchCommits(const std::string& searchString);
    TrackedFile::status getFileStatus(const TrackedFile& file);

    void restoreToParent(const std::string& commitId);
    Commit* getParentCommit(const std::string& commitId);
    void restore(const std::string& commitId,const std::string& restoreCommitId);
};

#endif