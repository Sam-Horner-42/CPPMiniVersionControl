/*
 * Implementation of the Repository class
 * Authors: 
 *  Jacob Dawes - 041169788
 *  Ethan Geary - 041032340
 */

#include "../includes/Repository.h"

using std::string;
using std::cout;
using std::endl;
using std::vector;

using json = nlohmann::json;
#define REPOWRAPPER ".vcm"


const std::string& Repository::getRepoName() const { return repoName; }


void Repository::buildJSONMetaData(const string& repoPath){

  json jObj;
  jObj["repositoryName"] = repoName;

  jObj["files"]=json::array(); 
  jObj["commits"]=json::array(); 
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

// This function will be called from RepositoryManager and return a repository for use in the manager class
void Repository::initRepository(const string& repoName,const string& repoPath) {
	this->repoPath = repoPath;
  
  // create snapshots , config folders
  // & create the initial config file.
	if (!std::filesystem::create_directory(REPOWRAPPER)
		|| !std::filesystem::create_directory(string(REPOWRAPPER) + "/snapshots")
		|| !std::filesystem::create_directory(string(REPOWRAPPER) + "/config")) {
	return;} // return null back to the manager so we can check repo != nullptr
  buildJSONMetaData(repoName);
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
  //if(currentCommit.fileSnapshot.at(filePath) {  //check for if file DOESNT exist, gui do thing
  //  throw std::runtime_error("no file tracked with that name"); //checking to see if file is NOT in the tracker vector
  //}

  TrackedFile* file = getSingleTrackedFile(fileName);
  TrackedFile::status status = file->getFileStatus();
  std::string content = file->getFileContent(); //added content back into file
  
  if(status == TrackedFile::status::Added) {
    updateFileStatus(fileName, TrackedFile::status::Staged);
    return;
  }

  updateFileStatus(fileName, TrackedFile::status::Modified);

  updateFileStatus(fileName, TrackedFile::status::Staged);
}

/*
update file status and file content
*/
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

/* find commits within the repostiroy's commits vector. */
StandardCommit* Repository::findCommit(const string& commitId) {
  if(commitId.empty()) return nullptr;

  for (const auto& commit : commits) {
    if(commit->getId() == commitId) return dynamic_cast<StandardCommit*>(commit.get());
  }

  return nullptr;
}