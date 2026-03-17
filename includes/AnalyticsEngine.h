/*
 * Definition for the Analytics Engine class
 * Author: Jacob Dawes - 041169788
 */
#pragma once

#ifndef ANALYTICSENGINE_H_
#define ANALYTICSENGINE_H_

#include "Repository.h"

class AnalyticsEngine {
  public:

  template<typename T>
  T computeTotalCommits(const Repository& repo) {
    return static_cast<T>(repo.getNumOfCommits());
  }
  
  template<typename T>
  T computeTrackedFilesCount(const Repository& repo) {
    return static_cast<T>(repo.getNumOfTrackedFiles());
  }
  
  template<typename T>
  T computeMostModifiedFiles(const Repository& repo) {
    TrackedFile bufferFile;
    vector<TrackedFile> repo_files = repo.getFiles();

    bufferFile = repo_files[0];

    for(const auto& file : (repo_files + 1)) {
      if(bufferFile.modifiedCount() < file.modifiedCount()) {
        bufferFile = file;
      }
    }
  }
};

#endif