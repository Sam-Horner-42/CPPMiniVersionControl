#include <TrackedFile.h>
#include <Commit.h>

class DataManager {
    public:
    void saveData(string repositoryName,  vector<TrackedFile> files, vector<unique_ptr<Commit>> commits);
    bool loadData(const std::string& repositoryName, vector<TrackedFile> files, vector<unique_ptr<Commit>> commits);
};