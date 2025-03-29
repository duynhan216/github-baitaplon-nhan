#include "TextObject.h"

TextObject::TextObject()
    : text_(""), texture_(nullptr)
{
    text_color_.r = 255;
    text_color_.g = 255;
    text_color_.b = 255;
    text_color_.a = 255;
    rect_.x = rect_.y = rect_.w = rect_.h = 0;
}

TextObject::~TextObject()
{

}

void TextObject::setText(const std::string& text)
{
    text_ = text;
}

void TextObject::setColor(Uint8 r, Uint8 g, Uint8 b)
{
    text_color_.r = r;
    text_color_.g = g;
    text_color_.b = b;
}

bool TextObject::UpdateTexture(TTF_Font* font, SDL_Renderer* renderer)
{
    Free();
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text_.c_str(), text_color_);
    if (textSurface != nullptr)
    {
        texture_ = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (texture_ != nullptr)
        {
            rect_.w = textSurface->w;
            rect_.h = textSurface->h;
        }
        SDL_FreeSurface(textSurface);
        return texture_ != nullptr;
    }
    return false;
}

void TextObject::RenderTextObject(TTF_Font* font, SDL_Renderer* renderer, int x, int y)
{
    if (UpdateTexture(font, renderer))
    {
        rect_.x = x;
        rect_.y = y;
        SDL_RenderCopy(renderer, texture_, nullptr, &rect_);
    }
}

void TextObject::Render(SDL_Renderer* renderer, int x, int y)
{
    rect_.x = x;
    rect_.y = y;
    if (texture_ != nullptr)
    {
        SDL_RenderCopy(renderer, texture_, nullptr, &rect_);
    }
}

void TextObject::Free()
{
    if (texture_ != nullptr)
    {
        SDL_DestroyTexture(texture_);
        texture_ = nullptr;
    }
}
