#pragma once;

/*
 * class definitions for the Repository class
 * Author: Jacob Dawes - 041169788, Spencer Scarlett 041151395, Ethan Geary 041032340
 */

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <memory>

#include "TrackedFile.h"
#include "Commit.h"


class Repository {
  std::string repoName;
  std::string repoPath;
  std::vector<std::unique_ptr<TrackedFile>> files;
  std::vector<std::unique_ptr<Commit>> commits;
public:
  Repository();
  ~Repository();
  bool initRepository(const std::string& repoName,const std::string& repoPath);
  void addFile(const std::string& filepath);
  void stageFile(const std::string& filepath);
  bool commitChanges();
  void getCommitHistory();
  int  getNumOfCommits();
  int  getNumOfTrackedFiles();
  std::vector<TrackedFile> getFiles() const;
  bool fileIsTracked(const std::string& filepath);
};
    std::vector<TrackedFile> files;
    std::vector<std::unique_ptr<Commit>> commits;
    enum class fileStatus {Added, Modified, Staged, Committed};
    
    public:
    
    Repository::Repository();
    Repository::~Repository();

    bool initRepository(const std::string& repoName);
    TrackedFile& Repository::getTrackedFile(const std::string& filepath); // helper class to get the tracked file used in addFile
    void addFile(const std::string& filepath);
    void stageFile(const std::string& filepath);
    bool commitChanges();
    std::vector<std::string> getCommitHistory();
    void updateFileStatus(TrackedFile& file, enum fileStatus);
    bool fileIsTracked(const std::string& filepath);
    std::vector<TrackedFile> getFileVector();
    };
