#include "../includes/Commit.h"

//functions
void Commit::displayCommit() {
    //TODO: DISPLAY COMMIT
}

void Commit::getSummary() {
    //TODO: GET COMMIT SUMMARY
}


//functions to be used in Repository.cpp
string const Commit::getId() {
    return commitID;
}

string const Commit::getDate() {
    return timestamp;
}

vector<TrackedFile> const Commit::getFiles() {
    return files;
}

string const Commit::getMessages() {
    return message;
}