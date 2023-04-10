#include "Instructions.hpp"
#include "Registers.hpp"

#ifndef GARAND_PROCESSOR_HPP
#define GARAND_PROCESSOR_HPP

constexpr int PIPELINE_STAGES = 4;

namespace Garand {
  enum Stage { 
      FETCH, 
      DECODE, 
      EXECUTE,
      WRITE_BACK
  };

  struct MemoryWk {
      uint8_t Source;
      uint32_t CyclesRemaining;
  };

  struct InstructionWk {
    GarandInstruction *Instruction;
    DecodedInstruction DecodedInstruction;
    unsigned int CycleCount;
    unsigned int CycleMax[PIPELINE_STAGES];
  };
  
  class Processor {
    private:
      Memory WkRAM;
      Registers WkRegs;
      uint8_t *Program;
      InstructionWk Pipeline[PIPELINE_STAGES];

      void Fetch();
      void Decode();
      void Execute();

    public:
      Processor(void *Program) : Program(Program);
      void Tick();
  };
} // namespace Garand

#endif