#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "Processor.hpp"

namespace Garand {
    void Processor::Tick() { 
        Execute();
        Clock += 1;
    }

    /**
     * Single stage pipeline for now
     * TODO: Split into multiple stages
     */
    void Processor::Execute() {
        
    }
} // namespace Garand