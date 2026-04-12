#include "../includes/RepositoryManager.h"

// do not touch, json work
#include "../includes/nlohmann/json.hpp"
using json = nlohmann::json;
using std::string;
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
        result.filePath = project["path"];
        return result;
    }
    
    return result;
}

RepositoryManager::RepositoryManager(Repository& repo) :
repo(repo) {}

RepositoryManager::~RepositoryManager() {}

void RepositoryManager::createRepository(const std::string& repoName,const std::string& repoPath) {
    // initialize the repo with a name
    repo.initRepository(repoName,repoPath);
}

// this will load from persistant storage (files)
// have full path as the function input param
// return a fully filled repo object containing the repo contents
bool RepositoryManager::loadRepostiory(std::string& repoName) {
    repo.setRepoName(repoName);
    data.loadData(repoName);

    return true;
}

void RepositoryManager::saveRepository() {
    std::string name = repo.getRepoName();
    data.saveData(name, dynamic_cast<Commit&>(repo.getCurrentCommit()));
}

/* SAM: this function diffs all the files in both the parent and the current
   as long as it exists in both. It returns a map of all the diffs 
   KEY: filename VALUE: DiffString */
std::unordered_map<string,string> RepositoryManager::callParentDifferentiation(StandardCommit& diffCommit) {
    std::unordered_map<string,string> diffMap;

    Commit* baseParent = getParentCommit(diffCommit.getId());
    StandardCommit* parent = dynamic_cast<StandardCommit*>(baseParent);
    auto parentFiles = parent->getTrackedFiles();

    auto& da_map = parent->getFileSnapshots();

    for (const auto& file : diffCommit.getTrackedFiles()) {
        if (!parent->hasFile(file.getFileName())) continue;

        auto it = find_if(parentFiles.begin(), parentFiles.end(), [&](const auto& f) {
            return f.getFileName() == file.getFileName();
        });

        if (it == parentFiles.end()) continue;

        differ.computeDiff(da_map.at(file.getFileName()), da_map.at((*it).getFileName()));
        diffMap.insert({file.getFileName(), differ.displayDiff()});
    }

    return diffMap;
}

void RepositoryManager::updateFileStatus(std::string& fileName, TrackedFile::status newStatus) {
    TrackedFile* fileToBeUpdated = repo.findFile(fileName);
	fileToBeUpdated->setStatus(newStatus);
	
}

void RepositoryManager::updateFileContent(const string& fileName,const string& newContent) {
    auto& c_c = repo.getCurrentCommit();
    c_c.updateSnapshot(fileName,newContent);
}

/* SAM: this does the same thing as the function above but for 2 chosen commits */
std::unordered_map<string,string> RepositoryManager::callRegularDifferentiation(StandardCommit& diffCommit1, StandardCommit& diffCommit2) {
    auto diffCommit2Files = diffCommit2.getIncomingFiles();
    unordered_map<string,string> diffMap;

    auto& f1_map = diffCommit1.getFileSnapshots();
    auto& f2_map = diffCommit2.getFileSnapshots();

    for (const auto& file : diffCommit1.getIncomingFiles()) {
        if (!diffCommit2.hasFile(file.getFileName())) continue;

        auto it = find_if(diffCommit2Files.begin(), diffCommit2Files.end(), [&](const auto& f) {
            return f.getFileName() == file.getFileName();
        });

        if (it == diffCommit2Files.end()) continue;

        differ.computeDiff(f1_map.at(file.getFileName()), f2_map.at((*it).getFileName()));
        diffMap.insert({file.getFileName(), differ.displayDiff()});
    }

    return diffMap;
}

/* Search for a commit if it exists in the repo's vector */
StandardCommit* RepositoryManager::searchCommits(const string& searchString) {
    return repo.findCommit(searchString);
}

// /* returns the parent commit as a pointer */
// StandardCommit* RepositoryManager::getParentCommit(const string& commitId) {
// 	StandardCommit* c = searchCommits(commitId);
// 	auto parent = searchCommits(c->getParentId());
// 	return static_cast<StandardCommit*>(parent);
// }

// /* performs the restoration to the parent. */
// void RepositoryManager::restoreToParent(const string& commitId) {
//     auto parent = getParentCommit(commitId);
//     auto current = searchCommits(commitId);

//     for (const auto& file : parent->getTrackedFiles()) {
//         current->updateSnapshot(file.getFileName(), file.getFileContent());
//     }
// }

// /* restore commit x to commit y */
// void RepositoryManager::restore(const string& commitId,const string& restoreCommitId) {
//     auto current = searchCommits(commitId);
//     auto restore = searchCommits(restoreCommitId);

//     for (const auto& file : restore->getTrackedFiles()) {
//         current->updateSnapshot(file.getFileName(), file.getFileContent());
//     }
// }

TrackedFile::status RepositoryManager::getFileStatus(const TrackedFile& file) {
    return file.getFileStatus();
}