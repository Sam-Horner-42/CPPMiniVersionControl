
#include "../includes/StandardCommit.h"
#include "../includes/DataManager.h"

using std::string;

// combination of all getters into a predicable ordered
// vector. ORDER: { ID, MESSAGE, AUTHOR, TIMESTAMP }
// @return: vector of commit data in order.
std::vector<std::string> StandardCommit::getAllAttributes() {
	std::vector<std::string> content;

	content.push_back(getId() + "\n");
	content.push_back(getMessage() + "\n");
	content.push_back(getAuthor() + "\n");
	content.push_back(getTimestamp() + "\n");

	return content;
}


void StandardCommit::createSnapshot(const std::string& filename, const std::string& content) {
	fileSnapshots.insert({ filename,content });
}

void StandardCommit::updateSnapshot(const std::string& filename, const std::string& content) {
	fileSnapshots[filename] = content;
}

// returns a full vector containing all commit info
std::vector<std::string> StandardCommit::displayCommit() {
	return this->getAllAttributes();
}

// returns a small summary commit id and message
std::string StandardCommit::getSummary() {
	return "Commit ID: " + getId() +
		" Parent ID: " + getParentId() + //StandardCommit* getParentCommit(const std::string& commitId);
		" Commit Message:\n" + getMessage();
}

// for Sam, this is the initial startingpoint for the commit button to call
// its just true for success, false for somethings gone wrong
bool StandardCommit::commitChanges(StandardCommit& commit) {
  std::vector<TrackedFile> fileVector = commit.getIncomingFiles();
  bool returnvar = checkStagedFiles(commit);
  return returnvar;
}

bool StandardCommit::checkStagedFiles(StandardCommit& commit) {
	for (const auto& file : commit.getTrackedFiles()) {
		if (file.getFileStatus() != TrackedFile::status::Staged) return false; //gui for asking if good or bad
	}
	return true;
}

// helper functions

// extract the file name from the path
std::string StandardCommit::extractFileName(const std::string& filepath) {
  return std::filesystem::path(filepath).filename().string();
}

/**
 * fileIsTracked iterates through the whole vector to return a true or false based on if the file already exists within the vector
 * the boolean output is false by default but if it matches the filepath to one found in the files vector then it sets the foundFile bool to true
 */
bool StandardCommit::fileIsTracked(const string& filepath) {
  int vectorSize = getTrackedFiles().size();
  bool foundFile = false;

  for(int i = 0; i < vectorSize; i++) {
    if(getTrackedFiles()[i].getFilePath() == filepath) {
      foundFile = true;
    }
  }
  return foundFile;
}

bool StandardCommit::compareHashedFiles(const std::string& content,const std::string& fileName) {
	//Mano-TODO: 
	//please make this function to compare the file hash of each file in the TrackedFiles vector
	//with the file hash of each file in the current commit of the repository
	//return true if hashes are different, false if hashes are the same
	DataManager manage;
	// for (const auto& line : comparingStagedFile.getFileContent()) {
	// 	currentText += line;
	// }
	std::string currentHash = manage.singleHash(content);

	std::string previousHash = "";
	if (fileSnapshots.count(fileName)) {
		auto& it = fileSnapshots.at(fileName);
		previousHash = manage.singleHash(it);
	}
	return (currentHash != previousHash);
}


// this function is a getter for the commit vector. The commit vector is just a vector of the tracked files that are specifically getting commit into the repo.
// How I have the committing set up is just that i have a few functions that narrow down the tracked files vector to just transfer staged files that have a different hash to the repo file
// this IncomingFile Vector just stores those files that have a different hash to the repository old version, and then they all have the status set to Commit.
// its entirely used for another function to get that IncomingFile Vector during the commit.

// for spencer, what i was thinking was just you take the IncomingFile Vector, which holds each file object being commit, and put that into the repo as the intended txt structure
std::vector<TrackedFile> StandardCommit::getIncomingFiles() const {
	return trackedFiles;
}


bool StandardCommit::hasFile(const std::string& filename) {
	auto it = fileSnapshots.find(filename);
	return it != fileSnapshots.end() ? true : false;
}