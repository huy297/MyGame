#include "Graphics.h"

SDL_Texture* Graphics::LoadTexture(const char* filename)
{
    SDL_Texture *texture;
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,"Loading %s", filename);
    texture = IMG_LoadTexture(Game::renderer,filename);
    return texture;
}


void Graphics::blitRect(SDL_Texture *texture, SDL_Rect *src, SDL_Rect *dest)
{

	SDL_RenderCopy(Game::renderer, texture, src, dest);
}
