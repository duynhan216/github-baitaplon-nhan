
#include "MainObject.h"

#define SCORE_TIME_FACTOR 1000
#define SHIELD_DURATION 5000
#define BULLET_COOLDOWN 3000
#define SHIELD_COOLDOWN 10000

extern bool is_game_paused;
extern ImpTimer fps_timer;

MainObject::MainObject()
{
    last_bullet_time_ = 0;
    frame_ = 0;
    x_pos_ = 0;
    y_pos_ = 0;
    x_val_ = 0;
    y_val_ = 0;
    width_frame_ = 0;
    height_frame_ = 0;
    status_ = WALK_NONE;
    input_type_.right_ = 0;
    input_type_.left_ = 0;
    input_type_.jump_ = 0;
    input_type_.up_ = 0;
    map_x_ = 0;
    map_y_ = 0;

    come_back_time_ = 0;

    start_time_ = SDL_GetTicks();

    shield_active_ = false;
    shield_start_time_ = 0;
    shield_on_cooldown_ = false;
    shield_cooldown_start_time_ = 0;
    shield_texture_ = NULL;
    shield_frame_ = 0;

    health_ = NUM_HP;
    score_ = 0;
    score_accumulator_ = 0;

}

MainObject::~MainObject()
{

}

bool MainObject::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = BaseObject::LoadImg(path, screen);
    if(ret == true)
    {
        width_frame_ = rect_.w/MAX_FRAME;
        height_frame_ = rect_.h;
    }
    return ret;
}

SDL_Rect MainObject::GetRectFrame()
{
    SDL_Rect rect;
    rect.x = rect_.x;
    rect.y = rect_.y;
    rect.w = width_frame_;
    rect.h = height_frame_;
    return rect;
}

void MainObject::set_clips()
{
    if(width_frame_>0 && height_frame_>0)
    {
        for(int i=0;i<MAX_FRAME;i++)
        {
            frame_clip_[i].x = i*width_frame_;
            frame_clip_[i].y = 0;
            frame_clip_[i].w = width_frame_;
            frame_clip_[i].h = height_frame_;
        }
    }
}

bool MainObject::LoadShieldImg(std::string path, SDL_Renderer* screen)
{
    if(shield_texture_ != NULL)
    {
        SDL_DestroyTexture(shield_texture_);
        shield_texture_ = NULL;
    }
    shield_texture_ = IMG_LoadTexture(screen, path.c_str());
    if(shield_texture_ == NULL)
    {
        return false;
    }

    int w, h;
    SDL_QueryTexture(shield_texture_, NULL, NULL, &w, &h);

    shield_frame_width_ = w / MAX_SHIELD_FRAME ;
    shield_frame_height_ = h;

    for(int i = 0; i < MAX_SHIELD_FRAME; i++)
    {
        shield_clip_[i].x = i * shield_frame_width_;
        shield_clip_[i].y = 0;
        shield_clip_[i].w = shield_frame_width_;
        shield_clip_[i].h = shield_frame_height_;
    }
    shield_frame_ = 0;

    return true;
}

void MainObject::UpdateShield()
{
    if(shield_active_)
    {
        unsigned elapsed = SDL_GetTicks() - shield_start_time_;
        if(elapsed >= SHIELD_DURATION)
        {
            shield_active_ = false;
            shield_on_cooldown_ = true;
            shield_cooldown_start_time_ = SDL_GetTicks();
        }
        else
        {
            unsigned frame_interval = 500 / MAX_SHIELD_FRAME;
            shield_frame_ = (elapsed / frame_interval) % MAX_SHIELD_FRAME;
        }
    }
    else
    {
        if (shield_on_cooldown_)
        {
            unsigned current_time = SDL_GetTicks();
            if (current_time - shield_cooldown_start_time_ >= SHIELD_COOLDOWN)
            {
                shield_on_cooldown_ = false;
            }
        }
    }
}

void MainObject::ActivateShield()
{
    if (!shield_active_ && !shield_on_cooldown_)
    {
        shield_active_ = true;
        shield_start_time_ = SDL_GetTicks();
    }
}


void MainObject::Show(SDL_Renderer* des)
{
    if(on_ground_)
    {
        UpdateImagePlayer(des);

        if(input_type_.right_ == 1)
        {
            LoadImg("img//right.png", des);
        }
        else if(input_type_.left_ == 1)
        {
            LoadImg("img//left.png", des);
        }
    }
    else
    {
        if(status_ == WALK_LEFT)
        {
            LoadImg("img//jump_left.png", des);
        }
        else
        {
            LoadImg("img//jump_right.png", des);
        }
    }

    if(input_type_.left_ == 1 || input_type_.right_ == 1)
    {
        frame_++;
    }
    else
    {
        frame_ = 0;
    }

    if(frame_ >= MAX_FRAME)
    {
        frame_ = 0;
    }
    if(shield_active_ && shield_texture_ != NULL)
    {
    SDL_Rect shieldQuad;
    shieldQuad.w = shield_frame_width_;
    shieldQuad.h = shield_frame_height_;

    shieldQuad.x = rect_.x + (width_frame_ - shield_frame_width_) / 2 + 1;
    shieldQuad.y = rect_.y + (height_frame_ - shield_frame_height_) / 2 - 50;
    SDL_RenderCopy(des, shield_texture_, &shield_clip_[shield_frame_], &shieldQuad);
    }
    if(come_back_time_ == 0)
    {
        rect_.x = x_pos_ - map_x_;
        rect_.y = y_pos_ - map_y_;

        SDL_Rect* current_clip = &frame_clip_[frame_];

        SDL_Rect renderQuad = {rect_.x, rect_.y, width_frame_, height_frame_};

        SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);
    }
}


void MainObject::HandleInputAction(SDL_Event events, SDL_Renderer* screen)
{
    if(events.type == SDL_KEYDOWN)
    {
        switch(events.key.keysym.sym)
        {
            case SDLK_d:
            {
                status_ = WALK_RIGHT;
                input_type_.right_ = 1;
                input_type_.left_ = 0;
                UpdateImagePlayer(screen);
            }
            break;
            case SDLK_a:
            {
                status_ = WALK_LEFT;
                input_type_.left_ = 1;
                input_type_.right_ = 0;
                UpdateImagePlayer(screen);
            }
            break;
            case SDLK_w:
            {
                input_type_.jump_ = 1;
            }
            break;
            case SDLK_s:
            {
                ActivateShield();
            }
            break;
            case SDLK_SPACE:
            {
                unsigned current_time = SDL_GetTicks();
                if ( current_time - last_bullet_time_ >= BULLET_COOLDOWN)
                {
                    BulletObject* p_bullet = new BulletObject();
                    if(status_ == WALK_LEFT)
                    {
                        p_bullet->LoadImg("img//player_bullet_left.png", screen);
                        p_bullet->set_bullet_dir(BulletObject::DIR_LEFT);
                        p_bullet->SetRect(this->rect_.x - 30, rect_.y + height_frame_*0.05);
                    }
                    else
                    {
                        p_bullet->LoadImg("img//player_bullet_right.png", screen);
                        p_bullet->set_bullet_dir(BulletObject::DIR_RIGHT);
                        p_bullet->SetRect(this->rect_.x , rect_.y - height_frame_*0.05);
                    }
                    p_bullet->set_is_move(true);
                    p_bullet_list_.push_back(p_bullet);
                    last_bullet_time_ = current_time;
                }
            }
            break;
            case SDLK_RETURN:
            {
                if (!is_game_paused)
                {
                    fps_timer.paused();
                    is_game_paused = true;
                }
                else
                {
                    fps_timer.unpaused();
                    is_game_paused = false;
                }
                break;
            }

        }
    }
    else if(events.type == SDL_KEYUP)
    {
        switch(events.key.keysym.sym)
        {
            case SDLK_d:
            {
                input_type_.right_ = 0;
            }
            break;
            case SDLK_a:
            {
                input_type_.left_ = 0;
            }
            break;
        }
    }
}


void MainObject::HandleBullet(SDL_Renderer* des)
{
    for(int i=0;i < p_bullet_list_.size();i++)
    {
        BulletObject* p_bullet = p_bullet_list_.at(i);
        if(p_bullet != NULL)
        {
            if(p_bullet->get_is_move() == true)
            {
                p_bullet->HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);
                p_bullet->Render(des);
            }
            else
            {
                p_bullet_list_.erase(p_bullet_list_.begin() + i);
                if(p_bullet != NULL)
                {
                    delete p_bullet;
                    p_bullet = NULL;
                }
            }
        }
    }
}

void MainObject::DoPlayer(Map& map_data)
{
    if(come_back_time_ == 0)
    {
        unsigned int current_time = SDL_GetTicks();
        unsigned int elapsed_time = current_time - start_time_;
        int cycles = elapsed_time / SPEED_UP_INTERVAL;
        float speed_multiplier = 1.0f + SPEED_INCREASE_FACTOR * cycles;
        if(speed_multiplier > MAX_INCREASE_PLAYER)
        {
            speed_multiplier = MAX_INCREASE_PLAYER;
        }
        int current_speed = (int) (PLAYER_SPEED * speed_multiplier);

        x_val_ = 0;
        y_val_ += GRAVITY_SPEED;

        if(input_type_.left_ == 1)
        {
            x_val_ -= current_speed;
        }
        else if(input_type_.right_ == 1)
        {
            x_val_ += current_speed;
        }

        if(input_type_.jump_ == 1 && on_ground_)
        {
            y_val_ = - PLAYER_JUMP_VAL;
            on_ground_ = false;
            input_type_.jump_ = 0;
        }

        if(!on_ground_)
        {
            input_type_.jump_ = 0;
        }

        static unsigned int last_time_ = current_time;
        unsigned int delta_time = current_time - last_time_;
        last_time_ = current_time;

        float dt_in_seconds = delta_time / 1000.0f;

        score_accumulator_ += dt_in_seconds * (SCORE_TIME_FACTOR / 1000) * speed_multiplier;

        score_ = (int) score_accumulator_;

        CheckToMap(map_data);
        CenterEntityOnMap(map_data);
    }
    if(come_back_time_ > 0)
    {
        come_back_time_ --;
        if(come_back_time_ == 0)
        {
            on_ground_ = false;
            if(x_pos_ > MAP_COME_BACK)
            {
                x_pos_ -= MAP_COME_BACK;
            }
            else
            {
                x_pos_ = 0;
            }
            y_pos_ = 0;
            x_val_ = 0;
            y_val_ = 0;
            shield_on_cooldown_ = false;
            ActivateShield();
        }
    }
}

void MainObject::CenterEntityOnMap(Map& map_data)
{
    map_data.start_x_ = x_pos_ - SCREEN_WIDTH / 2;

    if(map_data.start_x_ < 0)
    {
        map_data.start_x_ = 0;
    }
    else if(map_data.start_x_ + SCREEN_WIDTH >= map_data.max_x_)
    {
        map_data.start_x_ = map_data.max_x_ - SCREEN_WIDTH;
    }

    map_data.start_y_ = y_pos_ - (SCREEN_HEIGHT/2);
    if(map_data.start_y_ < 0)
    {
        map_data.start_y_ = 0;
    }
    else if(map_data.start_y_ + SCREEN_HEIGHT >= map_data.max_y_)
    {
        map_data.start_y_ = map_data.max_y_ - SCREEN_HEIGHT;
    }

    map_x_ = map_data.start_x_;
    map_y_ = map_data.start_y_;
}

void MainObject::CheckToMap(Map& map_data)
{
    int x1 = 0;
    int x2 = 0;
    int y1 = 0;
    int y2 = 0;

    //check x
    int height_min = height_frame_ < TILE_SIZE ? height_frame_ : TILE_SIZE;

    x1 = ( x_pos_ + x_val_ )/TILE_SIZE;
    x2 = ( x_pos_ + x_val_ + width_frame_ - 1 )/TILE_SIZE;

    y1 = ( y_pos_ )/TILE_SIZE;
    y2 = ( y_pos_ + height_min - 1)/TILE_SIZE;

    if(x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
    {
        if(x_val_ > 0)
        {
            if(map_data.tile[y1][x2] != BLANK_TILE || map_data.tile[y2][x2] != BLANK_TILE)
            {
                x_pos_ = x2*TILE_SIZE - (width_frame_ + 1);
                x_val_ = 0;

                on_ground_ = true;
                if(status_ == WALK_NONE)
                {
                    status_ = WALK_RIGHT;
                }
            }
        }
        else if(x_val_ < 0)
        {
            if(map_data.tile[y1][x1] != BLANK_TILE || map_data.tile[y2][x1] != BLANK_TILE)
            {
                x_pos_ = (x1 + 1)*TILE_SIZE;
                x_val_ = 0;
            }
        }
    }

    //check y
    int width_min = width_frame_ < TILE_SIZE ? width_frame_ : TILE_SIZE;
    x1 = (x_pos_)/TILE_SIZE;
    x2 = (x_pos_ + width_min)/TILE_SIZE;

    y1 = (y_pos_ + y_val_)/TILE_SIZE;
    y2 = (y_pos_ + y_val_ + height_frame_ - 1)/TILE_SIZE;

    if(x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
    {
        if(y_val_ > 0)
        {
            if(map_data.tile[y2][x1] != BLANK_TILE || map_data.tile[y2][x2] != BLANK_TILE)
            {
                y_pos_ = y2*TILE_SIZE - (height_frame_ + 1);
                y_val_ = 0;
                on_ground_ = true;
            }
        }
        else if (y_val_< 0)
        {
            if(map_data.tile[y1][x1] != BLANK_TILE || map_data.tile[y1][x2] != BLANK_TILE)
            {
                y_pos_ = (y1 + 1)*TILE_SIZE;
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
    else if (x_pos_ + width_frame_ > map_data.max_x_)
    {
        x_pos_ = map_data.max_x_ - width_frame_ -1;
    }
    if(y_pos_ >= map_data.max_y_)
    {
        DecreaseHealth(1);
        come_back_time_ = COME_BACK_TIME;
    }
}

void MainObject::UpdateImagePlayer(SDL_Renderer *des)
{
    if(on_ground_ == true)
    {
        if(status_ == WALK_LEFT)
        {
            LoadImg("img//left.png", des);
        }
        else
        {
            LoadImg("img//right.png", des);
        }
    }
    else
    {
        if(status_ == WALK_LEFT)
        {
            LoadImg("img//jump_left.png", des);
        }
        else
        {
            LoadImg("img//jump_right.png", des);
        }
    }
}

void MainObject::RemoveBullet(const int& idx)
{
    int size_ = p_bullet_list_.size();
    if(size_ > 0 && idx < size_)
    {
        BulletObject* p_bullet = p_bullet_list_.at(idx);
        p_bullet_list_.erase(p_bullet_list_.begin() + idx);
        if(p_bullet)
        {
            delete p_bullet;
            p_bullet = NULL;
        }
    }
}

void MainObject::DecreaseHealth(int value)
{
    health_ -= value;
    if (health_ < 0)
    {
        health_ = 0;
    }
}
void MainObject::IncreaseHealth(int value)
{
    health_ += value;
    if (health_ < 0)
    {
        health_ = 0;
    }
}

double MainObject::GetShieldTimeRemaining()
{
    if(shield_active_)
    {
        unsigned elapsed = SDL_GetTicks() - shield_start_time_;
        if(elapsed >= SHIELD_DURATION)
            return 0;
        else
            return (double)(SHIELD_DURATION - elapsed)/1000;
    }
    return 0;
}

double MainObject::GetShieldCooldownTime()
{
    if (!shield_on_cooldown_)
    {
        return 0;
    }
    unsigned current_time = SDL_GetTicks();
    double remaining = (double)(SHIELD_COOLDOWN - (current_time - shield_cooldown_start_time_)) / 1000;
    if (remaining < 0.0)
    {
        remaining = 0.0;
    }
    return remaining;
}

double MainObject::GetBulletCooldown()
{
    unsigned current_time = SDL_GetTicks();
    if(current_time - last_bullet_time_ >= BULLET_COOLDOWN)
    {
        return 0;
    }
    else
    {
        return (double)(BULLET_COOLDOWN - (current_time - last_bullet_time_))/1000;
    }
}


