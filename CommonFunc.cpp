#include "CommonFunc.h"

bool CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2)
{
    int left_a = object1.x;
    int right_a = object1.x + object1.w;
    int top_a = object1.y;
    int bottom_a = object1.y + object1.h;

    int left_b = object2.x;
    int right_b = object2.x + object2.w;
    int top_b = object2.y;
    int bottom_b = object2.y + object2.h;

    int verticalOverlap = std::min(bottom_a, bottom_b) - std::max(top_a, top_b);
    const int verticalTolerance = 2;
    if(verticalOverlap < verticalTolerance)
        return false;

    // size object1 < object2
    if(left_a > left_b && left_a < right_b)
    {
        if(top_a > top_b && top_a < bottom_b)
        {
            return true;
        }
                if(bottom_a > top_b && bottom_a < bottom_b)
        {
            return true;
        }
    }
    if(right_a > left_b && right_a < right_b)
    {
        if(top_a > top_b && top_a < bottom_b)
        {
            return true;
        }
        if(bottom_a > top_b && bottom_a < bottom_b)
        {
            return true;
        }
    }
    // size object1 < size object2
    if (left_b > left_a && left_b < right_a)
    {
        if (top_b > top_a && top_b < bottom_a)
        {
        return true;
        }
        if (bottom_b > top_a && bottom_b < bottom_a)
        {
        return true;
        }
    }
    if (right_b > left_a && right_b < right_a)
    {
        if (top_b > top_a && top_b < bottom_a)
        {
            return true;
        }
        if (bottom_b > top_a && bottom_b < bottom_a)
        {
        return true;
        }
    }
    // size object1 = size object2
    if(top_a == top_b && right_a == right_b && bottom_a == bottom_b)
    {
        return true;
    }
    return false;
}
