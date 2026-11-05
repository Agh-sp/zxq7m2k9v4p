#include "aether/core/Settings.h"
#include "aether/core/Log.h"

#include <fstream>

namespace aether {

nlohmann::json Settings::s_Data = nlohmann::json::object();
std::string    Settings::s_Path = "settings.json";

void Settings::Load(const std::string& path) {
    s_Path = path;
    std::ifstream file(path);
    if (!file.is_open()) {
        AE_CORE_WARN("Settings file not found: {}. Using defaults.", path);
        // Populate defaults
        s_Data = nlohmann::json::object();
        s_Data["window"] = {
            {"width", 1280},
            {"height", 720},
            {"fullscreen", false},
            {"vsync", true}
        };
        s_Data["input"] = {
            {"mouseSensitivity", 1.0}
        };
        Save(path);
        return;
    }

    try {
        file >> s_Data;
        AE_CORE_INFO("Settings loaded from {}", path);
    } catch (const std::exception& e) {
        AE_CORE_ERROR("Failed to parse settings: {}", e.what());
        s_Data = nlohmann::json::object();
    }
}

void Settings::Save(const std::string& path) {
    const std::string target = path.empty() ? s_Path : path;
    std::ofstream file(target);
    if (!file.is_open()) {
        AE_CORE_ERROR("Failed to open settings file for writing: {}", target);
        return;
    }
    file << s_Data.dump(4);
    AE_CORE_INFO("Settings saved to {}", target);
}

nlohmann::json& Settings::Get() { return s_Data; }

void Settings::Set(const std::string& key, const nlohmann::json& value) {
    s_Data[key] = value;
}

bool Settings::Has(const std::string& key) {
    return s_Data.contains(key);
}

void Settings::Reset() {
    s_Data = nlohmann::json::object();
}

} // namespace aether
