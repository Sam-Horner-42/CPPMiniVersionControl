#pragma once

#include <vector>
#include <string>
#include <map>

#include "../includes/Commit.h"
#include "../includes/TrackedFile.h"

class StandardCommit : public Commit {
private:
	std::map<std::string, std::string> fileSnapshot;
	std::vector<TrackedFile> commitVector;
public:
	StandardCommit(
		const std::string& commitId,
		const std::string& parentId,
		const std::string& message,
		const std::string& author,
		const std::string& timestamp
	);
	~StandardCommit();

	std::vector<std::string> getAllAttributes() override;
	std::vector<std::string> displayCommit() override;
	std::string getSummary() override;

	void createSnapshot(const std::string& filename, const std::vector<std::string>& content);
	void updateSnapshot(const std::string& filename, const std::vector<std::string>& content);
	bool hasFile(const std::string& filename) override;

	
	bool compareHashedFiles(TrackedFile comparingStagedFile);
	void addToCommitVector(bool tf, TrackedFile addingStagedFile);
	std::vector<TrackedFile> getCommitVector() const;
	std::vector<std::string> commitToRepo(std::vector<TrackedFile> commitFiles);
};