// src/core/actions/RenameAction.cpp

#include "core/actions/RenameAction.hpp"

RenameAction::RenameAction(std::string pattern, int rule_file_count) : Action(), pattern_(pattern), count(rule_file_count) {
    prepare_format();
}

ActionType RenameAction::type() const { return ActionType::RENAME; }

const std::string RenameAction::describe() const {
    return "Renaming [FILE] using pattern: " + pattern_;
}

const std::string RenameAction::describe(const FileInfo& file) const {
    return "Renaming " + file.filename() + " to: " + createNewName(file);
}

void RenameAction::execute(FileInfo& file) const {

    const std::string name = createNewName(file);
    const std::filesystem::path new_path = file.path.parent_path() / name;

    std::filesystem::rename(
        file.path,
        new_path
    );

    file.path = new_path;
}

const std::string RenameAction::createNewName(const FileInfo& file) const {
    const auto utf8 = file.path.stem().u8string();
    const std::string stem = std::string(utf8.begin(), utf8.end());
    std::string file_date; 
    if (file.last_modified.has_value()) {
        file_date = fmt::format("{:%Y-%m-%d-%H-%M-%S}", 
            std::chrono::clock_cast<std::chrono::system_clock>(file.last_modified.value()));
    }
    std::string new_name = fmt::format(fmt::runtime(fmt_format_), stem, file_date, count);
    new_name += file.extension();
    return new_name;
}

void RenameAction::prepare_format() {
    std::string pattern = pattern_;

    // Replace {name} with {0} for fmt positional args
    size_t namePos = pattern.find("{name}");
    if (namePos != std::string::npos) {
        pattern.replace(namePos, 6, "{0}");
    }

    // Replace {date} with {1} for fmt positional args
    size_t datePos = pattern.find("{date}");
    if (datePos != std::string::npos) {
        pattern.replace(datePos, 6, "{1}");
    }

    // Replace {count} with {2} for fmt positional args
    size_t countPos = pattern.find("{count}");
    if (countPos != std::string::npos) {
        pattern.replace(countPos, 7, "{2:03}");
    }

    fmt_format_ = pattern;
}

std::string RenameAction::pattern() const { return pattern_; }
