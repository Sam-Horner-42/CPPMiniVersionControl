/*
 * Implementation of the Repository class
 * Authors: 
 *  Jacob Dawes - 041169788
 *  Ethan Geary - 041032340
 */

#include "../includes/Repository.h"

#define REPOWRAPPER ".vcm"

Repository::Repository() {};

Repository::~Repository() {}

bool Repository::initRepository(const std::string& repoName, const std::string& repoPath) {

  this->repoPath = repoPath;
  
  // create snapshots , branches , config folders
  // & create the initial config file.
  if(!std::filesystem::create_directory(REPOWRAPPER)
     || !std::filesystem::create_directory(std::string(REPOWRAPPER) + "/snapshots")
     || !std::filesystem::create_directory(std::string(REPOWRAPPER) + "/Branches")
     || !std::filesystem::create_directory(std::string(REPOWRAPPER) + "/config")) {
    return false;
  }

  std::ofstream configFile(std::string(REPOWRAPPER) + "/config/repo_config",std::ios::out);

  if(!configFile) return false;

  configFile << "# this file contains the configurations of your repo.\n";
  configFile << "Repository Name: " + this->repoName;

  configFile.close();

  return true;
}

//this function is to check if the file is tracked and then get the file to pass into whatever has called it
TrackedFile& Repository::getTrackedFile(const std::string& filepath) {
  for(auto& file : files) {
    //if file is tracked then get the file object
    if(file.getFilePath() == filepath) {
      return file;
    }
  }
  //if no file tracked to return then do nothing since the next function will add it
  throw std::runtime_error("getTrackedFile() found no tracked file");
}

// begin tracking the file or staging if already tracked
// use the above function to check if the file is tracked and to then get the file and pass it back into this function
void Repository::addFile(const std::string& filepath) {
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
void Repository::stageFile(const std::string& filepath) {
  if(!fileIsTracked(filepath)) {
    throw std::runtime_error("no file tracked with that name"); //checking to see if file is NOT in the tracker vector
  }

  TrackedFile& file = getTrackedFile(filepath);
  std::vector<std::string> localContent = extractFileContent(filepath);
  std::vector<std::string> trackedContent = file.getFileContent();
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
  std::vector<TrackedFile> fileVector = getFileVector();
  bool returnvar = Commit::checkStagedFiles(fileVector);
  return returnvar;
}

// helper functions

// extract the file name from the path
std::string extractFileName(const std::string& filepath) {
  return std::filesystem::path(filepath).filename().string();
}

// get the content from the file into memory
std::vector<std::string> extractFileContent(const std::string& filepath) {
  std::ifstream inFile(filepath);
  if(!inFile) return {};

  std::string input = "";
  std::vector<std::string> content;
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
bool Repository::fileIsTracked(const std::string& filepath) {
  int vectorSize = files.size();
  bool foundFile = false;

  for(int i = 0; i < vectorSize; i++) {
    if(files[i].getFilePath() == filepath) {
      foundFile = true;
    }
  }
  return foundFile;
}


// 4. Repository
// Attributes:
// • repoName : string
// • files : vector<TrackedFile>
// • commits : vector<unique_ptr<Commit>>
// Behaviors:
// • initRepository()
// • addFile()
// • stageFile()
// • commitChanges()
// • restoreFile()
// • getCommitHistory()

void Repository::updateFileStatus(TrackedFile& file, TrackedFile::status newStatus) {
  file.updateContent(file.getFilePath(), newStatus);
}

// TODO: Update so that it reads froma JSON/TXT file, read the commit logs
// add to a vector of commit msgs, return vector to qt for use
std::vector<std::string> Repository::getCommitHistory() {

  // temp var, I assume it's already open so no need to reopen?
    std::vector<std::string> commitHistoryVec;
    for (auto& commit : commits) {
      std::string logCommit = "Commit ID: " + commit->getId() + 
       "Date: " + commit->getDate() + 
       "Message: " + commit->getMessage();
      // one commit pushed to vector
        commitHistoryVec.push_back(logCommit);
    }
    // holds full history
    return commitHistoryVec;
}   

const std::vector<TrackedFile>& Repository::getFileVector() const {
  return files;
}