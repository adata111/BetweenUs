## Between Us 
This is a simplified, single player version of Among Us. This 2D game involves a line maze and the player(`BLUE` Among Us character) has to complete 2 tasks and reach the end of the maze before reactor meltdown in order to win. There's one imposter (hence the name, *between* us :P). The imposter(`RED` Among Us character) uses shortest path algorithm to reach the Player and the game is over if the imposter reaches the player. There are 2 tasks - you have to reach 2 buttons situated at a random position in the maze. The `RED` button is the vapouriser and pressing that vapourises the imposter. The `GREEN` button releases power-ups/obstacles like coins(YELLOW circles) and bombs(dark grey mines). Reaching each of these buttons increases game score by 50. Collecting a coin increases score by 10 and hitting an obstacle reduces score by 10. The countdown timer until reactor meltdown is displayed in the HUD at the top of the screen. When the clock runs out the game gets over.

<img src="/game.png" alt="Game Image"/>

### Controls
- Arrow keys for moving the player in the repective direction

### How to run
In the game directory,
```bash
mkdir build
cd build
cmake ..
make
./Between-Us
```

Thanks to https://github.com/vallentin/glText for providing text rendering header file.
