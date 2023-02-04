#pragma once

#include <imgui.h>

struct float2
{
    float x;
    float y;

    // Cast operator
    operator ImVec2() { return { x, y }; }
};
