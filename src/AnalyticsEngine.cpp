#include <map>
#include <vector>
#include <string>

using namespace std;

// 1. Compute Total Commits
 // Accesses the commit vector from the Repository
template <typename T>
void AnalyticsEngine<T>::computeTotalCommits(const T& repo) {
    int total = static_cast<int>(repo.getCommits().size());
    cout << left << setw(25) << "Total Commits:" << total << endl;
}

// 2. Compute Tracked Files Count
template <typename T>
int AnalyticsEngine<T>::computeTrackedFilesCount(const T& repo) {
    return static_cast<int>(repo.getTrackedFiles().size());
}

// 3. Compute Most Modified Files
template <typename T>
void AnalyticsEngine<T>::computeMostModifiedFiles(const T& repo) {
}