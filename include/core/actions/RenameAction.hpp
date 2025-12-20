// include/core/actions/RenameAction.hpp

#pragma once

#include "Action.hpp"

class RenameAction: public Action {
    public: 
        RenameAction(std::string new_name);

        ActionType type() const override;
        std::string describe() const override;
        std::string describe(FileInfo& file) const override;
        void execute(FileInfo& file) const override;
        std::string new_name() const;

    private:
        std::string new_name_;

};