#include "Registers.hpp"

uint64_t* Garand::load_reg(uint64_t registers, uint8_t reg_index) {
    return (uint64_t*) (registers + (reg_index * sizeof(uint64_t)));
}