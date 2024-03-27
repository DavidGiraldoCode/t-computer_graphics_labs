#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <SDL2/SDL.h>

const int WIDTH = 640;
const int HEIGHT = 480;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
bool running = false;
int t = 0;

const float nx = WIDTH+0.0f; //Avoid truncation by unsing nx as float
const float ny = HEIGHT+0.0f;

const int STARS_COUNT = 1000;
std::vector<std::array<float, 3>> stars(STARS_COUNT);

void randomizePosition(std::vector<std::array<float, 3>> & stars)
{
    float randomXPos, randomYPos, randomZPos= 0.0;
    srand (time(nullptr)); //initialize random seed:

    for(size_t i = 0; i < stars.size(); i++)
    {
        randomXPos = float(rand()) / float(RAND_MAX);
        randomYPos = float(rand()) / float(RAND_MAX);
        randomZPos = float(rand()) / float(RAND_MAX);
        stars[i][0] = 2.0 * randomXPos - 1.0;
        stars[i][1] = 2.0 * randomYPos - 1.0;
        stars[i][2] = randomZPos; //1;//
    }
}

bool init(const char* title, int xPosition, int yPosition, int height, int width, int flags)
{
    if (SDL_Init(SDL_INIT_VIDEO) >= 0)
    {
        window = SDL_CreateWindow(title, xPosition, yPosition, height, width, flags);
        if(window != 0)
        {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        }
    
    }
    else
    {
        return false;
    }
    return true;
}

void render(SDL_Renderer* & renderer)
{
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);
    //std::cout << "Seconds passed: "<< (SDL_GetTicks()/1000) << "\n";
    float focalLenght = HEIGHT/2;
    for(size_t i = 0; i < stars.size() - 1; i++)
    {   
        float x = stars[i][0];
        float y = stars[i][1];
        float z = stars[i][2];

        float u = focalLenght * (x/z) + WIDTH/2; //106
        float v = focalLenght * (y/z) + HEIGHT/2; //90

        float zColor = 255 - (255 * z);// - ((z*255)/WIDTH);

        SDL_SetRenderDrawColor(renderer, zColor, zColor, zColor, 255);
        SDL_RenderDrawPoint(renderer, u, v);
    }
    SDL_RenderPresent(renderer);
}

void update()
{
    int t2 = SDL_GetTicks();
    float dt = float(t2-t);
    t = t2;
    float velocity = 0.001;

    for(size_t i = 0; i < stars.size(); i++)
    {   
        stars[i][2] -= velocity*dt;

        if( stars[i][2] <= 0) stars[i][2] += 1;
        if( stars[i][2] > 1) stars[i][2] -= 1;
    }
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
    std::cout << "Hello world on Starfiled project \n";

    if(init("Video Game - SDL window", 
    SDL_WINDOWPOS_UNDEFINED, 
    SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN))
    {
        running = true;
        t = SDL_GetTicks();
    }
    else
    {
        return 1;
    }

    randomizePosition(stars);
    SDL_Event event;
    while (running)
    {
        render(renderer);
        update();
        //events();
        if(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    running = false;
                break;
                default:
                break;
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}

/*
 //for (size_t i = 0; i < 10; i++)
    //{
        //std::cout << "Star (" << stars[i][0] << " ,";
        //std::cout << stars[i][1] << " ,";
        //std::cout << stars[i][2] << " )";

        //WIDTH/2 + stars[i][0]*WIDTH/2, HEIGHT/2 + stars[i][1]*HEIGHT/2
        // float x = WIDTH/2 + stars[i][0]*WIDTH/2;
        // float y = HEIGHT/2 + stars[i][1]*HEIGHT/2;
        // float z = WIDTH/2 + stars[i][2]*WIDTH/2;

        // std::cout << " 3D positions (X:" << x << ", ";
        // std::cout << "Y:" << y << ", ";
        // std::cout << "Z:" << z << ")\n";
    //}
*/