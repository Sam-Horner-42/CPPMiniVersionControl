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
    RepositoryManager(Repository& repo);
    ~RepositoryManager();

	  Project getProjectInfo();
    void createRepository(const std::string& repoName, const std::string& repoPath);
    bool loadRepostiory(std::string& repoName);
    void saveRepository();
    StandardCommit* searchCommits(const std::string& searchString);
    TrackedFile::status getFileStatus(const TrackedFile& file);
    void restoreToParent(const std::string& commitId);
	  void updateFileStatus(std::string& filePath, TrackedFile::status newStatus);
    std::unordered_map<std::string, std::string> callParentDifferentiation(StandardCommit& diffCommit);
    std::unordered_map<std::string,std::string> callRegularDifferentiation(StandardCommit& diffCommit1, StandardCommit& diffCommit2);

    void updateFileContent(const std::string& filename,const std::string& newContent);
    StandardCommit* getParentCommit(const std::string& commitId);
    void restore(const std::string& commitId, const std::string& restoreCommitId);
};