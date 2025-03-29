#include "ThreatsObject.h"

unsigned int ThreatsObject::game_start_time_ = 0;


ThreatsObject::ThreatsObject()
{
    width_frame_ = 0;
    height_frame_ = 0;
    x_val_ = 0;
    y_val_ = 0;
    x_pos_ = 0;
    y_pos_ = 0;
    frame_ = 0;
    on_ground_ = false;
    map_x_ = 0;
    map_y_ = 0;
    threat_type_ = THREAT_GROUND;
}

ThreatsObject::~ThreatsObject()
{

}

bool ThreatsObject::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = BaseObject::LoadImg(path, screen);
    if(ret == true)
    {
        width_frame_ = rect_.w / THREAT_FRAME_NUM;
        height_frame_ = rect_.h;
    }
    return ret;
}

SDL_Rect ThreatsObject::GetRectFrame()
{
    SDL_Rect rect;
    rect.x = rect_.x;
    rect.y = rect_.y;
    rect.w = width_frame_;
    rect.h = height_frame_;
    return rect;
}

void ThreatsObject::set_clips()
{
    if(width_frame_ > 0 && height_frame_ > 0)
    {
        for (int i = 0; i < THREAT_FRAME_NUM; i++)
        {
            frame_clip_[i].x = i * width_frame_;
            frame_clip_[i].y = 0;
            frame_clip_[i].w = width_frame_;
            frame_clip_[i].h = height_frame_;
        }
    }
}

void ThreatsObject::Show(SDL_Renderer* des)
{
    rect_.x = x_pos_ - map_x_;
    rect_.y = y_pos_ - map_y_;

    frame_++;
    if(frame_ >= THREAT_FRAME_NUM)
    {
        frame_ = 0;
    }
    SDL_Rect* currentClip = &frame_clip_[frame_];
    SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_, height_frame_ };
    SDL_RenderCopy(des, p_object_, currentClip, &renderQuad);
}

void ThreatsObject::DoPlayer(Map& map_data)
{
    unsigned int current_time = SDL_GetTicks();
    unsigned int elapsed_time = current_time - game_start_time_;
    int cycles = elapsed_time / SPEED_UP_INTERVAL;
    float speed_multiplier = 1.0f + SPEED_INCREASE_FACTOR * cycles;
    if(speed_multiplier > MAX_INCREASE_THREAT)
    {
        speed_multiplier = MAX_INCREASE_THREAT;
    }
    int current_ground_speed = static_cast<int>(SPEED_THREAT_GROUND* speed_multiplier);
    int current_flying_speed = static_cast<int>(SPEED_THREAT_FLYING* speed_multiplier);

    if(threat_type_ == THREAT_GROUND)

    {
        x_val_ = -current_ground_speed;
        CheckToMap(map_data);
    }
    else if(threat_type_ == THREAT_FLYING)
    {
        x_val_ = -current_flying_speed;
        y_val_ = 0;
        x_pos_ += x_val_;
        y_pos_ += y_val_;
    }
}

void ThreatsObject::CheckToMap(Map& map_data)
{
    int x1 = 0;
    int x2 = 0;
    int y1 = 0;
    int y2 = 0;

    int height_min = (height_frame_ < TILE_SIZE) ? height_frame_ : TILE_SIZE;
    x1 = (x_pos_ + x_val_) / TILE_SIZE;
    x2 = (x_pos_ + x_val_ + width_frame_ - 1) / TILE_SIZE;
    y1 = (y_pos_) / TILE_SIZE;
    y2 = (y_pos_ + height_min - 1) / TILE_SIZE;

    if(x1 >= 0 && x2 < map_data.max_x_ && y1 >= 0 && y2 < map_data.max_y_)
    {
        if(x_val_ < 0)
        {
            if(map_data.tile[y1][x1] != BLANK_TILE || map_data.tile[y2][x1] != BLANK_TILE)
            {
                x_pos_ = (x1 + 1) * TILE_SIZE;
                x_val_ = 0;
            }
        }
    }

    int width_min = (width_frame_ < TILE_SIZE) ? width_frame_ : TILE_SIZE;
    x1 = x_pos_ / TILE_SIZE;
    x2 = (x_pos_ + width_min) / TILE_SIZE;
    y1 = (y_pos_ + y_val_) / TILE_SIZE;
    y2 = (y_pos_ + y_val_ + height_frame_ - 1) / TILE_SIZE;

    if(x1 >= 0 && x2 < map_data.max_x_ && y1 >= 0 && y2 < map_data.max_y_)
    {
        if(y_val_ > 0)
        {
            if(map_data.tile[y2][x1] != BLANK_TILE || map_data.tile[y2][x2] != BLANK_TILE)
            {
                y_pos_ = y2 * TILE_SIZE - (height_frame_ + 1);
                y_val_ = 0;
                on_ground_ = true;
            }
        }
        else if(y_val_ < 0)
        {
            if(map_data.tile[y1][x1] != BLANK_TILE || map_data.tile[y1][x2] != BLANK_TILE)
            {
                y_pos_ = (y1 + 1) * TILE_SIZE;
                y_val_ = 0;
            }
        }
    }

    x_pos_ += x_val_;
    y_pos_ += y_val_;

    if(x_pos_ < 0)
    {
        x_pos_ = 0;
    }
    else if(x_pos_ + width_frame_ > map_data.max_x_)
    {
        x_pos_ = map_data.max_x_ - width_frame_ - 1;
    }
}

void ThreatsObject::SpawnRandom(Map& map_data, ThreatType t)
{
    threat_type_ = t;

    x_pos_ = map_data.max_x_ - width_frame_;
    x_pos_ += 100;

    if(threat_type_ == THREAT_GROUND)
    {
        y_pos_ = TILE_SIZE*7;
    }
    else
    {
        int minY = TILE_SIZE*4;
        int maxY = TILE_SIZE*6;
        y_pos_ = minY + rand() % (maxY - minY + 1);
    }

    x_val_ = 0;
    y_val_ = 0;
    frame_ = 0;
}

