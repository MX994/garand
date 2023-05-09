#include "Registers.hpp"

namespace Garand {

// Convert Garand::Register to uint64_t* :
// Garand::Register reg;
// uint64_t* reg_ptr = (uint64_t*)(&reg);
RegisterSize *load_reg(Registers *registers, uint8_t reg_index) {
    return ((RegisterSize *)registers + (reg_index));
}

}
