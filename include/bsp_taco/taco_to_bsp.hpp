#include "taco/tensor.h"
#include <binsparse/tensor.h>
#include <binsparse/read_tensor.h>
#include <taco.h>

#pragma once

namespace bsp_taco {
bsp_tensor_t makeBspTensor(taco::TensorBase tacoTensor);
}
