#ifndef GARAND_REGISTERS_H
#define GARAND_REGISTERS_H

#include <ctypes.h>

namespace Garand {
    /*
        Registers layout:
        - R0 -> R15: General Purpose Registers. [0, 15]
        - I0 -> I15: IO registers. [16, 31]
        - LR, SP: Link Register, Stack Pointer, 
        Condition Register, Program Counter [32, 33]
    */
    constexpr int REGISTERS_GP_CNT = 0x10;
    constexpr int REGISTERS_IO_CNT = 0x10;

    struct ConditionFlag {
        uint8_t Zero: 1;
        uint8_t Negative: 1;
        uint8_t Carry: 1;
        uint8_t Overflow: 1;
        uint64_t Reserved: 60;
    };

    struct Registers {
        uint64_t GeneralPurpose[REGISTERS_GP_CNT];
        uint64_t IO[REGISTERS_IO_CNT];
        ConditionFlag Condition;
        uint64_t LinkRegister;
        uint64_t StackPointer;
        uint64_t ProgramCounter;
    };
} // namespace Garand
#endif