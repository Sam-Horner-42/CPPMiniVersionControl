/*
 * Implementation of the Repository class
 * Authors: 
 *  Jacob Dawes - 041169788
 *  Ethan Geary - 041032340
 */

#include <filesystem>
#include <fstream>

#include "../includes/Repository.h"
#include "../includes/TrackedFile.h"
#include <../includes/Repository.h>

using namespace std;

#define REPOWRAPPER ".vcm"

Repository() {};
Repository::Repository() {}

Repository::~Repository() {}

bool Repository::initRepository(const string& repoName) {
  
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
  configFile << "Repository Name: " + this->repositoryName;

  configFile.close();

  return true;
}

// begin tracking the file or staging if already tracked
void Repository::addFile(const string& filepath) {
  if(fileIsTracked(filepath)) {
    stageFile(filepath);
  }
  else {
    TrackedFile newFile;

    newFile.updateContent(filepath);

    this->files.push_back(newFile);
  }
}

void Repository::stageFile(const string& filepath) {
  
}

bool Repository::commitChanges() {
  return false;
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

bool fileIsTracked(const string& filepath) {
  return false;
}


// 4. Repository
// Attributes:
// • repositoryName : string
// • files : vector<TrackedFile>
// • commits : vector<unique_ptr<Commit>>
// Behaviors:
// • initRepository()
// • addFile()
// • stageFile()
// • commitChanges()
// • restoreFile()
// • getCommitHistory()

String Repository::updateFileStatus(TrackedFile& file, enum fileStatus) {
  string status;

  

  return status;
}

// TODO: Update so that it reads froma JSON/TXT file, read the commit logs
// add to a vector of commit msgs, return vector to qt for use
vector<string> Repository::getCommitHistory() {

  // temp var, I assume it's already open so no need to reopen?
    vector<string> commitHistoryVec;
    for (auto& commit : commits) {
      string logCommit = "Commit ID: " + commit->getId() + 
       "Date: " + commit->getDate() + 
       "Message: " + commit->getMessages();
      // one commit pushed to vector
        commitHistoryVec.push_back(logCommit);
    }
    // holds full history
    return commitHistoryVec;
}   
