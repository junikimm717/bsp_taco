#include <iostream>
#include <taco.h>

using namespace taco;
using namespace std;

int main() {
  int dim1 = 5, dim2 = 5;

  // Define a 2D tensor in COO format

  // Seems like this format was fucked up :sob:
  // Format coo({Compressed, Singleton});

  /*
  order is 2, isUnique is false, isOrdered is true, isAOS is false.
  It seems like modeOrdering basically determines the order.

  Ok, so for COO, you effectively need to figure out how to load
  layers in this kind of format, as seen below:
  */

  // Format fmt({Dense({ModeFormat::ORDERED, ModeFormat::UNIQUE}),
  //             Compressed({ModeFormat::ORDERED, ModeFormat::UNIQUE})});
  // Format fmt = COO(2, false, true, false);

  Format fmt({Compressed({ModeFormat::ORDERED, ModeFormat::NOT_UNIQUE}),
              Singleton({ModeFormat::ORDERED, ModeFormat::NOT_UNIQUE})});
  Tensor<double> A({4, 2}, fmt);

  // TODO: generate with finch, use experimental parse_taco.cpp and check the
  // tensor below is the same.

  // Insert non-zero values with explicit coordinates
  A.insert({0, 1}, 1.0);
  A.insert({1, 0}, 1.0);
  A.insert({3, 0}, 3.0);
  A.insert({3, 1}, 4.0);

  A.pack();

  auto index = A.getStorage().getIndex();

  cout << "elements " << A.getAllocSize() << "\n";

  cout << "dimension:" << A.getDimensions().size() << "\n";
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
  for (int k = 0; k < 4; k++) {
    for (int i = 0; i < 2; i++) {
      cout << A(k, i) << " ";
    }
    cout << "\n\n";
  }
  cout << "===================\n";

  cout << "elements:\n";
  auto elements = A.getStorage().getValues();
  for (int i = 0; i < elements.getSize(); i++) {
    cout << elements.get(i).get().float64Value << " ";
  }
  cout << "\n";
  cout << "data: \n";
  auto data = (double*) elements.getData();
  for (int i = 0; i < elements.getSize(); i++) {
    cout << data[i] << " ";
  }
  cout << "\n";
  cout << endl;
  return 0;
}
