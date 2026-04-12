/*
 * Definition for the Analytics Engine class
 * Author: Jacob Dawes - 041169788
 */

#ifndef ANALYTICSENGINE_H
#define ANALYTICSENGINE_H

#include <string>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <map>
#include <vector>
#include <string>
#include "TrackedFile.h"

using namespace std;
template <typename T>
class AnalyticsEngine {


public:
    // 1. Compute Total Commits
     // Accesses the commit vector from the Repository

    int computeTotalCommits(const T& repo) {
        return static_cast<int>(repo.getCommits().size());
    }

    // 2. Compute Tracked Files Count
    int computeTrackedFilesCount(const T& repo) {
        return static_cast<int>(repo.getCurrentFiles().size());
    }

    // 3. Compute Most Modified Files
    std::vector<std::string> computeMostModifiedFiles(const T& repo) {
        std::vector<TrackedFile> sortedFiles = repo.getCurrentFiles();
        //used the built in sorting method jacob mentioned 
        sort(sortedFiles.begin(), sortedFiles.end(), compareEdits);
        int n = sortedFiles.size();
        // Convert the sorted objects into a string vector for the GUI
        std::vector<std::string> finalReport;
        for (int i = 0; i < n; i++) {
            std::string displayLine = sortedFiles[i].getFileName() +
                " - Edits: " + std::to_string(sortedFiles[i].getEditCount());
            finalReport.push_back(displayLine);
        }

        return finalReport;
    }
    static bool compareEdits(const TrackedFile& a, const TrackedFile& b) {
        return a.getEditCount() > b.getEditCount();
    }
};
#endif