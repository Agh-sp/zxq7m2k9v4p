#pragma once

#include <nlohmann/json.hpp>
#include <string>

namespace aether {

class Settings {
public:
    static void Load(const std::string& path = "settings.json");
    static void Save(const std::string& path = "settings.json");

    static nlohmann::json& Get();
    static void Set(const std::string& key, const nlohmann::json& value);

    static bool Has(const std::string& key);
    static void Reset();

private:
    static nlohmann::json s_Data;
    static std::string     s_Path;
};

} // namespace aether
