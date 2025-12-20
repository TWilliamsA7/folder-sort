// include/core/actions/ActionSpec.hpp

#include <string>
#include <unordered_map>

enum class ActionType {
    MOVE,
    RENAME,
    DELETE
};

struct ActionSpec {
    ActionType type;
    std::unordered_map<std::string, std::string> params;
};