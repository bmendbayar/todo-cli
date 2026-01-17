#pragma once

#include <string>

namespace todo {
struct UserInput {
    std::string text;   ///< User's raw string input.
    bool is_vi_mode;    ///< Flag for differentiating ViView input.
    bool is_cancelled;  ///< Flag for validating ViView input.
};
}  // namespace todo
