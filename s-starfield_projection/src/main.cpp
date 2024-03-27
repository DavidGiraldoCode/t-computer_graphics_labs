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
bool running = false;
int t = 0;

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
        randomYPos = float(rand()) / float(RAND_MAX);
        randomZPos = float(rand()) / float(RAND_MAX);
        stars[i][0] = 2.0 * randomXPos - 1.0;
        stars[i][1] = 2.0 * randomYPos - 1.0;
        stars[i][2] = randomZPos;//100;
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

void render(SDL_Renderer* & renderer)
{
    SDL_SetRenderDrawColor(renderer,0,0,0,0);
    SDL_RenderClear(renderer);
    std::cout << "Seconds passed: "<< (SDL_GetTicks()/1000) << "\n";
    float focalLenght = 240;

    for(size_t i = 0; i < stars.size() - 1; i++)
    {   
        point[0] = i % int(nx); // Linear index mapped to x-coodinate system
        point[1] = int(floor( int(i) / int(nx))) % int(ny); // Linear index mapped to y-coodinate system

        float x = WIDTH/2 + stars[i][0]*WIDTH/2;
        float y = HEIGHT/2 + stars[i][1]*HEIGHT/2;
        float z = DEPTH/2 + stars[i][2]*DEPTH/2;

        float u = 106 * (stars[i][0]/stars[i][2]) + WIDTH/2;
        float v = 90 * (stars[i][1]/stars[i][2]) + HEIGHT/2;

        float zColor = 255 - (255 * stars[i][2]);// - ((z*255)/WIDTH);

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

    // setup();
    // createWindowRenderer(WIDTH, HEIGHT, window, renderer);
    // if(status != 0)
    // {
    //     return -1;
    // }

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