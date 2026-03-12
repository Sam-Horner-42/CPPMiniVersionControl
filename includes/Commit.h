#ifndef COMMIT_H
#define COMMIT_H

#include <string>
#include <map>

class Commit {

    public:
    std::string getId();
    std::string getDate();
    std::string getFiles();
    std::string getMessages();
    void displayCommit();
    void getSummary();

    private:
    std::string commitID;
    std::string message;
    std::string author;
    std::string timestamp;

};

#endif