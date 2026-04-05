// Header for the defined converter
// This file converts data passed from the backend into QT version for easier display

// Author: Samuel Horner
#pragma once

#include <QString>
#include <QStringList>
#include <QList>
#include <QMap>
#include <QVariant>

#include <string>
#include <vector>

// Forward declare backend types
// This prevents double header inclusion
class TrackedFile;
class Commit;
class Repository;

namespace Converter {

	// inlne simply copy pastes the function in place rather than calling the original function
	inline QString toQString(const std::string& s) {
		return QString::fromStdString(s);
	}

	inline QString toQString(int n) {
		return QString::number(n);
	}

	// Pass in precision if you want, otherwise default is 2 decimal places
	inline QString toQString(double d, int precision = 2) {
		return QString::number(d, 'f', precision);
	}

	inline QString toQString(bool b) {
		return b ? QStringLiteral("Yes") : QStringLiteral("No");
	}

	// vector<string> to QStringList
	inline QStringList toQStringList(const std::vector<std::string>& vec) {
		QStringList result;
		// pre-allocates memory for the full size upfront so the list doesn't have to keep resizing itself as items are added
		result.reserve(static_cast<int>(vec.size()));
		for (const auto& s : vec)
			result << QString::fromStdString(s);
		return result;
	}

	// TrackedFile
	QString           toQString(const TrackedFile& file);
	QMap<QString, QString> toDisplayMap(const TrackedFile& file);
	QString           contentToQString(const TrackedFile& file);

	// Commit
	QString           toQString(const Commit& commit);
	QMap<QString, QString> toDisplayMap(const Commit& commit);

	// Repository commit history
	QStringList       historyToQStringList(const Repository& repo);

	// Analytics results to display-ready strings
	QMap<QString, QString> analyticsToDisplayMap(int totalCommits, int trackedFiles);

} // namespace Converter