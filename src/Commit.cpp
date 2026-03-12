#include "../includes/Commit.h"

//functions
void Commit::displayCommit() {
    //TODO: DISPLAY COMMIT
}

void Commit::getSummary() {
    //TODO: GET COMMIT SUMMARY
}


//functions to be used in Repository.cpp
string Commit::getId() {
    return commitID;
}

string Commit::getDate() {
    return timestamp;
}

vector<TrackedFile> getFiles() {
    return files;
}

string Commit::getMessages() {
    return message;
}