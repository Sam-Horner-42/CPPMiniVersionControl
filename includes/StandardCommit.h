#pragma once

#include <vector>
#include <string>
#include <map>

#include "Commit.h"


class StandardCommit : public Commit {
private:
	std::map<std::string, std::string> fileSnapshots;
	std::vector<TrackedFile> trackedFiles;
public:
	StandardCommit(
		const std::string& commitId,
		const std::string& parentId,
		const std::string& message,
		const std::string& author,
		const std::string& timestamp

	) : Commit(commitId, parentId, message, author, timestamp) {}
	~StandardCommit() {}

	StandardCommit() {}

	int  getNumOfTrackedFiles() { return trackedFiles.size(); }

  	std::vector<TrackedFile> getFiles() const;
  	bool fileIsTracked(const std::string& filepath);

	std::vector<std::string> getAllAttributes() override;
	std::vector<std::string> displayCommit() override;
	std::string getSummary() override;

	void createSnapshot(const std::string& filename, const std::string& content);
	void updateSnapshot(const std::string& filename, const std::string& content);
	bool hasFile(const std::string& filename) override;

	std::map<std::string,std::string>& getFileSnapshots() { return fileSnapshots; }

	StandardCommit* getParentCommit(std::string parentId);
	std::string extractFileName(const std::string& filepath);
	void stageFile(const std::string& filepath);
	bool compareHashedFiles(const std::string& content,const std::string& fileName);
	std::vector<TrackedFile> getIncomingFiles() const;

	bool checkStagedFiles(StandardCommit& commit);
  	bool commitChanges(StandardCommit& commit);

	bool operator==(StandardCommit& other) {
		return this->getId() == other.getId();
	}
};