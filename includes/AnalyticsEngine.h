/*
 * Definition for the Analytics Engine class
 * Author: Jacob Dawes - 041169788
 */

<<<<<<< HEAD
#ifndef ANALYTICSENGINE_H
#define ANALYTICSENGINE_H

#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

template <typename T>
class AnalyticsEngine {
public:
    void computeTotalCommits(const T& repo);
    int  computeTrackedFilesCount(const T& repo);
    void computeMostModifiedFiles(const T& repo);
};
#include "AnalyticsEngine.cpp" 

#endif
=======
class AnalyticsEngine {
public:
  void computeTotalCommits();
  int  computeTrackedFilesCount();
  void computeMostModifiedFiles();
};
>>>>>>> 9fd035e (changed hpp to h, worked on repo and diffEngine classes - Jacob)
