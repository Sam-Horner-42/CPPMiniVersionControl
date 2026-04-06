/*
 * Implementation of the Repository class
 * Authors: 
 *  Jacob Dawes - 041169788
 *  Ethan Geary - 041032340
 */

#include <filesystem>
#include <fstream>
#include <stdexcept>

#include "../includes/Repository.h"
#include "../includes/TrackedFile.h"

using string = std::string;
using cout = std::cout;
using endl = std::endl;

#define REPOWRAPPER ".vcm"

Repository::Repository() {};

Repository::~Repository() {};

std::string Repository::getRepoName() { return repoName; }

bool Repository::initRepository(const string& repoName,const string& repoPath) {

  this->repoPath = repoPath;
  
  // create snapshots , branches , config folders
  // & create the initial config file.
  if(!filesystem::create_directory(REPOWRAPPER)
     || !filesystem::create_directory(string(REPOWRAPPER) + "/snapshots")
     || !filesystem::create_directory(string(REPOWRAPPER) + "/Branches")
     || !filesystem::create_directory(string(REPOWRAPPER) + "/config")) {
    return false;
  }

  ofstream configFile(string(REPOWRAPPER) + "/config/repo_config",std::ios::out);

  if(!configFile) return false;

  configFile << "# this file contains the configurations of your repo.\n";
  configFile << "Repository Name: " + this->repoName;

  configFile.close();

  return true;
}

//this function is to check if the file is tracked and then get the file to pass into whatever has called it
TrackedFile& Repository::getTrackedFile(const string& filepath) {
  for(auto& file : files) {
    //if file is tracked then get the file object
    if(file.getFilePath() == filepath) {
      return file;
    }
  }
  //if no file tracked to return then do nothing since the next function will add it
  throw runtime_error("getTrackedFile() found no tracked file");
}

// begin tracking the file or staging if already tracked
// use the above function to check if the file is tracked and to then get the file and pass it back into this function
void Repository::addFile(const string& filepath) {
  if(fileIsTracked(filepath)) {
    TrackedFile& file = getTrackedFile(filepath);
    if (file.getFileStatus() == TrackedFile::status::Added) {
      return;
    }
    updateFileStatus(file, TrackedFile::status::Added);
  }
  
  else {
    TrackedFile newFile;
    newFile.updateContent(filepath, TrackedFile::status::Added);
    files.push_back(newFile);
  }
}


/**
 * This function is to stage files
 * what happens is it checks for different file status
 * if the file status is Added (has been added to the tracker vector) then stage it
 * if the file's content in the tracker vector differs from the local version of the file then set that file to modified rather than staging it
 * if the file is the same in both the tracker vector and local then stage it
 */
void Repository::stageFile(const string& filepath) {
  if(!fileIsTracked(filepath)) {
    throw runtime_error("no file tracked with that name"); //checking to see if file is NOT in the tracker vector
  }

  TrackedFile& file = getTrackedFile(filepath);
  vector<string> localContent = extractFileContent(filepath);
  vector<string> trackedContent = file.getFileContent();
  TrackedFile::status status = file.getFileStatus();
  
  if(status == TrackedFile::status::Added) {
    updateFileStatus(file, TrackedFile::status::Staged);
    return;
  }

  if(localContent != trackedContent) {
    updateFileStatus(file, TrackedFile::status::Modified);
    return;
  }

  updateFileStatus(file, TrackedFile::status::Staged);
}

// for Sam, this is the initial startingpoint for the commit button to call
// its just true for success, false for somethings gone wrong
bool Repository::commitChanges() {
  vector<TrackedFile> fileVector = getFileVector();
  bool returnvar = Commit::checkStagedFiles(fileVector);
  return returnvar;
}

// helper functions

// extract the file name from the path
string extractFileName(const string& filepath) {
  return filesystem::path(filepath).filename().string();
}

// get the content from the file into memory
vector<string> extractFileContent(const string& filepath) {
  ifstream inFile(filepath);
  if(!inFile) return {};

  string input = "";
  vector<string> content;
  while(getline(inFile,input)) {
    content.push_back(input);
  }

  inFile.close();
  return content;
}

/**
 * fileIsTracked iterates through the whole vector to return a true or false based on if the file already exists within the vector
 * the boolean output is false by default but if it matches the filepath to one found in the files vector then it sets the foundFile bool to true
 */
bool Repository::fileIsTracked(const string& filepath) {
  int vectorSize = files.size();
  bool foundFile = false;

  for(int i = 0; i < vectorSize; i++) {
    if(files[i].getFilePath() == filepath) {
      foundFile = true;
    }
  }
  return foundFile;
}

void Repository::updateFileStatus(TrackedFile& file, fileStatus newStatus) {
  string status;
  switch(newStatus) {
    case fileStatus::Added:
      status = "Added";
      break;
    case fileStatus::Modified:
      status = "Modified";
      break;
    case fileStatus::Staged:
      status = "Staged";
      break;
    case fileStatus::Committed:
      status = "Committed";
      break;
  }

  file.updateContent(file->getFilePath(), status);
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

vector<TrackedFile> Repository::getFileVector() {
  return files;
}

/* find commits within the repostiroy's commits vector. */
Commit* Repository::findCommit(const std::string& commitId) {
  if(commitId.empty()) return nullptr;

  for (const auto& commit : commits) {
    if(commit->getId() == commitId) return commit.get();
  }

  return nullptr;
}