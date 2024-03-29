// Removed from the Makefie && ./$(BUILD_DIR)/$(OBJ_NAME)
#include <iostream>
//#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <cmath>

//glm::vec3 v(2.f, 2.f, 2.f);
//float l = glm::length(v);


int status = 0;
const int WIDTH = 1024;
const int HEIGHT = 768;
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

const std::vector<float> topLeftSample {255.0, 255.0, 255.0};
const std::vector<float> topRigthtSample {180.0, 255.0, 250.0};
const std::vector<float> bottomLeftSample {200.0, 255.0, 200.0};
const std::vector<float> bottomRightSample {255.0, 255.0, 255.0};

std::vector<float> interpolatedSample (3,0.0); //3 sized vector with 0.0 in all positions

const float nx = WIDTH+0.0f; //Avoid truncation by unsing nx as float
const float ny = HEIGHT+0.0f;
float point[3] = {512.0, 512.0, 512};
const float maxPoint[2] = {nx, ny};

void interpolation(const int initialSample, const int finalSample, std::vector<float>& result)
{
    //Inerpolation in which "t" blongs to [0,1]
    if(result.size() > 2)
    {
        float t = 0;
        for(size_t i = 0; i < result.size(); i++)
        {
            std::cout << "t: "<< t <<"\n";
            result[i] = ((1 - t) * initialSample) + (t * finalSample);
            t +=  1.0 / (result.size()-1);      
        }   
        
    } else
    {
        std::cout << "Result vector must be greater than 2\n";
    }
}

void vectorInterpolation(const std::vector<float>& initialSample, const std::vector<float>& finalSample, std::vector<std::vector<float>>& result)
{
    //Inerpolation in which "t" blongs to [0,1]
    if(result.size() > 2)
    {
        float t = 0;
        for(size_t j = 0; j < result.size(); j++)
        {
            result[j] = {0,0,0};
            for(size_t i = 0; i < result[j].size(); i++)
            {
                result[j][i] = ((1 - t) * initialSample[i]) + (t * finalSample[i]);
            }
            t +=  1.0 / (result.size()-1);      
        }   
        
    } else
    {
        std::cout << "Result vector must be greater than 2\n";
    }
}

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

void setup()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        status = -1;
    }
    std::cout << "SDL_Init succeeded \n";
}

void createWindowRenderer(const int WIDTH, const int HEIGHT, SDL_Window* & window, SDL_Renderer* & renderer )
{
    window = SDL_CreateWindow("Video Game - SDL window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        status = -1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        status = -1;
    }
}

void draw(SDL_Renderer* & renderer)
{
    const int INTERPOLATED_VALUES = 3;
    std::vector<float> result(INTERPOLATED_VALUES, 0.0);
    interpolation(5, 14, result);
    std::cout << "result:" << "\n";

    for(size_t i = 0; i < INTERPOLATED_VALUES; i++)
    {
        std::cout << result[i] << "\n";
    }
 
    const int INTERPOLATED_VECTORS = 4;
    std::vector<std::vector<float>> resultVector(INTERPOLATED_VECTORS);
    std::vector<float> startVector = {1, 4, 9.2};
    std::vector<float> endVector = {4, 1, 9.8};

    vectorInterpolation(startVector, endVector, resultVector);
    for(size_t i = 0; i < resultVector.size(); i++)
    {
        std::cout << "( "<< resultVector[i][0] << ", ";
        std::cout << resultVector[i][1] << ", ";
        std::cout << resultVector[i][2] << ") \n";
    }

    SDL_RenderClear(renderer);
    for(size_t i = 0; i < (WIDTH*HEIGHT) - 1; i++)
    {   
        point[0] = i % int(nx); // Linear index mapped to x-coodinate system
        point[1] = int(floor( int(i) / int(nx))) % int(ny); // Linear index mapped to y-coodinate system
        bilinearInterpolation( point,  maxPoint, topLeftSample, topRigthtSample, bottomLeftSample, bottomRightSample, interpolatedSample);
        
        SDL_SetRenderDrawColor(renderer, interpolatedSample[0], interpolatedSample[1], interpolatedSample[2], 255);
        //SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderDrawPoint(renderer, point[0], point[1]);
    }
    SDL_RenderPresent(renderer);
}

void events()
{
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
}

int main(int argc, char * argv[])
{
    std::cout << "Hello Window! \n";
    setup();
    createWindowRenderer(WIDTH, HEIGHT, window, renderer);
    if(status != 0)
    {
        return -1;
    }
    draw(renderer);
    events();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

