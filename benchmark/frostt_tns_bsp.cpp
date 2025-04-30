#include <bsp_taco.hpp>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>

#include "formats.hpp"

using namespace std;

int main(int argc, char** argv) {
  if (argc < 4) {
    fprintf(stderr, "usage: ./frostt_tns_bsp [input_file.tns] [file_name.h5] "
                    "[coo|csf] [compression_level]\n");
    return 1;
  }
  char* input_file = argv[1];
  char* output_file = argv[2];
  int compression_level = 1;
  if (argc >= 5) {
    compression_level = std::atoi(argv[4]);
  }

  int dimensions = 0;
  {
    ifstream in(input_file);
    if (!in) {
      cerr << "Can't open file!";
      return 1;
    }
    string firstLine;
    if (getline(in, firstLine)) {
      istringstream iss(firstLine);
      std::string word;
      while (iss >> word) {
        ++dimensions;
      }
      --dimensions;
    }
  }

  assert(dimensions > 0);

  benchmark_format_t benchmarkFmt = getModeFormat(argv[3]);
  auto modeFormatPacks = getModeFormatPack(benchmarkFmt, dimensions);
  taco::Format format(modeFormatPacks);

  cerr << "Reading from " << input_file << "\n";
  cerr << "Outputting to " << output_file << "\n";
  cerr << "Using the " << getFormatName(benchmarkFmt) << " format with "
       << dimensions << " dimensions...\n";
  cerr << "Using compression level " << compression_level << "\n";
  cerr << "Beginning reading process" << endl;

  taco::TensorBase A = taco::read(std::string(input_file), format);
  cerr << "Finished reading, beginning writing..." << endl;
  bsp_taco::writeBinSparse(A, output_file, compression_level);
  return 0;
}
