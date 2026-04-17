/*
 * Implementation of the Repository class
 * Authors: 
 *  Jacob Dawes - 041169788
 *  Ethan Geary - 041032340
 *	Sam Horner - 040935005
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
	qDebug() << "Commits size: " << commits.size();
    for (const auto& commit : commits) {
		if (commit) {
			qDebug() << "We got into the for loop.";
			string logCommit = "Commit ID: " + commit->getId() +
				"Date: " + commit->getTimestamp() +
				"Message: " + commit->getMessage();
			// one commit pushed to vector
			commitHistoryVec.push_back(logCommit);
		}
		else {
			qDebug() << "getCommitHistory(): A null commit is found.";
		}
    }
    // holds full history
    return commitHistoryVec;
}

vector<string> Repository::getAllCommitIds() const {
	vector<string> out;
	for (const auto& commit : commits) {
		out.push_back(commit.get()->getId());
	}
	return out;
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
  TrackedFile* file = getSingleTrackedFile(fileName); //gets a single file via the file name
  TrackedFile::status status = file->getFileStatus(); //pulls the status from that file
  
  if(status == TrackedFile::status::Added || status == TrackedFile::status::Modified) { //if the file has its status as added or modified then you can stage the file
    updateFileStatus(fileName, TrackedFile::status::Staged); //updates the files status to staged
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
void Repository::restore(std::string commitId){
	qDebug() << "Repo path in restore(): " << repoPath;
	qDebug() << "Restore has been called with commit ID: " << commitId;
	auto current = findCommit(commitId);
	qDebug() << "Before null";
	if (current == nullptr) return;
	qDebug() << "Current is not null";

	for (auto& file : currentFiles) {
		//if (file == nullptr) continue;
		file.setStatus(TrackedFile::status::Committed);
	}

	auto& map = current->getFileSnapshot();

	for (auto it = map.begin(); it != map.end(); ++it) {
		qDebug() << "restore(): I have entered, the loop for the restore map.";
		string path = repoPath + "/" + it->first;
		qDebug() << "OUT PATH: " << path;
		std::ofstream out_file(path);
		if (!out_file) continue;
		out_file << it->second;
		out_file.close();
	}
	// find the restored pos
	auto index = std::find_if(commits.begin(), commits.end(),
		[&](const std::unique_ptr<Commit>& c) {
			return c->getId() == current->getId();
		}
	);
	// erase all commits after the restoration
	if(index != commits.end())
		commits.erase(index + 1);
	else qDebug() << "DEBUG: index not found in restore()";
}
/* find commits within the repostiroy's commits vector. */
StandardCommit* Repository::findCommit(const string& commitId) {
  if(commitId.empty()) return nullptr;

  for (const auto& commit : commits) {
    if(commit->getId() == commitId) return dynamic_cast<StandardCommit*>(commit.get());
  }

  return nullptr;
}

// Opens a file based on its path and retrieves the content to be added to the commits map
std::string Repository::readFileContent(const std::string& filepath) {
	std::ifstream file(filepath);

	// Check if the file is opened successfully
	if (!file.is_open()) {
		qDebug() << "Error: Could not open file at " << filepath;
		return ""; // Return no content if there is no content 
	}

	std::stringstream buffer;
	buffer << file.rdbuf(); // Reads the entire file buffer into the stringstream

	return buffer.str();
}

// Takes in the vector of current files and the commit to add the map to
// Adds the currently committed files within the vector to the commit map for this commit
void Repository::addCommitsToMap(std::vector<TrackedFile>& currentFiles, Commit& commit) {
	for (auto& file : currentFiles) {
		if (file.getFileStatus() == TrackedFile::status::Committed) {
			std::string content = readFileContent(file.getFilePath());
			dynamic_cast<StandardCommit&>(commit).createSnapshot(file.getFileName(), content);

		}
	}
}

void Repository::addCommit(std::string commitId, std::string parent, std::string commitMessage, 
	std::string admin, std::string timeStamp) { 
	//commits.push_back(std::move(commit)); 
	qDebug() << "Add Commit Successfully called";
	commits.emplace_back(std::make_unique<StandardCommit>(commitId, parent, commitMessage, admin, timeStamp));
	if(!commits.back().get()) qDebug() << "addCommit() the commit created is null";
	for (auto& trackedFile : currentFiles) {
		auto& commit = dynamic_cast<StandardCommit&>(*commits.back().get());
		commit.createSnapshot(trackedFile.getFileName(), trackedFile.getContent());
	}
	qDebug() << "Commits size: " << commits.size();
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

		std::ifstream inFile(files[i].getFilePath());
		std::string content;
		inFile >> content;
		files[i].setContent(content);
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
	//auto newCommit = std::make_unique<StandardCommit>(commitId, parent, commitMessage, "Admin", realTime);

	// Need to add the committed files from this commit to the commit's map of files, this should also add any files that were already committed
	//addCommitsToMap(files, *newCommit.get());
	// pass commit object into addCommit
	addCommit(commitId, parent, commitMessage, "Admin", realTime);

	// return bool TRUE if successful, FALSE if no currently staged files
	return true;
}