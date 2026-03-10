
// 4. Repository
// Attributes:
// • repositoryName : string
// • files : vector<TrackedFile>
// • commits : vector<unique_ptr<Commit>>
// Behaviors:
// • initRepository()
// • addFile()
// • stageFile()
// • commitChanges()
// • restoreFile()
// • getCommitHistory()


#include "../includes/Repository.h"
#include "../includes/TrackedFile.h"
#include "../includes/Commit.h"

void Repository::getCommitHistory() {
    for (auto& commit : commits){
        cout << "Commit ID: " << commit->getId() << endl;
        cout << "Date: " << commit->getDate() << endl;
        cout << "Files: " << endl;
        for (auto& file : commit->getFiles()) {
            cout << ", " << file->getFileName() << endl;
        }
        for (auto& message : commit->getMessages()) {
            cout << "Commit: " << commit->getMessages() << endl;
        }
    }
}