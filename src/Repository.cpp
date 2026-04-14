/*
 * Implementation of the Repository class
 * Authors: 
 *  Jacob Dawes - 041169788
 *  Ethan Geary - 041032340
 */

#include "../includes/Repository.h"
#include <QDebug>

using std::string;
using std::cout;
using std::endl;
using std::vector;

using json = nlohmann::json;
#define REPOWRAPPER repoPath + "/.vcm"


const std::string& Repository::getRepoName() const { return repoName; }
const std::string& Repository::getRepoPath() const { return repoPath; }

void Repository::buildJSONMetaData(const string repoName, const string repoPath){
	
  json jObj;
  jObj["files"]=json::array(); 
  jObj["repositoryName"] = repoName;
  // cout << "Build JSON MetaData Repo Path: " << repoPath; 
  string metadataPath = repoPath + "/metadata.json";
  
  // Write JSON to file
  std::ofstream outFile(metadataPath);
  if (outFile.is_open()) {
    outFile << jObj.dump(4); // spacing
    outFile.close();
  } else {
    std::cerr << "Failed to create metadata.json at: " << metadataPath << std::endl;
  }
}
void Repository::buildJSONSnapshots(const string repoName, const string repoPath){
  json jObj;
  jObj["commits"] = json::array();
  string snapPath = string(REPOWRAPPER) + "/snapshots/commits.json";

  qDebug() << "snap Path: " << snapPath;
  
  // Write JSON to file
  std::ofstream outFile(snapPath);
  if (outFile.is_open()) {
    outFile << jObj.dump(4); // spacing
    outFile.close();
  } else {
    qDebug() << "Failed to create commits.json at: " << snapPath;
  }
}

// This function will be called from RepositoryManager and return a repository for use in the manager class
void Repository::initRepository(const string& repoName,const string& repoPath) {
  this->repoPath = repoPath;
  
  
  // create snapshots , config folders
  // & create the initial config file.
	if (!std::filesystem::create_directory(REPOWRAPPER)
		|| !std::filesystem::create_directory(string(REPOWRAPPER) + "/snapshots")
		|| !std::filesystem::create_directory(string(REPOWRAPPER) + "/config")) {
	return;} // return null back to the manager so we can check repo != nullptr
  buildJSONMetaData(repoName, repoPath);
  buildJSONSnapshots(repoName, repoPath);
  std::ofstream configFile(string(REPOWRAPPER) + "/config/repo_config",std::ios::out);

  if(!configFile) return;

  configFile << "# this file contains the configurations of your repo.\n";
  configFile << "Repository Name: " << repoName;

  configFile.close();
}

vector<string> Repository::getCommitHistory() {

    vector<string> commitHistoryVec;
    for (auto& commit : commits) {
      string logCommit = "Commit ID: " + commit->getId() + 
       "Date: " + commit->getTimestamp() + 
       "Message: " + commit->getMessage();
      // one commit pushed to vector
        commitHistoryVec.push_back(logCommit);
    }
    // holds full history
    return commitHistoryVec;
}

TrackedFile* Repository::findFile(const std::string& filename) {
  for (auto& file : currentFiles) {
    if(file.getFileName() == filename) return &file;
  }
  return nullptr;
}

std::vector<std::unique_ptr<Commit>>& Repository::getRepoCommits() {
  return commits;
}

void Repository::stageFile(const string& fileName) {
  TrackedFile* file = getSingleTrackedFile(fileName);
  TrackedFile::status status = file->getFileStatus();
  std::string content = file->getFileContent(); //added content back into file
  
  if(status == TrackedFile::status::Added || status == TrackedFile::status::Modified) {
    updateFileStatus(fileName, TrackedFile::status::Staged);
    return;
  }
}

/*
update file status and file content
*/

void Repository::deleteTrackedFile(const std::string fileName) {
	currentFiles.erase(
		std::remove_if(currentFiles.begin(), currentFiles.end(),
			[&fileName](const TrackedFile& file) {
				return file.getFileName() == fileName;
			}),
		currentFiles.end()
	);
}
void Repository::updateFileStatus(const std::string& fileName, TrackedFile::status status) {
  auto* file = getSingleTrackedFile(fileName);
  file->setStatus(status);
}

std::string Repository::getStatusAsString(const std::string fileName) {
  auto* file = getSingleTrackedFile(fileName);
  return file->getStatusAsString();
}

void Repository::setStatusAsString(const std::string fileName, std::string newStatusString) {
  auto* file = getSingleTrackedFile(fileName);
  file->setStatusAsString(newStatusString);
}

void Repository::updateFileContent(const std::string fileName, std::string newContent) {
  auto* file = getSingleTrackedFile(fileName);
  file->setContent(newContent);
}

TrackedFile* Repository::getSingleTrackedFile(const string& fileName) {
	for (TrackedFile& file : currentFiles) {
		if (file.getFileName() == fileName) return &file;
	}
	return nullptr;
}

void Repository::addNewTrackedFile(TrackedFile& file) {
	currentFiles.push_back(file);
}

/* find commits within the repostiroy's commits vector. */
StandardCommit* Repository::findCommit(const string& commitId) {
  if(commitId.empty()) return nullptr;

  for (const auto& commit : commits) {
    if(commit->getId() == commitId) return dynamic_cast<StandardCommit*>(commit.get());
  }

  return nullptr;
}

bool Repository::commitStagedFiles(string commitMessage, string commitId) {
	// get trackedfiles vector
	auto& files = getCurrentFiles();
	 string parent;

	// only acknowledge staged files
	int vectorSize = files.size();
	bool isStaged = false;
	std::time_t currentTime = std::time(nullptr);

    std::string realTime = std::ctime(&currentTime);
    realTime.pop_back();

	// set status of staged files to committed
	for (int i = 0; i < vectorSize; i++) {
		if (files[i].getFileStatus() == TrackedFile::status::Staged) {
			isStaged = true;
			files[i].setStatus(TrackedFile::status::Committed);
		}
	}

	if (isStaged == false) {
		return false;
	}

	if (commits.empty()) {
    parent = commitId;
	}
  else {
     auto* p = commits.back().get();
     parent = p->getId();
  }

	// create new commit object, author is hardcoded to admin, first commit selfreferences parentcommit id to itself
	// populate commit objects.addTrackedFile with the committed files
	// passing commitId and parentId as hardcoded strings, once the functions are in place to use actual commitId and parentId then please replace them
	auto newCommit = std::make_unique<StandardCommit>(commitId, parent, commitMessage, "Admin", realTime);

	// pass commit object into addCommit
	addCommit(std::move(newCommit));
	// return bool TRUE if successful, FALSE if no currently staged files
	return true;
}