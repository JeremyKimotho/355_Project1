# CPSC 355 Project Part 1
#### A program written in C that emulates the vintage arcade game bomberman, 21/10/2020
#### By **Jeremy Kimotho**
## Description
The game consists of a 2D board of hidden rewards and scores. The scores are represented as floating point numbers, positive and negative. The board tiles are randomly populated by these numbers. In addition, the board has an exit tile and reward tiles that double the range of a bomb. When the user uncovers the exit tile, the game is won. The player starts with three lives and a score of zero and a specific number of bombs. S/He chooses a cell to place a bomb. The bomb explodes uncovering the immediate bordering tiles to the bomb. The user’s score is updated as the sum of all the values of the uncovered tiles. 
## Compilation/Running Requirements
* Compile using: gcc project1.c -o project1 -lm
* Run: ./project1 [playername] M N 
## Support and contact details
Contact me at projectsjeremy1000@gmail.com if you have any questions or comments.



