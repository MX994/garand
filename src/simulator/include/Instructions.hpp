#ifndef GARAND_INSTRUCTIONS_HPP
#define GARAND_INSTRUCTIONS_HPP

namespace Garand {
    class InstructionSet {
        public:
            void MemoryRead();
            void MemoryWrite();
            void Bind();
            void Unbind();
            void BRUHCC_AL();
            void BRUHCC_EQ();
            void BRUHCC_NE();
            void BRUHCC_LO();
            void BRUHCC_HS();
            void BRUHCC_LT();
            void BRUHCC_GE();
            void BRUHCC_HI();
            void BRUHCC_LS();
            void BRUHCC_GT();
            void BRUHCC_LE();
            void BRUHCC_VC();
            void BRUHCC_VS();
            void BRUHCC_PL();
            void BRUHCC_NG();
            void BCC_AL();
            void BCC_EQ();
            void BCC_NE();
            void BCC_LO();
            void BCC_HS();
            void BCC_LT();
            void BCC_GE();
            void BCC_HI();
            void BCC_LS();
            void BCC_GT();
            void BCC_LE();
            void BCC_VC();
            void BCC_VS();
            void BCC_PL();
            void BCC_NG();
            void Add();
            void AddImmediate();
            void FX_Add();
            void FX_AddImmediate();
            void Subtract();
            void SubtractImmediate();
            void Compare();
            void CompareImmediate();
            void FX_Subtract();
            void FX_SubtractImmediate();
            void Multiply();
            void MultiplyImmediate();
            void MultiplyAdd();
            void FX_Multiply();
            void FX_MultiplyImmediate();
            void FX_MultiplyAdd();
            void Divide();
            void DivideImmediate();
            void FX_Divide();
            void FX_DivideImmediate();
            void AND();
            void ANDImmediate();
            void Test();
            void NAND();
            void NANDImmediate();
            void OR();
            void ORImmediate();
            void XOR();
            void XORImmediate();
            void LogicalShiftLeft();
            void LogicalShiftLeftImmediate();
            void LogicalShiftRight();
            void LogicalShiftRightImmediate();
            void RotationalShiftRight();
            void RotationalShiftRightImmediate();
            void NOT();
    };
}

#endif