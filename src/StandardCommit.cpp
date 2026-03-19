
using namespace std;

class StandardCommit : public Commit {

using namespace std;

// default everything constructor
StandardCommit::StandardCommit(
        const std::string& commitId,
        const std::string& message,
        const std::string& author,
        const std::string& timestamp
    ) : commitId(commitId),message(message),
        author(author), timestamp(timestamp) {}

// default destructor
StandardCommit::~StandardCommit() {}

// combination of all getters into a predicable ordered
// vector. ORDER: ID, MESSAGE, AUTHOR, TIMESTAMP
// @return: vector of commit data in order.
vector<string> StandardCommit::getAllAttributes() override {
    vector<string> content;

    content.push_back(commitId);
    content.push_back(message);
    content.push_back(author);
    content.push_back(timestamp);

    return content;
}

// returns a full vector containing all commit info
vector<string> StandardCommit::displayCommit() override {
    return this->getAllAttributes();
}

// returns a small summary commit id and message
string StandardCommit::getSummary() override {
    return "Commit ID: " + this->getId() +
    "Commit Message:\n" + this->getMessage();
}