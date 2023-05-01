#include "Instructions.hpp"
#include "Instruction.hpp"
#include "Memory.hpp"

#include <iostream>

Garand::InstructionWriteBack Garand::InstructionSet::MemoryRead(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int imm_flag = (instr.InstructionSpecific >> 20) & 0b1;

    // Destination address will be same for all
    int dest = (instr.InstructionSpecific >> 14) & 0b111111;
    wb.reg = (Garand::load_reg(regs, dest));

    if (!imm_flag) {
        int src = (instr.InstructionSpecific >> 8) & 0b111111;
        int offset = (instr.InstructionSpecific >> 2) & 0b111111;

        uint32_t *reg_src = (uint32_t *) (Garand::load_reg(regs, src));
        uint32_t *reg_offset = (uint32_t *) (Garand::load_reg(regs, offset));

        Garand::LoadSize *addr = mem.load(*reg_src + * reg_offset);

        wb.value = *addr;
    } else {
        int imm = instr.InstructionSpecific & 0x3FFF;
        wb.value = *(mem.load(imm));
    }

    return wb;
}

// Example for `MWRITE R15, 50`
// Garand::GarandInstruction instr_b;
// instr_b.Operation = 0x0;
// instr_b.ConditionFlags = 0x1;
// instr_b.InstructionSpecific = 1 << 20 | 15 << 14 | 50;
Garand::InstructionWriteBack Garand::InstructionSet::MemoryWrite(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;
    wb.is_reg = false;

    int imm_flag = (instr.InstructionSpecific >> 20) & 0b1;

    if (!imm_flag) {
        int src_index = (instr.InstructionSpecific >> 14) & 0b111111;
        int dest_index = (instr.InstructionSpecific >> 8) & 0b111111;
        int offset = (instr.InstructionSpecific >> 2) & 0b111111;

        uint64_t *reg_src = Garand::load_reg(regs, src_index);
        uint64_t *reg_dest = Garand::load_reg(regs, dest_index);
        uint64_t *reg_offset = Garand::load_reg(regs, offset);

        wb.reg = (uint64_t*) (*reg_dest + *reg_offset);
        wb.value = *reg_src;
    } else {
        int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
        uint64_t *reg_dest = Garand::load_reg(regs, dest_index);

        int imm = instr.InstructionSpecific & 0x3FFF;

        wb.reg = reg_dest;
        wb.value = imm;
    }

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::Bind(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::Unbind(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_AL(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    wb.reg = &reg_struct->ProgramCounter;
    wb.value = reg_dest;

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_EQ(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Zero == 1) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = reg_dest;
    } else {
        wb.write_back = false;
    }
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_NE(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Zero == 0) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = reg_dest;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_LO(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Carry == 0) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = reg_dest;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_HS(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Carry == 1) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = reg_dest;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_LT(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Negative != reg_struct->Condition.Overflow) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = reg_dest;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_GE(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Negative == reg_struct->Condition.Overflow) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = reg_dest;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_HI(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Carry == 1 && reg_struct->Condition.Zero == 0) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = reg_dest;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_LS(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (!(reg_struct->Condition.Carry == 1 && reg_struct->Condition.Zero)) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = reg_dest;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_GT(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Zero == 0 && reg_struct->Condition.Negative == reg_struct->Condition.Overflow) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = reg_dest;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_LE(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (!(reg_struct->Condition.Zero == 0 && reg_struct->Condition.Negative == reg_struct->Condition.Overflow)) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = reg_dest;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_VC(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Overflow == 0) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = reg_dest;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_VS(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Overflow == 1) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = reg_dest;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_PL(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Negative == 0) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = reg_dest;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_NG(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Negative == 1) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = reg_dest;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_AL(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int8_t offset = instr.InstructionSpecific & 0xff;

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    wb.reg = &reg_struct->ProgramCounter;
    wb.value = (int64_t)reg_struct->ProgramCounter + offset;

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_EQ(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int8_t offset = instr.InstructionSpecific & 0xff;

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Zero == 1) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = (int64_t)reg_struct->ProgramCounter + offset;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_NE(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int8_t offset = instr.InstructionSpecific & 0xff;

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Zero == 0) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = (int64_t)reg_struct->ProgramCounter + offset;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_LO(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int8_t offset = instr.InstructionSpecific & 0xff;

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Carry == 0) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = (int64_t)reg_struct->ProgramCounter + offset;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_HS(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int8_t offset = instr.InstructionSpecific & 0xff;

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Carry == 1) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = (int64_t)reg_struct->ProgramCounter + offset;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_LT(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int8_t offset = instr.InstructionSpecific & 0xff;

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Negative != reg_struct->Condition.Overflow) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = (int64_t)reg_struct->ProgramCounter + offset;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_GE(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int8_t offset = instr.InstructionSpecific & 0xff;

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Negative == reg_struct->Condition.Overflow) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = (int64_t)reg_struct->ProgramCounter + offset;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_HI(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int8_t offset = instr.InstructionSpecific & 0xff;

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Carry == 1 && reg_struct->Condition.Zero == 0) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = (int64_t)reg_struct->ProgramCounter + offset;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_LS(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int8_t offset = instr.InstructionSpecific & 0xff;

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (!(reg_struct->Condition.Carry == 1 && reg_struct->Condition.Zero)) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = (int64_t)reg_struct->ProgramCounter + offset;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_GT(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int8_t offset = instr.InstructionSpecific & 0xff;

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Zero == 0 && reg_struct->Condition.Negative == reg_struct->Condition.Overflow) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = (int64_t)reg_struct->ProgramCounter + offset;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_LE(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int8_t offset = instr.InstructionSpecific & 0xff;

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (!(reg_struct->Condition.Zero == 0 && reg_struct->Condition.Negative == reg_struct->Condition.Overflow)) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = (int64_t)reg_struct->ProgramCounter + offset;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_VC(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int8_t offset = instr.InstructionSpecific & 0xff;

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Overflow == 0) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = (int64_t)reg_struct->ProgramCounter + offset;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_VS(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int8_t offset = instr.InstructionSpecific & 0xff;

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Overflow == 1) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = (int64_t)reg_struct->ProgramCounter + offset;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_PL(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int8_t offset = instr.InstructionSpecific & 0xff;

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Negative == 0) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = (int64_t)reg_struct->ProgramCounter + offset;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_NG(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int8_t offset = instr.InstructionSpecific & 0xff;

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Negative == 1) {
        wb.reg = &reg_struct->ProgramCounter;
        wb.value = (int64_t)reg_struct->ProgramCounter + offset;
    } else {
        wb.write_back = false;
    }

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::Add(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest = (instr.InstructionSpecific >> 14) & 0b111111;
    wb.reg = (Garand::load_reg(regs, dest));

    int src = (instr.InstructionSpecific >> 8) & 0b111111;
    int val = (instr.InstructionSpecific >> 2) & 0b111111;

    uint64_t *reg_src = (Garand::load_reg(regs, src));
    uint64_t *reg_val = (Garand::load_reg(regs, val));

    wb.value = *reg_src + *reg_val;

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::AddImmediate(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t *reg_dest = Garand::load_reg(regs, dest_index);

    int val_index = (instr.InstructionSpecific >> 8) & 0b111111;
    uint64_t *val_reg = Garand::load_reg(regs, val_index);
    int val_1 = *val_reg;

    int imm = instr.InstructionSpecific & 0xFF;

    wb.reg = reg_dest;
    wb.value = val_1 + imm;

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::FX_Add(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest = (instr.InstructionSpecific >> 14) & 0b111111;
    wb.reg = (Garand::load_reg(regs, dest));

    int src = (instr.InstructionSpecific >> 8) & 0b111111;
    int val = (instr.InstructionSpecific >> 2) & 0b111111;

    uint64_t reg_src = *(Garand::load_reg(regs, src));
    uint64_t reg_val = *(Garand::load_reg(regs, val));

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

    wb.value = (res_fractional & 0x7FFFFF) | (res_exponent << 23) | (r1_sign & r2_sign ? (r1_sign << 31) : 0);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::FX_AddImmediate(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    // TODO: Implement Instruction
    // Edit: Not doable since fixed point wont fit in immediate
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::Subtract(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest = (instr.InstructionSpecific >> 14) & 0b111111;
    wb.reg = (Garand::load_reg(regs, dest));

    int src = (instr.InstructionSpecific >> 8) & 0b111111;
    int val = (instr.InstructionSpecific >> 2) & 0b111111;

    uint64_t *reg_src = (Garand::load_reg(regs, src));
    uint64_t *reg_val = (Garand::load_reg(regs, val));

    wb.value = *reg_src - *reg_val;

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::SubtractImmediate(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t *reg_dest = Garand::load_reg(regs, dest_index);

    int val_index = (instr.InstructionSpecific >> 8) & 0b111111;
    uint64_t *val_reg = Garand::load_reg(regs, val_index);
    int val_1 = *val_reg;

    int imm = instr.InstructionSpecific & 0xFF;

    wb.reg = reg_dest;
    wb.value = val_1 - imm;

    return wb;
}

void set_cflags(uint64_t* regs, uint32_t val, uint32_t val_2, uint32_t val_1) {
    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    reg_struct->Condition.Zero = (val == 0);
    reg_struct->Condition.Negative = (val > 0x7fffffff);
    auto tmp = (((val_1 >> 31) & 1) << 2) | (((val_2 >> 31) & 1) << 1) | ((val >> 31) & 1);
    reg_struct->Condition.Carry = (tmp == 0b100 || tmp == 0b11);
    reg_struct->Condition.Overflow = (val >> 31) & 1;
}

Garand::InstructionWriteBack Garand::InstructionSet::Compare(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;
    wb.write_back = false;

    uint32_t r1 = (instr.InstructionSpecific >> 14) & 0b111111;
    uint32_t r2 = (instr.InstructionSpecific >> 8) & 0b111111;

    uint32_t r1_val = *(Garand::load_reg(regs, r1));
    uint32_t r2_val = *(Garand::load_reg(regs, r2));

    uint32_t val = r2_val - r1_val;

    set_cflags(regs, val, r2_val, r1_val);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::CompareImmediate(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;
    wb.write_back = false;

    uint32_t r1 = (instr.InstructionSpecific >> 14) & 0b111111;
    uint32_t imm = (instr.InstructionSpecific >> 2) & 0b111111111111;

    uint32_t r1_val = *(Garand::load_reg(regs, r1));
    uint32_t imm_val = imm;

    uint32_t val = imm_val - r1_val;

    set_cflags(regs, val, imm_val, r1_val);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::FX_Subtract(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest = (instr.InstructionSpecific >> 14) & 0b111111;
    wb.reg = (Garand::load_reg(regs, dest));

    int src = (instr.InstructionSpecific >> 8) & 0b111111;
    int val = (instr.InstructionSpecific >> 2) & 0b111111;

    uint64_t reg_src = *(Garand::load_reg(regs, src));
    uint64_t reg_val = *(Garand::load_reg(regs, val));

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

    wb.value = (res_fractional & 0x7FFFFF) | (res_exponent << 23) | (r1_sign & r2_sign ? (r1_sign << 31) : 0);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::FX_SubtractImmediate(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    // TODO: Implement Instruction
    // Edit: Not doable since fixed point wont fit in immediate
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::Multiply(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest = (instr.InstructionSpecific >> 14) & 0b111111;
    wb.reg = (Garand::load_reg(regs, dest));

    int src = (instr.InstructionSpecific >> 8) & 0b111111;
    int val = (instr.InstructionSpecific >> 2) & 0b111111;

    uint64_t *reg_src = (Garand::load_reg(regs, src));
    uint64_t *reg_val = (Garand::load_reg(regs, val));

    wb.value = *reg_src * *reg_val;
    
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::MultiplyImmediate(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t *reg_dest = Garand::load_reg(regs, dest_index);

    int val_index = (instr.InstructionSpecific >> 8) & 0b111111;
    uint64_t *val_reg = Garand::load_reg(regs, val_index);
    int val_1 = *val_reg;

    int imm = instr.InstructionSpecific & 0xFF;

    wb.reg = reg_dest;
    wb.value = val_1 * imm;

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::MultiplyAdd(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest = (instr.InstructionSpecific >> 14) & 0b111111;
    wb.reg = (Garand::load_reg(regs, dest));

    int src = (instr.InstructionSpecific >> 8) & 0b111111;
    int val = (instr.InstructionSpecific >> 2) & 0b111111;

    uint64_t *reg_dest = (Garand::load_reg(regs, dest));
    uint64_t *reg_src = (Garand::load_reg(regs, src));
    uint64_t *reg_val = (Garand::load_reg(regs, val));

    wb.value = *reg_dest + (*reg_src * *reg_val);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::FX_Multiply(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest = (instr.InstructionSpecific >> 14) & 0b111111;
    wb.reg = (Garand::load_reg(regs, dest));

    int src = (instr.InstructionSpecific >> 8) & 0b111111;
    int val = (instr.InstructionSpecific >> 2) & 0b111111;

    uint64_t reg_src = *(Garand::load_reg(regs, src));
    uint64_t reg_val = *(Garand::load_reg(regs, val));

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

    wb.value = (res_fractional & 0x7FFFFF) | (res_exponent << 23) | (r1_sign & r2_sign ? (r1_sign << 31) : 0);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::FX_MultiplyImmediate(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    // TODO: Implement Instruction
    // Edit: Not doable since fixed point wont fit in immediate
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::FX_MultiplyAdd(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest = (instr.InstructionSpecific >> 14) & 0b111111;
    wb.reg = (Garand::load_reg(regs, dest));

    int src = (instr.InstructionSpecific >> 8) & 0b111111;
    int val = (instr.InstructionSpecific >> 2) & 0b111111;

    uint64_t reg_src = *(Garand::load_reg(regs, src));
    uint64_t reg_val = *(Garand::load_reg(regs, val));

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

    wb.value = *wb.reg + ((res_fractional & 0x7FFFFF) | (res_exponent << 23) | (r1_sign & r2_sign ? (r1_sign << 31) : 0));

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::Divide(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest = (instr.InstructionSpecific >> 14) & 0b111111;
    wb.reg = (Garand::load_reg(regs, dest));

    int src = (instr.InstructionSpecific >> 8) & 0b111111;
    int val = (instr.InstructionSpecific >> 2) & 0b111111;

    uint64_t *reg_src = (Garand::load_reg(regs, src));
    uint64_t *reg_val = (Garand::load_reg(regs, val));

    wb.value = *reg_src - *reg_val;

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::DivideImmediate(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t *reg_dest = Garand::load_reg(regs, dest_index);

    int val_index = (instr.InstructionSpecific >> 8) & 0b111111;
    uint64_t *val_reg = Garand::load_reg(regs, val_index);
    int val_1 = *val_reg;

    int imm = instr.InstructionSpecific & 0xFF;

    wb.reg = reg_dest;
    wb.value = val_1 / imm;

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::FX_Divide(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;


    int dest = (instr.InstructionSpecific >> 14) & 0b111111;
    wb.reg = (Garand::load_reg(regs, dest));

    int src = (instr.InstructionSpecific >> 8) & 0b111111;
    int val = (instr.InstructionSpecific >> 2) & 0b111111;

    uint64_t reg_src = *(Garand::load_reg(regs, src));
    uint64_t reg_val = *(Garand::load_reg(regs, val));

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

    wb.value = (res_fractional & 0x7FFFFF) | (res_exponent << 23) | (r1_sign & r2_sign ? (r1_sign << 31) : 0);
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::FX_DivideImmediate(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    // TODO: Implement Instruction
    // Edit: Not doable since fixed point wont fit in immediate
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::AND(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest = (instr.InstructionSpecific >> 14) & 0b111111;
    wb.reg = (Garand::load_reg(regs, dest));

    int src = (instr.InstructionSpecific >> 8) & 0b111111;
    int val = (instr.InstructionSpecific >> 2) & 0b111111;

    uint64_t *reg_src = (Garand::load_reg(regs, src));
    uint64_t *reg_val = (Garand::load_reg(regs, val));

    wb.value = *reg_src & *reg_val;

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::ANDImmediate(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t *reg_dest = Garand::load_reg(regs, dest_index);

    int val_index = (instr.InstructionSpecific >> 8) & 0b111111;
    uint64_t *val_reg = Garand::load_reg(regs, val_index);

    int imm = instr.InstructionSpecific & 0xFF;

    wb.reg = reg_dest;
    wb.value = *val_reg & imm;

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::Test(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;
    wb.write_back = false;

    int r1 = (instr.InstructionSpecific >> 14) & 0b111111;
    int r2 = (instr.InstructionSpecific >> 8) & 0b111111;

    uint32_t r1_val = *(Garand::load_reg(regs, r1));
    uint32_t r2_val = *(Garand::load_reg(regs, r2));

    uint32_t val = r2_val & r1_val;

    set_cflags(regs, val, r2_val, r1_val);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::NAND(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest = (instr.InstructionSpecific >> 14) & 0b111111;
    wb.reg = (Garand::load_reg(regs, dest));

    int src = (instr.InstructionSpecific >> 8) & 0b111111;
    int val = (instr.InstructionSpecific >> 2) & 0b111111;

    uint64_t *reg_src = (Garand::load_reg(regs, src));
    uint64_t *reg_val = (Garand::load_reg(regs, val));

    wb.value = ~(*reg_src & *reg_val);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::NANDImmediate(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t *reg_dest = Garand::load_reg(regs, dest_index);

    int val_index = (instr.InstructionSpecific >> 8) & 0b111111;
    uint64_t *val_reg = Garand::load_reg(regs, val_index);

    int imm = instr.InstructionSpecific & 0xFF;

    wb.reg = reg_dest;
    wb.value = ~(*val_reg & imm);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::OR(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest = (instr.InstructionSpecific >> 14) & 0b111111;
    wb.reg = (Garand::load_reg(regs, dest));

    int src = (instr.InstructionSpecific >> 8) & 0b111111;
    int val = (instr.InstructionSpecific >> 2) & 0b111111;

    uint64_t *reg_src = (Garand::load_reg(regs, src));
    uint64_t *reg_val = (Garand::load_reg(regs, val));

    wb.value = *reg_src | *reg_val;

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::ORImmediate(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t *reg_dest = Garand::load_reg(regs, dest_index);

    int val_index = (instr.InstructionSpecific >> 8) & 0b111111;
    uint64_t *val_reg = Garand::load_reg(regs, val_index);

    int imm = instr.InstructionSpecific & 0xFF;

    wb.reg = reg_dest;
    wb.value = *val_reg | imm;

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::XOR(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest = (instr.InstructionSpecific >> 14) & 0b111111;
    wb.reg = (Garand::load_reg(regs, dest));

    int src = (instr.InstructionSpecific >> 8) & 0b111111;
    int val = (instr.InstructionSpecific >> 2) & 0b111111;

    uint64_t *reg_src = (Garand::load_reg(regs, src));
    uint64_t *reg_val = (Garand::load_reg(regs, val));

    wb.value = *reg_src ^ *reg_val;

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::XORImmediate(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t *reg_dest = Garand::load_reg(regs, dest_index);

    int val_index = (instr.InstructionSpecific >> 8) & 0b111111;
    uint64_t *val_reg = Garand::load_reg(regs, val_index);

    int imm = instr.InstructionSpecific & 0xFF;

    wb.reg = reg_dest;
    wb.value = *val_reg ^ imm;

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::LogicalShiftLeft(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest = (instr.InstructionSpecific >> 14) & 0b111111;
    wb.reg = (Garand::load_reg(regs, dest));

    int src = (instr.InstructionSpecific >> 8) & 0b111111;
    int val = (instr.InstructionSpecific >> 2) & 0b111111;

    uint64_t *reg_src = (Garand::load_reg(regs, src));
    uint64_t *reg_val = (Garand::load_reg(regs, val));

    wb.value = *reg_src << *reg_val;

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::LogicalShiftLeftImmediate(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t *reg_dest = Garand::load_reg(regs, dest_index);

    int val_index = (instr.InstructionSpecific >> 8) & 0b111111;
    uint64_t *val_reg = Garand::load_reg(regs, val_index);

    int imm = instr.InstructionSpecific & 0xFF;

    wb.reg = reg_dest;
    wb.value = *val_reg << imm;

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::LogicalShiftRight(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest = (instr.InstructionSpecific >> 14) & 0b111111;
    wb.reg = (Garand::load_reg(regs, dest));

    int src = (instr.InstructionSpecific >> 8) & 0b111111;
    int val = (instr.InstructionSpecific >> 2) & 0b111111;

    uint64_t *reg_src = (Garand::load_reg(regs, src));
    uint64_t *reg_val = (Garand::load_reg(regs, val));

    wb.value = *reg_src >> *reg_val;

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::LogicalShiftRightImmediate(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t *reg_dest = Garand::load_reg(regs, dest_index);

    int val_index = (instr.InstructionSpecific >> 8) & 0b111111;
    uint64_t *val_reg = Garand::load_reg(regs, val_index);

    int imm = instr.InstructionSpecific & 0xFF;

    wb.reg = reg_dest;
    wb.value = *val_reg >> imm;

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::RotationalShiftRight(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest = (instr.InstructionSpecific >> 14) & 0b111111;
    wb.reg = (Garand::load_reg(regs, dest));

    int src = (instr.InstructionSpecific >> 8) & 0b111111;
    int val = (instr.InstructionSpecific >> 2) & 0b111111;

    uint64_t reg_src = *(Garand::load_reg(regs, src));
    uint64_t reg_val = *(Garand::load_reg(regs, val));

    wb.value = (reg_src << reg_val) | (reg_src >> (sizeof(int)*8) - reg_val);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::RotationalShiftRightImmediate(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest = (instr.InstructionSpecific >> 14) & 0b111111;
    wb.reg = (Garand::load_reg(regs, dest));

    int src = (instr.InstructionSpecific >> 8) & 0b111111;
    int val = instr.InstructionSpecific & 0b11111111;

    uint64_t reg_src = *(Garand::load_reg(regs, src));

    wb.value = (reg_src << val) | (reg_src >> (sizeof(int)*8) - val);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::NOT(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest = (instr.InstructionSpecific >> 14) & 0b111111;
    wb.reg = (Garand::load_reg(regs, dest));

    int src = (instr.InstructionSpecific >> 8) & 0b111111;

    uint64_t *reg_src = (Garand::load_reg(regs, src));

    wb.value = ~(*reg_src);

    return wb;
}

char const *Garand::get_ins_mnemonic(Garand::GarandInstruction ins) {
    auto decoded_type = Garand::Instruction::Decode(ins);
    using Garand::DecodedInstruction;

    #define CASE_INS(MNEMONIC) case MNEMONIC: return #MNEMONIC
    switch(decoded_type) {
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
        CASE_INS(UNKNOWN);
        default:
            return "NOINFO";
    };
    return "ERR";
}
