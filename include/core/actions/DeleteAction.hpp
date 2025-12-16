// include/core/actions/DeleteAction.hpp

#pragma once

#include <string>
#include <filesystem>
#include "Action.hpp"


class DeleteAction: public Action {
    public: 
        DeleteAction();

        ActionType type() const override;
        std::string describe() const override;

};