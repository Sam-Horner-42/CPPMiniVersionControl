# C++ Mini Version Control System (`CPPMiniVersionControl`)

> **A lightweight, cross-platform Git-like version control engine and desktop GUI implemented in C++ using the Qt framework.**

## 🌟 Overview

`CPPMiniVersionControl` is a lightweight version control system and desktop application built in C++ with Qt. It models core version control operations like repository initialization, file staging, cryptographic snapshotting, line-by-line diffing, and commit history tracking through an interactive graphical interface.

## ✨ Core Features

* **Repository Initialization & Lifecycle**: Create and manage local version control tracking structures and index metadata.
* **Cryptographic File Hashing**: Hash utilities to detect changes, verify file integrity, and build snapshot trees.
* **Line-by-Line Diff Engine**: Compare working directory changes against repository snapshots with visual diffing.
* **Commit History & Revision Log**: Track sequential commits with author tags, timestamps, unique identifiers, and commit messages.
* **Qt-Powered Desktop UI**: Graphical interface built with Qt Widgets providing intuitive repository navigation, staging controls, and diff inspection views.

## 🏗 System Architecture

The project splits functionality into a decoupled C++ version control core engine and a Qt graphical user interface:

```
┌────────────────────────────────────────────────────────┐
│                   Qt GUI Application                   │
│    (Repository View, Commit Log, Diff Inspector UI)   │
└───────────────────────────┬────────────────────────────┘
                            │
                            ▼
┌────────────────────────────────────────────────────────┐
│             Version Control Core Engine                │
│ ┌───────────────────┐ ┌───────────────┐ ┌────────────┐ │
│ │ Repository Manager│ │ Hash Engine   │ │ Diff Engine│ │
│ └───────────────────┘ └───────────────┘ └────────────┘ │
└───────────────────────────┬────────────────────────────┘
                            │
                            ▼
┌────────────────────────────────────────────────────────┐
│             Local Storage & Index Tracking             │
│   (Object database, commit logs, HEAD & index state)   │
└───────────────────────────┴────────────────────────────┘
```

## 📂 Repository Structure

```
CPPMiniVersionControl/
├── src/                      # C++ source files
│   ├── main.cpp              # Entry point initializing Qt Application loop
│   ├── mainwindow.cpp        # UI controller and event binding
│   ├── repository.cpp        # Core VCS operations (init, stage, commit)
│   ├── diffengine.cpp        # File comparison and diff parsing logic
│   └── hashutils.cpp         # Hashing & file integrity helpers
├── include/                  # C++ header declarations
│   ├── mainwindow.h          # Main window class definition
│   ├── repository.h          # VCS core engine definitions
│   ├── diffengine.h          # Diff engine interfaces
│   └── hashutils.h           # Utility declarations
├── ui/                       # Qt Designer forms
│   └── mainwindow.ui         # Main UI layout
├── CPPMiniVersionControl.pro # Qt qmake project definition
├── CMakeLists.txt            # CMake build configuration
└── README.md                 # Project documentation
```

## ⚙️ Building & Execution

### Prerequisites

* **C++ Compiler**: GCC 9+, Clang 10+, or MSVC 2019+ with C++17 support
* **Qt Framework**: Qt 5.15+ or Qt 6.x (`Qt6Widgets` / `Qt5Widgets`)
* **Build Tools**: CMake (v3.16+) or `qmake`

### Option 1: Building with CMake

```bash
# Clone the repository
git clone https://github.com/Sam-Horner-42/CPPMiniVersionControl.git
cd CPPMiniVersionControl

# Configure and compile
mkdir build && cd build
cmake ..
cmake --build .
```

### Option 2: Building with Qt Creator / qmake

1. Open `CPPMiniVersionControl.pro` in **Qt Creator**.
2. Select your Desktop Kit (e.g., Qt 6.x GCC / Clang / MSVC).
3. Build the project (`Ctrl + B` or `Cmd + B`).
4. Run the application (`Ctrl + R`).

## 🧪 Key Capabilities & Usage

1. **Initialize a Repository**: Target any local folder to set up internal version tracking.
2. **Stage & Commit**: Stage untracked or modified files and save snapshots with custom log messages.
3. **Inspect Differences**: Compare modified files against previous commits using the built-in diff viewer.
4. **Browse History**: Review linear commit logs with commit IDs, timestamps, and revision details.

## 📜 License

Distributed under the MIT License. See `LICENSE` for details.
