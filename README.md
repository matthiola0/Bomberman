# Bomberman: Bocchi the Rock Edition

This is a single-player Bomberman-like game developed using C++ and the Allegro 5 game library, inspired by the popular anime *Bocchi the Rock!*.

Players control members of the "Kessoku Band," using their instruments as bombs to color the stage with their territory. When time runs out, the player who has covered the largest area wins!

---

## 🎮 Game Features

* **Classic Gameplay, New Twist**: Combines the traditional Bomberman mechanics with a new territory control scoring system.
* **Bocchi the Rock Theme**: Play as one of four main characters — Hitori Gotoh, Nijika Ijichi, Ryo Yamada, or Ikuyo Kita.
* **Multiplayer Battles**: Supports 1 or 2 players competing against AI-controlled opponents.
* **Varied Maps**: Includes three themed maps: School, STARRY live house, and Bocchi's Room.
* **Dynamic Environments**: Some obstacles on the map can be destroyed and may drop power-up items.
* **High Risk, High Reward**: Beware! If you're caught in an explosion — even your own — your captured territory will reset to zero!
* **🤖 Intelligent AI System**:
    * **Collision Awareness**: AI intelligently navigates around obstacles instead of moving randomly.
    * **Strategic Bombing**: AI targets destructible stones to clear paths and maximize scoring.
    * **Danger Evasion**: AI detects active bombs and proactively moves to safe zones before detonation.

---

## 🕹️ How to Play

### Objective

Within the time limit, use bomb explosions to color as many floor tiles as possible with your representative color. The player with the largest colored area when time is up wins.

### Controls

#### Player 1

* Move: `W A S D`
* Place Bomb: `Space`

#### Player 2

* Move: Arrow Keys (↑ ↓ ← →)
* Place Bomb: `Enter`

---

## 📷 Demo Images & Videos

### Screenshots

![Screenshot 1](images/image1.png)
![Screenshot 2](images/image2.png)
![Screenshot 3](images/image3.png)
![Screenshot 4](images/image4.png)


### Gameplay Video

click to open vedio
[![Watch the demo](https://img.youtube.com/vi/plwuGUrNsGw/0.jpg)](https://www.youtube.com/watch?v=plwuGUrNsGw)


---

## 🛠️ Installation & Compilation

This project uses the Allegro 5 library. Please ensure you have a C++ compiler (e.g., GCC/G++) and MinGW (mingw32-make) installed.

### Option 1: Using VS Code (Recommended)
1. Open the project folder in VS Code.
2. Press **F5** to compile and run the game automatically (using the provided `.vscode` configurations).

### Option 2: Using Command Line
1. Open a terminal in the project folder.
2. Run the following command to compile:
   ```bash
   mingw32-make
   ```
3. Run the game:
   ```bash
   ./Bomberman.exe
   ```

If you encounter any issues during installation or compilation, refer to the included guide: `tutorial/Allegro_install_i2p2.pdf`.

---

## 👨‍💻 Developers

* Chun-Yun Chang
* Po-Yu Pan
