#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdint>
#include <cstdlib>

namespace pathio {

// ---------- Models ----------

struct Config {
  double startHeading = 0.0;
  double startX = 0.0;
  double startY = 0.0;
  int timerSeconds = 0;
  int timerX = 0;
  int timerY = 0;
  bool valid = false;
};

enum class ActionType : uint8_t { Waypoint, Turn, Wait, Speed };

struct Action {
  ActionType type;

  // Waypoint
  double x = 0.0;
  double y = 0.0;
  double heading = 0.0;

  // Turn
  int time = 0;
  int speed = 0;

  // Wait
  double duration = 0.0;

  // Speed
  int scale = 100;

  explicit Action(ActionType t) : type(t) {}
};

// ---------- File helpers ----------

inline bool read_text_file(const char* path, std::string& out_text) {
  std::ifstream in(path);
  if (!in.is_open()) return false;
  std::ostringstream ss;
  ss << in.rdbuf();
  out_text = ss.str();
  return true;
}

// ---------- String helpers ----------

// find numeric value after a key, return default if not found
inline double extract_number(const std::string& obj, const std::string& key, double def=0.0) {
  size_t pos = obj.find("\"" + key + "\"");
  if (pos == std::string::npos) return def;
  pos = obj.find(':', pos);
  if (pos == std::string::npos) return def;
  const char* start = obj.c_str() + pos + 1;
  char* endptr;
  double val = std::strtod(start, &endptr);
  return val;
}

inline int extract_int(const std::string& obj, const std::string& key, int def=0) {
  return static_cast<int>(extract_number(obj, key, def));
}

inline std::string extract_string(const std::string& obj, const std::string& key, const std::string& def="") {
  size_t pos = obj.find("\"" + key + "\"");
  if (pos == std::string::npos) return def;
  pos = obj.find(':', pos);
  if (pos == std::string::npos) return def;
  pos = obj.find('"', pos);
  if (pos == std::string::npos) return def;
  size_t end = obj.find('"', pos+1);
  if (end == std::string::npos) return def;
  return obj.substr(pos+1, end-pos-1);
}

// ---------- Top-level loader ----------

inline bool load_path_file(const char* path,
                           Config& cfg_out,
                           std::vector<Action>& actions_out) {
  actions_out.clear();
  cfg_out = Config{};

  std::string text;
  if (!read_text_file(path, text)) {
    return false;
  }

  // crude split into objects between { }
  size_t pos = 0;
  while (true) {
    size_t start = text.find('{', pos);
    if (start == std::string::npos) break;
    size_t end = text.find('}', start);
    if (end == std::string::npos) break;

    std::string obj = text.substr(start, end - start + 1);
    pos = end + 1;

    std::string type = extract_string(obj, "type", "");
    if (type == "config") {
      cfg_out.startHeading = extract_number(obj, "startHeading");
      cfg_out.startX       = extract_number(obj, "startX");
      cfg_out.startY       = extract_number(obj, "startY");
      cfg_out.timerSeconds = extract_int(obj, "timerSeconds");
      cfg_out.timerX       = extract_int(obj, "timerX");
      cfg_out.timerY       = extract_int(obj, "timerY");
      cfg_out.valid = true;
    }
    else if (type == "waypoint") {
      Action a(ActionType::Waypoint);
      a.heading = extract_number(obj, "heading");
      a.x       = extract_number(obj, "x");
      a.y       = extract_number(obj, "y");
      actions_out.push_back(a);
    }
    else if (type == "turn") {
      Action a(ActionType::Turn);
      a.heading = extract_number(obj, "heading");
      a.time    = extract_int(obj, "time");
      a.speed   = extract_int(obj, "speed");
      actions_out.push_back(a);
    }
    else if (type == "wait") {
      Action a(ActionType::Wait);
      a.duration = extract_number(obj, "duration");
      actions_out.push_back(a);
    }
    else if (type == "speed") {
      Action a(ActionType::Speed);
      a.scale = extract_int(obj, "scale");
      actions_out.push_back(a);
    }
    // else: ignore unknown type
  }

  return true;
}

} // namespace pathio
