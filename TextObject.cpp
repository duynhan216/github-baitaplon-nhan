
#include "TextObject.h"

TextObject::TextObject()
{
    text_color_.r = 255;
    text_color_.g = 255;
    text_color_.b = 255;
    texture_ = nullptr;
    rect_.x = 0;
    rect_.y = 0;
    rect_.w = 0;
    rect_.h = 0;
}

TextObject::~TextObject()
{
    Free();
}

bool TextObject::LoadFromRenderText(TTF_Font* font, SDL_Renderer* screen)
{
    Free();

    SDL_Surface* text_surface = TTF_RenderText_Solid(font, str_val_.c_str(), text_color_);
    if (text_surface != nullptr)
    {
        texture_ = SDL_CreateTextureFromSurface(screen, text_surface);
        if (texture_ != nullptr)
        {
            rect_.w = text_surface->w;
            rect_.h = text_surface->h;
        }
        SDL_FreeSurface(text_surface);
    }
    return (texture_ != nullptr);
}

void TextObject::Free()
{
    if (texture_ != nullptr)
    {
        SDL_DestroyTexture(texture_);
        texture_ = nullptr;
    }
}

void TextObject::SetColor(Uint8 red, Uint8 green, Uint8 blue)
{
    text_color_.r = red;
    text_color_.g = green;
    text_color_.b = blue;
}

void TextObject::SetColor(int type)
{
    if (type == RED_TEXT) {
        SDL_Color color = {255, 0, 0};
        text_color_ = color;
    } else if (type == WHITE_TEXT) {
        SDL_Color color = {255, 255, 255};
        text_color_ = color;
    } else if (type == BLACK_TEXT) {
        SDL_Color color = {0, 0, 0};
        text_color_ = color;
    }
}

void TextObject::RenderText(SDL_Renderer* screen,
                            int x, int y,
                            SDL_Rect* clip,
                            double angle,
                            SDL_Point* center,
                            SDL_RendererFlip flip)
{
    if (texture_ == nullptr)
    {
        return;
    }
    SDL_Rect renderQuad = {x, y, rect_.w, rect_.h};
    if (clip != nullptr)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    SDL_RenderCopyEx(screen, texture_, clip, &renderQuad, angle, center, flip);
}
