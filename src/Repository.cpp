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


std::string Repository::getRepoName() { return repoName; }


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

bool Repository::initRepository(const string& repoName,const string& repoPath) {

  this->repoPath = repoPath;
  
  // create snapshots , branches , config folders
  // & create the initial config file.
  if(!std::filesystem::create_directory(REPOWRAPPER)
     || !std::filesystem::create_directory(string(REPOWRAPPER) + "/snapshots")
     || !std::filesystem::create_directory(string(REPOWRAPPER) + "/config")) {
    return false;
  }
  buildJSONMetaData(repoName);
  std::ofstream configFile(string(REPOWRAPPER) + "/config/repo_config",std::ios::out);

  if(!configFile) return false;

  configFile << "# this file contains the configurations of your repo.\n";
  configFile << "Repository Name: " + this->repoName;

  configFile.close();

  return true;
}

// TODO: Update so that it reads froma JSON/TXT file, read the commit logs
// add to a vector of commit msgs, return vector to qt for use
vector<string> Repository::getCommitHistory() {

  // temp var, I assume it's already open so no need to reopen?
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

void Repository::stageFile(const string& filePath) {
  //if(currentCommit.fileSnapshot.at(filePath) {  //check for if file DOESNT exist, gui do thing
  //  throw std::runtime_error("no file tracked with that name"); //checking to see if file is NOT in the tracker vector
  //}

  TrackedFile* file = getSingleTrackedFile(filePath);
  TrackedFile::status status = file->getFileStatus();
  
  if(status == TrackedFile::status::Added) {
    updateFileStatus(filePath, TrackedFile::status::Staged);
    return;
  }

  updateFileStatus(filePath, TrackedFile::status::Modified);

  updateFileStatus(filePath, TrackedFile::status::Staged);
}


void Repository::updateFileStatus(const std::string& filePath, TrackedFile::status status) {
  auto* file = getSingleTrackedFile(filePath);
  file->setStatus(status);
}

TrackedFile* Repository::getSingleTrackedFile(const string& filePath) {
	for (TrackedFile& file : currentFiles) {
		if (file.getFilePath() == filePath) return &file;
	}
	return nullptr;
}

const vector<TrackedFile>& Repository::getFileVector() {
  return currentFiles;
}

/* find commits within the repostiroy's commits vector. */
StandardCommit* Repository::findCommit(const string& commitId) {
  if(commitId.empty()) return nullptr;

  for (const auto& commit : commits) {
    if(commit->getId() == commitId) return dynamic_cast<StandardCommit*>(commit.get());
  }

  return nullptr;
}