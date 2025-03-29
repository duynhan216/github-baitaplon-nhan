
#include "BulletObject.h"

BulletObject::BulletObject()
{
    is_move_ = false;

    bullet_speed_ = 50;
}

BulletObject::~BulletObject()
{

}

void BulletObject::HandleMove(const int& x_border,const int& y_border)
{
    if(bullet_dir_ == DIR_RIGHT)
    {
        rect_.x += bullet_speed_;
        if(rect_.x > x_border)
        {
            is_move_ = false;
        }
    }
    else if(bullet_dir_ == DIR_LEFT)
    {
        rect_.x -= bullet_speed_;
        if(rect_.x < 0)
        {
            is_move_ = false;
        }
    }
}

