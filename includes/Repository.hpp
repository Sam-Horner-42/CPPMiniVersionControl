/*
 * class definitions for the Repository class
 * Author: Jacob Dawes - 041169788
 */

class Repository {
  std::string repoName;
  std::vector<TrackedFile> files;
  std::vector<std::unique_ptr<Commit>> commits;
public:
  bool initRepository(std::string repoName);
  void addFile(const std::string& filepath);
  void stageFile(const std::string& filepath);
  bool commitChanges();
};
