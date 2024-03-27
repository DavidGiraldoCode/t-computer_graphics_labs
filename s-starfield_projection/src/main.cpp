#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <SDL2/SDL.h>

int status = 0;
const int WIDTH = 1024;
const int HEIGHT = 768;
const int DEPTH = 2000;
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

const float nx = WIDTH+0.0f; //Avoid truncation by unsing nx as float
const float ny = HEIGHT+0.0f;
float point[3] = {512.0, 512.0, 512};
const float maxPoint[2] = {nx, ny};

const int STARS_COUNT = 1000;
std::vector<std::array<float, 3>> stars(STARS_COUNT);

void randomizePosition(std::vector<std::array<float, 3>> & stars)
{
    float randomXPos, randomYPos, randomZPos= 0.0;
    srand (time(nullptr)); //initialize random seed:

    for(size_t i = 0; i < stars.size(); i++)
    {
        randomXPos = float(rand()) / float(RAND_MAX);
        //rand() % WIDTH + 1; //generate secret number between 1 and max domain
        randomYPos = float(rand()) / float(RAND_MAX);
        randomZPos = float(rand()) / float(RAND_MAX);
        //rand() % HEIGHT + 1; //generate secret number between 1 and max domain
        stars[i][0] = 2.0 * randomXPos - 1.0;
        stars[i][1] = 2.0 * randomYPos - 1.0;
        stars[i][2] = randomZPos;//100;
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

    randomizePosition(stars);

    for (size_t i = 0; i < 10; i++)
    {
        std::cout << "Star (" << stars[i][0] << " ,";
        std::cout << stars[i][1] << " ,";
        std::cout << stars[i][2] << " )";

        //WIDTH/2 + stars[i][0]*WIDTH/2, HEIGHT/2 + stars[i][1]*HEIGHT/2
        float x = WIDTH/2 + stars[i][0]*WIDTH/2;
        float y = HEIGHT/2 + stars[i][1]*HEIGHT/2;
        float z = WIDTH/2 + stars[i][2]*WIDTH/2;

        std::cout << " 3D positions (X:" << x << ", ";
        std::cout << "Y:" << y << ", ";
        std::cout << "Z:" << z << ")\n";
    }
    float focalLenght = 240;
    SDL_RenderClear(renderer);
    for(size_t i = 0; i < stars.size() - 1; i++)
    {   
        point[0] = i % int(nx); // Linear index mapped to x-coodinate system
        point[1] = int(floor( int(i) / int(nx))) % int(ny); // Linear index mapped to y-coodinate system

        float x = WIDTH/2 + stars[i][0]*WIDTH/2;
        float y = HEIGHT/2 + stars[i][1]*HEIGHT/2;
        float z = DEPTH/2 + stars[i][2]*DEPTH/2;

        float u = 106 * (stars[i][0]/stars[i][2]) + WIDTH/2;
        float v = 90 * (stars[i][1]/stars[i][2]) + HEIGHT/2;

        float zColor = (z*255)/WIDTH;

        SDL_SetRenderDrawColor(renderer, zColor, zColor, zColor, 0);
        //SDL_RenderDrawPoint(renderer, point[0], point[1]);
        SDL_RenderDrawPoint(renderer, u, v);
        //SDL_RenderDrawPoint(renderer, WIDTH/2 + stars[i][0]*WIDTH/2, HEIGHT/2 + stars[i][1]*HEIGHT/2);
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
    std::cout << "Hello world on Starfiled project \n";

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