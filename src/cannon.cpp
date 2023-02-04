#include <stdio.h>
#include <math.h>

#include "calc.hpp"
#include "cannon.hpp"

CannonRenderer::CannonRenderer()
{
    // Do not use ImDrawList or ImGuiIO here because they are invalid at this point
    // Use this function only to load resources (images, sounds) if needed
}

CannonRenderer::~CannonRenderer()
{
    // Unload resources if needed
}

void CannonRenderer::PreUpdate()
{
    // Get frequently used variables
    dl = ImGui::GetBackgroundDrawList();
    io = &ImGui::GetIO();

    // Compute world space
    // ===================
    // Origin:
    //  - x at the center of the window
    //  - y at the 3/4 bottom of the window
    // Scale:
    //  50 meters from left of the window to the right
    //  Uniform scale in y except it's pointing to up
    worldOrigin.x = io->DisplaySize.x / 2.f;
    worldOrigin.y = io->DisplaySize.y - io->DisplaySize.y / 4.f;
    worldScale.x  = io->DisplaySize.x / 50.f;
    worldScale.y  = -worldScale.x; // Same scale as X bu invert it to invert y coordinates
}

float2 CannonRenderer::ToPixels(float2 coordinatesInMeters)
{
    return (coordinatesInMeters * worldScale) + worldOrigin;
}

float2 CannonRenderer::ToWorld(float2 coordinatesInPixels)
{
    return (coordinatesInPixels - worldOrigin) / worldScale;
}

void CannonRenderer::DrawGround()
{
    float2 left  = this->ToPixels({ -100.f, 0.f });
    float2 right = this->ToPixels({ +100.f, 0.f });

    dl->AddLine(left, right, IM_COL32_WHITE);
}

void CannonRenderer::DrawCannon(const CannonState& cannon)
{
    // TODO: Draw cannon
    
    // For example (Remove this and do your own)
    float2 pos = this->ToPixels(cannon.position);
    dl->AddCircle(pos, 10.f, IM_COL32_WHITE);
}

void CannonRenderer::DrawProjectileMotion(const CannonState& cannon, const std::list<Cannonball>& cannonballs)
{
    for (const Cannonball& cannonball : cannonballs)
    {
        float2 pos = ToPixels(cannonball.position);
        dl->AddCircleFilled(pos, 5.f, IM_COL32_BLACK);

        for (int i = 0; i < cannonball.prevPos.size() - 1; ++i)
        {
            float2 lastPos = ToPixels(cannonball.prevPos[i]);
            float2 curPos = ToPixels(cannonball.prevPos[i + 1]);
            dl->AddLine({ lastPos.x, lastPos.y }, {curPos.x, curPos.y}, IM_COL32(255, 0, 0, 255));
        }
    }

    // TODO: Draw cannon projectile using ImDrawList
    // e.g. dl->AddLine(...)
    // e.g. dl->PathLineTo(...)
    // etc...
}

CannonGame::CannonGame(CannonRenderer& renderer)
    : renderer(renderer)
    , idCannonball(0)
{
    cannonState.position.x = -15.f;
    cannonState.angle = 45.f;
    cannonState.initialSpeed = 15.f;
}

CannonGame::~CannonGame()
{

}

void CannonGame::UpdateAndDraw(float deltaTime)
{
    renderer.PreUpdate();

    if (ImGui::Begin("Canon state", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        // TODO: Add UI to edit other cannon state variables here
        ImGui::SliderFloat("CannonHeight", &cannonState.position.y, 0.f, 15.f);
        ImGui::SliderFloat("CannonAngle", &cannonState.angle, 0.f, 89.999f);
        ImGui::SliderFloat("Initial Speed", &cannonState.initialSpeed, 0.5f, 25.f);
        if (ImGui::Button("Fire !", ImVec2(300.f, 20.f)))
        {
            cannonballs.push_back(Cannonball{ idCannonball, cannonState.position, cannonState.initialSpeed, cannonState.angle * TAU / 360.f, 0.f });
            idCannonball++;
            if (cannonballs.size() > 5)
                cannonballs.pop_front();
        }
    }
    ImGui::End();

    if (ImGui::Begin("Stats", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        for (const Cannonball& cannonball : cannonballs)
        {
            ImGui::Text("MaxHeight %d %.2fm", cannonball.id, cannonball.maxHeight);
            ImGui::Text("FlyingTime %d %.2fs", cannonball.id, cannonball.time);
            ImGui::Text("MaxDistance %d %.2fm", cannonball.id, cannonball.landingDistance);
        }
    }
    ImGui::End();

    for (Cannonball& cannonball : cannonballs)
    {
        if (cannonball.landingDistance != 0.f)
            continue;

        if (cannonball.position.y < 0.f)
        {
            cannonball.landingDistance = cannonball.position.x - cannonball.initPosition.x;
            cannonball.position.y = 0.f;
            continue;
        }
        cannonball.position.x = cannonball.initSpeed * cosf(cannonball.angle) * cannonball.time + cannonball.initPosition.x;
        cannonball.position.y = -GRAVITY / 2 * cannonball.time * cannonball.time + cannonball.initSpeed * sinf(cannonball.angle) * cannonball.time + cannonball.initPosition.y;
        cannonball.prevPos.push_back(cannonball.position);
        if (cannonball.position.y > cannonball.maxHeight)
            cannonball.maxHeight = cannonball.position.y;
        cannonball.time += deltaTime;
    }

    // Draw cannon
    renderer.DrawGround();
    renderer.DrawCannon(cannonState);
    renderer.DrawProjectileMotion(cannonState, cannonballs);
}
