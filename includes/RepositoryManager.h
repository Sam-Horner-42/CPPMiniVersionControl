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

#include <string>

class RepositoryManager {
  private:
  
    Repository repo;
    DataManager data;
  public:
  struct Project{
      string name;
      string id;
      string path;
    };

    Project getProjectInfo();

    RepositoryManager (Repository& repo);
    ~RepositoryManager();
    Repository::Repository RepositoryManager::createRepository(const std::string& repoName);
    void loadRepostiory();
    void saveRepository();
    Commit* searchCommits(const std::string& searchString);
    void getFileStatus(const TrackedFile::TrackedFile& file);

    void restore(const std::string& commitId);
    Commit* getParentCommit(const std::string& commitId);
    void restore(const std::string& commitId,const std::string& restoreCommitId);
};

#endif