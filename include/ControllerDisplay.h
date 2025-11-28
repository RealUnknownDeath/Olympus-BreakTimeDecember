#pragma once
#include "vex.h"
#include <functional>
#include <atomic>
#include <vector>
#include <string>
#include <cstdarg>

class ControllerDisplay {
private:
    vex::task displayTask;
    std::atomic<bool> enabled{false};
    std::function<void()> displayFunction;

    //Scroll line helper:
    std::vector<std::string> lines;
    std::atomic<int> scrollOffset{0};
    std::atomic<int> maxScroll{0};

    static int loop(void* instance) {
      auto* self = static_cast<ControllerDisplay*>(instance);
      while (true) {
          if (self->enabled && self->displayFunction) {
              // collect new lines
              self->lines.clear();
              self->displayFunction();

              // clamp scroll
              int off = self->scrollOffset.load();
              int mx  = self->maxScroll.load();
              if (off < 0) off = 0;
              if (off > mx) off = mx;
              self->scrollOffset.store(off);

              Controller1.Screen.clearScreen();

              int visible = 3; // controller fits ~3 comfortably
              for (int i = 0; i < visible; i++) {
                  int idx = self->scrollOffset.load() + i;
                  if (idx >= (int)self->lines.size()) break;
                  Controller1.Screen.setCursor(i + 1, 1);
                  Controller1.Screen.print("%s", self->lines[idx].c_str());
              }

              // update maxScroll
              self->maxScroll.store(
                  (int)std::max(0, (int)self->lines.size() - visible)
              );
          }
          vex::this_thread::sleep_for(250);
      }
      return 0;
  }


public:
    ControllerDisplay() {}

    void start(std::function<void()> func) {
        displayFunction = func;
        enabled = true;
        displayTask = vex::task(loop, this);
    }

    void toggle() { enabled = !enabled; }
    void stop()   { enabled = false; }
    bool isEnabled() const { return enabled; }

    // --- Scrolling ---
    void scrollDown() {
        if (scrollOffset < maxScroll) scrollOffset++;
    }
    void scrollUp() {
        if (scrollOffset > 0) scrollOffset--;
    }
    void resetScroll() { scrollOffset = 0; }

    // --- Add lines inside displayFunction ---
    void addLine(const std::string& text) {
        lines.push_back(text);
    }

    void addFormattedLine(const char* fmt, ...) {
        char buf[64];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, sizeof(buf), fmt, args);
        va_end(args);
        lines.push_back(std::string(buf));
    }
};
