// include/core/actions/MoveAction.hpp

#pragma once

#include <string>
#include <filesystem>
#include "Action.hpp"


class MoveAction: public Action {
    public: 
        MoveAction(FileInfo file, std::filesystem::path destination);

        ActionType type() const override;
        std::string describe() const override;
        std::filesystem::path destination() const;

    private:
        std::filesystem::path destination_;

};