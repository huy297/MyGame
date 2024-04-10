#include <SDL2/SDL.h>
#include <iostream>

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Rotating Sword", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Surface* swordSurface = SDL_LoadBMP("sword.bmp");
    if (swordSurface == nullptr) {
        std::cerr << "Failed to load image: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture* swordTexture = SDL_CreateTextureFromSurface(renderer, swordSurface);
    if (swordTexture == nullptr) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(swordSurface);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Clear renderer
    SDL_RenderClear(renderer);

    // Set rotation angle (in degrees)
    double angle = 45.0; // Example: Rotate 45 degrees

    // Set pivot point (center of rotation)
    int centerX = swordSurface->w / 2;
    int centerY = swordSurface->h / 2;

    // Render rotated sword
    SDL_Rect destinationRect = {100, 100, swordSurface->w, swordSurface->h};
    SDL_RenderCopyEx(renderer, swordTexture, nullptr, &destinationRect, angle, nullptr, SDL_FLIP_NONE);

    // Present renderer
    SDL_RenderPresent(renderer);

    // Main loop
    bool quit = false;
    SDL_Event event;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }
    }

    // Cleanup
    SDL_DestroyTexture(swordTexture);
    SDL_FreeSurface(swordSurface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
