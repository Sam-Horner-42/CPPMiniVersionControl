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
#include <stdexcept>

#include "TrackedFile.h"
#include "Commit.h"


class Repository {
  std::string repoName;
  std::string repoPath;
  std::vector<TrackedFile> files;
  std::vector<std::unique_ptr<Commit>> commits;
public:
  Repository();
  ~Repository();

  bool initRepository(const std::string& repoName, const std::string& repoPath);
  void addFile(const std::string& filepath);
  void stageFile(const std::string& filepath);
  bool commitChanges();

  std::vector<std::string> getCommitHistory();
  int  getNumOfCommits();

  TrackedFile& Repository::getTrackedFile(const std::string& filepath); // helper class to get the tracked file used in addFile
  int  getNumOfTrackedFiles();

  std::vector<TrackedFile> getFiles() const;
  bool fileIsTracked(const std::string& filepath);

  //enum class fileStatus {Added, Modified, Staged, Committed};

  void updateFileStatus(TrackedFile& file, TrackedFile::status newStatus);
  
  std::vector<TrackedFile> getFileVector();

  const std::vector<TrackedFile>& Repository::getFileVector() const;
  
};
