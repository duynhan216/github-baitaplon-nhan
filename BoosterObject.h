

#ifndef BOOSTER_OBJECT_H_
#define BOOSTER_OBJECT_H_

#include "BaseObject.h"
#include "CommonFunc.h"
#include "game_map.h"

class BoosterObject : public BaseObject
{
public:
    BoosterObject();
    ~BoosterObject();

    bool LoadImg(std::string path, SDL_Renderer* screen);

    void set_clips();

    void Show(SDL_Renderer* des);

    void RandomSpawn(Map& map_data);

    void set_x_pos(const int& x) { x_pos_ = x; }
    void set_y_pos(const int& y) { y_pos_ = y; }
    int get_x_pos() const { return x_pos_; }
    int get_y_pos() const { return y_pos_; }
    SDL_Rect GetRectFrame();


    void SetMapXY(const int map_x, const int map_y) { map_x_ = map_x; map_y_ = map_y; }


private:
    int x_pos_;
    int y_pos_;

    static const int NUM_FRAME_BOOSTER = 8;

    SDL_Rect frame_clip_[NUM_FRAME_BOOSTER];

    int width_frame_;
    int height_frame_;

    int frame_;

    int map_x_;
    int map_y_;
};

#endif
