#include <bsp_taco.hpp>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

int main(int argc, char** argv) {
  if (argc < 3) {
    fprintf(stderr, "usage: ./frostt_tns_bsp [input_file.tns] [file_name.h5]\n");
    return 1;
  }
  char* input_file = argv[1];
  char* output_file = argv[2];

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

  cerr << "Reading from " << input_file << "\n";
  cerr << "Outputting to " << output_file << "\n";
  cerr << "Using " << dimensions << " dimensions..." << endl;

  vector<taco::ModeFormatPack> modeFormats = {taco::Dense};
  for (int i = 1; i < dimensions; i++) {
    modeFormats.push_back(taco::Sparse);
  }
  taco::Format format(modeFormats);
  cerr << "Beginning reading process" << endl;
  taco::TensorBase A = taco::read(std::string(input_file), format);
  cerr << "Finished reading, beginning writing..." << endl;
  bsp_taco::writeBinSparse(A, output_file);
  return 0;
}
