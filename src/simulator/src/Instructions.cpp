#include "Instructions.hpp"
#include "Instruction.hpp"
#include "Memory.hpp"

#include <iostream>

Garand::InstructionWriteBack Garand::InstructionSet::MemoryRead(Garand::GarandInstruction instr, Garand::Memory mem, Garand::Registers regs) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    int dest = (instr.InstructionSpecific >> 15) & 0b1111111;
    int src = (instr.InstructionSpecific >> 8) & 0b1111111;
    int offset = (instr.InstructionSpecific >> 1) & 0b1111111;

    uint64_t *reg_src = (Garand::load_reg((uint64_t)(&regs), src));
    uint64_t *reg_offset = (Garand::load_reg((uint64_t)(&regs), offset));

    Garand::LoadSize *addr = mem.load(*reg_src + *reg_offset);

    wb.reg = (Garand::load_reg((uint64_t)(&regs), dest));
    wb.value = *addr;

    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::MemoryWrite(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::Bind(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::Unbind(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_AL(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_EQ(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_NE(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_LO(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_HS(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_LT(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_GE(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_HI(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_LS(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_GT(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_LE(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_VC(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_VS(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_PL(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BRUHCC_NG(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_AL(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_EQ(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_NE(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_LO(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_HS(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_LT(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_GE(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_HI(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_LS(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_GT(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_LE(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_VC(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_VS(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_PL(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::BCC_NG(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::Add(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::AddImmediate(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::FX_Add(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::FX_AddImmediate(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::Subtract(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::SubtractImmediate(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::Compare(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::CompareImmediate(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::FX_Subtract(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::FX_SubtractImmediate(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::Multiply(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::MultiplyImmediate(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::MultiplyAdd(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::FX_Multiply(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::FX_MultiplyImmediate(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::FX_MultiplyAdd(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::Divide(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::DivideImmediate(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::FX_Divide(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::FX_DivideImmediate(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::AND(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::ANDImmediate(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::Test(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::NAND(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::NANDImmediate(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::OR(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::ORImmediate(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::XOR(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::XORImmediate(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::LogicalShiftLeft(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::LogicalShiftLeftImmediate(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::LogicalShiftRight(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::LogicalShiftRightImmediate(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::RotationalShiftRight(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::RotationalShiftRightImmediate(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}

Garand::InstructionWriteBack Garand::InstructionSet::NOT(Garand::GarandInstruction instr, Garand::Memory mem) {
    // TODO: Implement Instruction
    Garand::InstructionWriteBack wb;
    return wb;
}
