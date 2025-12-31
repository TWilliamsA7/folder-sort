// include/core/actions/CopyAction.hpp

#pragma once

#include "Action.hpp"


class CopyAction: public Action {
    public: 
        CopyAction(std::filesystem::path destination);

        ActionType type() const override;
        const std::string describe() const override;
        const std::string describe(const FileInfo& file) const override;
        void execute(FileInfo& file) const override;

        std::filesystem::path destination() const;

    private:
        std::filesystem::path destination_;

};