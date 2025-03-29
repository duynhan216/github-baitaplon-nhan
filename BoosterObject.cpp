
#include "BoosterObject.h"

BoosterObject::BoosterObject()
{
    x_pos_ = 0;
    y_pos_ = 0;
    width_frame_ = 0;
    height_frame_ = 0;
    frame_ = 0;
    map_x_ = 0;
    map_y_ = 0;
}

BoosterObject::~BoosterObject()
{

}

bool BoosterObject::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = BaseObject::LoadImg(path, screen);
    if(ret)
    {
        width_frame_ = rect_.w / NUM_FRAME_BOOSTER;
        height_frame_ = rect_.h;
    }
    return ret;
}

SDL_Rect BoosterObject::GetRectFrame()
{
    SDL_Rect rect = { x_pos_ - map_x_, y_pos_ - map_y_, width_frame_, height_frame_ };
    return rect;
}

void BoosterObject::set_clips()
{
    for(int i = 0; i < NUM_FRAME_BOOSTER; i++)
    {
        frame_clip_[i].x = i * width_frame_;
        frame_clip_[i].y = 0;
        frame_clip_[i].w = width_frame_;
        frame_clip_[i].h = height_frame_;
    }
}

void BoosterObject::Show(SDL_Renderer* des)
{
    frame_++;
    if(frame_ >= NUM_FRAME_BOOSTER)
    {
        frame_ = 0;
    }
    SDL_Rect* currentClip = &frame_clip_[frame_];
    SDL_Rect renderQuad = { x_pos_ - map_x_, y_pos_ - map_y_, width_frame_, height_frame_ };
    SDL_RenderCopy(des, p_object_, currentClip, &renderQuad);
}


void BoosterObject::RandomSpawn(Map& map_data)
{
    int maxPosX = map_data.max_x_ - width_frame_;
    x_pos_ = 1 + rand() % (maxPosX / TILE_SIZE) * TILE_SIZE;


    int minY = TILE_SIZE*4;
    y_pos_ = minY + (rand() % (7-4) * TILE_SIZE);
}
