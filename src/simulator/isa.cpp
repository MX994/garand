#include "IO/IO.h"
#include "REX/Interpreter.h"
#include "System/Mjolnir.h"


namespace Garand {
    
// uint16_t WKRAMAlloc(uint16_t Size) {
//     // Find free WKRAM.
//     uint16_t WKRAMPtr = 0;
//     while (WKRAM_MAP.count(WKRAMPtr)) {
//         if (WKRAMPtr > MAX_WKRAM_SIZE) {
//             // Cannot allocate any WKRAM; return.
//             return -1;
//         }
//         WKRAMPtr += WKRAM_MAP[WKRAMPtr];
//     }
//     // Found WKRAM; add to the entry.
//     WKRAM_MAP.insert({WKRAMPtr, Size});

//     for (int i = WKRAMPtr; i < WKRAMPtr + Size; ++i) {
//         // Init with all 0.
//         WKRAM[i] = 0;
//     }
//     return WKRAMPtr;
// }

// void WKRAMFree(uint16_t Ptr) {
//     if (!WKRAM_MAP.count(Ptr)) {
//         return;
//     }
//     WKRAM_MAP.erase(Ptr);
// }

// INTERPRETER





// void CompareSetCondReg(uint16_t LHS, uint16_t RHS, uint8_t Compare, uint16_t *Condition) {
//     switch (Compare) {
//         case LT:
//             *Condition = LHS < RHS; 
//             break;
//         case GT:
//             *Condition = LHS > RHS;
//             break;
//         case LTEQ:
//             *Condition = LHS <= RHS;
//             break;
//         case GTEQ:
//             *Condition = LHS >= RHS;
//             break;
//         case EQ:
//             *Condition = LHS == RHS;
//             break;
//         case NEQ:
//             *Condition = LHS != RHS;
//             break;
//     }
// }

}
