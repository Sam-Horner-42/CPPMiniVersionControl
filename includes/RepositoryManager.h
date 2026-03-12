/*
 * definition of repo manager
 * Author: Jacob Dawes - 041169788
 */
#include <fstream>

class RepositoryManager {
  public:
    void createRepository();
    void loadRepostiory();
    void saveRepository();
    void searchCommits();
    void getFileStatus();
};
