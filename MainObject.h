
#ifndef MAIN_OBJECT_H_
#define MAIN_OBJECT_H_

#include "CommonFunc.h"
#include "BaseObject.h"
#include "BulletObject.h"

class MainObject : public BaseObject
{
public:
    MainObject();
    ~MainObject();

    enum WalkType
    {
        WALK_NONE = 0,
        WALK_RIGHT = 1,
        WALK_LEFT  = 2,
    };

    bool LoadImg(std::string path, SDL_Renderer* screen);
    void Show(SDL_Renderer* des);
    void HandleInputAction(SDL_Event events,SDL_Renderer* screen);
    void set_clips();

    void DoPlayer(Map& map_data);
    void CheckToMap(Map& map_data);

    void SetMapXY(const int map_x, const int map_y) {map_x_ = map_x; map_y_ = map_y;};
    void CenterEntityOnMap(Map& map_data);
    void UpdateImagePlayer(SDL_Renderer* des);
    SDL_Rect GetRectFrame();

    void set_bullet_list(std::vector<BulletObject*> bullet_list)
    {
        p_bullet_list_ = bullet_list;
    }
    std::vector<BulletObject*> get_bullet_list() const {return p_bullet_list_;}
    void HandleBullet(SDL_Renderer* des);
    void RemoveBullet(const int& idx);
    void set_comeback_time(const int& cb_time) {come_back_time_ = cb_time;}

    int get_health() const {return health_;}
    void DecreaseHealth(int value);
    void IncreaseHealth(int value);

    void ActivateShield();

    void UpdateShield();

    bool IsShieldActive() const {return shield_active_;}
    bool LoadShieldImg(std::string path, SDL_Renderer* screen);
    int GetScore() const {return score_;}

    double GetShieldTimeRemaining();
    double GetShieldCooldownTime();
    double GetBulletCooldown();


private:

    unsigned last_bullet_time_;
    std::vector<BulletObject*> p_bullet_list_;

    float x_val_;
    float y_val_;

    float x_pos_;
    float y_pos_;

    int width_frame_;
    int height_frame_;

    SDL_Rect frame_clip_[MAX_FRAME];
    Input input_type_;
    int frame_;
    int status_;
    bool on_ground_;

    int map_x_;
    int map_y_;

    int come_back_time_;

    unsigned int start_time_;

    int health_;

    bool shield_active_;
    unsigned int shield_start_time_;
    bool shield_on_cooldown_;
    unsigned int shield_cooldown_start_time_;

    SDL_Texture* shield_texture_;
    SDL_Rect shield_clip_[MAX_SHIELD_FRAME];
    int shield_frame_;
    int shield_frame_width_;
    int shield_frame_height_;

    int score_;
    float score_accumulator_;

};

#endif
