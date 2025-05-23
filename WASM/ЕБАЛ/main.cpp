#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <iostream>
#include <cstdio>

//const int WINDOW_WIDTH = 1280;
const int WINDOW_WIDTH = 900;
const int WINDOW_HEIGHT = 625;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* texture = nullptr;
TTF_Font* font = nullptr;
SDL_Texture* textTexture = nullptr;

void create_text(const std::string& message, SDL_Color color) {
    if (textTexture) SDL_DestroyTexture(textTexture);
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, message.c_str(), color);
    if (!textSurface) {
        std::cerr << "TTF_RenderText Error: " << TTF_GetError() << std::endl;
        return;
    }
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
}

void main_loop() {

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            emscripten_cancel_main_loop();
        }
    }

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Загружаем BMP и создаём текстуру один раз
    static bool bmpLoaded = false;
    if (!bmpLoaded) {
        SDL_Surface* bmpSurface = SDL_LoadBMP("image.bmp");
        if (!bmpSurface) {
            std::cerr << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
        } else {
            texture = SDL_CreateTextureFromSurface(renderer, bmpSurface);
            SDL_FreeSurface(bmpSurface);
        }
        bmpLoaded = true;
    }

    // Рисуем текстуру в позиции курсора
    if (texture) {
        int texW, texH;
        SDL_QueryTexture(texture, nullptr, nullptr, &texW, &texH);
        SDL_Rect dst = { mouseX - texW/2, mouseY - texH/2, texW, texH };
        SDL_RenderCopy(renderer, texture, nullptr, &dst);
    }

    // Рисуем текст координат мыши
    std::string coords = "Mouse: (" + std::to_string(mouseX) + ", " + std::to_string(mouseY) + ")";
    create_text(coords, {255, 255, 255, 255});
    if (textTexture) {
        int tw, th;
        SDL_QueryTexture(textTexture, nullptr, nullptr, &tw, &th);
        SDL_Rect textRect = { 10, 10, tw, th };
        SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
    }

    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    printf("=======================[ DRONE PENIS HERE ]=======================\n");
    printf(" [>] Loading temkas...\n");
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    if (TTF_Init() != 0) {
        std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    window = SDL_CreateWindow("SDL BMP + Text (Emscripten)",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        TTF_Quit(); SDL_Quit();
        return 1;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        TTF_Quit(); SDL_Quit();
        return 1;
    }

    // Загружаем шрифт
    font = TTF_OpenFont("font.ttf", 24);
    if (!font) {
        std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
    }
    printf(" [>] Done!\n");
    printf("\n");
    printf("a\n");
    printf("b\n");
    printf("c\n");
    printf("d\n");

    emscripten_set_wheel_callback(
      "#canvas",      // целевой элемент: ваш <canvas id="canvas">
      nullptr,        // userData (не важно)
      false,          // useCapture
      nullptr         // callback = NULL → deregister
    );

    emscripten_set_main_loop(main_loop, 0, true);

    // Очистка ресурсов
    if (textTexture) SDL_DestroyTexture(textTexture);
    if (texture) SDL_DestroyTexture(texture);
    if (font) TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}

// Компиляция через Emscripten без SDL_image:
// emcc main.cpp -o index.html \
//   -s USE_SDL=2 -s USE_SDL_TTF=2 \
//   --preload-file image.bmp \
//   --preload-file font.ttf \
//   -s ALLOW_MEMORY_GROWTH=1