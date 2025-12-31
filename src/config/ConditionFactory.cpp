// src/config/ConditionFactory.cpp

#include "config/ConditionFactory.hpp"

std::vector<std::unique_ptr<Condition>> ConditionFactory::build(const YAML::Node& node) {

    std::vector<std::unique_ptr<Condition>> conditions;

    if (node["extension"]) {
        conditions.push_back(buildExtensionCondition(node));
    }

    if (node["size"]) {
        conditions.push_back(buildSizeCondition(node));
    }

    if (node["last-modified"]) {
        conditions.push_back(buildTimeCondition(node));
    }

    return conditions;
}

std::unique_ptr<ExtensionCondition> ConditionFactory::buildExtensionCondition(const YAML::Node& node) {
    if (node["extension"].IsSequence()) {
        return std::make_unique<ExtensionCondition>(
            node["extension"].as<std::vector<std::string>>()
        );
    } else {
        return std::make_unique<ExtensionCondition>(
            std::vector<std::string>{ node["extension"].as<std::string>() }
        );
    }
}

std::unique_ptr<SizeCondition> ConditionFactory::buildSizeCondition(const YAML::Node& node) {
    std::string size_string = node["size"].as<std::string>();
    Comparison comp = size_string.find(">") != std::string::npos ? Comparison::GREATER : Comparison::LESS;
    char sign = comp == Comparison::GREATER ? '>' : '<';
    int pos = size_string.find(sign);

    std::uintmax_t val = pos == std::string::npos ? getSize(size_string) : getSize(size_string.substr(pos + 1));
    return std::make_unique<SizeCondition>(val, comp);
}

std::unique_ptr<TimeCondition> ConditionFactory::buildTimeCondition(const YAML::Node& node) {
    std::string time_string = node["last-modified"].as<std::string>();
    TimeComp comp = time_string.find(">") != std::string::npos ? TimeComp::AFTER : TimeComp::BEFORE;
    char sign = comp == TimeComp::AFTER ? '>' : '<';
    int pos = time_string.find(sign);

    std::chrono::system_clock::time_point val = pos == std::string::npos ? getTimePoint(time_string) : getTimePoint(time_string.substr(pos + 1));
    return std::make_unique<TimeCondition>(val, comp);
}

std::uintmax_t ConditionFactory::getSize(std::string_view inp) {
    inp.remove_prefix(std::min(inp.find_first_not_of(" "), inp.size()));
    inp.remove_suffix(inp.size() - (inp.find_last_not_of(" ") + 1));

    if (inp.empty()) throw std::invalid_argument("Empty size string provided!");

    size_t suffix_start = inp.find_first_not_of("0123456789.");
    std::string_view num = inp.substr(0, suffix_start);
    std::string_view suffix = inp.substr(num.size());

    double value;
    auto [ptr, ec] = std::from_chars(num.data(), num.data() + num.size(), value);

    if (ec != std::errc{} || ptr != num.data() + num.size()) {
        throw std::runtime_error(fmt::format("Invalid Numeric Part: {}", num));
    }

    uint64_t mult = 1;

    std::string unit {suffix};
    for (auto& c : unit) c = toupper(c);

    static const std::map<std::string_view, uint64_t> units = {
        {"B", 1ULL},
        {"KB", 1000ULL}, {"K", 1024ULL}, {"KIB", 1024ULL},
        {"MB", 1000ULL * 1000}, {"M", 1024ULL * 1024}, {"MIB", 1024ULL * 1024},
        {"GB", 1000ULL * 1000 * 1000}, {"G", 1024ULL * 1024 * 1024}, {"GIB", 1024ULL * 1024 * 1024},
        {"TB", 1000ULL * 1000 * 1000 * 1000}, {"T", 1024ULL * 1024 * 1024 * 1024}, {"TIB", 1024ULL * 1024 * 1024 * 1024}
    };

    if (!unit.empty()) {
        if (auto it = units.find(unit); it != units.end()) {
            mult = it->second;
        } else {
            throw std::runtime_error(fmt::format("Unknown unit suffix: '{}'", suffix));
        }
    }

    double final_val = value * mult;
    if (final_val > static_cast<double>(std::numeric_limits<uintmax_t>::max())) {
        throw std::overflow_error(fmt::format("Value '{}' exceeds uintmax_t", inp));
    }

    return static_cast<uintmax_t>(final_val);

}

std::chrono::system_clock::time_point ConditionFactory::getTimePoint(std::string_view inp) {
    int y = 0, m = 1, d = 1, h = 1, min = 0, s = 0;

    int read = std::sscanf(inp.data(), "%d-%d-%dT%d:%d:%d", &y, &m, &d, &h, &min, &s);

    if (read < 1) {
        throw std::runtime_error(fmt::format("Invalid timestamp format: '{}'", inp));
    }

    std::chrono::year_month_day ymd{std::chrono::year{y}, std::chrono::month{m}, std::chrono::day{d}};
    
    if (!ymd.ok()) {
        throw std::runtime_error(fmt::format("Invalid date: {}-{}-{}", y, m, d));
    }

    auto tp = std::chrono::sys_days{ymd} + std::chrono::hours{h} + std::chrono::minutes{min} + std::chrono::seconds{s};
    return tp;
}