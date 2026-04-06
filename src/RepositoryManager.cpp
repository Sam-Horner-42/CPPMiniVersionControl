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

void RepositoryManager::createRepository(const string& repoName,const string& repoPath) {
    // initialize the repo with a name
    repo.initRepository(repoName,repoPath);
}

// this will load from persistant storage (files)
// have full path as the function input param
// return a fully filled repo object containing the repo contents
bool RepositoryManager::loadRepostiory(const string& repoName) {
    repo.setRepoName(repoName);
    data.loadData(repoName,repo.getFileVector(),repo.getCommitHistory());

    return true;
}

void RepositoryManager::saveRepository() {
    // I have no idea what's really required here but this is best solution I believe??
    string name = repo.getRepository();
    vector<TrackedFile> files = Repository::getFileObject();
    vector<unique_ptr<Commit>> commits = Commit::getCommitVector(); 
    data.saveData(name, files, commits);
}

/* SAM: this function diffs all the files in both the parent and the current
   as long as it exists in both. It returns a map of all the diffs 
   KEY: filename VALUE: DiffString */
unordered_map<string,string> RepositoryManager::callParentDifferentiation(Commit* diffCommit) {
    unordered_map<string,string> diffMap;
    auto parent = getParentCommit(diffCommit->getId());
    auto parentFiles = parent->getTrackedFiles();

    for (const auto& file : diffCommit->getTrackedFiles()) {
        if (!parent->hasFile(file->getFileName())) continue;

        auto it = find_if(parentFiles.begin(), parentFiles.end(), [&](const auto& f) {
            return f->getFileName() == file->getFileName();
        });

        if (it == parentFiles.end()) continue;

        analyzer.computeDiff(file->getFileContent(), (*it)->getFileContent());
        diffMap.insert({file->getFileName(), analyzer.displayDiff()});
    }

    return diffMap;
}

/* SAM: this does the same thing as the function above but for 2 chosen commits */
unordered_map<string,string> RepositoryManager::callRegularDifferentiation(Commit* diffCommit1,Commit* diffCommit2) {
    auto diffCommit2Files = diffCommit2->getTrackedFiles();
    unordered_map<string,string> diffMap;

    for (const auto& file : diffCommit1->getTrackedFiles()) {
        if (!diffCommit2->hasFile(file->getFileName())) continue;

        auto it = find_if(diffCommit2Files.begin(), diffCommit2Files.end(), [&](const auto& f) {
            return f->getFileName() == file->getFileName();
        });

        if (it == diffCommit2Files.end()) continue;

        analyzer.computeDiff(file->getFileContent(), (*it)->getFileContent());
        diffMap.insert({file->getFileName(), analyzer.displayDiff()});
    }

    return diffMap;
}

/* Search for a commit if it exists in the repo's vector */
Commit* RepositoryManager::searchCommits(const string& searchString) {
    return repo.findCommit(searchString);
}

/* returns the parent commit as a pointer */
Commit* RepositoryManager::getParentCommit(const string& commitId) {
    auto c = searchCommits(commitId);
    auto parent = searchCommits(c->getParentId());

    return parent;
}

/* performs the restoration to the parent. */
void RepositoryManager::restoreToParent(const string& commitId) {
    auto parent = getParentCommit(commitId);
    auto current = searchCommits(commitId);

    for (const auto& file : parent->getTrackedFiles()) {
        current->updateSnapshot(file->getFileName(), file->getFileContent());
    }
}

/* restore commit x to commit y */
void RepositoryManager::restore(const string& commitId,const string& restoreCommitId) {
    auto current = searchCommits(commitId);
    auto restore = searchCommits(restoreCommitId);

    for (const auto& file : restore->getTrackedFiles()) {
        current->updateSnapshot(file->getFileName(), file->getFileContent());
    }
}

TrackedFile::status RepositoryManager::getFileStatus(const TrackedFile& file) {
    return file.getFileStatus();
}