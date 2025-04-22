#pragma once

#include <cstring>
#include <taco.h>

using namespace std;

enum benchmark_format_t { CSF = 0, COO = 1 };

benchmark_format_t getModeFormat(char* fmt = NULL) {
  if (fmt != NULL && strcmp(fmt, "coo") == 0) {
    return COO;
  }
  return CSF;
}

const string getFormatName(benchmark_format_t t) {
  switch (t) {
  case CSF:
    return "csf";
  case COO:
    return "coo";
  }
}

vector<taco::ModeFormatPack> getModeFormatPack(benchmark_format_t t,
                                               int dimensions) {
  switch (t) {
  case CSF: {
    vector<taco::ModeFormatPack> modeFormats = {taco::Dense};
    for (int i = 1; i < dimensions; i++) {
      modeFormats.push_back(taco::Sparse);
    }
    return modeFormats;
  }
  case COO: {
    vector<taco::ModeFormatPack> modeFormats;
    modeFormats.push_back(taco::Compressed(
        {taco::ModeFormat::ORDERED, taco::ModeFormat::NOT_UNIQUE}));
    for (int i = 1; i < dimensions; i++) {
      modeFormats.push_back(taco::Singleton(
          {taco::ModeFormat::ORDERED, taco::ModeFormat::NOT_UNIQUE}));
    }
    return modeFormats;
  }
  }
}
