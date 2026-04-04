
using namespace std;

class StandardCommit : public Commit {

// default constructor
StandardCommit::StandardCommit(
        const std::string& commitId,
        const std::string& parentId,
        const std::string& message,
        const std::string& author,
        const std::string& timestamp
    ) : Commit(commitId,parentId,message,author,timestamp) {}

// default destructor
StandardCommit::~StandardCommit() {}

// combination of all getters into a predicable ordered
// vector. ORDER: { ID, MESSAGE, AUTHOR, TIMESTAMP }
// @return: vector of commit data in order.
vector<string> StandardCommit::getAllAttributes() {
    vector<string> content;

    content.push_back(getId());
    content.push_back(getMessage());
    content.push_back(getAuthor());
    content.push_back(getTimestamp());

    return content;
}

// returns a full vector containing all commit info
vector<string> StandardCommit::displayCommit() {
    return this->getAllAttributes();
}

// returns a small summary commit id and message
string StandardCommit::getSummary() {
    return "Commit ID: " + getId() +
    " Parent ID: " + getParentId() +
    " Commit Message:\n" + getMessage();
}