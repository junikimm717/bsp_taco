#include <binsparse/tensor.h>
#include <binsparse/read_tensor.h>
#include <taco.h>

taco::TensorBase makeTacoTensor(bsp_tensor_t& tensor);

taco::TensorBase readBinSparse(std::string filename);
