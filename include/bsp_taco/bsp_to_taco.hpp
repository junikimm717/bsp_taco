#include <binsparse/tensor.h>
#include <binsparse/read_tensor.h>
#include <taco.h>

#pragma once

namespace bsp_taco {
/*
Creates a taco object from a bsp tensor.
Note that this function **consumes** the bsp tensor object!
*/
taco::TensorBase makeTacoTensor(bsp_tensor_t& tensor);

/*
Creates a taco object from a binsparse file.
*/
taco::TensorBase readBinSparse(std::string filename);

/*
Creates a taco object from a binsparse file.
*/
taco::TensorBase readBinSparse(char* filename);
}
