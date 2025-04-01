#include <binsparse/tensor.h>
#include <binsparse/read_tensor.h>
#include <binsparse/write_tensor.h>
#include <bsp_taco.hpp>

using namespace std;

int main(int argc, char** argv) {
  if (argc < 3) {
    fprintf(stderr, "usage: ./tensor_test [file_name.h5] [output_file_name.h5]\n");
    return 1;
  }
  char* file_name = argv[1];
  bsp_tensor_t tensor = bsp_read_tensor(argv[1], NULL);
  taco::TensorBase taco = bsp_taco::makeTacoTensor(tensor);
  tensor = bsp_taco::makeBspTensor(taco);
  {
    bsp_level_t* curLevel = tensor.level;
    while(true) {
      bool breakvar = false;
      switch(curLevel->kind) {
        case BSP_TENSOR_DENSE: {
          cout << "dense layer!" << endl;
          bsp_dense_t* data = (bsp_dense_t*) curLevel->data;
          curLevel = data->child;
          break;
        }
        case BSP_TENSOR_SPARSE: {
          cout << "sparse layer!" << endl;
          bsp_sparse_t* data = (bsp_sparse_t*) curLevel->data;
          curLevel = data->child;
          break;
        }
        case BSP_TENSOR_ELEMENT: {
          breakvar = true;
          break;
        }
      }
      if (breakvar) break;
    }
  }
  bsp_write_tensor(argv[2], tensor, NULL, NULL, 9);
  bsp_destroy_tensor_t(tensor);
  return 0;
}
