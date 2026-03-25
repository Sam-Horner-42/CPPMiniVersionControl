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

class RepositoryManager {
  private:
    Repository::Repository repo;
  public:
    RepositoryManager ();
    ~RepositoryManager();
    Repository::Repository RepositoryManager::createRepository(const std::string& repoName);
    void loadRepostiory();
    void saveRepository();
    Commit::Commit searchCommits(const std::string& searchString);
    void getFileStatus(const TrackedFile::TrackedFile& file);
};

#endif
