
#include "../includes/RepositoryManager.h"

RepositoryManager::RepositoryManager() {}
RepositoryManager::~RepositoryManager() {}

Repository RepositoryManager::createRepository(const string& repoName) {
    Repository repo;
    // initialize the repo with a name 
    repo.initRepository(repoName);
    // return initialized repo
    return repo;
}

    // this will load from persistant storage (files)
    // have full path as the function input param
    // return a fully filled repo object containing the repo contents
void RepositoryManager::loadRepostiory() {
    data.loadData(repo.getRepoName(),repo.getFileVector(),repo.getCommitVector());
}

bool RepositoryManager::saveRepository() {

    // I have no idea what's really required here but this is best solution I believe??
    string name = repo.getRepository();
    vector<TrackedFile> files = Repository::getFileObject(); // should get files - ethan work
    vector<unique_ptr<Commit>> commits = Commit::getCommitVector(); 
    data.saveData(name, files, commits);
        
    return true;    
}

Commit searchCommits(const std::string& searchString) {
    
}

void RepositoryManager::getFileStatus(const TrackedFile& file) {
    return file.getFileStatus();
}