#pragma once

#include <imgui.h>

struct Texture
{
    ImTextureID id;
    int width;
    int height;
};

class ImGuiUtils
{
public:
    static Texture LoadTexture(const char* file);
    static void UnloadTexture(Texture texture);

    void DrawTextureEx(Texture tex, ImVec2 pos, ImVec2 scale, float angle);
};
