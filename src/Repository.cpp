
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
// TODO: Update so that it reads froma JSON/TXT file, read the commit logs
// add to a vector of commit msgs, return vector to qt for use
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