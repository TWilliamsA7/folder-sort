// include/core/actions/DeleteAction.hpp

#pragma once

#include "Action.hpp"

class DeleteAction: public Action {
    public: 
        DeleteAction();

        ActionType type() const override;
        std::string describe() const override;
        std::string describe(const FileInfo& file) const override;
        void execute(const FileInfo& file) const override;

};