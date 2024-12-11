#pragma once

#include <chrono>

namespace Game {
    struct Player {
        std::chrono::time_point<std::chrono::system_clock> lastJump = std::chrono::system_clock::now();

        bool CanJump() {
            auto now = std::chrono::system_clock::now();
            auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastJump).count();
            return diff >= 800;
        }

        void Jump() {
            CanJump();

            lastJump = std::chrono::system_clock::now();
        }
    };
}
