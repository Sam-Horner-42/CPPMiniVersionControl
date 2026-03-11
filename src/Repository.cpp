/*
 * Implementation of the Repository class
 * Authors: 
 *  Jacob Dawes - 041169788
 *  Ethan Geary - 0411-----
 */

#include <filesystem>
#include <fstream>

#include <../includes/TrackedFile.hpp>

using namespace std;

#define REPOWRAPPER ".vcm"

public Repository() {}

bool initRepository(string repoName) {
  
  // create snapshots , branches , config folders
  // & create the initial config file.
  if(!filesystem::create_directory(REPOWRAPPER)
     || !filesystem::create_directory(REPOWRAPPER + "/snapshots"
     || !filesystem::create_directory(REPOWRAPPER + "/Branches"
     || !filesystem::create_directory(REPOWRAPPER + "/config") {
    return false;
  }

  ofstream configFile(REPOWRAPPER + "/config/repo_config",std::ios::out);

  if(!configFile) return false;

  configFile << "# this file contains the configurations of your repo.\n";
  configFile << "Repository Name: " + this->repoName;

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

    newFile.setFilePath(filepath);
    newFile.setFileName(extractFileName(filepath));
    newFile.setContent(extractFileContent(filepath));

    *(this)->files.push_back(newFile);
  }
}

void Repository::stageFile(const string& filepath) {

}

bool Repository::commitChanges() {

}

// helper functions

// extract the file name from the path
string extractFileName(const string& filepath) {
  return filesystem::path(filepath).filename().string();
}

// get the content from the file into memory
vector<string> extractFileContent(const string& filepath) {
  ifstream inFile(filepath);
  if(!inFile) return nullptr;

  string input = "";
  vector<string> content;
  while(getline(inFile,input)) {
    content.push_back(input);
  }

  inFile.close();
  return content;
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

// TODO: Update so that it reads froma JSON/TXT file, read the commit logs
// add to a vector of commit msgs, return vector to qt for use
void Repository::getCommitHistory() {
    for (auto& commit : commits){
        cout << "Commit ID: " << commit->getId() << endl;
        cout << "Date: " << commit->getDate() << endl;
        cout << "Files: " << endl;
        for (auto& file : commit->getFiles()) {
            cout << ", " << file->getFileName() << endl;
        }
        for (auto& message : commit->getMessages()) {
            cout << "Commit: " << commit->getMessages() << endl;
        }
    }
}   
