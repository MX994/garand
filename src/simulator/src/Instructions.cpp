#include "Instructions.hpp"
#include "Instruction.hpp"
#include "Memory.hpp"

// #include <fmt/format.h>
#include <tuple>

namespace Garand {

Garand::InstructionWriteBack
Garand::InstructionSet::MemoryRead(Garand::GarandInstruction instr,
                                   Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t imm_flag = (instr.InstructionSpecific >> 20) & 0b1;

    // Destination address will be same for all
    uint8_t dest = (instr.InstructionSpecific >> 14) & 0b111111;
    wb.reg = (Garand::load_reg(regs, dest));

    if (!imm_flag) {
        uint8_t src = (instr.InstructionSpecific >> 8) & 0b111111;
        uint8_t offset = (instr.InstructionSpecific >> 2) & 0b111111;

        auto *reg_src = Garand::load_reg(regs, src);
        auto *reg_offset = Garand::load_reg(regs, offset);

        auto Addr = *reg_src + *reg_offset;
        wb.execute_cost = mem.GetCacheCycle(static_cast<AddressSize>(Addr));

        auto *real_addr =
            mem.load<Garand::LoadSize>(static_cast<AddressSize>(Addr));

        wb.value = *real_addr;
    } else {
        uint16_t imm = instr.InstructionSpecific & 0x3FFF;
        wb.execute_cost = mem.GetCacheCycle(imm);
        wb.value = *(mem.load<Garand::LoadSize>(imm));
    }

    return wb;
}

// Example for `MWRITE R15, 50`
// Garand::GarandInstruction instr_b;
// instr_b.Operation = 0x0;
// instr_b.ConditionFlags = 0x1;
// instr_b.InstructionSpecific = 1 << 20 | 15 << 14 | 50;
Garand::InstructionWriteBack
Garand::InstructionSet::MemoryWrite(Garand::GarandInstruction instr,
                                    Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;
    wb.is_reg = false;

    uint8_t imm_flag = (instr.InstructionSpecific >> 20) & 0b1;

    if (!imm_flag) {
        uint8_t src_index = (instr.InstructionSpecific >> 14) & 0b111111;
        uint8_t dest_index = (instr.InstructionSpecific >> 8) & 0b111111;
        uint8_t offset = (instr.InstructionSpecific >> 2) & 0b111111;

        RegisterSize *reg_src = Garand::load_reg(regs, src_index);
        RegisterSize *reg_dest = Garand::load_reg(regs, dest_index);
        RegisterSize *reg_offset = Garand::load_reg(regs, offset);

        wb.reg = reinterpret_cast<RegisterSize *>(*reg_dest + *reg_offset);
        wb.value = *reg_src;
    } else {
        uint8_t dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
        RegisterSize *reg_dest = Garand::load_reg(regs, dest_index);

        uint16_t imm = instr.InstructionSpecific & 0x3FFF;

        wb.reg = reg_dest;
        wb.value = imm;
    }

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::Bind(Garand::GarandInstruction instr,
                             Garand::Memory &mem, Registers *regs) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::Unbind(Garand::GarandInstruction instr,
                               Garand::Memory &mem, Registers *regs) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::BRUHCC_AL(Garand::GarandInstruction instr,
                                  Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    RegisterSize reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers *reg_struct = (Garand::Registers *)regs;

    wb.reg = &reg_struct->ProgramCounter;
    wb.value = reg_dest;

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::BRUHCC_EQ(Garand::GarandInstruction instr,
                                  Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    RegisterSize reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers *reg_struct = (Garand::Registers *)regs;

    if (reg_struct->Condition.Zero == 1) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = reg_dest;
    } else {
        wb.write_back = false;
    }
    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::BRUHCC_NE(Garand::GarandInstruction instr,
                                  Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    RegisterSize reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers *reg_struct = (Garand::Registers *)regs;

    if (reg_struct->Condition.Zero == 0) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = reg_dest;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::BRUHCC_LO(Garand::GarandInstruction instr,
                                  Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    RegisterSize reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers *reg_struct = (Garand::Registers *)regs;

    if (reg_struct->Condition.Carry == 0) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = reg_dest;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::BRUHCC_HS(Garand::GarandInstruction instr,
                                  Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    RegisterSize reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers *reg_struct = (Garand::Registers *)regs;

    if (reg_struct->Condition.Carry == 1) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = reg_dest;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::BRUHCC_LT(Garand::GarandInstruction instr,
                                  Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    RegisterSize reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers *reg_struct = (Garand::Registers *)regs;

    if (reg_struct->Condition.Negative != reg_struct->Condition.Overflow) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = reg_dest;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::BRUHCC_GE(Garand::GarandInstruction instr,
                                  Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    RegisterSize reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers *reg_struct = (Garand::Registers *)regs;

    if (reg_struct->Condition.Negative == reg_struct->Condition.Overflow) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = reg_dest;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::BRUHCC_HI(Garand::GarandInstruction instr,
                                  Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    RegisterSize reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers *reg_struct = (Garand::Registers *)regs;

    if (reg_struct->Condition.Carry == 1 && reg_struct->Condition.Zero == 0) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = reg_dest;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::BRUHCC_LS(Garand::GarandInstruction instr,
                                  Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    RegisterSize reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers *reg_struct = (Garand::Registers *)regs;

    if (!(reg_struct->Condition.Carry == 1 && reg_struct->Condition.Zero)) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = reg_dest;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::BRUHCC_GT(Garand::GarandInstruction instr,
                                  Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    RegisterSize reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers *reg_struct = (Garand::Registers *)regs;

    if (reg_struct->Condition.Zero == 0 &&
        reg_struct->Condition.Negative == reg_struct->Condition.Overflow) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = reg_dest;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::BRUHCC_LE(Garand::GarandInstruction instr,
                                  Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    RegisterSize reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers *reg_struct = (Garand::Registers *)regs;

    if (!(reg_struct->Condition.Zero == 0 &&
          reg_struct->Condition.Negative == reg_struct->Condition.Overflow)) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = reg_dest;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::BRUHCC_VC(Garand::GarandInstruction instr,
                                  Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    RegisterSize reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers *reg_struct = (Garand::Registers *)regs;

    if (reg_struct->Condition.Overflow == 0) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = reg_dest;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::BRUHCC_VS(Garand::GarandInstruction instr,
                                  Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    RegisterSize reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers *reg_struct = (Garand::Registers *)regs;

    if (reg_struct->Condition.Overflow == 1) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = reg_dest;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::BRUHCC_PL(Garand::GarandInstruction instr,
                                  Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    RegisterSize reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers *reg_struct = (Garand::Registers *)regs;

    if (reg_struct->Condition.Negative == 0) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = reg_dest;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::BRUHCC_NG(Garand::GarandInstruction instr,
                                  Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    RegisterSize reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers *reg_struct = (Garand::Registers *)regs;

    if (reg_struct->Condition.Negative == 1) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = reg_dest;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::BCC_AL(Garand::GarandInstruction instr,
                               Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    int8_t offset = instr.InstructionSpecific & 0xff;

    Garand::Registers *reg_struct = (Garand::Registers *)regs;

    wb.reg = &reg_struct->ProgramCounter;
    wb.value = (int64_t)reg_struct->ProgramCounter + offset;

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::BCC_EQ(Garand::GarandInstruction instr,
                               Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    int8_t offset = instr.InstructionSpecific & 0xff;

    Garand::Registers *reg_struct = (Garand::Registers *)regs;

    if (reg_struct->Condition.Zero == 1) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = (int64_t)reg_struct->ProgramCounter + offset;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::BCC_NE(Garand::GarandInstruction instr,
                               Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    int8_t offset = instr.InstructionSpecific & 0xff;

    Garand::Registers *reg_struct = (Garand::Registers *)regs;

    if (reg_struct->Condition.Zero == 0) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = (int64_t)reg_struct->ProgramCounter + offset;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::BCC_LO(Garand::GarandInstruction instr,
                               Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    int8_t offset = instr.InstructionSpecific & 0xff;

    Garand::Registers *reg_struct = (Garand::Registers *)regs;

    if (reg_struct->Condition.Carry == 0) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = (int64_t)reg_struct->ProgramCounter + offset;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::BCC_HS(Garand::GarandInstruction instr,
                               Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    int8_t offset = instr.InstructionSpecific & 0xff;

    Garand::Registers *reg_struct = (Garand::Registers *)regs;

    if (reg_struct->Condition.Carry == 1) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = (int64_t)reg_struct->ProgramCounter + offset;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::BCC_LT(Garand::GarandInstruction instr,
                               Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    int8_t offset = instr.InstructionSpecific & 0xff;

    Garand::Registers *reg_struct = (Garand::Registers *)regs;

    if (reg_struct->Condition.Negative != reg_struct->Condition.Overflow) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = (int64_t)reg_struct->ProgramCounter + offset;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::BCC_GE(Garand::GarandInstruction instr,
                               Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    int8_t offset = instr.InstructionSpecific & 0xff;

    Garand::Registers *reg_struct = (Garand::Registers *)regs;

    if (reg_struct->Condition.Negative == reg_struct->Condition.Overflow) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = (int64_t)reg_struct->ProgramCounter + offset;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::BCC_HI(Garand::GarandInstruction instr,
                               Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    int8_t offset = instr.InstructionSpecific & 0xff;

    Garand::Registers *reg_struct = (Garand::Registers *)regs;

    if (reg_struct->Condition.Carry == 1 && reg_struct->Condition.Zero == 0) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = (int64_t)reg_struct->ProgramCounter + offset;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::BCC_LS(Garand::GarandInstruction instr,
                               Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    int8_t offset = instr.InstructionSpecific & 0xff;

    Garand::Registers *reg_struct = (Garand::Registers *)regs;

    if (!(reg_struct->Condition.Carry == 1 && reg_struct->Condition.Zero)) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = (int64_t)reg_struct->ProgramCounter + offset;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::BCC_GT(Garand::GarandInstruction instr,
                               Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    int8_t offset = instr.InstructionSpecific & 0xff;

    Garand::Registers *reg_struct = (Garand::Registers *)regs;

    if (reg_struct->Condition.Zero == 0 &&
        reg_struct->Condition.Negative == reg_struct->Condition.Overflow) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = (int64_t)reg_struct->ProgramCounter + offset;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::BCC_LE(Garand::GarandInstruction instr,
                               Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    int8_t offset = instr.InstructionSpecific & 0xff;

    Garand::Registers *reg_struct = (Garand::Registers *)regs;

    if (!(reg_struct->Condition.Zero == 0 &&
          reg_struct->Condition.Negative == reg_struct->Condition.Overflow)) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = (int64_t)reg_struct->ProgramCounter + offset;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::BCC_VC(Garand::GarandInstruction instr,
                               Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    int8_t offset = instr.InstructionSpecific & 0xff;

    Garand::Registers *reg_struct = (Garand::Registers *)regs;

    if (reg_struct->Condition.Overflow == 0) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = (int64_t)reg_struct->ProgramCounter + offset;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::BCC_VS(Garand::GarandInstruction instr,
                               Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    int8_t offset = instr.InstructionSpecific & 0xff;

    Garand::Registers *reg_struct = (Garand::Registers *)regs;

    if (reg_struct->Condition.Overflow == 1) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = (int64_t)reg_struct->ProgramCounter + offset;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::BCC_PL(Garand::GarandInstruction instr,
                               Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    int8_t offset = instr.InstructionSpecific & 0xff;

    Garand::Registers *reg_struct = (Garand::Registers *)regs;

    if (reg_struct->Condition.Negative == 0) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = (int64_t)reg_struct->ProgramCounter + offset;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::BCC_NG(Garand::GarandInstruction instr,
                               Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    int8_t offset = instr.InstructionSpecific & 0xff;

    Garand::Registers *reg_struct = (Garand::Registers *)regs;

    if (reg_struct->Condition.Negative == 1) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = (int64_t)reg_struct->ProgramCounter + offset;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::Add(Garand::GarandInstruction instr,
                            Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest = (instr.InstructionSpecific >> 14) & 0b111111;
    wb.reg = (Garand::load_reg(regs, dest));

    uint8_t src = (instr.InstructionSpecific >> 8) & 0b111111;
    uint8_t val = (instr.InstructionSpecific >> 2) & 0b111111;

    RegisterSize *reg_src = (Garand::load_reg(regs, src));
    RegisterSize *reg_val = (Garand::load_reg(regs, val));

    wb.value = *reg_src + *reg_val;

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::AddImmediate(Garand::GarandInstruction instr,
                                     Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    RegisterSize *reg_dest = Garand::load_reg(regs, dest_index);

    uint8_t val_index = (instr.InstructionSpecific >> 8) & 0b111111;
    RegisterSize *val_reg = Garand::load_reg(regs, val_index);
    auto val_1 = *val_reg;

    uint8_t imm = instr.InstructionSpecific & 0xFF;

    wb.reg = reg_dest;
    wb.value = val_1 + imm;

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::FX_Add(Garand::GarandInstruction instr,
                               Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest = (instr.InstructionSpecific >> 14) & 0b111111;
    wb.reg = (Garand::load_reg(regs, dest));

    uint8_t src = (instr.InstructionSpecific >> 8) & 0b111111;
    uint8_t val = (instr.InstructionSpecific >> 2) & 0b111111;

    RegisterSize reg_src = *(Garand::load_reg(regs, src));
    RegisterSize reg_val = *(Garand::load_reg(regs, val));

    int r1_fractional = reg_src & 0x7FFFFF;
    int r1_exponent = (reg_src >> 23) & 0xFF;
    int r1_sign = (reg_src >> 31) & 1;

    if (r1_sign) {
        r1_fractional = -r1_fractional;
        r1_exponent = -r1_exponent;
    }

    int r2_fractional = reg_src & 0x7FFFFF;
    int r2_exponent = (reg_src >> 23) & 0xFF;
    int r2_sign = (reg_src >> 31) & 1;

    if (r2_sign) {
        r2_fractional = -r2_fractional;
        r2_exponent = -r2_exponent;
    }

    int res_fractional = r2_fractional + r1_fractional;
    int final_fractional = res_fractional & 0x7FFFFF;
    int fractional_overflow = res_fractional & 0x800000;

    int res_exponent = (r2_exponent + r1_exponent) + fractional_overflow;

    wb.value = (res_fractional & 0x7FFFFF) | (res_exponent << 23) |
               (r1_sign & r2_sign ? (r1_sign << 31) : 0);

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::FX_AddImmediate(Garand::GarandInstruction instr,
                                        Garand::Memory &mem, Registers *regs) {
    // TODO: Implement Instruction
    // Edit: Not doable since fixed point wont fit in immediate
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::Subtract(Garand::GarandInstruction instr,
                                 Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest = (instr.InstructionSpecific >> 14) & 0b111111;
    wb.reg = (Garand::load_reg(regs, dest));

    uint8_t src = (instr.InstructionSpecific >> 8) & 0b111111;
    uint8_t val = (instr.InstructionSpecific >> 2) & 0b111111;

    RegisterSize *reg_src = (Garand::load_reg(regs, src));
    RegisterSize *reg_val = (Garand::load_reg(regs, val));

    wb.value = *reg_src - *reg_val;

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::SubtractImmediate(Garand::GarandInstruction instr,
                                          Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    RegisterSize *reg_dest = Garand::load_reg(regs, dest_index);

    uint8_t val_index = (instr.InstructionSpecific >> 8) & 0b111111;
    RegisterSize *val_reg = Garand::load_reg(regs, val_index);
    auto val_1 = *val_reg;

    int imm = instr.InstructionSpecific & 0xFF;

    wb.reg = reg_dest;
    wb.value = val_1 - imm;

    return wb;
}

std::tuple<uint64_t, Garand::ConditionFlag> AddWithCarry(RegisterSize x, RegisterSize y, uint8_t carry) {
    // Based on ARM Reference Manual - shared/functions/integer/AddWithCarry
    auto constexpr get_msb = [](auto val) -> uint8_t {
        return (val >> (sizeof(val) * 8 - 1)) & 1;
    };
    auto unsigned_sum = x + y + static_cast<uint64_t>(carry);
    auto tmp = (get_msb(x) << 2) | (get_msb(y) << 1) | get_msb(unsigned_sum);
    constexpr auto top = ~(1ULL << (sizeof(uint64_t) * 8 - 1));
    auto x0 = x < top + 1;
    auto y0 = y < top + 1;
    auto x1 = x < top;
    auto y1 = y < top;
    auto flag = Garand::ConditionFlag {
        .Zero = (unsigned_sum == 0),
        .Negative = get_msb(unsigned_sum),
        .Carry = static_cast<uint8_t>((x1 && y1) || (x0 && y1) || (x1 && y0)),
        .Overflow = (tmp == 0b1 || tmp == 0b110),
    };
    return std::make_tuple(unsigned_sum, flag);
}


Garand::InstructionWriteBack
Garand::InstructionSet::Compare(Garand::GarandInstruction instr,
                                Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;
    wb.write_back = false;

    uint8_t r1 = (instr.InstructionSpecific >> 14) & 0b111111;
    uint8_t r2 = (instr.InstructionSpecific >> 8) & 0b111111;

    auto r1_val = *(Garand::load_reg(regs, r1));
    auto r2_val = *(Garand::load_reg(regs, r2));

    auto &condition = reinterpret_cast<Garand::Registers *>(regs)->Condition;
    condition = std::get<1>(AddWithCarry(r1_val, ~r2_val, 1));

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::CompareImmediate(Garand::GarandInstruction instr,
                                         Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;
    wb.write_back = false;

    uint8_t r1 = (instr.InstructionSpecific >> 14) & 0b111111;
    RegisterSize imm = (instr.InstructionSpecific >> 2) & 0b111111111111;

    auto r1_val = *(Garand::load_reg(regs, r1));

    auto &condition = reinterpret_cast<Garand::Registers *>(regs)->Condition;
    condition = std::get<1>(AddWithCarry(r1_val, ~imm, 1));

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::FX_Subtract(Garand::GarandInstruction instr,
                                    Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest = (instr.InstructionSpecific >> 14) & 0b111111;
    wb.reg = (Garand::load_reg(regs, dest));

    uint8_t src = (instr.InstructionSpecific >> 8) & 0b111111;
    uint8_t val = (instr.InstructionSpecific >> 2) & 0b111111;

    RegisterSize reg_src = *(Garand::load_reg(regs, src));
    RegisterSize reg_val = *(Garand::load_reg(regs, val));

    int r1_fractional = reg_src & 0x7FFFFF;
    int r1_exponent = (reg_src >> 23) & 0xFF;
    int r1_sign = (reg_src >> 31) & 1;

    if (r1_sign) {
        r1_fractional = -r1_fractional;
        r1_exponent = -r1_exponent;
    }

    int r2_fractional = reg_src & 0x7FFFFF;
    int r2_exponent = (reg_src >> 23) & 0xFF;
    int r2_sign = (reg_src >> 31) & 1;

    if (r2_sign) {
        r2_fractional = -r2_fractional;
        r2_exponent = -r2_exponent;
    }

    int res_fractional = r1_fractional - r2_fractional;
    int final_fractional = res_fractional & 0x7FFFFF;
    int fractional_overflow = res_fractional & 0x800000;

    int res_exponent = (r1_exponent - r2_exponent) + fractional_overflow;

    wb.value = (res_fractional & 0x7FFFFF) | (res_exponent << 23) |
               (r1_sign & r2_sign ? (r1_sign << 31) : 0);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::FX_SubtractImmediate(
    Garand::GarandInstruction instr, Garand::Memory &mem, Registers *regs) {
    // TODO: Implement Instruction
    // Edit: Not doable since fixed point wont fit in immediate
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::Multiply(Garand::GarandInstruction instr,
                                 Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest = (instr.InstructionSpecific >> 14) & 0b111111;
    wb.reg = (Garand::load_reg(regs, dest));

    uint8_t src = (instr.InstructionSpecific >> 8) & 0b111111;
    uint8_t val = (instr.InstructionSpecific >> 2) & 0b111111;

    RegisterSize *reg_src = (Garand::load_reg(regs, src));
    RegisterSize *reg_val = (Garand::load_reg(regs, val));

    wb.value = *reg_src * *reg_val;

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::MultiplyImmediate(Garand::GarandInstruction instr,
                                          Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    RegisterSize *reg_dest = Garand::load_reg(regs, dest_index);

    uint8_t val_index = (instr.InstructionSpecific >> 8) & 0b111111;
    RegisterSize *val_reg = Garand::load_reg(regs, val_index);
    auto val_1 = *val_reg;

    uint8_t imm = instr.InstructionSpecific & 0xFF;

    wb.reg = reg_dest;
    wb.value = val_1 * imm;

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::MultiplyAdd(Garand::GarandInstruction instr,
                                    Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest = (instr.InstructionSpecific >> 14) & 0b111111;
    wb.reg = (Garand::load_reg(regs, dest));

    uint8_t src = (instr.InstructionSpecific >> 8) & 0b111111;
    uint8_t val = (instr.InstructionSpecific >> 2) & 0b111111;

    RegisterSize *reg_dest = (Garand::load_reg(regs, dest));
    RegisterSize *reg_src = (Garand::load_reg(regs, src));
    RegisterSize *reg_val = (Garand::load_reg(regs, val));

    wb.value = *reg_dest + (*reg_src * *reg_val);

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::FX_Multiply(Garand::GarandInstruction instr,
                                    Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest = (instr.InstructionSpecific >> 14) & 0b111111;
    wb.reg = (Garand::load_reg(regs, dest));

    uint8_t src = (instr.InstructionSpecific >> 8) & 0b111111;
    uint8_t val = (instr.InstructionSpecific >> 2) & 0b111111;

    RegisterSize reg_src = *(Garand::load_reg(regs, src));
    RegisterSize reg_val = *(Garand::load_reg(regs, val));

    int r1_fractional = reg_src & 0x7FFFFF;
    int r1_exponent = (reg_src >> 23) & 0xFF;
    int r1_sign = (reg_src >> 31) & 1;

    if (r1_sign) {
        r1_fractional = -r1_fractional;
        r1_exponent = -r1_exponent;
    }

    int r2_fractional = reg_src & 0x7FFFFF;
    int r2_exponent = (reg_src >> 23) & 0xFF;
    int r2_sign = (reg_src >> 31) & 1;

    if (r2_sign) {
        r2_fractional = -r2_fractional;
        r2_exponent = -r2_exponent;
    }

    int res_fractional = r1_fractional * r2_fractional;
    int final_fractional = res_fractional & 0x7FFFFF;
    int fractional_overflow = res_fractional & 0x800000;

    int res_exponent = (r1_exponent * r2_exponent) + fractional_overflow;

    wb.value = (res_fractional & 0x7FFFFF) | (res_exponent << 23) |
               (r1_sign & r2_sign ? (r1_sign << 31) : 0);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::FX_MultiplyImmediate(
    Garand::GarandInstruction instr, Garand::Memory &mem, Registers *regs) {
    // TODO: Implement Instruction
    // Edit: Not doable since fixed point wont fit in immediate
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::FX_MultiplyAdd(Garand::GarandInstruction instr,
                                       Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest = (instr.InstructionSpecific >> 14) & 0b111111;
    wb.reg = (Garand::load_reg(regs, dest));

    uint8_t src = (instr.InstructionSpecific >> 8) & 0b111111;
    uint8_t val = (instr.InstructionSpecific >> 2) & 0b111111;

    RegisterSize reg_src = *(Garand::load_reg(regs, src));
    RegisterSize reg_val = *(Garand::load_reg(regs, val));

    int r1_fractional = reg_src & 0x7FFFFF;
    int r1_exponent = (reg_src >> 23) & 0xFF;
    int r1_sign = (reg_src >> 31) & 1;

    if (r1_sign) {
        r1_fractional = -r1_fractional;
        r1_exponent = -r1_exponent;
    }

    int r2_fractional = reg_src & 0x7FFFFF;
    int r2_exponent = (reg_src >> 23) & 0xFF;
    int r2_sign = (reg_src >> 31) & 1;

    if (r2_sign) {
        r2_fractional = -r2_fractional;
        r2_exponent = -r2_exponent;
    }

    int res_fractional = r1_fractional * r2_fractional;
    int final_fractional = res_fractional & 0x7FFFFF;
    int fractional_overflow = res_fractional & 0x800000;

    int res_exponent = (r1_exponent * r2_exponent) + fractional_overflow;

    wb.value = *wb.reg + ((res_fractional & 0x7FFFFF) | (res_exponent << 23) |
                          (r1_sign & r2_sign ? (r1_sign << 31) : 0));

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::Divide(Garand::GarandInstruction instr,
                               Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest = (instr.InstructionSpecific >> 14) & 0b111111;
    wb.reg = (Garand::load_reg(regs, dest));

    uint8_t src = (instr.InstructionSpecific >> 8) & 0b111111;
    uint8_t val = (instr.InstructionSpecific >> 2) & 0b111111;

    RegisterSize *reg_src = (Garand::load_reg(regs, src));
    RegisterSize *reg_val = (Garand::load_reg(regs, val));

    wb.value = *reg_src - *reg_val;

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::DivideImmediate(Garand::GarandInstruction instr,
                                        Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    RegisterSize *reg_dest = Garand::load_reg(regs, dest_index);

    uint8_t val_index = (instr.InstructionSpecific >> 8) & 0b111111;
    RegisterSize *val_reg = Garand::load_reg(regs, val_index);
    auto val_1 = *val_reg;

    uint8_t imm = instr.InstructionSpecific & 0xFF;

    wb.reg = reg_dest;
    wb.value = val_1 / imm;

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::FX_Divide(Garand::GarandInstruction instr,
                                  Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest = (instr.InstructionSpecific >> 14) & 0b111111;
    wb.reg = (Garand::load_reg(regs, dest));

    uint8_t src = (instr.InstructionSpecific >> 8) & 0b111111;
    uint8_t val = (instr.InstructionSpecific >> 2) & 0b111111;

    RegisterSize reg_src = *(Garand::load_reg(regs, src));
    RegisterSize reg_val = *(Garand::load_reg(regs, val));

    int r1_fractional = reg_src & 0x7FFFFF;
    int r1_exponent = (reg_src >> 23) & 0xFF;
    int r1_sign = (reg_src >> 31) & 1;

    if (r1_sign) {
        r1_fractional = -r1_fractional;
        r1_exponent = -r1_exponent;
    }

    int r2_fractional = reg_src & 0x7FFFFF;
    int r2_exponent = (reg_src >> 23) & 0xFF;
    int r2_sign = (reg_src >> 31) & 1;

    if (r2_sign) {
        r2_fractional = -r2_fractional;
        r2_exponent = -r2_exponent;
    }

    int res_fractional = r1_fractional / r2_fractional;
    int final_fractional = res_fractional & 0x7FFFFF;
    int fractional_overflow = res_fractional & 0x800000;

    int res_exponent = (r1_exponent / r2_exponent) + fractional_overflow;

    wb.value = (res_fractional & 0x7FFFFF) | (res_exponent << 23) |
               (r1_sign & r2_sign ? (r1_sign << 31) : 0);
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::FX_DivideImmediate(
    Garand::GarandInstruction instr, Garand::Memory &mem, Registers *regs) {
    // TODO: Implement Instruction
    // Edit: Not doable since fixed point wont fit in immediate
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::AND(Garand::GarandInstruction instr,
                            Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest = (instr.InstructionSpecific >> 14) & 0b111111;
    wb.reg = (Garand::load_reg(regs, dest));

    uint8_t src = (instr.InstructionSpecific >> 8) & 0b111111;
    uint8_t val = (instr.InstructionSpecific >> 2) & 0b111111;

    RegisterSize *reg_src = (Garand::load_reg(regs, src));
    RegisterSize *reg_val = (Garand::load_reg(regs, val));

    wb.value = *reg_src & *reg_val;

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::ANDImmediate(Garand::GarandInstruction instr,
                                     Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    RegisterSize *reg_dest = Garand::load_reg(regs, dest_index);

    uint8_t val_index = (instr.InstructionSpecific >> 8) & 0b111111;
    RegisterSize *val_reg = Garand::load_reg(regs, val_index);

    uint8_t imm = instr.InstructionSpecific & 0xFF;

    wb.reg = reg_dest;
    wb.value = *val_reg & imm;

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::Test(Garand::GarandInstruction instr,
                             Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;
    wb.write_back = false;

    uint8_t r1 = (instr.InstructionSpecific >> 14) & 0b111111;
    uint8_t r2 = (instr.InstructionSpecific >> 8) & 0b111111;

    auto r1_val = *(Garand::load_reg(regs, r1));
    auto r2_val = *(Garand::load_reg(regs, r2));

    auto val = r2_val & r1_val;

    auto &condition = reinterpret_cast<Garand::Registers *>(regs)->Condition;
    condition = {
        .Zero = (val == 0),
        .Negative = static_cast<uint8_t>((val >> (sizeof(val) * 8 - 1)) & 1),
        .Carry = 0,
        .Overflow = 0,
    };
    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::NAND(Garand::GarandInstruction instr,
                             Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest = (instr.InstructionSpecific >> 14) & 0b111111;
    wb.reg = (Garand::load_reg(regs, dest));

    uint8_t src = (instr.InstructionSpecific >> 8) & 0b111111;
    uint8_t val = (instr.InstructionSpecific >> 2) & 0b111111;

    RegisterSize *reg_src = (Garand::load_reg(regs, src));
    RegisterSize *reg_val = (Garand::load_reg(regs, val));

    wb.value = ~(*reg_src & *reg_val);

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::NANDImmediate(Garand::GarandInstruction instr,
                                      Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    RegisterSize *reg_dest = Garand::load_reg(regs, dest_index);

    uint8_t val_index = (instr.InstructionSpecific >> 8) & 0b111111;
    RegisterSize *val_reg = Garand::load_reg(regs, val_index);

    uint8_t imm = instr.InstructionSpecific & 0xFF;

    wb.reg = reg_dest;
    wb.value = ~(*val_reg & imm);

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::OR(Garand::GarandInstruction instr, Garand::Memory &mem,
                           Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest = (instr.InstructionSpecific >> 14) & 0b111111;
    wb.reg = (Garand::load_reg(regs, dest));

    uint8_t src = (instr.InstructionSpecific >> 8) & 0b111111;
    uint8_t val = (instr.InstructionSpecific >> 2) & 0b111111;

    RegisterSize *reg_src = (Garand::load_reg(regs, src));
    RegisterSize *reg_val = (Garand::load_reg(regs, val));

    wb.value = *reg_src | *reg_val;

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::ORImmediate(Garand::GarandInstruction instr,
                                    Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    RegisterSize *reg_dest = Garand::load_reg(regs, dest_index);

    uint8_t val_index = (instr.InstructionSpecific >> 8) & 0b111111;
    RegisterSize *val_reg = Garand::load_reg(regs, val_index);

    uint8_t imm = instr.InstructionSpecific & 0xFF;

    wb.reg = reg_dest;
    wb.value = *val_reg | imm;

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::XOR(Garand::GarandInstruction instr,
                            Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest = (instr.InstructionSpecific >> 14) & 0b111111;
    wb.reg = (Garand::load_reg(regs, dest));

    uint8_t src = (instr.InstructionSpecific >> 8) & 0b111111;
    uint8_t val = (instr.InstructionSpecific >> 2) & 0b111111;

    RegisterSize *reg_src = (Garand::load_reg(regs, src));
    RegisterSize *reg_val = (Garand::load_reg(regs, val));

    wb.value = *reg_src ^ *reg_val;

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::XORImmediate(Garand::GarandInstruction instr,
                                     Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    RegisterSize *reg_dest = Garand::load_reg(regs, dest_index);

    uint8_t val_index = (instr.InstructionSpecific >> 8) & 0b111111;
    RegisterSize *val_reg = Garand::load_reg(regs, val_index);

    uint8_t imm = instr.InstructionSpecific & 0xFF;

    wb.reg = reg_dest;
    wb.value = *val_reg ^ imm;

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::LogicalShiftLeft(Garand::GarandInstruction instr,
                                         Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest = (instr.InstructionSpecific >> 14) & 0b111111;
    wb.reg = (Garand::load_reg(regs, dest));

    uint8_t src = (instr.InstructionSpecific >> 8) & 0b111111;
    uint8_t val = (instr.InstructionSpecific >> 2) & 0b111111;

    RegisterSize *reg_src = (Garand::load_reg(regs, src));
    RegisterSize *reg_val = (Garand::load_reg(regs, val));

    wb.value = *reg_src << *reg_val;

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::LogicalShiftLeftImmediate(
    Garand::GarandInstruction instr, Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    RegisterSize *reg_dest = Garand::load_reg(regs, dest_index);

    uint8_t val_index = (instr.InstructionSpecific >> 8) & 0b111111;
    RegisterSize *val_reg = Garand::load_reg(regs, val_index);

    uint8_t imm = instr.InstructionSpecific & 0xFF;

    wb.reg = reg_dest;
    wb.value = *val_reg << imm;

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::LogicalShiftRight(Garand::GarandInstruction instr,
                                          Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest = (instr.InstructionSpecific >> 14) & 0b111111;
    wb.reg = (Garand::load_reg(regs, dest));

    uint8_t src = (instr.InstructionSpecific >> 8) & 0b111111;
    uint8_t val = (instr.InstructionSpecific >> 2) & 0b111111;

    RegisterSize *reg_src = (Garand::load_reg(regs, src));
    RegisterSize *reg_val = (Garand::load_reg(regs, val));

    wb.value = *reg_src >> *reg_val;

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::LogicalShiftRightImmediate(
    Garand::GarandInstruction instr, Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    RegisterSize *reg_dest = Garand::load_reg(regs, dest_index);

    uint8_t val_index = (instr.InstructionSpecific >> 8) & 0b111111;
    RegisterSize *val_reg = Garand::load_reg(regs, val_index);

    uint8_t imm = instr.InstructionSpecific & 0xFF;

    wb.reg = reg_dest;
    wb.value = *val_reg >> imm;

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::RotationalShiftRight(
    Garand::GarandInstruction instr, Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest = (instr.InstructionSpecific >> 14) & 0b111111;
    wb.reg = (Garand::load_reg(regs, dest));

    uint8_t src = (instr.InstructionSpecific >> 8) & 0b111111;
    uint8_t val = (instr.InstructionSpecific >> 2) & 0b111111;

    RegisterSize reg_src = *(Garand::load_reg(regs, src));
    RegisterSize reg_val = *(Garand::load_reg(regs, val));

    wb.value = (reg_src << reg_val) | (reg_src >> (sizeof(reg_src) * 8 - reg_val));

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::RotationalShiftRightImmediate(
    Garand::GarandInstruction instr, Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest = (instr.InstructionSpecific >> 14) & 0b111111;
    wb.reg = (Garand::load_reg(regs, dest));

    uint8_t src = (instr.InstructionSpecific >> 8) & 0b111111;
    uint8_t val = instr.InstructionSpecific & 0b11111111;

    RegisterSize reg_src = *(Garand::load_reg(regs, src));

    wb.value = (reg_src << val) | (reg_src >> (sizeof(reg_src) * 8 - val));

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::NOT(Garand::GarandInstruction instr,
                            Garand::Memory &mem, Registers *regs) {
    Garand::InstructionWriteBack wb;

    uint8_t dest = (instr.InstructionSpecific >> 14) & 0b111111;
    wb.reg = (Garand::load_reg(regs, dest));

    uint8_t src = (instr.InstructionSpecific >> 8) & 0b111111;

    RegisterSize *reg_src = (Garand::load_reg(regs, src));

    wb.value = ~(*reg_src);

    return wb;
}

Garand::InstructionWriteBack
Garand::InstructionSet::Call(Garand::GarandInstruction instr,
                             Garand::Memory &mem, Registers *Regs) {
    Garand::InstructionWriteBack Writeback;
    uint32_t Address = instr.InstructionSpecific;
    Regs->LinkRegister = Regs->ProgramCounter;
    Writeback.write_back = true;
    Writeback.reg = Garand::load_reg(Regs, 33);
    Writeback.value = Regs->ProgramCounter;
    return Writeback;
}

Garand::InstructionWriteBack
Garand::InstructionSet::Return(Garand::GarandInstruction instr,
                               Garand::Memory &mem, Registers *Regs) {
    Garand::InstructionWriteBack Writeback;
    Writeback.write_back = true;
    Writeback.reg = Garand::load_reg(Regs, 35);
    Writeback.value = Regs->LinkRegister;
    Regs->LinkRegister = 0;
    return Writeback;
}

char const *get_ins_mnemonic(Garand::GarandInstruction ins) {
    auto decoded_type = Garand::Instruction::DecodeMnemonic(ins);
    using Garand::DecodedInstruction;

#define CASE_INS(MNEMONIC)                                                     \
    case MNEMONIC:                                                             \
        return #MNEMONIC
    switch (decoded_type) {
        CASE_INS(MREAD);
        CASE_INS(MWRITE);
        CASE_INS(BIND);
        CASE_INS(UNBIND);
        CASE_INS(BRUHCC_AL);
        CASE_INS(BRUHCC_EQ);
        CASE_INS(BRUHCC_NE);
        CASE_INS(BRUHCC_LO);
        CASE_INS(BRUHCC_HS);
        CASE_INS(BRUHCC_LT);
        CASE_INS(BRUHCC_GE);
        CASE_INS(BRUHCC_HI);
        CASE_INS(BRUHCC_LS);
        CASE_INS(BRUHCC_GT);
        CASE_INS(BRUHCC_LE);
        CASE_INS(BRUHCC_VC);
        CASE_INS(BRUHCC_VS);
        CASE_INS(BRUHCC_PL);
        CASE_INS(BRUHCC_NG);
        CASE_INS(BCC_AL);
        CASE_INS(BCC_EQ);
        CASE_INS(BCC_NE);
        CASE_INS(BCC_LO);
        CASE_INS(BCC_HS);
        CASE_INS(BCC_LT);
        CASE_INS(BCC_GE);
        CASE_INS(BCC_HI);
        CASE_INS(BCC_LS);
        CASE_INS(BCC_GT);
        CASE_INS(BCC_LE);
        CASE_INS(BCC_VC);
        CASE_INS(BCC_VS);
        CASE_INS(BCC_PL);
        CASE_INS(BCC_NG);
        CASE_INS(ADD);
        CASE_INS(ADDI);
        CASE_INS(FX_ADD);
        CASE_INS(FX_ADDI);
        CASE_INS(SUB);
        CASE_INS(SUBI);
        CASE_INS(CMP);
        CASE_INS(CMPI);
        CASE_INS(FX_SUB);
        CASE_INS(FX_SUBI);
        CASE_INS(MUL);
        CASE_INS(MULI);
        CASE_INS(MADD);
        CASE_INS(FX_MUL);
        CASE_INS(FX_MULI);
        CASE_INS(FX_MADD);
        CASE_INS(DIV);
        CASE_INS(DIVI);
        CASE_INS(FX_DIV);
        CASE_INS(FX_DIVI);
        CASE_INS(AND);
        CASE_INS(ANDI);
        CASE_INS(TEST);
        CASE_INS(NAND);
        CASE_INS(NANDI);
        CASE_INS(OR);
        CASE_INS(ORI);
        CASE_INS(XOR);
        CASE_INS(XORI);
        CASE_INS(LSL);
        CASE_INS(LSLI);
        CASE_INS(LSR);
        CASE_INS(LSRI);
        CASE_INS(RSR);
        CASE_INS(RSRI);
        CASE_INS(NOT);
        CASE_INS(CALL);
        CASE_INS(RETURN);
        CASE_INS(UNKNOWN);
    default:
        return "NOINFO";
    };
    return "ERR";
}

}