
/*
 * definition of repo manager
 * Author: Jacob Dawes - 041169788
 */

#ifndef REPOSITORY_MANAGER_H
#define REPOSITORY_MANAGER_H

class RepositoryManager {
  public:
    void createRepository();
    void loadRepostiory();
    void saveRepository();
    void searchCommits();
    void getFileStatus();
};

#endif
