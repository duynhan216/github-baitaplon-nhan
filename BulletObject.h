
#ifndef BULLET_OBJECT_H_
#define BULLET_OBJECT_H_

#include "BaseObject.h"
#include "CommonFunc.h"

class BulletObject : public BaseObject
{
public:
    BulletObject();
    ~BulletObject();

    enum BulletDir
    {
        DIR_RIGHT = 20,
        DIR_LEFT = 21,
    };

    void set_is_move(const bool& isMove) {is_move_ = isMove;}
    bool get_is_move() const {return is_move_;}

    void HandleMove(const int& x_border, const int& y_border);
    void set_bullet_dir(const unsigned int& bulletDir) {bullet_dir_ = bulletDir;}
    int get_bullet_dir() const {return bullet_dir_;}

private:
    bool is_move_;
    unsigned int bullet_dir_;
    int bullet_speed_;

};

#endif // BULLET_OBJECT_H_
