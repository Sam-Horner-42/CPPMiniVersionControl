#pragma once

/*
 * class definitions for the Repository class
 * Author: Jacob Dawes - 041169788, Spencer Scarlett 041151395, Ethan Geary 041032340
 */

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

#include "TrackedFile.h"
#include "StandardCommit.h"
#include "../includes/nlohmann/json.hpp"

class Repository {
  std::string repoName; //name for repository instance
  std::string repoPath; //path for repository instance
  std::vector<std::unique_ptr<Commit>> commits; //vector of commits to a repository instance
  std::vector<TrackedFile> currentFiles; //vector for currently tracked files within an instance of a repository
  StandardCommit currentCommit; 
public:
	Repository() {} // default constructor
	Repository(std::string repoName, std::string repoPath):
		repoName(repoName), repoPath(repoPath) {} //parametized constructor to initialize a repository using its name and file path
	~Repository() {} //deconstructor for the repository object

  // copy constructor
  Repository(const Repository& other) 
    : repoName(other.repoName),
      repoPath(other.repoPath),
      currentFiles(other.currentFiles),
      currentCommit(other.currentCommit)
  {
      for (const auto& commit : other.commits) {
          if (commit) {
              StandardCommit* sc = dynamic_cast<StandardCommit*>(commit.get());
              if (sc) {
                  commits.push_back(std::make_unique<StandardCommit>(*sc));
              }
          }
      }
  }
  
  void updateFileStatus(const std::string& file, TrackedFile::status newStatus); //updating the status of a tracked file
  void updateFileContent(const std::string fileName, std::string newContent); //updating the content of a tracked file
  
  std::string getStatusAsString(const std::string fileName); //converter for getting a tracked file's status, but as a string for the GUI
  void setStatusAsString(const std::string fileName, std::string newStatusString); //converter for setting a tracked file's status, but as a string for the GUI

  void stageFile(const std::string& fileName); //stage a single file in the trackedfiles vector
  void initRepository(const std::string& repoName, const std::string& repoPath); //create a new repository
  void setRepoName(const std::string& repoName) { this->repoName = repoName; } //set the name of the repository
  TrackedFile* getSingleTrackedFile(const std::string& fileName); //returns a pointer of a single tracked file
  
  std::vector<std::string> getCommitHistory();
  int  getNumOfCommits();
  void addNewTrackedFile(TrackedFile& file); //adding a file into the tracked file vector
  const std::string& getRepoName() const; //getter for repository name
  const std::string& getRepoPath() const; //getter for repository path

  TrackedFile* findFile(const std::string& filename); //locating a file in the tracked file vector by taking in the files name

  void setCurrentCommit(Commit& c) { currentCommit = dynamic_cast<StandardCommit&>(c); }
  StandardCommit& getCurrentCommit() { return dynamic_cast<StandardCommit&>(currentCommit); }

  std::vector<std::unique_ptr<Commit>>& getRepoCommits(); //return all commits from the repository

  std::vector<TrackedFile>& getCurrentFiles() { return currentFiles; } //returns the current file vector for the repository
  void deleteTrackedFile(const std::string fileName); // Removes a single tracked file from the vector of tracked files
  void buildJSONMetaData(const std::string repoName, const std::string repoPath);
  void buildJSONSnapshots(const std::string repoName, const std::string repoPath);
  StandardCommit* findCommit(const std::string& commitId);

  void addCommit(std::unique_ptr<Commit> commit) { commits.push_back(std::move(commit)); } //adds a single commit object to the commits vector
  bool commitStagedFiles(std::string commitMessage, std::string commitId); //adds the file vector commit to the commit vector, takes in commit message and the commit ID
};
