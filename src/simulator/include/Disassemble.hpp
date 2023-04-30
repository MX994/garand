#include "Instruction.hpp"
#include "Memory.hpp"
#include <string>

#ifndef GARAND_DISASSEMBLE_HPP
#define GARAND_DISASSEMBLE_HPP

namespace Garand {
std::string disassemble(GarandInstruction ins);
} // namespace Garand

#endif