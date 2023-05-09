#include "Disassemble.hpp"
#include "Instructions.hpp"


namespace Garand {

std::string disassemble(GarandInstruction ins) {
    using fmt::format;
    auto raw = ins.InstructionSpecific;
    auto opcode = Instruction::DecodeMnemonic(ins);
    using namespace Parameter;
    switch (opcode) {
    case MREAD:
    case MWRITE:
    case ADD:
    case SUB:
    case MUL:
    case MADD:
    case DIV:
    case AND:
    case NAND:
    case OR:
    case XOR:
    case LSL:
    case LSR:
    case RSR:
        return std::apply(
            [&](auto &&...args) {
                return format("{} {}, {}, {}", get_ins_mnemonic(ins), args...);
            },
            Reg3(raw));
    case ADDI:
    case SUBI:
    case MULI:
    case DIVI:
    case ANDI:
    case NANDI:
    case ORI:
    case XORI:
    case LSLI:
    case LSRI:
    case RSRI:
        return std::apply(
            [&](auto &&...args) {
                return format("{} {}, {}, {}", get_ins_mnemonic(ins), args...);
            },
            Reg2Imm1(raw));
    case BRUHCC_AL:
    case BRUHCC_EQ:
    case BRUHCC_NE:
    case BRUHCC_LO:
    case BRUHCC_HS:
    case BRUHCC_LT:
    case BRUHCC_GE:
    case BRUHCC_HI:
    case BRUHCC_LS:
    case BRUHCC_GT:
    case BRUHCC_LE:
    case BRUHCC_VC:
    case BRUHCC_VS:
    case BRUHCC_PL:
    case BRUHCC_NG:
    case UNBIND:
        return std::apply(
            [&](auto &&...args) {
                return format("{} {}", get_ins_mnemonic(ins), args...);
            },
            Reg1(raw));
    case BCC_AL:
    case BCC_EQ:
    case BCC_NE:
    case BCC_LO:
    case BCC_HS:
    case BCC_LT:
    case BCC_GE:
    case BCC_HI:
    case BCC_LS:
    case BCC_GT:
    case BCC_LE:
    case BCC_VC:
    case BCC_VS:
    case BCC_PL:
    case BCC_NG:
    case CALL:
        return std::apply(
            [&](auto &&...args) {
                return format("{} {}", get_ins_mnemonic(ins), args...);
            },
            Add1(raw));
    case NOT:
    case CMP:
    case TEST:
        return std::apply(
            [&](auto &&...args) {
                return format("{} {}, {}", get_ins_mnemonic(ins), args...);
            },
            Reg2(raw));
    case BIND:
    case CMPI:
        return std::apply(
            [&](auto &&...args) {
                return format("{} {}, {}", get_ins_mnemonic(ins), args...);
            },
            Reg1Imm1(raw));
    case RETURN:
        return format("{}", get_ins_mnemonic(ins));
    case FX_ADD:
    case FX_ADDI:
    case FX_SUB:
    case FX_SUBI:
    case FX_MUL:
    case FX_MULI:
    case FX_MADD:
    case FX_DIV:
    case FX_DIVI:
    default:
        return format("<Unimplemented>");
    }
}
} // namespace Garand
