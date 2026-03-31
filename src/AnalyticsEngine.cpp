#include <map>
#include <vector>
#include <string>

using namespace std;

// 1. Compute Total Commits
 // Accesses the commit vector from the Repository
template <typename T>
int AnalyticsEngine<T>::computeTotalCommits(const T& repo) {
    return static_cast<int>(repo.getCommits().size());
}

// 2. Compute Tracked Files Count
template <typename T>
int AnalyticsEngine<T>::computeTrackedFilesCount(const T& repo) {
    vector<TrackedFile> allFiles = repo.getFileVector();
    return static_cast<int>(allFiles.size());
}

// 3. Compute Most Modified Files
template <typename T>
void AnalyticsEngine<T>::computeMostModifiedFiles(const T& repo) {
    vector<TrackedFile> sortedFiles = repo.getFileVector();
    int n = sortedFiles.size();

    // Simple Bubble Sort Moving highest editCounts to the front(changeble)
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (sortedFiles[j].getEditCount() < sortedFiles[j + 1].getEditCount()) {
                // Swap them
                TrackedFile temp = sortedFiles[j];
                sortedFiles[j] = sortedFiles[j + 1];
                sortedFiles[j + 1] = temp;
            }
        }
    }

    // Convert the sorted objects into a string vector for the GUI
    vector<string> finalReport;
    for (int i = 0; i < n; i++) {
        string displayLine = sortedFiles[i].getFileName() +
            " - Edits: " + to_string(sortedFiles[i].getEditCount());
        finalReport.push_back(displayLine);
    }

    return finalReport;
}