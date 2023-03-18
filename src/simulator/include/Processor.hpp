#include "Registers.hpp"

#ifndef GARAND_PROCESSOR_HPP
#define GARAND_PROCESSOR_HPP

namespace Garand {
    class Processor {
        private:
            Registers WkRegs;
            uint8_t *Program;

        void Execute();

        public:
            Processor(void *Program) : Program(Program);
            void Tick();
    };
} // namespace Garand

#endif