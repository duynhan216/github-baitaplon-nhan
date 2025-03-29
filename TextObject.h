#ifndef TEXT_OBJECT_H_
#define TEXT_OBJECT_H_

#include "CommonFunc.h"

class TextObject {
public:
    TextObject();
    ~TextObject();

    void setText(const std::string& text);

    void setColor(Uint8 r, Uint8 g, Uint8 b);

    bool UpdateTexture(TTF_Font* font, SDL_Renderer* renderer);

    void RenderTextObject(TTF_Font* font, SDL_Renderer* renderer, int x, int y);

    void Render(SDL_Renderer* renderer, int x, int y);

    void Free();

private:
    std::string text_;
    SDL_Color text_color_;
    SDL_Texture* texture_;
    SDL_Rect rect_;
};

#endif // TEXT_OBJECT_H_
