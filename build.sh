rm -rf ./build
mkdir build

(cd build && \
emcc ../src/*.c \
    -o app.html -g \
    -s USE_SDL=2 \
    -s USE_SDL_IMAGE=2 \
    --preload-file ../assets/ \
    --use-preload-plugins)

rm -rf ./public
mkdir public

cp ./static/* public

cp build/app.wasm public/app.wasm
cp build/app.js public/app.js
cp build/app.data public/app.data

# Build to /docs also so we can serve with gh-pages.
rm -rf ./docs
mkdir docs
cp public/* docs
