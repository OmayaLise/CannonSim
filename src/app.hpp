#pragma once

#include "cannon.hpp"

class App
{
public:
    App();
    ~App();

    void Update(float deltaTime);

private:
    CannonRenderer cannonRenderer;
    CannonGame cannonGame;
};
