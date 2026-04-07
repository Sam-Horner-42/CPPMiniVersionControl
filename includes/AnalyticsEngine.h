/*

Definition for the Analytics Engine class
Author: Jacob Dawes - 041169788*/

//#ifndef ANALYTICSENGINE_H
//#define ANALYTICSENGINE_H

//#include <string>
//#include <iostream>
//#include <iomanip>
//
//#include <map>
//#include <vector>
//#include <string>
//
//using namespace std;
//
//// 1. Compute Total Commits
// // Accesses the commit vector from the Repository
//template <typename T>
//int AnalyticsEngine<T>::computeTotalCommits(const T& repo) {
//	return static_cast<int>(repo.getCommits().size());
//}
//
//// 2. Compute Tracked Files Count
//template <typename T>
//int AnalyticsEngine<T>::computeTrackedFilesCount(const T& repo) {
//	return static_cast<int>(repo.getFileVector().size());
//}
//
//// 3. Compute Most Modified Files
//template <typename T>
//std::vector<std::string> AnalyticsEngine<T>::computeMostModifiedFiles(const T& repo) {
//	std::vector<TrackedFile> sortedFiles = repo.getFileVector();
//	//used the built in sorting method jacob mentioned 
//	std::sort(sortedFiles.begin(), sortedFiles.end(), compareEdits);
//
//	// Convert the sorted objects into a string vector for the GUI
//	vector<string> finalReport;
//	for (int i = 0; i < n; i++) {
//		std::string displayLine = sortedFiles[i].getFileName() +
//			" - Edits: " + std::to_string(sortedFiles[i].getEditCount());
//		finalReport.push_back(displayLine);
//	}
//
//	return finalReport;
//}
//static bool compareEdits(const TrackedFile& a, const TrackedFile& b) {
//	return a.getEditCount() > b.getEditCount();
//}
//
//#endif
