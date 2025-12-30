// include/core/actions/RenameAction.hpp

#pragma once

#include <chrono>
#include <spdlog/fmt/fmt.h>
#include <spdlog/fmt/chrono.h>
#include "Action.hpp"

class RenameAction: public Action {
    public: 
        RenameAction(std::string pattern, int rule_file_count);

        ActionType type() const override;
        const std::string describe() const override;
        const std::string describe(const FileInfo& file) const override;
        void execute(const FileInfo& file) const override;
        std::string pattern() const;

    private:
        const std::string createNewName(const FileInfo& file) const;
        void prepare_format(void);
        std::string pattern_;
        std::string fmt_format_;
        int count = 0;

};