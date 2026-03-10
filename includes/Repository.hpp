/*
 * class definitions for the Repository class
 * Author: Jacob Dawes - 041169788
 */

class Repository {
  std::string repoName;
  std::vector<TrackedFile> files;
  std::vector<std::unique_ptr<Commit>> commits;
public:
  bool initRepository();
  bool addFile();
  bool stageFile();
  bool commitChanges();
};
