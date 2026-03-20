#pragma once

#include <chrono>

// Xorshift128+ - simple and fast pseudo-rng
class FastRng {
public:
    FastRng(uint64_t seed = 0) {
        if (seed == 0) {
            seed = std::chrono::steady_clock::now().time_since_epoch().count();
        }
        state_[0] = seed;
        state_[1] = seed ^ 0x5deece66dULL;
        for (int i = 0; i < 10; ++i) next(); // warmup
    }

    uint32_t next() {
        uint64_t s1 = state_[0];
        const uint64_t s0 = state_[1];
        const uint64_t result = s0 + s1;
        state_[0] = s0;
        s1 ^= s1 << 23;
        state_[1] = s1 ^ s0 ^ (s1 >> 17) ^ (s0 >> 26);
        return static_cast<uint32_t>(result);
    }

    uint32_t range(uint32_t min, uint32_t max) {
        return min + next() % (max - min + 1);
    }

private:
    uint64_t state_[2];
};
