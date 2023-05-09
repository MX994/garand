#include <string>
#include <tuple>

#ifndef GARAND_DISASSEMBLE_HPP
#define GARAND_DISASSEMBLE_HPP
#include "Instruction.hpp"
#include "Memory.hpp"

namespace Garand {

namespace Parameter {
    
struct Reg {
    uint8_t N;
    Reg(uint8_t N) : N(N) {}
};

struct Imm {
    uint32_t N;
    Imm(uint32_t N) : N(N) {}
};

static auto Reg3(InstructionSize raw) {
    auto parameter = raw & 0x3fffff;
    Reg dest = (parameter >> 14) & 0x3f;
    Reg par1 = (parameter >> 8) & 0x3f;
    Reg par2 = (parameter >> 2) & 0x3f;
    return std::make_tuple(dest, par1, par2);
}

static auto Reg2Imm1(InstructionSize raw) {
    auto parameter = raw & 0x3fffff;
    Reg dest = (parameter >> 14) & 0x3f;
    Reg par1 = (parameter >> 8) & 0x3f;
    Imm imm = parameter & 0xff;
    return std::make_tuple(dest, par1, imm);
}

static auto Imm1(InstructionSize raw) {
    auto parameter = raw & 0xff;
    return std::make_tuple(Imm{parameter});
}

static auto Add1(InstructionSize raw) {
    auto parameter = raw & 0x3fffff;
    return std::make_tuple(Imm{parameter});
}

static auto Reg1(InstructionSize raw) {
    auto parameter = raw & 0x3fffff;
    Reg dest = (parameter >> 14) & 0x3f;
    return std::make_tuple(dest);
}

static auto Reg2(InstructionSize raw) {
    auto parameter = raw & 0x3fffff;
    Reg par1 = (parameter >> 14) & 0x3f;
    Reg par2 = (parameter >> 8) & 0x3f;
    return std::make_tuple(par1, par2);
}

static auto Reg1Imm1(InstructionSize raw) {
    auto parameter = raw & 0x3fffff;
    Reg par1 = (parameter >> 14) & 0x3f;
    Imm imm = parameter & 0xfff;
    return std::make_tuple(par1, imm);
}

}

std::string disassemble(GarandInstruction ins);
} // namespace Garand

#endif