// include/core/actions/MoveAction.hpp

#pragma once

#include "Action.hpp"


class MoveAction: public Action {
    public: 
        MoveAction(std::filesystem::path destination);

        ActionType type() const override;
        const std::string describe() const override;
        const std::string describe(const FileInfo& file) const override;
        void execute(const FileInfo& file) const override;

        std::filesystem::path destination() const;

    private:
        std::filesystem::path destination_;

};