// Converter Implementation
// This file handles conversions from regular C++ data to QT data
// This will allow the GUI to remain seperate from the backend logic
// This aids in abstraction and reduces duplicated code
#include "Converter.h"

#include "../includes/TrackedFile.h"
#include "../includes/Commit.h"
#include "../includes/Repository.h"

namespace Converter {

	// Tracked File

	// Single-line summary for list displays
	QString toQString(const TrackedFile& file) {
		return QString("[%1] %2")
			.arg(QString::fromStdString(file.getFileStatus()))
			.arg(QString::fromStdString(file.getFileName()));
	}

	// Full row data for QTableWidget columns
	QMap<QString, QString> toDisplayMap(const TrackedFile& file) {
		return {
			{ "File Name", QString::fromStdString(file.getFileName())   },
			{ "Path",      QString::fromStdString(file.getFilePath())   },
			{ "Status",    QString::fromStdString(file.getFileStatus()) },
			{ "Size",      QString::number(file.getFileContent().size()) + " lines" }
		};
	}

	// Joins content vector<string> lines into a single displayable QString
	// Useful for a diff panel or file preview area
	QString contentToQString(const TrackedFile& file) {
		QString result;
		for (const auto& line : file.getFileContent())
			result += QString::fromStdString(line) + "\n";
		return result;
	}

	// Commit
	// Single-line summary for history list
	QString toQString(const Commit& commit) {
		return QString("[%1] %2")
			.arg(QString::fromStdString(commit.getId()).left(7))  // short hash
			.arg(QString::fromStdString(commit.getMessages()));
	}

	// Full row data for QTableWidget columns
	QMap<QString, QString> toDisplayMap(const Commit& commit) {
		return {
			{ "ID",        QString::fromStdString(commit.getId()).left(7) },
			{ "Message",   QString::fromStdString(commit.getMessages())   },
			{ "Date",      QString::fromStdString(commit.getDate())       }
		};
	}

	// Repository
	// Uses the already-built getCommitHistory() to vector<string>
	QStringList historyToQStringList(const Repository& repo) {
		return toQStringList(repo.getCommitHistory());
	}

	// Analytics
	// Usefule for computeTotalCommits()
	QMap<QString, QString> analyticsToDisplayMap(int totalCommits, int trackedFiles) {
		return {
			{ "Total Commits",   QString::number(totalCommits)  },
			{ "Tracked Files",   QString::number(trackedFiles)  }
		};
	}

} // namespace Converter