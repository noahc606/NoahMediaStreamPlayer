#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string>

#include <nch/cpp-utils/fs/FsUtils.h>
#include <nch/cpp-utils/fs/FilePath.h>
#include <nch/ffmpeg-utils/media/MediaPlayer.h>
#include <nch/sdl-utils/MainLoopDriver.h>

SDL_Renderer* renderer;
SDL_Window* window;
nch::MediaPlayer* mp;

void tick()
{

}

void draw(SDL_Renderer* rend)
{
    SDL_RenderClear(rend);
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    SDL_RenderFillRect(rend, NULL);
    
    //Outer border
    SDL_SetRenderDrawColor(rend, 0, 0, 255, 255);
    SDL_RenderFillRect(rend, NULL);

    //Video
    SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);

    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    SDL_Rect r; r.x = 25; r.y = 25; r.w = w-50; r.h = h-50;
    mp->renderCurrentVidFrame(NULL, &r);
        
    SDL_RenderPresent(rend);
}

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_TIMER)!=0) {
        printf("Could not initialize SDL - %s\n.", SDL_GetError());
    }

    window = SDL_CreateWindow(
        "Noah's MediaStream Player",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        640, 480,
        SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI
    );
    SDL_SetWindowResizable(window, SDL_TRUE);
    if(!window) {
        printf("SDL: could not create window.\n");
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);


    std::vector<std::string> dirFileList = nch::FsUtils::getDirContents("", nch::FsUtils::ONLY_FILES, false);
    std::string chosenURL = "???null???";
    for(int i = 0; i<dirFileList.size(); i++) {
        nch::FilePath fp(dirFileList[i]);
        std::string fpe = fp.getExtension();
        if( fpe=="mp4" || fpe=="webm" || fpe=="mov" || fpe=="mpeg" ) {
            chosenURL = fp.get();

        }
    }

    printf("Found valid video \"%s\", attempting to play...", chosenURL.c_str());

    //std::string url = nch::FsUtils::getPathWithInferredExtension("video");
    mp = new nch::MediaPlayer(chosenURL, renderer, 10000, true, true);
    mp->decodeFull();
    mp->startPlayback(true);

    nch::MainLoopDriver mld(renderer, &tick, 40, &draw, 300);

    return 0;
}