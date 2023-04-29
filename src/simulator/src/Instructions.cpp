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

void do_jump(Garand::Registers* regs, uint32_t dest) {
    regs->ProgramCounter = dest;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_AL(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    do_jump(reg_struct, reg_dest);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_EQ(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Zero == 1)
        do_jump(reg_struct, reg_dest);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_NE(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Zero == 0)
        do_jump(reg_struct, reg_dest);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_LO(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Carry == 0)
        do_jump(reg_struct, reg_dest);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_HS(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Carry == 1)
        do_jump(reg_struct, reg_dest);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_LT(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Negative != reg_struct->Condition.Overflow)
        do_jump(reg_struct, reg_dest);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_GE(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Negative == reg_struct->Condition.Overflow)
        do_jump(reg_struct, reg_dest);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_HI(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Carry == 1 && reg_struct->Condition.Zero == 0)
        do_jump(reg_struct, reg_dest);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_LS(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (!(reg_struct->Condition.Carry == 1 && reg_struct->Condition.Zero))
        do_jump(reg_struct, reg_dest);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_GT(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Zero == 0 && reg_struct->Condition.Negative == reg_struct->Condition.Overflow)
        do_jump(reg_struct, reg_dest);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_LE(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (!(reg_struct->Condition.Zero == 0 && reg_struct->Condition.Negative == reg_struct->Condition.Overflow))
        do_jump(reg_struct, reg_dest);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_VC(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Overflow == 0)
        do_jump(reg_struct, reg_dest);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_VS(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Overflow == 1)
        do_jump(reg_struct, reg_dest);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_PL(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Negative == 0)
        do_jump(reg_struct, reg_dest);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_NG(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Negative == 1)
        do_jump(reg_struct, reg_dest);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_AL(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    do_jump(reg_struct, reg_struct->ProgramCounter + reg_dest);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_EQ(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Zero == 1)
        do_jump(reg_struct, reg_struct->ProgramCounter + reg_dest);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_NE(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Zero == 0)
        do_jump(reg_struct, reg_struct->ProgramCounter + reg_dest);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_LO(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Carry == 0)
        do_jump(reg_struct, reg_struct->ProgramCounter + reg_dest);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_HS(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Carry == 1)
        do_jump(reg_struct, reg_struct->ProgramCounter + reg_dest);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_LT(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Negative != reg_struct->Condition.Overflow)
        do_jump(reg_struct, reg_struct->ProgramCounter + reg_dest);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_GE(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Negative == reg_struct->Condition.Overflow)
        do_jump(reg_struct, reg_struct->ProgramCounter + reg_dest);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_HI(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Carry == 1 && reg_struct->Condition.Zero == 0)
        do_jump(reg_struct, reg_struct->ProgramCounter + reg_dest);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_LS(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (!(reg_struct->Condition.Carry == 1 && reg_struct->Condition.Zero))
        do_jump(reg_struct, reg_struct->ProgramCounter + reg_dest);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_GT(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Zero == 0 && reg_struct->Condition.Negative == reg_struct->Condition.Overflow)
        do_jump(reg_struct, reg_struct->ProgramCounter + reg_dest);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_LE(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (!(reg_struct->Condition.Zero == 0 && reg_struct->Condition.Negative == reg_struct->Condition.Overflow))
        do_jump(reg_struct, reg_struct->ProgramCounter + reg_dest);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_VC(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Overflow == 0)
        do_jump(reg_struct, reg_struct->ProgramCounter + reg_dest);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_VS(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Overflow == 1)
        do_jump(reg_struct, reg_struct->ProgramCounter + reg_dest);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_PL(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Negative == 0)
        do_jump(reg_struct, reg_struct->ProgramCounter + reg_dest);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_NG(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;

    int dest_index = (instr.InstructionSpecific >> 14) & 0b111111;
    uint64_t reg_dest = *(Garand::load_reg(regs, dest_index));

    Garand::Registers* reg_struct = (Garand::Registers*) regs;

    if (reg_struct->Condition.Negative == 1)
        do_jump(reg_struct, reg_struct->ProgramCounter + reg_dest);

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
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::FX_AddImmediate(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    // TODO: Implement Instruction
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

    if (val == 0) {
        reg_struct->Condition.Zero = 1;
    } else {
        reg_struct->Condition.Zero = 0;
    }

    if (val < 0) {
        reg_struct->Condition.Negative = 1;
    } else {
        reg_struct->Condition.Negative = 0;
    }

    if (val_2 > val_1) {
        reg_struct->Condition.Carry = 1;
    } else {
        reg_struct->Condition.Carry = 0;
    }

    // TODO: Implement V flag (overflow) - not sure how
}

Garand::InstructionWriteBack Garand::InstructionSet::Compare(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;
    wb.write_back = false;

    int r1 = (instr.InstructionSpecific >> 14) & 0b111111;
    int r2 = (instr.InstructionSpecific >> 8) & 0b111111;

    uint32_t r1_val = *(Garand::load_reg(regs, r1));
    uint32_t r2_val = *(Garand::load_reg(regs, r2));

    uint32_t val = r2_val - r1_val;

    set_cflags(regs, val, r2_val, r1_val);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::CompareImmediate(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    Garand::InstructionWriteBack wb;
    wb.write_back = false;

    int r1 = (instr.InstructionSpecific >> 14) & 0b111111;
    int imm = (instr.InstructionSpecific >> 2) & 0b111111111111;

    uint32_t r1_val = *(Garand::load_reg(regs, r1));
    uint32_t imm_val = *(Garand::load_reg(regs, imm));

    uint32_t val = imm_val - r1_val;

    set_cflags(regs, val, imm_val, r1_val);

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::FX_Subtract(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::FX_SubtractImmediate(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    // TODO: Implement Instruction
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
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::FX_MultiplyImmediate(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::FX_MultiplyAdd(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
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
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::FX_DivideImmediate(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    // TODO: Implement Instruction
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
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::RotationalShiftRightImmediate(Garand::GarandInstruction instr, Garand::Memory &mem, uint64_t* regs) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
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

    #define CASE_INS(MNEMONIC) case MNEMONIC: return #MNEMONIC "..."
    switch(decoded_type) {
        CASE_INS(Garand::DecodedInstruction::MREAD);
        CASE_INS(Garand::DecodedInstruction::MWRITE);
        CASE_INS(Garand::DecodedInstruction::BIND);
        CASE_INS(Garand::DecodedInstruction::UNBIND);
        CASE_INS(Garand::DecodedInstruction::BRUHCC_AL);
        CASE_INS(Garand::DecodedInstruction::BRUHCC_EQ);
        CASE_INS(Garand::DecodedInstruction::BRUHCC_NE);
        CASE_INS(Garand::DecodedInstruction::BRUHCC_LO);
        CASE_INS(Garand::DecodedInstruction::BRUHCC_HS);
        CASE_INS(Garand::DecodedInstruction::BRUHCC_LT);
        CASE_INS(Garand::DecodedInstruction::BRUHCC_GE);
        CASE_INS(Garand::DecodedInstruction::BRUHCC_HI);
        CASE_INS(Garand::DecodedInstruction::BRUHCC_LS);
        CASE_INS(Garand::DecodedInstruction::BRUHCC_GT);
        CASE_INS(Garand::DecodedInstruction::BRUHCC_LE);
        CASE_INS(Garand::DecodedInstruction::BRUHCC_VC);
        CASE_INS(Garand::DecodedInstruction::BRUHCC_VS);
        CASE_INS(Garand::DecodedInstruction::BRUHCC_PL);
        CASE_INS(Garand::DecodedInstruction::BRUHCC_NG);
        CASE_INS(Garand::DecodedInstruction::BCC_AL);
        CASE_INS(Garand::DecodedInstruction::BCC_EQ);
        CASE_INS(Garand::DecodedInstruction::BCC_NE);
        CASE_INS(Garand::DecodedInstruction::BCC_LO);
        CASE_INS(Garand::DecodedInstruction::BCC_HS);
        CASE_INS(Garand::DecodedInstruction::BCC_LT);
        CASE_INS(Garand::DecodedInstruction::BCC_GE);
        CASE_INS(Garand::DecodedInstruction::BCC_HI);
        CASE_INS(Garand::DecodedInstruction::BCC_LS);
        CASE_INS(Garand::DecodedInstruction::BCC_GT);
        CASE_INS(Garand::DecodedInstruction::BCC_LE);
        CASE_INS(Garand::DecodedInstruction::BCC_VC);
        CASE_INS(Garand::DecodedInstruction::BCC_VS);
        CASE_INS(Garand::DecodedInstruction::BCC_PL);
        CASE_INS(Garand::DecodedInstruction::BCC_NG);
        CASE_INS(Garand::DecodedInstruction::ADD);
        CASE_INS(Garand::DecodedInstruction::ADDI);
        CASE_INS(Garand::DecodedInstruction::FX_ADD);
        CASE_INS(Garand::DecodedInstruction::FX_ADDI);
        CASE_INS(Garand::DecodedInstruction::SUB);
        CASE_INS(Garand::DecodedInstruction::SUBI);
        CASE_INS(Garand::DecodedInstruction::CMP);
        CASE_INS(Garand::DecodedInstruction::CMPI);
        CASE_INS(Garand::DecodedInstruction::FX_SUB);
        CASE_INS(Garand::DecodedInstruction::FX_SUBI);
        CASE_INS(Garand::DecodedInstruction::MUL);
        CASE_INS(Garand::DecodedInstruction::MULI);
        CASE_INS(Garand::DecodedInstruction::MADD);
        CASE_INS(Garand::DecodedInstruction::FX_MUL);
        CASE_INS(Garand::DecodedInstruction::FX_MULI);
        CASE_INS(Garand::DecodedInstruction::FX_MADD);
        CASE_INS(Garand::DecodedInstruction::DIV);
        CASE_INS(Garand::DecodedInstruction::DIVI);
        CASE_INS(Garand::DecodedInstruction::FX_DIV);
        CASE_INS(Garand::DecodedInstruction::FX_DIVI);
        CASE_INS(Garand::DecodedInstruction::AND);
        CASE_INS(Garand::DecodedInstruction::ANDI);
        CASE_INS(Garand::DecodedInstruction::TEST);
        CASE_INS(Garand::DecodedInstruction::NAND);
        CASE_INS(Garand::DecodedInstruction::NANDI);
        CASE_INS(Garand::DecodedInstruction::OR);
        CASE_INS(Garand::DecodedInstruction::ORI);
        CASE_INS(Garand::DecodedInstruction::XOR);
        CASE_INS(Garand::DecodedInstruction::XORI);
        CASE_INS(Garand::DecodedInstruction::LSL);
        CASE_INS(Garand::DecodedInstruction::LSLI);
        CASE_INS(Garand::DecodedInstruction::LSR);
        CASE_INS(Garand::DecodedInstruction::LSRI);
        CASE_INS(Garand::DecodedInstruction::RSR);
        CASE_INS(Garand::DecodedInstruction::RSRI);
        CASE_INS(Garand::DecodedInstruction::NOT);
        CASE_INS(Garand::DecodedInstruction::UNKNOWN);
        default:
            return "NOINFO";
    };
    return "ERR";
}
