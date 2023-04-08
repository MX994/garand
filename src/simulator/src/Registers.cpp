#include "Registers.hpp"

// Convert Garand::Register to uint64_t* :
// Garand::Register reg;
// uint64_t* reg_ptr = (uint64_t*)(&reg);
uint64_t* Garand::load_reg(uint64_t* registers, uint8_t reg_index) {
    return (uint64_t*) (registers + (reg_index));
}