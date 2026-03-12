/*
 * definition of repo manager
 * Author: Jacob Dawes - 041169788
 */
#pragma once

#ifndef REPOSITORYMANAGER_H
#define REPOSITORYMANAGER_H

#include "../includes/Repository.h"
#include "../includes/TrackedFile.h"

class RepositoryManager {
  public:
    RepositoryManager ();
    ~RepositoryManager();
    Repository::Repository RepositoryManager::createRepository(const std::string& repoName);
    void loadRepostiory();
    void saveRepository();
    void searchCommits();
    void getFileStatus();
};

#endif
