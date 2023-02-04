

// Consigne, la modification du code doit seulement être au niveau du commentaire TODO
// 1. Changer la signature de la fonction lerp afin qu'elle soit une fonction template
// 2. Ajouter les opérateurs mathématiques nécessaires afin que le code ci-dessous fonctionne

#include <stdio.h>
#include <stdio.h>

struct float2
{
    float x;
    float y;
};

// Définition des surcharges d'opérateur pour le type float2
float2 operator*(const float2& a, float b)
{
    return {a.x * b, a.y * b};
}

float2 operator+(const float2& a, const float2& b)
{
    return {a.x + b.x, a.y + b.y};
}

template<typename T>
T lerp(T a, T b, float t)
{
    return a * (t-1) + b * t;
}

int main()
{
    const float t = 0.6f;
    {
        float a = 0.f;
        float b = 20.f;
        float c = lerp(a, b, t);

        printf("c = %.2f\n", c);
    }
    {
        float2 a = {0,0};
        float2 b = {10,20};

        float2 c = lerp(a, b, 0.6f);
        printf("c = { %.2f, %.2f}\n", c.x, c.y);
    }
}