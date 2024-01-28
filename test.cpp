#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
using namespace std;

#undef main

// Direction enum declaration
enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

// Constants
const int SCREEN_WIDTH = 780;
const int SCREEN_HEIGHT = 570;
const int TILE_SIZE = 30;
const int REGULAR_FOOD_SIZE = TILE_SIZE;
const int BONUS_FOOD_SIZE = TILE_SIZE; // Double the size
const int TARGET_FRAME_RATE = 60;          // Frames per second
const int MOVEMENT_DELAY = 150;            // Milliseconds delay between movements
const int BONUS_FOOD_DURATION = 4000;      // 4 seconds

//SDL_Texture* obs1=nullptr;
SDL_Texture* wall1=nullptr;
SDL_Texture* wall2=nullptr;
SDL_Texture* wall3=nullptr;
SDL_Texture* wall4=nullptr;

SDL_Texture* foodTexture;
SDL_Texture* bonusFoodTexture;

SDL_Texture* snakeHeadTexture;
SDL_Texture* snakeBodyTexture;
SDL_Texture* snakeTailTexture;



// Snake structure
struct SnakeSegment {
    int x, y;
};

// Function prototypes
void spawnFood();
void spawnBonusFood();
void eatBonusFood();
void update();
void render();
void handleInput();
void handleBonusFoodDuration();
void displayGameOver();
bool showWelcomeScreen();

// Obstacle rectangle
SDL_Rect wallRect1{50, 90, 180, 15};
SDL_Rect wallRect2{360, 450, 180, 15};
SDL_Rect wallRect3{75, 420, 15, 180};
SDL_Rect wallRect4{630, 10, 15, 180};

// Global variables
SDL_Window* window;
SDL_Renderer* renderer;
vector<SnakeSegment> snake;
SnakeSegment food, bonusFood;
int level=0;
Direction snakeDirection = Direction::RIGHT; // Initialize the direction
int score = 0;
int regularFoodEaten = 0;
Uint32 bonusFoodTimer = 0;
bool bonusFoodActive = false;
bool gamePaused = false;

// TTF Font and Textures
TTF_Font* font;
SDL_Texture* backgroundTexture;
SDL_Texture* scoreTexture;
SDL_Texture* levelTexture;
SDL_Texture* welcomeTexture;

Mix_Music* backgroundMusic;
Mix_Chunk* buttonClickSound;
Mix_Chunk* eatFoodSound;
Mix_Chunk* bonusFoodSound;
Mix_Chunk* gameOverSound;


// Button Rectangles
SDL_Rect yesButton = {200, 350, 100, 50};
SDL_Rect noButton = {440, 350, 100, 50};

int main(int argc, char* args[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL initialization failed: " << SDL_GetError() << endl;
        return 1;
    }

    if (TTF_Init() < 0) {
        cerr << "TTF initialization failed: " << TTF_GetError() << endl;
        SDL_Quit();
        return 1;
    }

    if (IMG_Init(IMG_INIT_JPG) != IMG_INIT_JPG) {
    cerr << "SDL_image initialization failed: " << IMG_GetError() << endl;
    SDL_Quit();
    TTF_Quit();
    return 1;
}

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT,2,1024);

 eatFoodSound = Mix_LoadWAV("food.mp3");
 bonusFoodSound = Mix_LoadWAV("bonus.mp3");
 backgroundMusic = Mix_LoadMUS("background.mp3");
 gameOverSound = Mix_LoadWAV("game over.mp3");
 buttonClickSound = Mix_LoadWAV("click.mp3");

// Check if loading was successful
if (!eatFoodSound || !bonusFoodSound || !backgroundMusic || !gameOverSound || !buttonClickSound) {
    cerr << "Failed to load audio files! SDL_mixer Error: " << Mix_GetError() << endl;
    // Handle loading error
}
    Mix_PlayMusic(backgroundMusic, -1);

    window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    srand(static_cast<unsigned>(time(0)));

    snake.push_back({0, 0});

    spawnFood();

    SDL_Surface* backgroundSurface = IMG_Load("sree5.jpg");
     backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);

     SDL_Surface* wallo1=IMG_Load("bamboo2.jpg");
    wall1=SDL_CreateTextureFromSurface(renderer,wallo1);
    SDL_FreeSurface(wallo1);


     SDL_Surface* wallo2=IMG_Load("bamboo2.jpg");
    wall2=SDL_CreateTextureFromSurface(renderer,wallo2);
    SDL_FreeSurface(wallo2);


     SDL_Surface* wallo3=IMG_Load("bamboo3.jpg");
    wall3=SDL_CreateTextureFromSurface(renderer,wallo3);
    SDL_FreeSurface(wallo3);

     SDL_Surface* wallo4=IMG_Load("bamboo3.jpg");
    wall4=SDL_CreateTextureFromSurface(renderer,wallo4);
    SDL_FreeSurface(wallo4);
    

SDL_Surface* foodSurface = IMG_Load("cherry1.png");
foodTexture = SDL_CreateTextureFromSurface(renderer, foodSurface);
SDL_FreeSurface(foodSurface);

SDL_Surface* bonusFoodSurface = IMG_Load("orange.png");
bonusFoodTexture = SDL_CreateTextureFromSurface(renderer, bonusFoodSurface);
SDL_FreeSurface(bonusFoodSurface);
    

SDL_Surface* snakeHeadSurface = IMG_Load("unnamed.png"); 
snakeHeadTexture = SDL_CreateTextureFromSurface(renderer, snakeHeadSurface);
SDL_FreeSurface(snakeHeadSurface);

SDL_Surface* snakeBodySurface = IMG_Load("yc.png"); 
snakeBodyTexture = SDL_CreateTextureFromSurface(renderer, snakeBodySurface);
SDL_FreeSurface(snakeBodySurface);

SDL_Surface* snakeTailSurface = IMG_Load("yc.png");
snakeTailTexture = SDL_CreateTextureFromSurface(renderer, snakeTailSurface);
SDL_FreeSurface(snakeTailSurface);


    // Load font
    font = TTF_OpenFont("Milk Mango.otf", 24); 

    if (!font) {
        cerr << "Failed to load font: " << TTF_GetError() << endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

// Show welcome screen
    bool startGame = showWelcomeScreen();

    if (!startGame) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_CloseFont(font);
        TTF_Quit();
        SDL_Quit();
        return 0;
    }

    snake.push_back({0, 0});

    spawnFood();

    // Main game loop
    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_p) {
                    gamePaused = !gamePaused;
                }
            }else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                if (mouseX >= yesButton.x && mouseX <= yesButton.x + yesButton.w &&
                    mouseY >= yesButton.y && mouseY <= yesButton.y + yesButton.h) {
                    Mix_PlayChannel(-1, buttonClickSound, 0);
                    startGame = true;
                    snake.clear();
                    snake.push_back({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2});
                    spawnFood();
                    score = 0;
                    regularFoodEaten = 0;
                    bonusFoodActive = false;
                    bonusFoodTimer = 0;
                    gamePaused = false;
                } else if (mouseX >= noButton.x && mouseX <= noButton.x + noButton.w &&
                           mouseY >= noButton.y && mouseY <= noButton.y + noButton.h) {
                        Mix_PlayChannel(-1, buttonClickSound, 0);
                    startGame = false;
                    quit = true;
                }
            }

            handleInput();
        }

        if (!gamePaused && startGame) {
            update();
        }

        render();
        
        SDL_Delay(MOVEMENT_DELAY);
    }

    // Cleanup and exit
    displayGameOver();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 0;
}

bool showWelcomeScreen() {
    SDL_SetRenderDrawColor(renderer, 219, 112, 147, 255);
    SDL_RenderClear(renderer);

    SDL_Color textColor = {255, 255, 255, 255};
    string welcomeText = "WELCOME TO SNAKE GAME.";
    string qText = "DO YOU WANT TO PLAY?";

    // Render welcome message
    SDL_Surface* welcomeSurface = TTF_RenderText_Solid(font, welcomeText.c_str(), textColor);
    welcomeTexture = SDL_CreateTextureFromSurface(renderer, welcomeSurface);

    SDL_Rect welcomeRect = {(SCREEN_WIDTH - welcomeSurface->w) / 2, (SCREEN_HEIGHT/2 - welcomeSurface->h),
                             welcomeSurface->w, welcomeSurface->h};
    SDL_RenderCopy(renderer, welcomeTexture, nullptr, &welcomeRect);

    SDL_Surface* qSurface = TTF_RenderText_Solid(font, qText.c_str(), textColor);
    SDL_Texture* qTexture = SDL_CreateTextureFromSurface(renderer, qSurface);

    SDL_Rect qRect = {(SCREEN_WIDTH - welcomeSurface->w) / 2, (SCREEN_HEIGHT/2 + welcomeSurface->h),
                             welcomeSurface->w, welcomeSurface->h};
    SDL_RenderCopy(renderer, qTexture, nullptr, &qRect);

    SDL_FreeSurface(qSurface);

    SDL_FreeSurface(welcomeSurface);

    // Render buttons
    SDL_SetRenderDrawColor(renderer, 75, 0, 130, 255);
    SDL_RenderFillRect(renderer, &yesButton);

    SDL_SetRenderDrawColor(renderer, 220, 20, 60, 255);
    SDL_RenderFillRect(renderer, &noButton);

    // Render button text
    string yesButtonText = "Yes";
    SDL_Surface* yesButtonSurface = TTF_RenderText_Solid(font, yesButtonText.c_str(), textColor);
    SDL_Texture* yesButtonTexture = SDL_CreateTextureFromSurface(renderer, yesButtonSurface);

    SDL_Rect yesButtonRect = {yesButton.x + (yesButton.w - yesButtonSurface->w) / 2,
                              yesButton.y + (yesButton.h - yesButtonSurface->h) / 2,
                              yesButtonSurface->w, yesButtonSurface->h};
    SDL_RenderCopy(renderer, yesButtonTexture, nullptr, &yesButtonRect);

    SDL_FreeSurface(yesButtonSurface);

    string noButtonText = "No";
    SDL_Surface* noButtonSurface = TTF_RenderText_Solid(font, noButtonText.c_str(), textColor);
    SDL_Texture* noButtonTexture = SDL_CreateTextureFromSurface(renderer, noButtonSurface);

    SDL_Rect noButtonRect = {noButton.x + (noButton.w - noButtonSurface->w) / 2,
                             noButton.y + (noButton.h - noButtonSurface->h) / 2,
                             noButtonSurface->w, noButtonSurface->h};
    SDL_RenderCopy(renderer, noButtonTexture, nullptr, &noButtonRect);

    SDL_FreeSurface(noButtonSurface);

    SDL_RenderPresent(renderer);

    // Wait for user input
    SDL_Event e;
    while (true) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                return false;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                if (mouseX >= yesButton.x && mouseX <= yesButton.x + yesButton.w &&
                    mouseY >= yesButton.y && mouseY <= yesButton.y + yesButton.h) {
                    Mix_PlayChannel(-1, buttonClickSound, 0);
                    return true;
                    
                } else if (mouseX >= noButton.x && mouseX <= noButton.x + noButton.w &&
                           mouseY >= noButton.y && mouseY <= noButton.y + noButton.h) {
                    Mix_PlayChannel(-1, buttonClickSound, 0);
                    return false;
                    
                }
            }
        }
    }
}

void spawnFood() {
    int maxX = SCREEN_WIDTH / TILE_SIZE;
    int maxY = SCREEN_HEIGHT / TILE_SIZE;

    food.x = rand() % maxX * TILE_SIZE;
    food.y = rand() % maxY * TILE_SIZE;

    if ((food.x + TILE_SIZE > wallRect1.x && food.x < wallRect1.x + wallRect1.w &&
     food.y + TILE_SIZE > wallRect1.y && food.y < wallRect1.y + wallRect1.h) ||
    (food.x + TILE_SIZE > wallRect2.x && food.x < wallRect2.x + wallRect2.w &&
     food.y + TILE_SIZE > wallRect2.y && food.y < wallRect2.y + wallRect2.h) ||
    (food.x + TILE_SIZE > wallRect3.x && food.x < wallRect3.x + wallRect3.w &&
     food.y + TILE_SIZE > wallRect3.y && food.y < wallRect3.y + wallRect3.h) ||
    (food.x + TILE_SIZE > wallRect4.x && food.x < wallRect4.x + wallRect4.w &&
     food.y + TILE_SIZE > wallRect4.y && food.y < wallRect4.y + wallRect4.h)) {
    spawnFood();
    Mix_PlayChannel(-1, eatFoodSound, 0);
    return;
}


}

void spawnBonusFood() {
    int maxX = (SCREEN_WIDTH - 2 * TILE_SIZE) / TILE_SIZE;
    int maxY = (SCREEN_HEIGHT - 2 * TILE_SIZE) / TILE_SIZE;

    bonusFood.x = TILE_SIZE + rand() % maxX * TILE_SIZE;
    bonusFood.y = TILE_SIZE + rand() % maxY * TILE_SIZE;

    if ((bonusFood.x + TILE_SIZE > wallRect1.x && bonusFood.x < wallRect1.x + wallRect1.w &&
     bonusFood.y + TILE_SIZE > wallRect1.y && bonusFood.y < wallRect1.y + wallRect1.h) ||
    (bonusFood.x + TILE_SIZE > wallRect2.x && bonusFood.x < wallRect2.x + wallRect2.w &&
     bonusFood.y + TILE_SIZE > wallRect2.y && bonusFood.y < wallRect2.y + wallRect2.h) ||
    (bonusFood.x + TILE_SIZE > wallRect3.x && bonusFood.x < wallRect3.x + wallRect3.w &&
     bonusFood.y + TILE_SIZE > wallRect3.y && bonusFood.y < wallRect3.y + wallRect3.h) ||
    (bonusFood.x + TILE_SIZE > wallRect4.x && bonusFood.x < wallRect4.x + wallRect4.w &&
     bonusFood.y + TILE_SIZE > wallRect4.y && bonusFood.y < wallRect4.y + wallRect4.h) ) {
    spawnBonusFood();
   
    return;
}



    bonusFoodActive = true;
    bonusFoodTimer = SDL_GetTicks();
}

void eatBonusFood() {
    bonusFoodActive = false;
    // Add any other logic you may need after eating the bonus food
}

void update() {
  
    SnakeSegment head = snake.front();
    switch (snakeDirection) {
        case Direction::UP:
            head.y -= TILE_SIZE;
            break;
        case Direction::DOWN:
            head.y += TILE_SIZE;
            break;
        case Direction::LEFT:
            head.x -= TILE_SIZE;
            break;
        case Direction::RIGHT:
            head.x += TILE_SIZE;
            break;
    }


    // Wrap around the screen if the head hits the boundary
    if (head.x < 0) {
        head.x = SCREEN_WIDTH - TILE_SIZE;
    } else if (head.x >= SCREEN_WIDTH) {
        head.x = 0;
    }

    if (head.y < 0) {
        head.y = SCREEN_HEIGHT - TILE_SIZE;
    } else if (head.y >= SCREEN_HEIGHT) {
        head.y = 0;
    }

    if ((head.x + TILE_SIZE > wallRect1.x && head.x < wallRect1.x + wallRect1.w &&
         head.y + TILE_SIZE > wallRect1.y && head.y < wallRect1.y + wallRect1.h) ||
        (head.x + TILE_SIZE > wallRect2.x && head.x < wallRect2.x + wallRect2.w &&
         head.y + TILE_SIZE > wallRect2.y && head.y < wallRect2.y + wallRect2.h) ||
        (head.x + TILE_SIZE > wallRect3.x && head.x < wallRect3.x + wallRect3.w &&
         head.y + TILE_SIZE > wallRect3.y && head.y < wallRect3.y + wallRect3.h) ||
        (head.x + TILE_SIZE > wallRect4.x && head.x < wallRect4.x + wallRect4.w &&
         head.y + TILE_SIZE > wallRect4.y && head.y < wallRect4.y + wallRect4.h)) {
        displayGameOver();
        return;
    }

    if (head.x == food.x && head.y == food.y) {
        regularFoodEaten++;
        Mix_PlayChannel(-1, eatFoodSound, 0);
        
        if (regularFoodEaten == 7) {
            regularFoodEaten = 0;
            score += 10;
            spawnFood();
            spawnBonusFood();
            
        } else {
            score += 10;
            spawnFood();
        }
    } else if (bonusFoodActive && head.x == bonusFood.x && head.y == bonusFood.y) {
        score += 20;
        bonusFoodActive = false;
        spawnFood();
        level++;
        Mix_PlayChannel(-1, bonusFoodSound, 0);
        
    } else {
        snake.pop_back();
    }

    for (const auto& segment : snake) {
        if (head.x == segment.x && head.y == segment.y) {
            displayGameOver();
            return;
        }
    }

    snake.insert(snake.begin(), head);

    handleBonusFoodDuration();
}

void render() {
   
    // Render the background
    SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);


    SDL_Rect foodRect = {food.x, food.y, REGULAR_FOOD_SIZE, REGULAR_FOOD_SIZE};
SDL_RenderCopy(renderer, foodTexture, nullptr, &foodRect);

if (bonusFoodActive) {
    SDL_Rect bonusFoodRect = {bonusFood.x, bonusFood.y, BONUS_FOOD_SIZE, BONUS_FOOD_SIZE};
    SDL_RenderCopy(renderer, bonusFoodTexture, nullptr, &bonusFoodRect);
}

 
 for (size_t i = 0; i < snake.size(); ++i) {
        SDL_Texture* snakeSegmentTexture = nullptr;

        if (i == 0) {
            // Snake head
            snakeSegmentTexture = snakeHeadTexture;  // Replace headTexture with your snake head image
        } else if (i == snake.size() - 1) {
            // Snake tail
            snakeSegmentTexture = snakeTailTexture;  // Replace tailTexture with your snake tail image
        } else {
            // Snake body segment
            snakeSegmentTexture = snakeBodyTexture;  // Replace bodyTexture with your snake body image
        }

        SDL_Rect segmentRect = {snake[i].x, snake[i].y, TILE_SIZE, TILE_SIZE};
        SDL_RenderCopy(renderer, snakeSegmentTexture, nullptr, &segmentRect);
    }




    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_RenderFillRect(renderer, &wallRect1);
    
    SDL_RenderFillRect(renderer, &wallRect2);
    
    SDL_RenderFillRect(renderer, &wallRect3);
    
    SDL_RenderFillRect(renderer, &wallRect4);

    // Render score, level, and other UI elements

    SDL_Color textColor = {0, 0, 0, 255};
    string scoreText = "Score: " + to_string(score);
    // Render score
    SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);

    SDL_Rect scoreRect = {10, 10, scoreSurface->w, scoreSurface->h};
    SDL_RenderCopy(renderer, scoreTexture, nullptr, &scoreRect);

    SDL_RenderCopy(renderer, wall1, nullptr, &wallRect1);

    SDL_RenderCopy(renderer, wall2, nullptr, &wallRect2);

    SDL_RenderCopy(renderer, wall3, nullptr, &wallRect3);

    SDL_RenderCopy(renderer, wall4, nullptr, &wallRect4);



    SDL_FreeSurface(scoreSurface);

    // Render level board (you can customize it based on your game's logic)
    string levelText = "Level: " + to_string(level); // Customize based on your game's logic
    SDL_Surface* levelSurface = TTF_RenderText_Solid(font, levelText.c_str(), textColor);
    levelTexture = SDL_CreateTextureFromSurface(renderer, levelSurface);

    SDL_Rect levelRect = {SCREEN_WIDTH - levelSurface->w - 10, 10, levelSurface->w, levelSurface->h};
    SDL_RenderCopy(renderer, levelTexture, nullptr, &levelRect);

    SDL_RenderPresent(renderer);
}


void handleInput() {
    const Uint8* currentKeyStates = SDL_GetKeyboardState(nullptr);

    if (currentKeyStates[SDL_SCANCODE_UP] && snakeDirection != Direction::DOWN) {
        snakeDirection = Direction::UP;
    } else if (currentKeyStates[SDL_SCANCODE_DOWN] && snakeDirection != Direction::UP) {
        snakeDirection = Direction::DOWN;
    } else if (currentKeyStates[SDL_SCANCODE_LEFT] && snakeDirection != Direction::RIGHT) {
        snakeDirection = Direction::LEFT;
    } else if (currentKeyStates[SDL_SCANCODE_RIGHT] && snakeDirection != Direction::LEFT) {
        snakeDirection = Direction::RIGHT;
    }
}

void handleBonusFoodDuration() {
    if (bonusFoodActive && (SDL_GetTicks() - bonusFoodTimer >= BONUS_FOOD_DURATION)) {
        bonusFoodActive = false;
        spawnFood();
    }
}

void displayGameOver() {
     // Play game over sound
    Mix_PauseMusic();
    Mix_PlayChannel(-1, gameOverSound, 0);
    SDL_SetRenderDrawColor(renderer, 255,228,225, 255);
    SDL_RenderClear(renderer);

    SDL_Color textColor = {0, 0, 0, 255};
    string gameOverText = "Game Over!!!";
    // Render "Game Over" message with score
    SDL_Surface* gameOverSurface = TTF_RenderText_Solid(font, gameOverText.c_str(), textColor);
    SDL_Texture* gameOverTexture = SDL_CreateTextureFromSurface(renderer, gameOverSurface);

    SDL_Rect gameOverRect = {(SCREEN_WIDTH - gameOverSurface->w)/2, (SCREEN_HEIGHT - gameOverSurface->h) / 2,
                             gameOverSurface->w, gameOverSurface->h};
    SDL_RenderCopy(renderer, gameOverTexture, nullptr, &gameOverRect);

    string scoreText = "Score: " + to_string(score);

    // Render score
    SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);

    SDL_Rect scoreRect = {(SCREEN_WIDTH - gameOverSurface->w)/2, (SCREEN_HEIGHT/2 + gameOverSurface->h),
                             gameOverSurface->w, gameOverSurface->h};
    SDL_RenderCopy(renderer, scoreTexture, nullptr, &scoreRect);

    SDL_FreeSurface(scoreSurface);

    SDL_FreeSurface(gameOverSurface);
    SDL_DestroyTexture(gameOverTexture);

    SDL_DestroyTexture(backgroundTexture);

    SDL_DestroyTexture(wall1);
    SDL_DestroyTexture(wall2);
    SDL_DestroyTexture(wall3);
    SDL_DestroyTexture(wall4);

    SDL_DestroyTexture(foodTexture);
    SDL_DestroyTexture(bonusFoodTexture);

    SDL_DestroyTexture(snakeHeadTexture);

    SDL_RenderPresent(renderer);

    // Wait for a few seconds before exiting
    SDL_Delay(2000);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    exit(0);
}