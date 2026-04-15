#include "../includes/RepositoryManager.h"

// do not touch, json work
#include "../includes/nlohmann/json.hpp"
using json = nlohmann::json;
using std::string;

/*
 * Sam Required function
 */
std::vector<RepositoryManager::Project> RepositoryManager::getProjectInfo() {
    std::vector<RepositoryManager::Project> resultVec;
    std::ifstream file("data/dataHandler.json");

    // returns an empty json object
    if (!file) {
        
        return {};
    }
    json data = json::parse(file);
    
    for (auto& project : data["projects"]) {
		Project result;
        result.name = project["name"];
        result.id = project["id"];
        result.filePath = project["path"];
        resultVec.push_back(result);
    }
    return resultVec;
}

RepositoryManager::RepositoryManager() {}

RepositoryManager::~RepositoryManager() {}

void RepositoryManager::createRepository(const std::string& repoName,const std::string& repoPath) {
    // initialize the repo with a name
	std::cout << "We are in createRepository now";
    repo.initRepository(repoName, repoPath);
}

bool RepositoryManager::checkIfReal(std::string repoName){
    bool check = data.checkProjectExist(repoName);
    return check;
}

// this will load from persistant storage (files)
// have full path as the function input param
// return a fully filled repo object containing the repo contents
bool RepositoryManager::loadRepository(std::string repoName) {
    repo.setRepoName(repoName);
    data.loadData(repoName, repo);

    return true;
}

void RepositoryManager::saveRepository(std::string repoName, std::string repoPath) {
    data.saveData(repo, repoName, repoPath);
}

void RepositoryManager::addNewTrackedFile(TrackedFile& file) {
	repo.addNewTrackedFile(file);
}
/* SAM: this function diffs all the files in both the parent and the current
   as long as it exists in both. It returns a map of all the diffs 
   KEY: filename VALUE: DiffString */
std::unordered_map<string,string> RepositoryManager::callParentDifferentiation(const string& diffCommitId) {
    std::unordered_map<string,string> diffMap;

    auto base = repo.findCommit(diffCommitId);
    Commit* baseParent = getParentCommit(base->getId());
    auto parent = dynamic_cast<StandardCommit*>(baseParent);
    auto parentFiles = parent->getTrackedFiles();

    auto& da_map = parent->getFileSnapshot();

    for (auto& file : base->getIncomingFiles()) {
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
	if (fileToBeUpdated) { // Check to make sure the file exists
		fileToBeUpdated->setStatus(newStatus);
	}
	else {
		std::cout << "Warning: Attempted to update status for untracked file:" << fileName;
	}
}

std::string RepositoryManager::getStatusAsString(const std::string fileName) {
  auto* file = repo.getSingleTrackedFile(fileName);
  return file->getStatusAsString();
}

std::vector<TrackedFile>& RepositoryManager::getCurrentFiles() {
    return repo.getCurrentFiles();
}

void RepositoryManager::deleteTrackedFile(std::string fileName) {
	repo.deleteTrackedFile(fileName);
}

void RepositoryManager::setStatusAsString(const std::string fileName, std::string newStatusString) {
  auto* file = repo.getSingleTrackedFile(fileName);
  file->setStatusAsString(newStatusString);
}

void RepositoryManager::updateFileContent(const string& fileName,const string& newContent) {
    TrackedFile* fileToBeUpdated = repo.findFile(fileName);
    fileToBeUpdated->setContent(newContent);
    fileToBeUpdated->incrementEditCount();
}

/* Search for a commit if it exists in the repo's vector */
StandardCommit* RepositoryManager::searchCommits(const string& searchString) {
    return repo.findCommit(searchString);
}

// /* returns the parent commit as a pointer */
StandardCommit* RepositoryManager::getParentCommit(const string& commitId) {
	auto c = searchCommits(commitId);
	auto parent = searchCommits(c->getParentId());
	return static_cast<StandardCommit*>(parent);
}


/* performs the restoration to the parent. */
void RepositoryManager::restoreToParent(const string& commitId) {
    auto parent = getParentCommit(commitId);
    auto current = searchCommits(commitId);

    if (parent == nullptr || current == nullptr) return;

    for (const auto& file : parent->getIncomingFiles()) {
        current->updateSnapshot(file.getFileName(),parent->getFileSnapshot().at(file.getFileName()));
    }
}

/* make a function that restores to the previous commit */

std::vector<std::string> RepositoryManager::getCommitHistory() {
    return repo.getCommitHistory();
}

TrackedFile::status RepositoryManager::getFileStatus(std::string fileName) {
    for (const auto & file : repo.getCurrentFiles()) {
        if (file.getFileName() == fileName) {
            return file.getFileStatus();
        }
    }
    throw std::runtime_error("File named " + fileName + " could not be found...");
}

std::string RepositoryManager::getFileContent(std::string fileName) {
    for (const auto& file : repo.getCurrentFiles()) {
        if (file.getFileName() == fileName) {
            return file.getContent();
        }
    }
    throw std::runtime_error("File named " + fileName + " could not be found...");
}

void RepositoryManager::addNewTrackedFile(const std::string& filePath, const std::string& fileName) {
	TrackedFile newFile(filePath, fileName);
    repo.addNewTrackedFile(newFile);
}

void RepositoryManager::stageFile(const string& fileName) {
  repo.stageFile(fileName);  
}

void RepositoryManager::stageAllFiles() {
  // take in whole vector, loop this function for the entire vector
  auto& files = getCurrentFiles();
  
  for (const auto& file : files) {
    stageFile(file.getFileName());
  }
}

void RepositoryManager::addCommit(std::string commitId, std::string parent, std::string commitMessage,
	std::string admin, std::string timeStamp) {
    repo.addCommit(commitId, parent, commitMessage, admin, timeStamp);

	// repo.addCommit(commitId, parent, commitMessage, admin, timeStamp);
}

bool RepositoryManager::commitStagedFiles(std::string commitMessage) {
    string newID = data.generateId(repo.getRepoName(), repo.getCurrentFiles());
	return repo.commitStagedFiles(commitMessage, newID);
}

std::vector<std::unique_ptr<Commit>>& RepositoryManager::getRepoCommits() {
	return repo.getRepoCommits();
}
//std::vector<std::string> RepositoryManager::getAnalytics() const{
//    std::vector<std::string> finalRport;
//    int totalCommits = analyzer.computeTotalCommits(repo);
//    int totalTrackedFiles= analyzer.computeTotalTrackedFiles(repo);
//    finalReport.push_back("Total Commits: " +std::to_string(totalCommits));
//    finalReport.puch_back("total Tracked files: " + std::to_string(totalTrackedFile)); 
//    std::vector<std::string> mostEditedFiles=analyzer.computeMostModifiedFiles(repo);
//    finalReport.insert(finalReport.end(), mostEditedFiles.begin(), mostEditedFiles.end());
//    return finalReport; 
//}