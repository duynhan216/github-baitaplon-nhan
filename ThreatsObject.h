#ifndef THREATS_OBJECT_H_
#define THREATS_OBJECT_H_

#include "BaseObject.h"
#include "game_map.h"
#include "CommonFunc.h"

#define THREAT_FRAME_NUM 6
#define SPEED_THREAT_GROUND 8
#define SPEED_THREAT_FLYING 10

enum ThreatType
{
    THREAT_GROUND = 0,
    THREAT_FLYING = 1,
};

class ThreatsObject : public BaseObject
{
public:
    ThreatsObject();
    ~ThreatsObject();

    bool LoadImg(std::string path, SDL_Renderer* screen);
    void set_clips();
    void Show(SDL_Renderer* des);

    void DoPlayer(Map& map_data);

    void CheckToMap(Map& map_data);

    void SpawnRandom(Map& map_data, ThreatType t);

    void SetMapXY(const int map_x, const int map_y) { map_x_ = map_x; map_y_ = map_y; }

    int get_width_frame() const {return width_frame_;}
    int get_height_frame() const {return height_frame_;}
    SDL_Rect GetRectFrame();


    void set_threat_type(ThreatType t) { threat_type_ = t; }
    ThreatType get_threat_type() const { return threat_type_;}

    static unsigned game_start_time_;

    SDL_Rect GetRect() const { return rect_; }

private:

    float speed_multiplier;
    static const unsigned SPEED_UPDATE_INTERVAL = 10000;

    int width_frame_;
    int height_frame_;

    int x_val_;
    int y_val_;
    int x_pos_;
    int y_pos_;

    int frame_;
    bool on_ground_;

    int map_x_;
    int map_y_;

    SDL_Rect frame_clip_[THREAT_FRAME_NUM];

    ThreatType threat_type_;
};

#endif
