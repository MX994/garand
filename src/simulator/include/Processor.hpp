#include "Instructions.hpp"
#include "Instruction.hpp"
#include "Registers.hpp"
#include <queue>
#include <memory>
#include <array>

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

  struct InstructionWk {
    GarandInstruction Instruction;
    DecodedInstruction decodedInstruction;
    InstructionWriteBack WriteBack;
    unsigned int CycleCount;
    unsigned int CycleMax[PIPELINE_STAGES];
  };
  
  class Processor {
    private:
      Memory WkRAM;
      Registers WkRegs;
      std::shared_ptr<InstructionWk> Pipeline[PIPELINE_STAGES];
      std::queue<InstructionWk> InstructionQueue;
      uint64_t Clock = 0;

      void Fetch();
      void Decode();
      void Execute();
      void WriteBack();
      void Tick();
      void ExecuteMemload();
      void Flush(Stage);

    public:
      void Step();
      void Queue(GarandInstruction);
      decltype(Pipeline) &View();
      decltype(WkRegs) const &Regs();
      void ResetRegs();
      decltype(Clock) ReadClock();
      Memory& ReadMem();
  };
} // namespace Garand

#endif