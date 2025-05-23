cd emsdk
emsdk_env.bat
cd C:\Users\user\Desktop\Скрипты\WASM
emcc main.cpp -o index.html --shell-file test.html -s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2 -s USE_WEBGL2=1 -s USE_SDL=2
emrun --no_browser --port 8080 .

emcc main.cpp -o penis.html -s USE_SDL=2 -s USE_SDL_TTF=2 --preload-file image.bmp --preload-file l.bmp --preload-file d.bmp --preload-file font.ttf --preload-file fontb.ttf -s ALLOW_MEMORY_GROWTH=1

-s DISABLE_EMSCRIPTEN_CONSOLE=1 \
-s DISABLE_EXCEPTION_CATCHING=1 \
-s NO_EXIT_RUNTIME=1 \
-s OFFSCREEN_FRAMEBUFFER=1 \
-s EXIT_RUNTIME=0 \
-s NO_FILESYSTEM=1

emcc main.cpp -s WASM=1 -s MODULARIZE=1 -s EXPORT_NAME="createModule" -s ALLOW_MEMORY_GROWTH=1 -o map_module.js --bind

emcc main.cpp -o penis.html -s USE_SDL=2 -s USE_SDL_TTF=2 --preload-file image.bmp --preload-file l.bmp --preload-file d.bmp --preload-file tl.ttf --preload-file tb.ttf -s ALLOW_MEMORY_GROWTH=1