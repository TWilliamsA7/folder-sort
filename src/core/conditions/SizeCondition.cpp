// src/core/conditions/SizeCondition.cpp

#include "core/conditions/SizeCondition.hpp"

SizeCondition::SizeCondition(std::uintmax_t size, Comparison comp)
 : size_(size), comp_(comp) {}

bool SizeCondition::check(const FileInfo& file) const {

    if (file.size.has_value()) {
        if (file.size.value() > size_) {
            return comp_ == Comparison::GREATER;
        } else {
            return comp_ == Comparison::LESS;
        }
    }

    return false;

}

const std::uintmax_t SizeCondition::size() const { return size_; }
const Comparison SizeCondition::comparison() const { return comp_; }