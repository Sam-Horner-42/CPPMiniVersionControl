
#include "../includes/RepositoryManager.h"

// do not touch, json work
#include "../includes/nlohmann/json.hpp"
using json = nlohmann::json;

/*
 * Sam Required function
 */
RepositoryManager::Project RepositoryManager::getProjectInfo() {
    Project result;  
    
    std::ifstream file("dataHandler.json");
    json data = json::parse(file);
    
    for (auto& project : data["projects"]) {
        result.name = project["name"];
        result.id = project["id"];
        result.path = project["path"];
        return result;
    }
    
    return result;
}

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
    vector<unique_ptr<Commit>> commits = getCommitVector(); 
    data.saveData(name, files, commits);
        
    return true;    
}

Commit* RepositoryManager::searchCommits(const string& searchString) {
    return repo.findCommit(searchString);
}

Commit* RepositoryManager::getParentCommit(const string& commitId) {
    auto c = searchCommits(commitId);
    auto parent = searchCommits(c->getParentId());

    return parent;
}

/* performs the restoration to the parent. */
void RepositoryManager::restoreToParent(const string& commitId) {
    auto parent = getParentCommit(commitId);
    auto current = searchCommits(commitId);

    for (auto& file : parent->commits) {
        current->updateSnapshot(file->getFileName(),file->getFileContent());
    }
}

/* restore commit x to commit y */
void RepositoryManager::restore(const string& commitId,const string& restoreCommitId) {
    auto current = searchCommit(commitId);
    auto restore = searchCommits(restoreCommitId);

    for (auto& file : restore->commits) {
        current->updateSnapshot(file->getFileName(),file->getFileContent());
    }
}

Commit* RepositoryManager::getParentCommit(const string& commitId) {
    auto c = searchCommits(commitId);
    auto parent = searchCommits(c->getParentId());

    return parent;
}

/* performs the restoration to the parent. */
void RepositoryManager::restoreToParent(const string& commitId) {
    auto parent = getParentCommit(commitId);
    auto current = searchCommits(commitId);

    for (auto& file : parent->commits) {
        current->updateSnapshot(file->getFileName(),file->getFileContent());
    }
}

/* restore commit x to commit y */
void RepositoryManager::restore(const string& commitId,const string& restoreCommitId) {
    auto current = searchCommit(commitId);
    auto restore = searchCommits(restoreCommitId);

    for (auto& file : restore->commits) {
        current->updateSnapshot(file->getFileName(),file->getFileContent());
    }
}

Status::Status RepositoryManager::getFileStatus(const TrackedFile& file) {
    return file.getFileStatus();
}