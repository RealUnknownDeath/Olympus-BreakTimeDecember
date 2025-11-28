#pragma once
#include <vector>
#include <cstddef>

struct SavedModel;

// Reads a text .usd/.txt model from SD card.
bool load_model(const char* filename, SavedModel& m);
