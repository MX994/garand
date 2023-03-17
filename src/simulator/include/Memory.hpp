#ifndef GARAND_MEMORY_HPP
#define GARAND_MEMORY_HPP

namespace Garand {
    class Memory {
        private:
            // Placeholders for now
            long size;
            long *memory_region;

        public:
            // These two are placeholders for now
            long *load();
            void store();

    };

    class Cache : Memory {
        private:
            
    }
}

#endif