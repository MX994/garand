#ifndef GARAND_INSTRUCTIONS_HPP
#define GARAND_INSTRUCTIONS_HPP

namespace Garand {
    class InstructionSet {
        public:
            void MemoryRead();
            void MemoryWrite();
            void Bind();
            void BRUHCC();
            void Add();
            void Multiply();
            void Divide();
            void AND();
            void NAND();
            void OR();
            void XOR();
            void NOT();
            void ArithmeticShiftRight();
            void LogicalShiftRight();
            void LogicalShiftLeft();
            void RotationalShiftRight();
            void RotationalShiftLeft();
            void Break();
            void Halt();
    };

    enum Instruction {
        MREAD,
        MRITE,
        BIND,
        BRUHCC,
        ADD,
        MUL,
        DIV,
        AND,
        NAND,
        OR,
        XOR,
        NOT,
        ASR,
        LSR,
        LSL,
        RSR,
        RSL,
        BREAK,
        HALT
    };
}

#endif