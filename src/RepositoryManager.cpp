#include "../includes/RepositoryManager.h"

using namespace std;

RepositoryManager::RepositoryManager(Repository& repo) :
repo(repo) {}

RepositoryManager::~RepositoryManager() {}

void RepositoryManager::createRepository(const string& repoName,const string& repoPath) {
    // initialize the repo with a name
    this->repo = repo.initRepository(repoName,repoPath);
}

// this will load from persistant storage (files)
// have full path as the function input param
// return a fully filled repo object containing the repo contents
bool RepositoryManager::loadRepostiory(const string& repoName) {
    repo.setRepoName(repoName);
    data.loadData(repoName, repo);

    return true;
}

void RepositoryManager::saveRepository(StandardCommit& commit) {
    // I have no idea what's really required here but this is best solution I believe??
    std::string name = repo.getRepoName();
    std::vector<TrackedFile> files = commit.getIncomingFiles(); // should get files - ethan work
    data.saveData(name, files, commit);
}

/* SAM: this function diffs all the files in both the parent and the current
   as long as it exists in both. It returns a map of all the diffs 
   KEY: filename VALUE: DiffString */
std::unordered_map<std::string, std::string> RepositoryManager::callParentDifferentiation(StandardCommit& diffCommit) {
    std::unordered_map<string,string> diffMap;
    StandardCommit* parent = getParentCommit(diffCommit.getId());
    auto parentFiles = parent->getIncomingFiles();

    for (const auto& file : diffCommit.getIncomingFiles()) {
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
StandardCommit* RepositoryManager::searchCommits(const string& searchString) {
    return repo.findCommit(searchString);
}

/* returns the parent commit as a pointer */
StandardCommit* RepositoryManager::getParentCommit(const string& commitId) {
	StandardCommit* c = searchCommits(commitId);
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
        current->updateSnapshot(file.getFileName(), file.getFileContent());
    }
}

TrackedFile::status RepositoryManager::getFileStatus(const TrackedFile& file) {
    return file.getFileStatus();
}