/*
 * definition of repo manager
 * Author: Jacob Dawes - 041169788
 */
#pragma once

#ifndef REPOSITORYMANAGER_H
#define REPOSITORYMANAGER_H

#include "../includes/Repository.h"
#include "../includes/TrackedFile.h"
#include "../includes/Commit.h"
#include "../includes/StandardCommit.h"
#include "../includes/DataManager.h"

#include <string>

class RepositoryManager {
  private:
    Repository::Repository repo;
    DataManager::DataManager data;
  public:
    RepositoryManager (Repository& repo);
    ~RepositoryManager();
    Repository::Repository RepositoryManager::createRepository(const std::string& repoName);
    void loadRepostiory();
    bool saveRepository();
    Commit::Commit searchCommits(const std::string& searchString);
    void getFileStatus(const TrackedFile::TrackedFile& file);

    void restore(const std::string& commitId);
};

#endif