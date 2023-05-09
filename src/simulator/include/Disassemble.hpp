#include <string>
#include <tuple>
#include <fmt/format.h>

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

#endif