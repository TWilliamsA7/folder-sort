# **FolderSort**

---

A rule-based CLI tool for automatically organizing files and directories using a recursive filesystem scanner, YAML-defined rules, and pluggable actions with a verbose logging system.

## Motivation

Manually organizing influxes of files (specifically directories like Downloads) is tedious and can be error-prone. This tool provides a configurable, rule-based system that can be used to automatically categorize and act on the filesystem using simple conditions and actions to implement complex rules. This tool could be set to be executed on a schedule to consistently clean up a target directory.

## Features :star:

- Recursive directory scanning
- Condition matching using:
  - File name RegEx patterns
  - File extensions
  - File size
  - File last write time
  - File contents (text-based, regex matching)
- YAML-based rule configuration
- Actions:
  - Move
  - Copy
  - Rename
  - Delete
- Catch All Action support for unmatched files
- Dry-run support
- Structured logging
- Cross-platform C++ implementation

## How It Works

1. The tool loads a YAML file containing the rules.
2. The tool scans a target directory recursively.
3. Matching the file against the user-defined rules yielding actions to be executed.
4. Executes the actions in order with logging for traceability.

## Installation

### Prebuilt Binary

Download the latest release from the GitHub Releases page and place the executable somewhere on your PATH.

### Build From Source

#### Requirements

- C++20 compatiable compiler
- CMake 3.20+
- Git

#### Build Steps

```bash
git clone https://github.com/TWilliamsA7/folder-sort.git
cd folder-sort
cmake -S . -B build
cmake --build build --config Release
```

## Usage

```bash
# Act on downloads folder using rules.yaml
# Produce a log file in logs directory
folder-sort -r ./Downloads -c ./rules.yaml -l ./logs
# For Additional Help
folder-sort -h
```

### Common Options

- `-r`, `--root`: **Path to Target Search Directory**
- `-c`, `--config`: **Path to YAML rules file**
- `-l`, `--log`: Path to Directory to be used for a log file (Enables Creation)
- `--max-depth`: Define max recursion depth of scanner (otherwise unlimited)

### Common Flags

- `-h`, `--help`: Produce help section for options
- `--dry-run`: Disables execution of actions
- `-v`, `--verbose`: Enable Console Logging
- `-s`, `--symlink`: Enable Following of Symlinks in scan
- `--hidden`: Enable the inclusion of hidden elements in scan
- `-p`, `--permission`: Allow permission errors to occur during directory scan
- `-d`, `--dir`: Enable inclusion of directories as objects in the scan
- `-a`, `--absolute`: Disable normalization of paths

## Rule Configuration

Rules are defined in YAML and are applied from top-to-bottom on each entry scanned. A set of catch actions can be defined at the bottom of the file to act on any unmatched files.

More examples can be found within the repository

Example:

```yaml
rules:
  - name: "Move PDFs"
    when:
      extension: ".pdf"
    then:
      - action: move
        to: pdfs/
catch:
  - action: move
    to: misc/
```

## Actions

| Action | Description                    | YAML property                            |
| ------ | ------------------------------ | ---------------------------------------- |
| move   | Moves a file to a destination  | to: dest                                 |
| copy   | Copies a file to a destination | to: dest                                 |
| rename | Renames a file using a pattern | pattern: custom\_{name}\_{date}\_{count} |
| delete | Delete a file                  | none                                     |

## Conditions

| Condition          | Description                                           | YAML property |
| ------------------ | ----------------------------------------------------- | ------------- |
| extension          | Match files based on their extension                  | extension     |
| size               | Match files based on their size (> or <)              | size          |
| last modified time | Match files based on their last write time            | last-modified |
| name               | Match files using RegEx on their name                 | filename      |
| content            | Match simple text files using RegEx on their contents | contains      |

## Logging & Safety

- The full execution can be logged in both the console and also to a log file
- Dry-run mode is ==recommended== before first use to verify behavior
- Rules are evaluated deterministically

## Project Structure

```text
+---.github ( GitHub Actions )
|   +---workflows
+---include ( Public Headers )
|   +---config
|   +---core
|   |   +---actions
|   |   +---app
|   |   +---conditions
|   |   +---filesystem
|   |   |   +---platform
|   |   +---rules
|   +---logging
+---src ( Core Implementation )
|   +---cli
|   +---config
|   +---core
|   |   +---actions
|   |   +---app
|   |   +---conditions
|   |   +---filesystem
|   |   |   +---platform
|   |   +---rules
|   +---logging
+---tests ( Unit Tests )
    +---config
    +---core
    |   +---app
    |   +---conditions
    |   +---filesystem
    +---flows
    +---helpers
    |   +---posix
    +---logging
```

## Contributing

Issues and pull requests are welcome.
