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
  std::string repoName;
  std::string repoPath;
  std::vector<std::unique_ptr<Commit>> commits;
  std::vector<TrackedFile> currentFiles;
  StandardCommit currentCommit;
public:
	Repository() {};
	~Repository() {};

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

  void updateFileStatus(const std::string& file, TrackedFile::status newStatus);
  void stageFile(const std::string& filepath);
  bool initRepository(const std::string& repoName, const std::string& repoPath);
  void setRepoName(const std::string& repoName) { this->repoName = repoName; }
  TrackedFile* getSingleTrackedFile(const std::string& filePath);
  
  std::vector<std::string> getCommitHistory();
  int  getNumOfCommits();

  std::string getRepoName();

  TrackedFile* findFile(const std::string& filename);

  void setCurrentCommit(Commit& c) { currentCommit = dynamic_cast<StandardCommit&>(c); }
  StandardCommit& getCurrentCommit() { return dynamic_cast<StandardCommit&>(currentCommit); }

  std::vector<TrackedFile> getCurrentFiles() { return currentFiles; }
  
  const std::vector<TrackedFile>& getFileVector();
  void buildJSONMetaData(const std::string& repoPath);
  StandardCommit* findCommit(const std::string& commitId);

  void addCommit(std::unique_ptr<StandardCommit> commit) { commits.push_back(std::move(commit)); }
};
