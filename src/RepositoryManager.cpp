

RepositoryManager::RepositoryManager() {}
RepositoryManager::~RepositoryManager() {}

Repository::Repository RepositoryManager::createRepository(const string& repoName) {
    Repository repo;
    // initialize the repo with a name 
    repo.initRepository(repoName);
    // return initialized repo
    return repo;
}

void RepositoryManager::loadRepostiory() {

}

void RepositoryManager::saveRepository() {

}

void RepositoryManager::searchCommits() {

}

void RepositoryManager::getFileStatus() {

}