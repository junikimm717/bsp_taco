#include <iostream>
#include <bsp_taco.hpp>
using namespace std;

int main(int argc, char** argv) {
  if (argc < 2) {
    fprintf(stderr, "usage: ./parse_taco [file_name.h5]\n");
    return 1;
  }
  auto bsp = bsp_read_tensor(argv[1], NULL);
  taco::Tensor<double> tensor = bsp_taco::makeTacoTensor(bsp);

  auto index = tensor.getStorage().getIndex();
  cout << "dimension:" << tensor.getDimensions().size() << "\n";
  for (int i = 0; i < index.numModeIndices(); i++) {
    cout << "mode index " << i
         << ", indexArrays: " << index.getModeIndex(i).numIndexArrays()
         << "\n====\n";
    for (int j = 0; j < index.getModeIndex(i).numIndexArrays(); j++) {
      auto array = index.getModeIndex(i).getIndexArray(j);
      for (int k = 0; k < array.getSize(); k++) {
        cout << array.get(k).get().int32Value << " ";
      }
      cout << "\n";
    }
    cout << "====\n";
  }

  cout << "===================\n";
  cout << "Extract some values: \n";
  for (int k = 0; k < 10; k++) {
    for (int i = 0; i < 10; i++) {
      cout << tensor(k, i) << " ";
    }
    cout << "\n\n";
  }
  cout << "===================\n";

  auto dims = tensor.getDimensions();
  cout << "dims:\n";
  for (auto &x : dims) std::cout << x << " ";
  cout << "\nvalues:\n";
  auto vals = tensor.getStorage().getValues();
  for (int i = 0; i < vals.getSize(); i++) std::cout << vals.get(i).get().float64Value << " ";
  cout << "\n";
  auto format = tensor.getStorage().getFormat();
  return 0;
}
