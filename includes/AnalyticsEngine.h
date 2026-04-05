/*
 * Definition for the Analytics Engine class
 */

#ifndef ANALYTICSENGINE_H
#define ANALYTICSENGINE_H

#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

template <typename T>
class AnalyticsEngine {
public:
    int computeTotalCommits(const T& repo);
    int  computeTrackedFilesCount(const T& repo);
    void computeMostModifiedFiles(const T& repo);
};
#include "AnalyticsEngine.cpp" 

#endif
