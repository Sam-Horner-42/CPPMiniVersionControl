#ifndef STUDENT_H 
#define STUDENT_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;


class Repository {

    public:
    string repositoryName;
    vector<TrackedFile> files;
    vector<unique_ptr<Commit>> commits;
    void getCommitHistory();
};

#endif // STUDENT_H