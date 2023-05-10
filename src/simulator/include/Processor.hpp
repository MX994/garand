#include "Instructions.hpp"
#include "Instruction.hpp"
#include "Registers.hpp"
#include <queue>
#include <memory>
#include <array>
#include <optional>

#ifndef GARAND_PROCESSOR_HPP
#define GARAND_PROCESSOR_HPP

constexpr int PIPELINE_STAGES = 4;

namespace Garand {
  using Cycle = uint64_t;
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
    AddressSize Pointer;
    // Fix the wrong offset of PC on relative branching
    AddressSize StagnatePCDiff = 0;
    Cycle CycleCounter = 0;
    Cycle CycleNeeded[PIPELINE_STAGES] = {0};
    bool Processed = false;
  };
  
  class Processor {
    private:
      Memory WkRAM;
      Registers WkRegs;
      std::shared_ptr<InstructionWk> Pipeline[PIPELINE_STAGES];
      std::queue<InstructionWk> InstructionQueue;
      uint64_t Clock = 0;
      AddressSize InstructionCounter = 0;

      std::optional<Cycle> Fetch();
      std::optional<Cycle> Decode();
      std::optional<Cycle> Execute();
      std::optional<Cycle> WriteBack();
      void Tick();
      void Flush(Stage);

    public:
      void Step();
      void Queue(GarandInstruction);
      decltype(Pipeline) &View();
      decltype(WkRegs) const &Regs();
      void ResetRegs();
      decltype(Clock) ReadClock();
      Memory& ReadMem();
      AddressSize ReadExecPC();
      // By default, this processor implement Pipeline.
      // Set this to Off to disable pipeline,
      // where Fetch only works when the pipeline is empty
      bool pipeline = true;
  };
} // namespace Garand

#endif