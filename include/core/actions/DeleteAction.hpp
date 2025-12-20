// include/core/actions/DeleteAction.hpp

#pragma once

#include "Action.hpp"

class DeleteAction: public Action {
    public: 
        DeleteAction();

        ActionType type() const override;
        std::string describe() const override;
        std::string describe(FileInfo& file) const override;
        void execute(FileInfo& file) const override;

};