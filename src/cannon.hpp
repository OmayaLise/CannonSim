#pragma once

#include <imgui.h>
#include <list>
#include <vector>

#include "types.hpp"

struct CannonState
{
    float2 position;
    float angle;
    float initialSpeed;
};

struct Cannonball
{
    unsigned int id;
    float2 initPosition;
    float initSpeed;
    float angle;
    float time;
    float2 position;
    float speed;
    std::vector<float2> prevPos;
    float maxHeight = 0.f;
    float landingDistance = 0.f;
};

// Renderer class
// Used to load resources
class CannonRenderer
{
public:
    CannonRenderer();
    ~CannonRenderer();

    void PreUpdate();

    // Fast access to drawlist and io
    ImDrawList* dl;
    ImGuiIO* io;

    // World coordinates conversion (world is expressed in meters)
    float2 worldOrigin; // Origin in pixels
    float2 worldScale;  // Scale relative to pixels
    float2 ToPixels(float2 coordinatesInMeters);
    float2 ToWorld(float2 coordinatesInPixels);

    // Draw functions
    void DrawGround();
    void DrawCannon(const CannonState& cannon);
    void DrawProjectileMotion(const CannonState& cannon, const std::list<Cannonball>& cannonballs);
};

class CannonGame
{
public:
    CannonGame(CannonRenderer& renderer);
    ~CannonGame();

    std::list<Cannonball> cannonballs;
    unsigned int idCannonball;

    void UpdateAndDraw(float deltaTime);

private:
    CannonRenderer& renderer;
    CannonState cannonState;
};
