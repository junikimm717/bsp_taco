#include <bsp_taco.hpp>

using namespace std;

int main(int argc, char** argv) {
  if (argc < 4) {
    fprintf(stderr, "usage: ./tensor_test [input_file.tns] [file_name.h5] [dimensions]\n");
    return 1;
  }
  char* input_file = argv[1];
  char* output_file = argv[2];
  int dimensions = atoi(argv[3]);
  assert(dimensions > 0);

  vector<taco::ModeFormatPack> modeFormats = {taco::Dense};
  for (int i = 1; i < dimensions; i++) {
    modeFormats.push_back(taco::Sparse);
  }
  taco::Format format(modeFormats);
  taco::TensorBase A = taco::read(input_file, format);
  bsp_taco::writeBinSparse(A, output_file);
  return 0;
}
