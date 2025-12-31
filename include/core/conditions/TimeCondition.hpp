// include/core/conditions/TimeCondition.hpp

#pragma once

#include "Condition.hpp"

#include <chrono>
#include <string_view>
#include <cstdio>

enum class TimeComp {
    BEFORE,
    AFTER
};

class TimeCondition : public Condition {
    public:
        TimeCondition(std::chrono::system_clock::time_point tp, TimeComp timeComp);

        bool check (const FileInfo&) const override;

        const std::chrono::system_clock::time_point tp() const;
        const TimeComp timeComparison() const;

    private:
        std::chrono::system_clock::time_point tp_;
        TimeComp timeComp_;
};