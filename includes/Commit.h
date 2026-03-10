

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