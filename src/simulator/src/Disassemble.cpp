#include <fmt/format.h>

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

template <> struct fmt::formatter<Garand::Parameter::Reg> {
    constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin()) {
        return ctx.end();
    }
    template <typename FormatContext>
    auto format(const Garand::Parameter::Reg &instance, FormatContext &ctx) const
        -> decltype(ctx.out()) {
        auto &n = instance.N;
        switch (n) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
            return fmt::format_to(ctx.out(), "R{}", n);
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
        case 30:
        case 31:
            return fmt::format_to(ctx.out(), "I{}", n);
        case 32:
            return fmt::format_to(ctx.out(), "SP");
        case 33:
            return fmt::format_to(ctx.out(), "PC");
        case 34:
            return fmt::format_to(ctx.out(), "LR");
        default:
            return fmt::format_to(ctx.out(), "R?");
        }
    }
};

template <> struct fmt::formatter<Garand::Parameter::Imm> {
    constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin()) {
        return ctx.end();
    }
    template <typename FormatContext>
    auto format(const Garand::Parameter::Imm &instance, FormatContext &ctx) const
        -> decltype(ctx.out()) {
        return fmt::format_to(ctx.out(), "#0x{:0x}", instance.N);
    }
};