/*
 * definition of repo manager
 * Author: Jacob Dawes - 041169788
 */
#pragma once

#include <string>
#include <unordered_map>
#include <algorithm>
#include <ctime>

#include "../includes/Repository.h"
#include "../includes/TrackedFile.h"
#include "../includes/StandardCommit.h"
#include "../includes/DataManager.h"
#include "../includes/AnalyticsEngine.h"
#include "../includes/DiffEngine.h"
#include "../includes/Repository.h"



class RepositoryManager {
  private:
    Repository repo;
    DataManager data;
    AnalyticsEngine<Repository> analyzer;
    DiffEngine differ;
  public:
	struct Project {
		std::string name;
		std::string filePath;
		std::string id;
	};
	  RepositoryManager();
    ~RepositoryManager();

	std::vector<Project> ProjectInfo();
  void createRepository(const std::string& repoName, const std::string& repoPath); //wrapper function for createRepository in repository.cpp
  bool loadRepository(std::string repoName); //wrapper function to call repository::loadRepository() through the repo object
  void saveRepository(std::string repoName, const  std::string repoPath); //wrapper function to call repository::saveRepository()
  StandardCommit* searchCommits(const std::string& searchString); 
  void addNewTrackedFile(TrackedFile& file); //wrapper functioin to call repository::addNewTrackedFile()
  void deleteTrackedFile(std::string fileName); //wrapper function to call repository::deleteTrackedFile()
  bool checkIfReal(std::string repoName);
  TrackedFile::status getFileStatus(std::string fileName);
  std::string getFileContent(std::string fileName);

	std::vector<std::unique_ptr<Commit>>& getRepoCommits();
	void stageAllFiles();
	std::string getStatusAsString(const std::string fileName);
  void setStatusAsString(const std::string fileName, std::string newStatusString);

  void restoreToParent(const std::string& commitId);

  std::vector<std::string> getCommitHistory();
    
	void updateFileStatus(std::string& fileName, TrackedFile::status newStatus); //wrapper to Repository::updateFileStatus()
  void updateFileContent(const std::string& filename,const std::string& newContent); //wrapper to Repository::updateFileContent()

  std::vector<TrackedFile>& getCurrentFiles(); //wrapper to call Repository::getCurrentFiles(), it just returns the reference to the tracked file vector
	std::vector<Project> getProjectInfo();
  void addNewTrackedFile(const std::string& filePath, const std::string& fileName); //function to initialize a new file into the tracked file vector, has the filepath and filename, has added status by default, and will have null for the others

  std::unordered_map<std::string, std::string> callParentDifferentiation(const std::string& diffCommitId);
  StandardCommit* getParentCommit(const std::string& commitId);
  void stageFile(const string& fileName); //wrapper for Repository::stageFile(), changes file status to staged when good
  //void addCommit(std::unique_ptr<StandardCommit> commit); //wrapper for Repository::addCommit(), takes the commit object from the below function and puts it into the commits vector
  void addCommit(std::string commitId, std::string parent, std::string commitMessage,
	  std::string admin, std::string timeStamp);
  bool commitStagedFiles(std::string commitMessage); //wrapper for Repository::commitStagedFiles() using commit message and commit ID, works through repo object
  std::vector<std::string> getAnalytics() const;//
};