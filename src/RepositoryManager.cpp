

RepositoryManager::RepositoryManager() {}
RepositoryManager::~RepositoryManager() {}

Repository::Repository RepositoryManager::createRepository(const string& repoName) {
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

}

void RepositoryManager::saveRepository() {

}

Commit::Commit searchCommits(const std::string& searchString) {
    
}

void RepositoryManager::getFileStatus(const TrackedFile& file) {
    return file.getFileStatus();
}