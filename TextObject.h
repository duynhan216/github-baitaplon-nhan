#ifndef TEXTOBJECT_H_
#define TEXTOBJECT_H_

#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

class TextObject {
public:
    enum TextColor {
        RED_TEXT = 0,
        WHITE_TEXT = 1,
        BLACK_TEXT = 2,
    };

    TextObject();
    ~TextObject();

    bool LoadFromRenderText(TTF_Font* font, SDL_Renderer* screen);

    void Free();

    void SetColor(Uint8 red, Uint8 green, Uint8 blue);
    void SetColor(int type);

    void RenderText(SDL_Renderer* screen,
                    int x, int y,
                    SDL_Rect* clip = nullptr,
                    double angle = 0.0,
                    SDL_Point* center = nullptr,
                    SDL_RendererFlip flip = SDL_FLIP_NONE);

    void setText(const std::string& text) { str_val_ = text; }
    std::string getText() const { return str_val_; }

    // Setter/Getter cho vị trí
    void setRect(int x, int y) { rect_.x = x; rect_.y = y; }
    SDL_Rect getRect() const { return rect_; }

private:
    std::string str_val_;   // Chuỗi text cần hiển thị
    SDL_Color text_color_;  // Màu chữ
    SDL_Texture* texture_;  // Texture sau khi render text
    SDL_Rect rect_;         // Vùng chữ (toạ độ, kích thước)
};

#endif
