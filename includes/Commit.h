/*
 * class definitions for the commit class
 * Author: Jacob Dawes - 041169788, Spencer Scarlett 041151395, Ethan Geary 041032340
 */


#include <string>
#include <map>

using namespace std;

class Commit {

    public:
    string getId();
    string getDate();
    string getFiles();
    string getMessages();
    void displayCommit();
    void getSummary();

    private:
    string commitID;
    string message;
    string author;
    string timestamp;

};