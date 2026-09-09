# 🐍 Snake Game

A feature-rich snake game built with **C++** and **SDL2** library, featuring immersive graphics, sound effects, and engaging gameplay mechanics.

---

## 📋 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Project Structure](#project-structure)
- [Requirements](#requirements)
- [Installation & Setup](#installation--setup)
- [Compilation](#compilation)
- [How to Play](#how-to-play)
- [Game Mechanics](#game-mechanics)
- [Asset Information](#asset-information)
- [Technologies Used](#technologies-used)
- [License](#license)

---

## 🎮 Overview

This is a classic snake game implementation with modern graphics and sound effects. The game features obstacles, bonus foods, level progression, and an intuitive UI with a welcome screen and game-over display. The project uses SDL2 (Simple DirectMedia Layer) for rendering, text display, and audio playback.

---

## ✨ Features

- **Rich Graphics**: Custom sprite-based rendering for snake, food, obstacles, and backgrounds
- **Sound Effects**: 
  - Background music during gameplay
  - Sound effects for eating food, collecting bonuses, and game over
  - Click sounds for UI interactions
- **Game Mechanics**:
  - Regular food collection (10 points per food)
  - Bonus food system (20 points, appears after eating 7 regular foods)
  - Bonus food timer (4 seconds duration)
  - Level progression system
  - Obstacle/wall collision detection
  - Screen wrapping (seamless movement across boundaries)
- **UI Elements**:
  - Welcome screen with Yes/No buttons
  - Real-time score and level display
  - Game over screen with final score
  - Pause functionality (Press 'P')
- **Customizable Visuals**:
  - Custom fonts (Milk Mango, sree)
  - Themed background images
  - Unique sprite textures for snake head, body, and tail

---

## 📁 Project Structure

```
snake-project/
├── test.cpp                    # Main game implementation (97.6% C++)
├── main.cpp                    # SDL2 image rendering example
├── Snakegame p.cpp            # Previous version
├── new.cpp                     # Alternative implementation
├── makefile                    # Build configuration
│
├── Assets/
│   ├── Fonts/
│   │   ├── Milk Mango.ttf
│   │   ├── Milk Mango.otf
│   │   └── sree.otf
│   │
│   ├── Images/
│   │   ├── sree5.jpg           # Background
│   │   ├── bamboo2.jpg         # Wall textures
│   │   ├── bamboo3.jpg
│   │   ├── cherry1.png         # Food sprite
│   │   ├── orange.png          # Bonus food sprite
│   │   ├── unnamed.png         # Snake head
│   │   ├── yc.png              # Snake body/tail
│   │   ├── obstacle.png
│   │   └── Snake.png
│   │
│   ├── Audio/
│   │   ├── background.mp3      # BGM
│   │   ├── food.mp3            # Food eating sound
│   │   ├── bonus.mp3           # Bonus collection sound
│   │   ├── click.mp3           # UI click sound
│   │   └── game over.mp3       # Game over sound
│   │
│   └── DLLs/
│       ├── SDL2.dll
│       ├── SDL2_image.dll
│       ├── SDL2_mixer.dll
│       └── SDL2_ttf.dll
│
├── Executables/
│   ├── main.exe
│   └── test.exe
│
└── README.md
```

---

## 📦 Requirements

### System Requirements
- **OS**: Windows (precompiled with mingw32)
- **Processor**: Any modern processor
- **RAM**: Minimum 256 MB

### Software Dependencies
- **C++ Compiler**: g++ (with MinGW for Windows)
- **SDL2 Libraries**:
  - libSDL2 (core library)
  - libSDL2_ttf (text rendering)
  - libSDL2_image (image loading)
  - libSDL2_mixer (audio playback)

---

## 🛠️ Installation & Setup

### Windows Setup

1. **Ensure SDL2 libraries are installed** in the `src/lib` directory and headers in `src/include`:
   ```
   src/
   ├── include/          (SDL2 headers)
   └── lib/              (SDL2 libraries)
   ```

2. **Place all asset files** (fonts, images, audio) in the root project directory

3. **Ensure DLL files are present** in the root directory:
   - SDL2.dll
   - SDL2_image.dll
   - SDL2_mixer.dll
   - SDL2_ttf.dll

### Linux/Mac Setup

Modify the makefile to use appropriate paths and system libraries:
```bash
g++ -std=c++11 -o test test.cpp `pkg-config --cflags --libs sdl2 SDL2_image SDL2_ttf SDL2_mixer`
```

---

## 🔧 Compilation

### Using Makefile (Windows)
```bash
make
```

### Manual Compilation
```bash
# For the main game (test.cpp)
g++ -I src/include -L src/lib -o test test.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer

# For the image rendering demo (main.cpp)
g++ -I src/include -L src/lib -o main main.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image
```

---

## 🎮 How to Play

### Launch the Game
```bash
./test.exe    # On Windows
./test        # On Linux/Mac
```

### Game Controls
- **Arrow Keys**: Move the snake (↑ ↓ ← →)
- **P**: Pause/Resume the game
- **Mouse**: Click "Yes" or "No" on the welcome screen to start or exit
- **Close Window**: Quit the game

### Gameplay Flow
1. **Welcome Screen**: Click "Yes" to start playing
2. **Gameplay**: Guide the snake to eat food while avoiding:
   - Obstacles (walls inside the game area)
   - Your own snake body
3. **Scoring**:
   - Regular Food: **10 points** (colored cherry)
   - Bonus Food: **20 points** (colored orange, appears after eating 7 regular foods)
4. **Level Up**: Collecting bonus food increases your level
5. **Game Over**: Happens when you hit:
   - An obstacle/wall
   - Your own body
6. **Final Screen**: View your final score and exit

---

## 🎯 Game Mechanics

### Snake Movement
- Smooth tile-based movement (30x30 pixel tiles)
- Screen wrapping: exiting one side brings you to the opposite side
- 150ms delay between movements (configurable)

### Food System
- **Regular Food**: 
  - Spawns randomly on the game board
  - Avoids obstacle placement
  - 10 points per food
  - Grows the snake by 1 segment
  
- **Bonus Food**:
  - Appears after eating 7 regular foods
  - Lasts for 4 seconds (configurable)
  - 20 points
  - Increases level counter
  - Different sprite (orange)

### Obstacles
- 4 fixed bamboo walls positioned strategically:
  - Top wall
  - Bottom wall
  - Left wall
  - Right wall
- Collision detection prevents snake from passing through
- Food cannot spawn on obstacles

### Score System
- **Score Display**: Real-time on top-left
- **Level Display**: Real-time on top-right
- **Final Score**: Shown on game-over screen

---

## 🎨 Asset Information

### Fonts Used
- **Milk Mango.ttf/otf**: Main UI font (provided by khurasantype)
- **sree.otf**: Alternative font option

### Image Assets
- **Dimensions**: 30x30 tiles (adjustable)
- **Background**: sree5.jpg (800x600 resolution)
- **Sprites**:
  - Snake Head: unnamed.png
  - Snake Body/Tail: yc.png
  - Regular Food: cherry1.png
  - Bonus Food: orange.png
  - Walls: bamboo2.jpg, bamboo3.jpg

### Audio Assets
- **Sample Rate**: 44100 Hz
- **Format**: MP3
- **Background Music**: Loops continuously (background.mp3)
- **SFX**: One-shot playback on events

---

## 💻 Technologies Used

| Technology | Purpose | Version |
|-----------|---------|---------|
| C++ | Core game logic | C++11 standard |
| SDL2 | Graphics & rendering | 2.x |
| SDL2_image | Image loading (PNG, JPG) | 2.x |
| SDL2_ttf | Text rendering | 2.x |
| SDL2_mixer | Audio playback | 2.x |
| MinGW | C++ compilation (Windows) | Latest |

---

## 📊 Code Statistics

- **Primary Language**: C (97.6%)
- **Secondary Language**: C++ (1.2%)
- **Other**: 1.2%
- **Repository Size**: ~44 MB (includes large image/audio assets)

### File Breakdown
- **Game Logic**: test.cpp (21 KB) - Full-featured game implementation
- **Demo**: main.cpp (1.5 KB) - Simple image rendering example
- **Build System**: makefile (431 bytes)

---

## 🚀 Future Enhancements

Potential features for future versions:
- [ ] High score tracking and persistence
- [ ] Difficulty levels (easy, medium, hard)
- [ ] Power-ups system
- [ ] Multiplayer mode
- [ ] Leaderboard
- [ ] Additional game modes (timed mode, endless mode)
- [ ] Custom color themes
- [ ] Mobile/touchscreen support

---

## 📄 License & Attribution

This project includes assets from:
- **Font Assets**: "Milk Mango" font - Free for personal and commercial use (credited to khurasantype)
  - Contact: khurasantype@gmail.com
  - Portfolio: https://www.behance.net/khurasan

The code is provided as-is for educational and personal use purposes.

---

## 🤝 Contributing

Feel free to fork this project and submit pull requests for improvements!

---

## 📧 Support

For issues or questions:
1. Check the "More Info.txt" file in the repository
2. Review the asset attribution information above
3. Examine the makefile for compilation troubleshooting

---

## 🎉 Enjoy the Game!

Have fun playing! 🐍✨

---

*Last Updated: 2024*  
*Repository: [SnahaSree/snake-project](https://github.com/SnahaSree/snake-project)*
