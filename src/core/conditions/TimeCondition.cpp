// src/core/conditions/TimeCondition.cpp

#include "core/conditions/TimeCondition.hpp"

TimeCondition::TimeCondition(std::chrono::system_clock::time_point tp, TimeComp timeComp) 
    : tp_(tp), timeComp_(timeComp) {}

bool TimeCondition::check(const FileInfo& file) const {
    if (file.last_modified.has_value()) {
        auto file_tp = std::chrono::clock_cast<std::chrono::system_clock>(file.last_modified.value());
        if (tp_ > file_tp) {
            return timeComp_ == TimeComp::BEFORE;
        } else {
            return timeComp_ == TimeComp::AFTER;
        }
    }   
    return false;
}

const std::chrono::system_clock::time_point TimeCondition::tp() const { return tp_; }
const TimeComp TimeCondition::timeComparison() const { return timeComp_; }
