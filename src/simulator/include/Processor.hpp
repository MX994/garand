#include "Instructions.hpp"
#include "Instruction.hpp"
#include "Registers.hpp"
#include <queue>
#include <memory>

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
    GarandInstruction Instruction;
    DecodedInstruction DecodedInstruction;
    InstructionWriteBack WriteBack;
    unsigned int CycleCount;
    unsigned int CycleMax[PIPELINE_STAGES];
  };
  
  class Processor {
    private:
      Memory WkRAM;
      Registers WkRegs;
      uint8_t *Program;
      std::unique_ptr<InstructionWk> Pipeline[PIPELINE_STAGES];
      std::queue<InstructionWk> InstructionQueue;
      uint64_t Clock = 0;

      void Fetch();
      void Decode();
      void Execute();
      void WriteBack();
      void Tick();

    public:
      Processor(uint8_t *Program) : Program(Program) {};
      void Step();
      void Queue(GarandInstruction);
  };
} // namespace Garand

#endif