#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <cstdlib>
#include <ctime>


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


const int SNAKE_SIZE = 20;


const int GAME_SPEED = 150;


int score = 0;


struct Snake {
    int x, y;
};


struct Fruit {
    int x, y;
};


enum Direction { UP, DOWN, LEFT, RIGHT };


void handleInput(Direction& dir);
void moveSnake(std::vector<Snake>& snake, Direction dir);
bool checkCollision(const std::vector<Snake>& snake);
void generateFruit(Fruit& fruit, const std::vector<Snake>& snake);
void renderScore(SDL_Renderer* renderer);
void renderSnake(SDL_Renderer* renderer, const std::vector<Snake>& snake);
void renderFruit(SDL_Renderer* renderer, const Fruit& fruit);

int main(int argc, char* args[]) {
   
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    
    SDL_Window* window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    
    std::srand(std::time(0));

    
    std::vector<Snake> snake = {{SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2}};

    
    Fruit fruit;
    generateFruit(fruit, snake);

    
    bool quit = false;
    Direction dir = RIGHT;
    Uint32 lastTime = SDL_GetTicks();

    while (!quit) {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            handleInput(dir);
        }

        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastTime >= GAME_SPEED) {
            moveSnake(snake, dir);
            if (checkCollision(snake)) {
                std::cout << "Game Over! Your Score: " << score << std::endl;
                quit = true;
            }

            if (snake.front().x == fruit.x && snake.front().y == fruit.y) {
                
                score += 10;
                generateFruit(fruit, snake);
                snake.push_back(snake.back());  
            }

            lastTime = currentTime;
        }

        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        
        renderScore(renderer);
        renderSnake(renderer, snake);
        renderFruit(renderer, fruit);

        
        SDL_RenderPresent(renderer);
    }

    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void handleInput(Direction& dir) {
    const Uint8* currentKeyStates = SDL_GetKeyboardState(nullptr);

    if (currentKeyStates[SDL_SCANCODE_UP] && dir != DOWN) {
        dir = UP;
    } else if (currentKeyStates[SDL_SCANCODE_DOWN] && dir != UP) {
        dir = DOWN;
    } else if (currentKeyStates[SDL_SCANCODE_LEFT] && dir != RIGHT) {
        dir = LEFT;
    } else if (currentKeyStates[SDL_SCANCODE_RIGHT] && dir != LEFT) {
        dir = RIGHT;
    }
}

void moveSnake(std::vector<Snake>& snake, Direction dir) {
    for (int i = snake.size() - 1; i > 0; --i) {
        snake[i] = snake[i - 1];
    }

    switch (dir) {
        case UP:
            snake[0].y -= SNAKE_SIZE;
            break;
        case DOWN:
            snake[0].y += SNAKE_SIZE;
            break;
        case LEFT:
            snake[0].x -= SNAKE_SIZE;
            break;
        case RIGHT:
            snake[0].x += SNAKE_SIZE;
            break;
    }
}

bool checkCollision(const std::vector<Snake>& snake) {
    
    if (snake.front().x < 0 || snake.front().x >= SCREEN_WIDTH || snake.front().y < 0 || snake.front().y >= SCREEN_HEIGHT) {
        return true;
    }

    
    for (size_t i = 1; i < snake.size(); ++i) {
        if (snake[i].x == snake.front().x && snake[i].y == snake.front().y) {
            return true;
        }
    }

    return false;
}

void generateFruit(Fruit& fruit, const std::vector<Snake>& snake) {
    bool valid = false;

    while (!valid) {
        valid = true;
        fruit.x = (std::rand() % (SCREEN_WIDTH / SNAKE_SIZE)) * SNAKE_SIZE;
        fruit.y = (std::rand() % (SCREEN_HEIGHT / SNAKE_SIZE)) * SNAKE_SIZE;

        // Make sure the fruit is not generated on the snake
        for (const auto& segment : snake) {
            if (fruit.x == segment.x && fruit.y == segment.y) {
                valid = false;
                break;
            }
        }
    }
}

void renderScore(SDL_Renderer* renderer) {
    std::string scoreText = "Score: " + std::to_string(score);

    SDL_Rect textRect = {10, 10, static_cast<int>(scoreText.length()) * 10, 20}; 
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    
    for (size_t i = 0; i < scoreText.length(); ++i) {
        SDL_Surface* textSurface = SDL_CreateRGBSurfaceWithFormat(0, 10, 20, 32, SDL_PIXELFORMAT_RGBA32);
        SDL_FillRect(textSurface, NULL, SDL_MapRGBA(textSurface->format, 0, 0, 0, 0));

        SDL_Color textColor = {255, 255, 255, 255};
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_FreeSurface(textSurface);

        SDL_Rect charRect = {10 + static_cast<int>(i) * 10, 10, 10, 20}; 
        SDL_RenderCopy(renderer, textTexture, nullptr, &charRect);
        SDL_DestroyTexture(textTexture);
    }
}

void renderSnake(SDL_Renderer* renderer, const std::vector<Snake>& snake) {
    for (const auto& segment : snake) {
        SDL_Rect rect = {segment.x, segment.y, SNAKE_SIZE, SNAKE_SIZE};
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}

void renderFruit(SDL_Renderer* renderer, const Fruit& fruit) {
    SDL_Rect rect = {fruit.x, fruit.y, SNAKE_SIZE, SNAKE_SIZE};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}