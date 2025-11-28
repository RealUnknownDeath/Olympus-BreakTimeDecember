#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstdio>
#include <cstdint>

// Simple, VEX-friendly CSV logger that writes to the SD card (/usd).
// - Call open("/usd/mydata.csv", {"t_ms","x","y","heading"}) once
// - Call append_row({t, x, y, h}) in your loop
// - Optionally use buffered mode with set_buffered_flush_ms(50)
//
// Notes:
// * Make sure a microSD card is inserted in the V5 brain.
// * CSV header is written automatically if the file did not exist or was empty.

class CsvLogger {
public:
  CsvLogger() : precision_(6), buffered_flush_ms_(0), last_flush_ms_(0) {}

  // Open file; if file is new/empty, writes header.
  // Pass absolute path like "/usd/log.csv"
  bool open(const std::string& path, const std::vector<std::string>& header) {
    path_ = path;
    header_ = header;

    // Detect if file exists and non-empty
    bool need_header = true;
    {
      std::ifstream test(path_, std::ios::in | std::ios::binary);
      need_header = !test.good() || test.peek() == std::ifstream::traits_type::eof();
    }

    out_.open(path_, std::ios::out | std::ios::app);
    if (!out_.good()) return false;

    if (need_header && !header_.empty()) {
      write_line_string(join(header_, ","));
    }
    return true;
  }

  // Close file explicitly (also called by destructor).
  void close() {
    if (out_.is_open()) {
      out_.flush();
      out_.close();
    }
  }

  ~CsvLogger() { close(); }

  // Set numeric precision for floating-point values (default 6)
  void set_precision(int p) { precision_ = p; }

  // Set buffered flush interval in milliseconds (0 = flush every write)
  void set_buffered_flush_ms(uint32_t ms) { buffered_flush_ms_ = ms; }

  // Append a row of numeric values.
  // Example: append_row({t_ms, x, y, heading});
  template<typename T>
  bool append_row(const std::vector<T>& values, uint32_t now_ms = 0) {
    if (!out_.good()) return false;

    std::ostringstream ss;
    ss.setf(std::ios::fixed);             // fixed for consistent decimals
    ss << std::setprecision(precision_);

    for (size_t i = 0; i < values.size(); ++i) {
      ss << values[i];
      if (i + 1 < values.size()) ss << ',';
    }
    return write_line_string(ss.str(), now_ms);
  }

  // Append a row of strings (already formatted).
  bool append_row(const std::vector<std::string>& cells, uint32_t now_ms = 0) {
    return write_line_string(join(cells, ","), now_ms);
  }

  // Utility: escape a cell if it may contain commas/newlines/quotes.
  static std::string csv_escape(const std::string& s) {
    bool need_quotes = s.find_first_of(",\"\n\r") != std::string::npos;
    if (!need_quotes) return s;
    std::string out; out.reserve(s.size() + 2);
    out.push_back('"');
    for (char c : s) {
      if (c == '"') out.push_back('"'); // double the quote
      out.push_back(c);
    }
    out.push_back('"');
    return out;
  }

private:
  std::string path_;
  std::vector<std::string> header_;
  std::ofstream out_;
  int precision_;
  uint32_t buffered_flush_ms_;
  uint32_t last_flush_ms_;

  static std::string join(const std::vector<std::string>& parts, const char* sep) {
    std::ostringstream ss;
    for (size_t i=0;i<parts.size();++i) {
      ss << parts[i];
      if (i+1<parts.size()) ss << sep;
    }
    return ss.str();
  }

  bool write_line_string(const std::string& line, uint32_t now_ms = 0) {
    if (!out_.good()) return false;
    out_ << line << "\n";
    if (buffered_flush_ms_ == 0) {
      out_.flush();
    } else if (now_ms == 0) {
      // If caller didn't pass time, flush every line as a fallback
      out_.flush();
    } else if ((now_ms - last_flush_ms_) >= buffered_flush_ms_) {
      out_.flush();
      last_flush_ms_ = now_ms;
    }
    return out_.good();
  }
};
