// include/core/actions/RenameAction.hpp

#pragma once

#include <string>
#include <filesystem>
#include "Action.hpp"


class RenameAction: public Action {
    public: 
        RenameAction(std::string new_name);

        ActionType type() const override;
        std::string describe() const override;
        std::string new_name() const;

    private:
        std::string new_name_;

};