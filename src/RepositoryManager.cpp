 #include "../includes/RepositoryManager.h"

// do not touch, json work
#include "../includes/nlohmann/json.hpp"
using json = nlohmann::json;
using std::string;

RepositoryManager::RepositoryManager() {}

RepositoryManager::~RepositoryManager() {}


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
	//repo.setRepoPath();
    data.loadData(repoName, repo);
    qDebug() << "repo path after loading: " << repo.getRepoPath();
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
string RepositoryManager::callParentDifferentiation(const string& filename) {
    auto* file = repo.findFile(filename);
	qDebug() << "Files status is: " << file->getStatusAsString();
	// If the file were checking hasn't been committed already
	if (file->getStatusAsString() != "Committed") {
		qDebug() << "The file " << filename << " does not have the committed status";
		string path = file->getFilePath();
		qDebug() << "Diff file path: " << path;
		ifstream inFile(path);
		if (!inFile) return "__INVALID_DIFF__";
		string currentContent;
		// read all the lines in the file
		currentContent = std::string(
			std::istreambuf_iterator<char>(inFile),
			std::istreambuf_iterator<char>()
		);
		qDebug() << "Current file content: " << currentContent;
		// get map, get content
		if (repo.getRepoCommits().size() < 2) return "This file has never been committed.";
		auto* item = repo.getRepoCommits().back().get();
		auto& map = dynamic_cast<StandardCommit&>(*item).getFileSnapshot();
		std::string newContent = map.at(filename);
		differ.computeDiff(currentContent, map.at(filename));
	}
	else {
		if (repo.getRepoCommits().size() < 2) return "This is the initial commit.";
		// Get the most recent commit
		qDebug() << "The file " << filename << " does have the committed status";
		const auto& commits = repo.getRepoCommits();
		if (commits.empty()) return "No commits exist to diff.";

		auto* latestCommit = commits.back().get();
		//qDebug() << "This commit has "

		// Safely cast to StandardCommit to access the snapshot
		auto* latestStdCommit = dynamic_cast<StandardCommit*>(latestCommit);
		if (!latestStdCommit) return "__INVALID_COMMIT_TYPE__";

		// Get the parent ID from the most recent commit
		std::string parentId = latestCommit->getParentId();

		if (parentId.empty()) {
			return "This is the initial commit. No parent exists to diff against.";
		}

		// Find the parent commit using the findCommit function
		auto* parentCommit = repo.findCommit(parentId);
		if (!parentCommit) return "Parent commit not found.";

		// Safely cast the parent commit
		auto* parentStdCommit = dynamic_cast<StandardCommit*>(parentCommit);
		if (!parentStdCommit) return "__INVALID_PARENT_COMMIT_TYPE__";

		// Retrieve both snapshots
		const auto& latestMap = latestStdCommit->getFileSnapshot();
		const auto& parentMap = parentStdCommit->getFileSnapshot();

		// Verify the file exists in BOTH maps before calling .at() 
		// This prevents std::out_of_range exceptions if a file was newly created or deleted
		if (latestMap.find(filename) == latestMap.end()) {
			return "File not found in the latest commit.";
		}
		if (parentMap.find(filename) == parentMap.end()) {
			return "File was newly created in this commit (no parent version to diff).";
		}

		// Compute the diff 
		differ.computeDiff(latestMap.at(filename), parentMap.at(filename));
	}
     
    return differ.displayDiff().size() > 1 ? differ.displayDiff() : "No changes Made.";
}

void RepositoryManager::updateFileStatus(std::string& fileName, TrackedFile::status newStatus) {
	TrackedFile* fileToBeUpdated = repo.findFile(fileName);
	if (fileToBeUpdated) { // Check to make sure the file exists
		fileToBeUpdated->setStatus(newStatus);
		if (newStatus == TrackedFile::status::Modified) {
			fileToBeUpdated->incrementEditCount();
		}
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
void RepositoryManager::restore(const string& commitId) {
	repo.restore(commitId);
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

std::vector<std::string> RepositoryManager::getAllCommitIds() const {
	return repo.getAllCommitIds();
}

std::vector<std::string> RepositoryManager::getAnalytics() {
    std::vector<std::string> finalReport;
    int totalCommits = analyzer.computeTotalCommits(repo);
    int totalTrackedFiles = analyzer.computeTrackedFilesCount(repo);
    finalReport.push_back("Total Commits: " +std::to_string(totalCommits));
    finalReport.push_back("Total Tracked files: " + std::to_string(totalTrackedFiles)); 
    std::vector<std::string> mostEditedFiles = analyzer.computeMostModifiedFiles(repo);
    finalReport.insert(finalReport.end(), mostEditedFiles.begin(), mostEditedFiles.end());
    return finalReport; 
}