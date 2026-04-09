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

#define REPOWRAPPER ".vcm"

Repository::Repository() {};

Repository::~Repository() {};

std::string Repository::getRepoName() { return repoName; }

bool Repository::initRepository(const string& repoName,const string& repoPath) {

  this->repoPath = repoPath;
  
  // create snapshots , branches , config folders
  // & create the initial config file.
  if(!std::filesystem::create_directory(REPOWRAPPER)
     || !std::filesystem::create_directory(string(REPOWRAPPER) + "/snapshots")
     || !std::filesystem::create_directory(string(REPOWRAPPER) + "/Branches")
     || !std::filesystem::create_directory(string(REPOWRAPPER) + "/config")) {
    return false;
  }

  std::ofstream configFile(string(REPOWRAPPER) + "/config/repo_config",std::ios::out);

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
  throw std::runtime_error("getTrackedFile() found no tracked file");
}

// begin tracking the file or return if already tracked
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
    TrackedFile newFile(filepath, TrackedFile::status::Added);
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
    throw std::runtime_error("no file tracked with that name"); //checking to see if file is NOT in the tracker vector
  }

  TrackedFile& file = getTrackedFile(filepath);
  std::vector<string> localContent = extractFileContent(filepath);
  std::vector<string> trackedContent = file.getFileContent();
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
bool Repository::commitChanges(StandardCommit& commit) {
  std::vector<TrackedFile> fileVector = commit.getIncomingFiles();
  bool returnvar = checkStagedFiles(commit);
  return returnvar;
}

bool Repository::checkStagedFiles(StandardCommit& commit) {
	//TODO: Check TrackedFile vector for which files are staged
	// it should iterate through the whole vector to make sure everything is staged
	// if a file is not staged then we return a false value

	//get vector size for the looping
	int vectorSize = commit.getIncomingFiles().size();

	//for loop to iterate through vector
	for (int i = 0; i < vectorSize; i++) {
		// check each file in the vector for if it is staged
		// if not every file is staged then return false and abort the current commit
		// the false return will cause the popup for listing every file that isnt staged and give the user the prompt for if they wish to try and stage those files, if that succeeds it will try to do a new commit
		if (commit.getIncomingFiles()[i].getFileStatus() != TrackedFile::status::Staged) {
			commit.getIncomingFiles().clear();
			return false;
			break;
		}

		// pass the current iterated file through compareHashedFiles to check
		// nomatter true or false it is a parameter to addToIncomingFiles
		commit.addToIncomingFiles(commit.compareHashedFiles(commit.getIncomingFiles()[i]), commit.getIncomingFiles()[i]);

	}
	return true;
}
// helper functions

// extract the file name from the path
std::string extractFileName(const std::string& filepath) {
  return std::filesystem::path(filepath).filename().string();
}

// get the content from the file into memory
std::vector<string> extractFileContent(const string& filepath) {
  std::ifstream inFile(filepath);
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

void Repository::updateFileStatus(TrackedFile& file, TrackedFile::status newStatus) {

  file.updateContent(file.getFilePath(), newStatus);
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

const vector<TrackedFile>& Repository::getFileVector() const {
  return files;
}

/* find commits within the repostiroy's commits vector. */
Commit* Repository::findCommit(const string& commitId) {
  if(commitId.empty()) return nullptr;

  for (const auto& commit : commits) {
    if(commit->getId() == commitId) return commit.get();
  }

  return nullptr;
}