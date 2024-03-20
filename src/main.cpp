// Removed from the Makefie && ./$(BUILD_DIR)/$(OBJ_NAME)
#include <iostream>
#include <SDL2/SDL.h>
#include <cmath>

const int WIDTH = 1024;
const int HEIGHT = 768;

const std::vector<float> topLeftSample {255.0, 0.0, 0.0};
const std::vector<float> topRigthtSample {0.0, 0.0, 255.0};
const std::vector<float> bottomLeftSample {0.0, 255.0, 0.0};
const std::vector<float> bottomRightSample {255.0, 255.0, 0.0};

std::vector<float> interpolatedSample (3,0.0); //3 sized vector with 0.0 in all positions

const float nx = WIDTH+0.0f; //Avoid truncation by unsing nx as float
const float ny = HEIGHT+0.0f;
float point[3] = {512.0, 512.0, 512};
const float maxPoint[2] = {nx, ny};

void linearInterpolation(int x, float domain, 
    const std::vector<float> & initialSample, 
    const std::vector<float> & finalSample,
    std::vector<float> & resultSample)
{
    for(size_t i = 0; i < 3; i++)
    {
        resultSample[i] = (((domain - x)/domain) * initialSample[i] ) + ((x/domain) * finalSample[i]);
    }
}

void bilinearInterpolation(
    float point[], 
    const float maxPoint[],
    const std::vector<float>& topLeftSample,
    const std::vector<float>& topRigthtSample,
    const std::vector<float>& bottomLeftSample,
    const std::vector<float>& bottomRightSample,
    std::vector<float>& interpolatedSample)
{
    float topRowInterpolation[3] = {};
    float bottomRowInterpolation[3] = {};

    for(size_t i = 0; i < 3; i++)
    {
        topRowInterpolation[i] = (((maxPoint[0] - point[0])/maxPoint[0]) * topLeftSample[i] ) + ((point[0]/maxPoint[0]) * topRigthtSample[i]);
        bottomRowInterpolation[i] = (((maxPoint[0] - point[0])/maxPoint[0]) * bottomLeftSample[i] ) + ((point[0]/maxPoint[0]) * bottomRightSample[i]);
        interpolatedSample[i]  = (((maxPoint[1] - point[1])/maxPoint[1]) * topRowInterpolation[i] ) + ((point[1]/maxPoint[1]) * bottomRowInterpolation[i]);

    }
}


int main(int argc, char * argv[])
{
    std::cout << "Hello Window! \n";

    //linearInterpolation(nx/4, nx, topLeftSample, topRigthtSample, interpolatedSample);
    //bilinearInterpolation( point,  maxPoint, topLeftSample, topRigthtSample, bottomLeftSample, bottomRightSample, interpolatedSample);

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return -1;
    }
    else
    {
        std::cout << "SDL_Init succeeded \n";
    }

    SDL_Window* window = SDL_CreateWindow("Video Game - SDL window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Set the background color to black
    //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Render a single white pixel in the middle
    for(size_t i = 0; i < (WIDTH*HEIGHT) - 1; i++)
    {   
        
        point[0] = i % int(nx);
        point[1] = int(floor( int(i) / int(nx))) % int(ny);
        bilinearInterpolation( point,  maxPoint, topLeftSample, topRigthtSample, bottomLeftSample, bottomRightSample, interpolatedSample);
        
        SDL_SetRenderDrawColor(renderer, interpolatedSample[0], interpolatedSample[1], interpolatedSample[2], 255);
        SDL_RenderDrawPoint(renderer, point[0], point[1]);
    }
    
    //SDL_Point();
    //SDL_RenderDrawPoints();
    SDL_RenderPresent(renderer);

    bool isRunning = true;
    SDL_Event event;
    while (isRunning)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                isRunning = false;
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;

}

