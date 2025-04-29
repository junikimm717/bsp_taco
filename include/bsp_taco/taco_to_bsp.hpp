#include "taco/tensor.h"
#include <binsparse/tensor.h>
#include <binsparse/write_tensor.h>
#include <taco.h>

#pragma once

namespace bsp_taco {
/*
Creates a bsp tensor object from a taco tensor.
Note that this bsp tensor object
*/
bsp_tensor_t makeBspTensor(taco::TensorBase tacoTensor);

/*
Take a bsp tensor object, and write it as binsparse to a file.
*/
void writeBinSparse(taco::TensorBase taco, std::string filename, int compression_level);

void writeBinSparse(taco::TensorBase taco, char* filename, int compression_level);
}
