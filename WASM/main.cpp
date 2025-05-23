#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <iostream>
#include <vector>
#include <cstdio>

//const int WINDOW_WIDTH = 1280;
const int WINDOW_WIDTH = 500;
const int WINDOW_HEIGHT = 800;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* texture = nullptr;
SDL_Texture* dt = nullptr;
SDL_Texture* lt = nullptr;
TTF_Font* font = nullptr;
TTF_Font* fontb = nullptr;
SDL_Texture* textTexture = nullptr;

struct element
{
    int sx=0; int sy=0;
    int ex=50; int ey=0;
    float color=0;
    std::string t="";
    std::vector<std::string> m;
};

std::vector<element> list;

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

void create_text_bold(const std::string& message, SDL_Color color) {
    if (textTexture) SDL_DestroyTexture(textTexture);
    SDL_Surface* textSurface = TTF_RenderText_Blended(fontb, message.c_str(), color);
    if (!textSurface) {
        std::cerr << "TTF_RenderText Error: " << TTF_GetError() << std::endl;
        return;
    }
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
}

void main_loop() {

    bool click=0;
    unsigned char nowel=0;
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            emscripten_cancel_main_loop();}
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            click=1;}
    }

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Загружаем BMP и создаём текстуру один раз
    static bool bmpLoaded = false;
    if (!bmpLoaded) {
        //SDL_Surface* ls = SDL_LoadBMP("l.bmp");
        //SDL_Surface* ds = SDL_LoadBMP("d.bmp");
        SDL_Surface* bmpSurface = SDL_LoadBMP("image.bmp");
        SDL_Surface* ls = SDL_LoadBMP("l.bmp");
        SDL_Surface* ds = SDL_LoadBMP("d.bmp");
        if (!bmpSurface or !ls or !ds) {
            std::cerr << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
        } else {
            texture = SDL_CreateTextureFromSurface(renderer, bmpSurface);
            SDL_FreeSurface(bmpSurface);
            
            lt = SDL_CreateTextureFromSurface(renderer, ls);
            SDL_FreeSurface(ls);
            dt = SDL_CreateTextureFromSurface(renderer, ds);
            SDL_FreeSurface(ds);
        }
        bmpLoaded = true;
    }

    // Рисуем текстуру в позиции курсора
    /*if (texture) {
        int texW, texH;
        SDL_QueryTexture(texture, nullptr, nullptr, &texW, &texH);
        SDL_Rect dst = { mouseX - texW/2, mouseY - texH/2, texW, texH };
        SDL_RenderCopy(renderer, lt, nullptr, &dst);
    }*/

    /*for (unsigned int i=0; i<list.size(); i++){
    SDL_Rect src={ 0, 0, 28, 28 };
    SDL_Rect dst = { list[i].sx, list[i].sy, 28, 28 };
    SDL_RenderCopy(renderer, dt, &src, &dst);
    src={ 28, 0, 28, 28 };
    dst = { list[i].ex-14, list[i].sy, 28, 28 };
    SDL_RenderCopy(renderer, dt, &src, &dst);
    src={ 0, 28, 28, 28 };
    dst = { list[i].sx, list[i].ey-14, 28, 28 };
    SDL_RenderCopy(renderer, dt, &src, &dst);
    src={ 28, 28, 28, 28 };
    dst = { list[i].ex-14, list[i].ey-14, 28, 28 };
    SDL_RenderCopy(renderer, dt, &src, &dst);

    src={ 28, 28, 1, 1 };
    dst = { list[i].sx, list[i].sy+28, list[i].ex-list[i].sx+14, list[i].ey-list[i].sy-42 };
    SDL_RenderCopy(renderer, dt, &src, &dst);
    dst = { list[i].sx+28, list[i].sy, list[i].ex-list[i].sx-42, list[i].ey-list[i].sy+14 };
    SDL_RenderCopy(renderer, dt, &src, &dst);
    create_text("Hello world!", {255, 255, 255, 255});
    int tw, th;
    SDL_QueryTexture(textTexture, nullptr, nullptr, &tw, &th);
    SDL_Rect textRect = { list[i].sx+14, list[i].sy+14, tw, th };
    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
    }*/

    for (unsigned int i=0; i<list.size(); i++){

    if (list[i].sx<mouseX && mouseX<list[i].ex
     && list[i].sy<mouseY && mouseY<list[i].ey)
    {
    if (click) {nowel=i+1;std::cout<<"Clicked!\n";}
    list[i].color+=(100-list[i].color)/10;}
    else {list[i].color+=(-list[i].color)/10;}

    SDL_SetRenderDrawColor(renderer, 240-(Uint8)(list[i].color*200/100), 240-(Uint8)(list[i].color*106/100), 240+(Uint8)(list[i].color*15/100), 255);
    //SDL_SetRenderDrawColor(renderer, 40, 134, 255, 255);
    SDL_Rect rect = { list[i].sx, list[i].sy, list[i].ex-list[i].sx, list[i].ey-list[i].sy };
    SDL_RenderFillRect(renderer, &rect);

    create_text_bold(list[i].t, { (Uint8)(list[i].color*255/100), (Uint8)(list[i].color*255/100), (Uint8)(list[i].color*255/100), 255}); int tw, th;
    SDL_QueryTexture(textTexture, nullptr, nullptr, &tw, &th);
    SDL_Rect textRect = { list[i].sx+30, list[i].sy+25, tw, th };
    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

    for (int u=0; u<list[i].m.size(); u++) {
    create_text(list[i].m[u], { (Uint8)(list[i].color*255/100), (Uint8)(list[i].color*255/100), (Uint8)(list[i].color*255/100), 255});
    SDL_QueryTexture(textTexture, nullptr, nullptr, &tw, &th);
    textRect = { list[i].sx+30, list[i].sy+70+u*30, tw, th };
    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
    }}

    //SDL_SetRenderDrawColor(renderer, 83, 110, 245, 105);
    /*SDL_SetRenderDrawColor(renderer, 40,134,255, 105);
    SDL_Rect rect = { 200, 150, 400, 300 };
    SDL_RenderFillRect(renderer, &rect);

    create_text_bold("Drnoe 1488:", {255, 255, 255, 255}); int tw, th;
    SDL_QueryTexture(textTexture, nullptr, nullptr, &tw, &th);
    SDL_Rect textRect = { 230, 180, tw, th };
    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

    create_text("Here some information!", {255, 255, 255, 255});
    SDL_QueryTexture(textTexture, nullptr, nullptr, &tw, &th);
    textRect = { 230, 230, tw, th };
    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);*/

    // Рисуем текст координат мыши
    /*std::string coords = "Mouse: (" + std::to_string(mouseX) + ", " + std::to_string(mouseY) + ")";
    create_text(coords, {255, 255, 255, 255});
    if (textTexture) {
        int tw, th;
        SDL_QueryTexture(textTexture, nullptr, nullptr, &tw, &th);
        SDL_Rect textRect = { 10, 10, tw, th };
        SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
    }*/

    SDL_RenderPresent(renderer);
    SDL_Delay(15);
}

int main(int argc, char* argv[]) {

    element ret;

    ret.sx=10;
    ret.sy=10;
    ret.ex=490;
    ret.ey=200;
    ret.t="UI 1 :";
    ret.m.push_back("TEST11");
    ret.m.push_back("TEST12");
    list.push_back(ret);

    ret.sx=10;
    ret.sy=210;
    ret.ex=490;
    ret.ey=400;
    ret.t="UI 2 :";
    ret.m.clear();
    ret.m.push_back("TEST21");
    ret.m.push_back("TEST22");
    list.push_back(ret);

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
    font = TTF_OpenFont("tl.ttf", 24);
    fontb = TTF_OpenFont("tb.ttf", 30);
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