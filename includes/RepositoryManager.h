/*
 * definition of repo manager
 * Author: Jacob Dawes - 041169788
 */
#pragma once

#include <string>
#include <unordered_map>
#include <algorithm>

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
    AnalyticsEngine<StandardCommit> analyzer;
    DiffEngine differ;
  public:
	struct Project {
		std::string name;
		std::string filePath;
		std::string id;
	};
	  RepositoryManager();
    ~RepositoryManager();

	std::vector<Project> getProjectInfo();
    void createRepository(const std::string& repoName, const std::string& repoPath);
    bool loadRepository(std::string repoName);
    void saveRepository(std::string& repoName);
    StandardCommit* searchCommits(const std::string& searchString);

    TrackedFile::status getFileStatus(std::string fileName);
    std::string getFileContent(std::string fileName);

	void stageAllFiles();
    std::string getStatusAsString(const std::string fileName);
    void setStatusAsString(const std::string fileName, std::string newStatusString);

    void restoreToParent(const std::string& commitId);

    std::vector<std::string> getCommitHistory();
    
	  void updateFileStatus(std::string& fileName, TrackedFile::status newStatus);
    void updateFileContent(const std::string& filename,const std::string& newContent);

    const std::vector<TrackedFile> getCurrentFiles() const;

    void addNewFile(const std::string& filePath, const std::string& fileName); //function to initialize a new file into the tracked file vector, has the filepath and filename, has added status by default, and will have null for the others

    std::unordered_map<std::string, std::string> callParentDifferentiation(const std::string& diffCommitId);
    StandardCommit* getParentCommit(const std::string& commitId);
    void stageFile(const string& fileName);
};