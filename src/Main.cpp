#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string>

#include <nch/cpp-utils/fs-utils.h>
#include <nch/cpp-utils/filepath.h>
#include <nch/cpp-utils/log.h>
#include <nch/ffmpeg-utils/simplemediaplayer.h>
#include <nch/sdl-utils/main-loop-driver.h>

SDL_Renderer* renderer;
SDL_Window* window;
nch::SimpleMediaPlayer* smp;

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
    smp->renderCurrentVidFrame(NULL, &r);
        
    SDL_RenderPresent(rend);
}

int main(int argc, char *argv[])
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


    nch::FsUtils::ListSettings ls; ls.includeDirs = false;
    std::vector<std::string> dirFileList = nch::FsUtils::getDirContents("", ls);
    std::string chosenURL = "???null???";
    for(int i = 0; i<dirFileList.size(); i++) {
        nch::FilePath fp(dirFileList[i]);
        std::string fpe = fp.getExtension();
        if( fpe=="mp4" || fpe=="webm" || fpe=="mov" || fpe=="mpeg" ) {
            chosenURL = fp.get();

        }
    }

    if(chosenURL=="???null???") {
        nch::Log::log("Could not find any valid videos within \"/bin\" (.mp4, .webm, .mov, .mpeg).");
    } else {
        nch::Log::log("Found valid video \"%s\", attempting to play...", chosenURL.c_str());
    }

    //std::string url = nch::FsUtils::getPathWithInferredExtension("video");
    smp = new nch::SimpleMediaPlayer(chosenURL, renderer, -1, true, false);
    smp->decodeFull();
    smp->startPlayback();
    //mp->initMediaPlaybackData();
    //mp->startDecodingFrom(150000);
    //mp->startPlayback(true);

    nch::MainLoopDriver mld(renderer, &tick, 40, &draw, 300);

    nch::Log::log("Exiting.");
    return 0;
}

#if ( defined(_WIN32) || defined(WIN32) )
int WinMain()
{
    char** x = new char*[1];
    return main(0, x);
}
#endif