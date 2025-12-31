// include/core/conditions/SizeCondition.hpp

#pragma once

#include <cstdint>
#include "Condition.hpp"

enum class Comparison {
    GREATER,
    LESS
};

class SizeCondition : public Condition {
    public:
        SizeCondition(std::uintmax_t size, Comparison comp);

        bool check (const FileInfo&) const override;

        const std::uintmax_t size() const;
        const Comparison comparison() const;

    private:
        std::uintmax_t size_;
        Comparison comp_;
};