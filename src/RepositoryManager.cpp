
#include "../includes/RepositoryManager.h"

RepositoryManager::RepositoryManager(Repository& repo) :
repo(repo) {}

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

Commit* RepositoryManager::searchCommits(const std::string& searchString) {
    return repo.findCommit(searchString);
}

Commit* RepositoryManager::getParentCommit(const string& commitId) {
    auto c = searchCommits(commitId);
    auto parent = searchCommits(c->getParentId());

    return parent.get();
}

void RepositoryManager::restoreToParent(const string& commitId) {
    auto parent = getParentCommit(commitId);
    auto current = searchCommit(commitId);

    for (auto file& : parent->commits) {
        current->updateSnapshot(file->getFileName(),file->getFileContent());
    }
}

void RepositoryManager::restore(const string& commitId,const string& restoreCommitId) {
    auto current = searchCommit(commitId);
    auto restore = searchCommit(restoreCommitId);

    for (auto file& : restore->commits) {
        current->updateSnapshot(file->getFileName(),file->getFileContent());
    }
}

void RepositoryManager::getFileStatus(const TrackedFile& file) {
    return file.getFileStatus();
}