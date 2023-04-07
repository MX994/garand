#include <iostream>
#include "Instruction.hpp"

Garand::Instruction Garand::Instruction::Encode() {
    return Garand::Instruction();
}

void Garand::Instruction::Decode(GarandInstruction instruction) {
    std::cout << "Decode" << std::endl;
    switch (instruction.Operation)
    {
        // Memory OPs
        case 0x0:
            switch (instruction.ConditionFlags)
            {
                // MREAD
                case 0x0:
                    // TODO
                    break;
                // MWRITE
                case 0x1:
                    // TODO
                    break;
                // BIND
                case 0x2:
                    // TODO
                    break;
                // UNBIND
                case 0x3:
                    // TODO
                    break;
                default:
                    // TODO
                    break;
            }
            std::cout << "Decode" << std::endl;
            break;
        // BRUH.CC
        case 0x2:
            // TODO
            break;
        // B.CC
        case 0x3:
            // TODO
            break;
        // ADDs
        case 0x4:
            switch (instruction.ConditionFlags)
            {
                // ADD
                case 0x0:
                    // TODO
                    break;
                // ADDI
                case 0x1:
                    // TODO
                    break;
                // FX_ADD
                case 0x8:
                    // TODO
                    break;
                // FX_ADDI
                case 0x9:
                    // TODO
                    break;
                default:
                    // TODO
                    break;
            }
            break;
        // SUBs
        case 0x5:
            switch (instruction.ConditionFlags)
            {
                // SUB
                case 0x0:
                    // TODO
                    break;
                // SUBI
                case 0x1:
                    // TODO
                    break;
                // CMP
                case 0x2:
                    // TODO
                    break;
                // CMPI
                case 0x3:
                    // TODO
                    break;
                // FX_SUB
                case 0x8:
                    // TODO
                    break;
                // FX_SUBI
                case 0x9:
                    // TODO
                    break;
                default:
                    // TODO
                    break;
            }
            break;
        // MULs
        case 0x6:
            switch (instruction.ConditionFlags)
            {
                // MUL
                case 0x0:
                    // TODO
                    break;
                // MULI
                case 0x1:
                    // TODO
                    break;
                // MADD
                case 0x4:
                    // TODO
                    break;
                // FX_MUL
                case 0x8:
                    // TODO
                    break;
                // FX_MULI
                case 0x9:
                    // TODO
                    break;
                // FX_MADD
                case 0xC:
                    // TODO
                    break;
                default:
                    // TODO
                    break;
            }
            break;
        // DIVs
        case 0x7:
            switch (instruction.ConditionFlags)
            {
                // DIV
                case 0x0:
                    // TODO
                    break;
                // MULI
                case 0x1:
                    // TODO
                    break;
                // FX_DIV
                case 0x8:
                    // TODO
                    break;
                // FX_MULI
                case 0x9:
                    // TODO
                    break;
                default:
                    // TODO
                    break;
            }
            break;
        // ANDs
        case 0x8:
            switch (instruction.ConditionFlags)
            {
                // AND
                case 0x0:
                    // TODO
                    break;
                // ANDI
                case 0x1:
                    // TODO
                    break;
                // TEST
                case 0x2:
                    // TODO
                    break;
                default:
                    // TODO
                    break;
            }
            break;
        // NANDs
        case 0x9:
            switch (instruction.ConditionFlags)
            {
                // NAND
                case 0x0:
                    // TODO
                    break;
                // NANDI
                case 0x1:
                    // TODO
                    break;
                default:
                    // TODO
                    break;
            }
            break;
        // ORs
        case 0xA:
            switch (instruction.ConditionFlags)
            {
                // OR
                case 0x0:
                    // TODO
                    break;
                // ORI
                case 0x1:
                    // TODO
                    break;
                default:
                    // TODO
                    break;
            }
            break;
        // XORs
        case 0xB:
            switch (instruction.ConditionFlags)
            {
                // XOR
                case 0x0:
                    // TODO
                    break;
                // XORI
                case 0x1:
                    // TODO
                    break;
                default:
                    // TODO
                    break;
            }
            break;
        // Shifts
        case 0xC:
            switch (instruction.ConditionFlags)
            {
                // LSL
                case 0x0:
                    // TODO
                    break;
                // LSLI
                case 0x1:
                    // TODO
                    break;
                // LSR
                case 0x2:
                    // TODO
                    break;
                // LSRI
                case 0x3:
                    // TODO
                    break;
                // RSR
                case 0x4:
                    // TODO
                    break;
                // RSRI
                case 0x5:
                    // TODO
                    break;
                default:
                    // TODO
                    break;
            }
            break;
        // NOT
        case 0xF:
            switch (instruction.ConditionFlags)
            {
                // NOT
                case 0x0:
                    // TODO
                    break;
                default:
                    // TODO
                    break;
            }
            break;


        default:
            std::cout << "Unkown Operation" << std::endl;
    }
}