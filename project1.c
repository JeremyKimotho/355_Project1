/*
    Author: Jeremy Kimotho
    Date: 21/10/2020
    Description:
    This game is inspired by the retro bomberman game, but it is an over simplified version. The game consists of a 2D board of hidden rewards and scores.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

int closestPower(int n){
    /*
        This function is used for the bitwise arithmetic in the random generation of numbers. It first checks if the inputted n is one less than a power of 2. If not it checks n+1 and so on until it finds such a value and returns it to be used.
    */

    while (ceil(log2(n + 1)) == floor(log2(n + 1)) != 1){
        n++;
    }
    return n;    
}

void insertExit(float (*a)[1000], int r, int c)
{
    /*
        Takes the board as input along with the row and column values then will generate 2 random integers with a maximum of the row and column values. Then assigns to a random position on the board. The exit tile is represented as *
    */
    // generates a random number between 0 and row value
    int random_r = rand() % r;
    // generates a random number between 0 and row column
    int random_c = rand() %c;
    // assigns the exit tile with a random position in table
    a[random_r][random_c] = 25.0;
}

void insertBonusSpecials(float (*a)[1000],  int r, int c){
    /*
        Takes the board as input along with the row and column values then will generate 2 random integers with a maximum of the row and column values. Then assigns the specials from those randoms.

        Extra lives instantly (27.0)- Represented as #

        Score instantly doubled (28.0)- Represented as @
    */
    // generates a random number between 0 and row value
    int random_r = rand() % r;
    // generates a random number between 0 and row column
    int random_c = rand() % c;
    // assigns the all sides destroyed tile with a random position in table
    a[random_r][random_c] = 27.0;
    // generates a random number between 0 and row value
    random_r = rand() % r;
    // generates a random number between 0 and row column
    random_c = rand() % c;
    // assigns the instant double score tile with a random position in table
    a[random_r][random_c] = 28.0;
}

float randomNum(int n){
    /* 
        The function takes an upper bound n as input. Will generate an integer random between 1 and around n. Will then subtract a random float between 0 and 1. 
    */
    float randomInt = rand() & closestPower(n);
    float randomDecimals = (float)(rand())/(float)(RAND_MAX);

    float random =  randomInt-randomDecimals;

    // Our masking may produce a value larger than our limit so we do a check before we return it.
    while(random>n*100){
        randomInt = rand() & closestPower(n);
        randomDecimals = (float)(rand()) / (float)(RAND_MAX);

        random = randomInt - randomDecimals;
    }

    return random;
}

float toNegative(float randomNumber, int negative_count,  int negative_max){
    /*
        The function decides whether or not to negative a value. Takes number of negative already present and the maximum permitted. If quota hasn't been reached 50/50 whether will be made negative.
    */

    //  If we've maxed out our quota we don't do anything
    if(negative_count == negative_max){
        return randomNumber;
    }else{
        // generates a random number between 0 and 10
        int chances = rand() % 12;
        
        // if chances is an even number will make our random  number negative by multiplying by 1
        if(chances%2==0){
            return randomNumber*-1;
        }else{
            return randomNumber;
        }
    }
}

float toSpecial(float randomNumber, int special_count, int special_max, int row_count, int row_max)
{
    /*
        The function decides whether or not to make a value a special. Takes number of specials already present and the maximum permitted. If quota hasn't been reached 40% chance it will be made a special. We also check how many values are already in the row,  if the row max has been reached we don't do anything.
    
        Bomb-range doubled (26.0)- Represented as $
    */

    //  If we've maxed out our row quota we don't do anything
    if (row_count == row_max)
    {
        return randomNumber;
    }
    else
    {
        //  If we've maxed out our quota we don't do anything
        if (special_count == special_max)
        {
            return randomNumber;
        }
        else
        {
            // generates a random number between 0 and 10
            int chances = rand() % 10;

            // if chances is an even number will make our random number a special
            if (chances % 2 == 0|| chances % 5 == 0)
            {
                return randomNumber;
            }
            else
            {
                return randomNumber = 26.0;
            }
        }
    }
}

int initialize(float (*a)[1000], int r, int c)
{
    /* 
        The function takes a pointer to an array as input as well as the row and column number and will populate the array with random integers. The randomNum function is called to produce an integer and it then assigns every value in the array a random integer.
    */

    // seeds the random function
    srand(time(NULL));  

    // keeps track of our ratios which is around 40% negative tiles, around 40% positive tiles and around 20% special tiles (exit tile included)
    int special_count = 0;
    // we only want a maximum of 20% of the total tiles containing specials
    int special_max = 0.2 * (r * c);
    int negative_count = 0;
    // we only want a maximum of 40% of the total tiles to be negative
    int negative_max = 0.4 * (r * c);

    for (int i = 0; i < r; i++)
    {
        // Keeping track of how many specials we've assigned to the particular row
        int row_count = 0;
        int row_max = ceil(special_max / r);
        for (int j = 0; j < c; j++)
        {
            // generates a random float no larger than 15
            float random = randomNum(15);

            // Passing random to the toNegative function which may or may not make the value negative
            random = toNegative(random, negative_count, negative_max);
            // Passing random to the toSpecial function which may or may not make the value a special
            random = toSpecial(random, special_count, special_max, row_count, row_max);

            // if random is negative we increment our negative counter and if it's a special we increment that counter as well as the row special counter
            if (random < 0){
                negative_count++;
            }else if(random>15){
                special_count++;
                row_count++;
            }

            // now we assign the random to row, column
            a[i][j] = random;
        }
        
    }

    // Contingency because sometimes negative count can have extreme low values like 27 so if lower than threshold of 36, we run a loop and put our values into the toNegative and it will raise the negative count
    if(negative_count<37){
        for (int x=0;x<3;x++){
            for (int i = 0; i < r; i++)
            {
                for (int j = 0; j < c; j++)
                {
                    // ensuring we're not making a special negative
                    if (a[i][j] < 15)
                    {
                        toNegative(a[i][j], negative_count, negative_max);
                        // making sure we increment the max counter
                        if (a[i][j] < 0)
                            negative_max++;
                    }
                }
            }
        }
    }

    // inserts my bonus specials
    insertBonusSpecials(a, r, c);

    // insert the exit tile to a random position
    insertExit(a, r, c);
    // returns the number of negatives to display to the user
    return negative_count;
}

int display(char (*a)[1000], int r, int c)
{
    /* 
        The function displays all values in a 2d array line-by-line  
    */
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            // Prints the elements of table as characters
            printf("%c ", a[i][j]);
        }
        printf("\n");
    }
    return 0;
}

void logScore(char name[100], float score, double seconds, char logFile[20]){
    /*
        We write to a file here to save the results of the game. We take as input the name of the file to write to as a string as well as the information we're going to write to the file which is score, duration, and player name.
    */
    // Initialising a file variable 
    FILE *loggingFile;
    // Opening the file in append mode
    loggingFile = fopen(logFile, "a");
    // Writing the information we want into the file
    fprintf(loggingFile, "%s had a score of %0.02f in %0.1f seconds of play\n", name, score, seconds);
    // Closing the file to free  up memory
    fclose(loggingFile);
}

int calculateScore(float (*a)[1000], char (*b)[1000], int r, int c, int bomb_x, int bomb_y, float *score, int *special_double_range, float *specials,  int *exit_tile, int *lives){
    /*
        This function will take the two tables as input as well as the bomb placement information, and will calculate the score using the floating point board and will edit the char board that is displayed to the user. Will return 0 or 1 based on if the score is negative and if we have to decrement the lives. The input bomb_x and bomb_y tell us the first and second positions respectively. The float score will be to change the players total score. The int special_double_range is to track how many specials of the double type we found and the specials is for any other specials. 
    */

    // Will print to the user that because of they uncovered a double range tile in their last roll, they will have increased range
    if(*special_double_range!=0){
        printf("Bang!! Your bomb range will be increased\n");
    }

    // Will work out the number of layers we'll be uncovering by calculating 2 to the power of how many double ranges were found last roll
    int layers = pow(2, *special_double_range);

    // Resetting the double range to 0 after we record how many layers we'll be uncovering
    *special_double_range=0;

    // printf("The double range is %d and the layers is %d\n", *special_double_range, layers);

    // Working out which tiles we'll be uncovering by taking the value the user entered and minusing or adding the number of layers to uncover
    int row_start = bomb_x-layers;
    int column_start = bomb_y-layers;
    int row_end = bomb_x+layers;
    int column_end = bomb_y+layers;


    // To track the score for this particular roll
    float running_score = 0.00;

    for(int i=row_start;i<row_end+1;i++){
        for(int j=column_start;j<column_end+1;j++){
            // Making sure we stay in range of our board size
            if(i>-1&&j>-1&&i<r&&j<c){
                // We make sure the tile is still covered and we're not double counting
                if (a[i][j] > 15 && b[i][j]=='X')
                {
                    if (a[i][j] == 26.00)
                    {
                        // Adding 1 to our double range counter and replacing X with $
                        *special_double_range+=1;
                        b[i][j] = '$';
                    }
                    else if (a[i][j] == 27.00)
                    {
                        *specials+=27.00;
                        b[i][j] = '#';
                    }
                    else if (a[i][j] == 28.00)
                    {
                        *specials+=28.00;
                        b[i][j] = '@';
                    }
                    else if (a[i][j] == 25.00)
                    {
                        b[i][j] = '*';
                        printf("You have found the exit tile and won the game!\n");
                        *exit_tile+=1;
                    }
                }
                else
                {
                    // We make sure the tile is still covered and we're not double counting
                    if (a[i][j] < 0 && b[i][j]=='X')
                    {
                        // If the uncovered tile was negative a negative sign is placed on the board where a X was
                        b[i][j] = '-';
                        // Adding the uncovered tile floating point value to the score for this roll
                        running_score += a[i][j];
                    }
                    // We make sure the tile is still covered and we're not double counting
                    else if(a[i][j]>0 && b[i][j]=='X')
                    {
                        // If the uncovered tile was positive a positive sign is placed on the board where a X was
                        b[i][j] = '+';
                        // Adding the uncovered tile floating point value to the score for this roll
                        running_score += a[i][j];
                    }
                }
            }
        }
    }

    // Checking if the extra lives bonus special was found
    if(*specials==27.0){
        printf("You have found the extra lives bonus special tile! Your lives will be doubled immediately! \n");
        *lives+=3;
        *specials=0;
    }


    // Checking if the double score bonus special was found and their running score was positive
    if(*specials>=28.0&&running_score<0){
        printf("You have found the score double bonus tile! Your score was negative so it has been made positive and doubled\n");
        running_score*=-1;
        running_score *= 2;
        *specials = 0;
    }
    else if (*specials >= 28.0){
        printf("You have found the score double bonus tile! Your score for this roll has been doubled\n");
        running_score *= 2;
        *specials = 0;
    }

    // Printing to the user the score they uncovered
    printf("Total uncovered score of %0.02f points\n", running_score);

    // Adding the score the user uncovered to the total score
    *score+=running_score;

    // If the score the user uncovered is below 0 a 1 is returned and that will decrement the lives 
    if(running_score<0){
        return 1;
    }else{
        return 0;
    }
}

void coverTable(char (*a)[1000], int r, int c)
{
    /*  
        The function creates the game table. The function replaces the floating point values in the initialised table with Xs. 
    */
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            a[i][j]='X';
        }
    }
}

void displayUncovered(float (*a)[1000], int row, int column)
{
    /*
        We print out the values of our float array. We first check if the value is larger than 15 because this would denote a special value. We first convert those, if it's not a special we print it out using float precision.
    */
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            if(a[i][j]>15){
                // Printing our specials using their float equivalents to identify them
                if (a[i][j] == 25.0)
                {
                    printf(" ");
                    printf("*");
                }
                else if (a[i][j] == 26.0)
                {
                    printf(" ");
                    printf("$");
                }
                else if (a[i][j] == 27.0)
                {
                    printf(" ");
                    printf("#");
                }
                else if (a[i][j] == 28.0)
                {
                    printf(" ");
                    printf("@");
                }
            }else{
                // printing out all our values that aren't specials
                printf(" ");
                printf("%.02f", a[i][j]);
            }
        }
        printf("\n");
    }
}

float substring(char line[100]){
    /*
        A function that will extract the score we'll need for the displaying of top scores. It takes the line as input in the form of a string and loops through it to find the word of which precedes every score  and from there we keep taking the score till we run into i which is after every score.
    */

    // The length is how long of a string to expect as the absolute max for a score value. So with 20 we extract scores till roughly 10 quintillion
    int length = 20;
    int start = 0;

    // Where we store the scorein string form
    char extractedScore[10];
    for(int i=0;i<strlen(line);i++){
        // Searching for the word of
        if(line[i]=='o'&&line[i+1]=='f'){
            while(start<length){
                // Searching for the start of word in which signals end of score 
                if(line[i+3+start]=='i'){
                    start+=8;
                }else{
                    // Assigning score character by character
                    extractedScore[start] = line[i + 3 + start];
                    start++;
                }
            }
        }
    }

    // returning score as a floating point value
    return atof(extractedScore);
}

void displayTopScores(char logScores[20]){

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    char lines[100][100];

    FILE *loggedScores;

    // Asking the user if they want to view top n scores
    char topScores[5];
    int n;
    int document_size=0;
    do
    {
        printf("\n");
        printf("Would you like to view the top scores? (y/n) ");
        scanf("%s", topScores);
        printf("\n");
        if (strcmp(topScores, "y") == 0)
        {
            printf("How many top scores would you like to view? ");
            scanf("%d", &n);

            loggedScores = fopen(logScores, "r");

            int i=0;
            while ((read = getline(&line, &len, loggedScores)) != -1)
            {
                document_size+=1;
                strcpy(lines[i], line);
                i+=1;
            }

            for(int i=0;i<document_size-1;i++){
                for (int counter = 0; counter < document_size - 1; counter++)
                {
                    float first = substring(lines[counter]);
                    float second = substring(lines[counter + 1]);
                    if (first < second)
                    {
                        char intermediate[100];
                        strcpy(intermediate, lines[counter]);
                        strcpy(lines[counter], lines[counter + 1]);
                        strcpy(lines[counter + 1], intermediate);
                    }
                }
            }

            printf("\n");
            for(int i=0;i<n;i++){
                printf("%s", lines[i]);
            }

            fclose(loggedScores);
        }
    } while (strcmp(topScores, "y") == 0);

    if(document_size==0){
        printf("There were no scores to display\n");
    }

    // Flushing the buffer because scanf leaves next line character which will interfere with game loop
    fflush(stdin);
    
}

void exitGame(time_t *beginning, char name[100], float score, char logFile[20])
{
    /*
        We end the game in this function. We pass the required informstion to the logScore function which will write to a file then we ask the user if they want to view the top n scores then we end the program. We take as input the time when the game started, as well as the information we'll be writing to file which is name and score.
    */

    // End the timer that tracks how long the player has been playing for
    time_t end = time(NULL);
    // Works out the play length in seconds
    double seconds = difftime(end, *beginning);
    printf("\n");
    // Passes information to the logScore function which will write to a file
    logScore(name, score, seconds, logFile);
    printf("Game over! Your name, score, and duration of play have been recorded.\n");

    // Ask the user if they want to see the top scores for the game
    displayTopScores(logFile);
    printf("\n");
    printf("Program ended\n");
    exit(1);
}

int main(int argc, char *argv[])
{

    if (argc == 4)
    {
        
        // Assigning the row and column from the command line arguments
        int row = atoi(argv[2]);
        int column = atoi(argv[3]);

        // If the user enters a row or column value lower than 10, an error message is displayed and the program exits. 
        if (row < 10 || column < 10)
        {
            printf("You have entered a row or column value smaller than 10. The minimum value of row and column is 10\n");
            exit(1);
        }

        // Assigning the player name from command line arguments
        char player_name[100]; 
        strcpy(player_name, argv[1]);
        printf("Your username is %s. The row size is %d and column size %d\n", player_name, row, column);

        // Creating file that will be used for the logging of scores and names if it doesn't exist
        FILE *logFile;
        logFile = fopen("project1.log", "a+");
        fclose(logFile);
        char loggingFile[20] = "project1.log";

        // Asking the user if they want to see the top scores
        displayTopScores(loggingFile);

        // Table is used as game table and is initially populated by Xs. uncoveredTable is the table as float values and is what is initialised. uncoveredSymbol is to display the state of the game before the game starts and is a mixture of float valeues converted to strings, as well as exit tile and bonus packs.
        char table[1000][1000];
        float uncoveredTable[1000][1000];

        // Tracking negative count as compared to total cells
        int negative_count;
        int numberOfCells = row*column;

        // Initialising the uncovered table and then initialising the covered game table
        negative_count = initialize(uncoveredTable, row, column);

        coverTable(table, row, column);

        // Displaying the uncovered cable and negative count
        printf("\n");
        displayUncovered(uncoveredTable, row, column);
        printf("\n");
        printf("\n");
        printf("Total negative numbers is %d/%d\n", negative_count, numberOfCells);

        // Displaying the covered game and starting game
        printf("\n");
        display(table, row, column);

        /*
            Check commenting 
            Check code on uni servers
            script file
        */

        // Assigning bombs based on the number of tiles in the game
        int bombs;
        if(row*column<=100){
            bombs = 3;
        }else if(row*column<=400){
            bombs  = 4;
        }else{
            bombs = 5;
        }

        // Initialising the values we'll need for the game loop
        int lives = 3;
        float score = 0;
        float other_specials = 0.00;
        int double_range = 0;
        

        // Tracks if the exit tile has been found
        int exit_tile = 0;

        printf("\n");

        // Starting the timer
        time_t beginning = time(NULL);

        do{
            
            // Where the bomb locations will be kept
            int bomb_1, bomb_2;
            char exiting[10];

            // Prompting the user for the bomb positions 
            printf("Enter bomb position (x,y):");

            // Flushing the output to ensure buffering does not prevent the displaying
            fflush(stdout);

            //Where the string will be copied into
            char buffer[10];
            if (fgets(buffer, sizeof buffer, stdin) == NULL){
                // Returning to the user to recheck their input and then exiting 
                printf("\n");
                printf("There was an error in your input\n");
                printf("Please enter input in the form x,y\n");
                printf("\n");
                exit(1);
            }else{
                // Assigning the integers we collected to our bomb integers
                if (sscanf(buffer, "%d,%d", &bomb_1, &bomb_2) == 2)
                {
                    if(bomb_1<row&&bomb_2<column&&bomb_1>-1&&bomb_2>-1){
                        // Returning what the user entered back to them
                        printf("Your input was %d,%d\n", bomb_1, bomb_2);
                        printf("\n");

                        // Calculating the score and editing the game board. The calculate score will return a value 0 or 1 and if 0 is returned then we will not decrement a life and if 1 is returned we will decrement a life.
                        int livesToDecrement = calculateScore(uncoveredTable, table, row, column, bomb_1, bomb_2, &score, &double_range, &other_specials, &exit_tile, &lives);
                        lives -= livesToDecrement;

                        // Decrementing the bomb because the user used one
                        bombs--;

                        // When the user starts a new life, the score is zeroed
                        if (livesToDecrement == 1)
                        {
                            score = 0;
                        }

                        // If the score is below zero the player loses a life  and score becomes 0
                        if (score <= 0)
                        {
                            printf("\n");
                            printf("Your score has dipped below zero and therefore you've lost a life\n");
                            lives--;
                            score = 0;
                        }

                        // Displaying the board to the user as well as the number of lives, the score, and the bomb
                        display(table, row, column);
                        printf("Lives: %d\n", lives);
                        printf("Score: %0.02f\n", score);
                        printf("Bombs: %d\n", bombs);

                        // If the exit tile was found we end the game at this point after the current roll score was added to the total score
                        if (exit_tile == 1)
                        {
                            exitGame(&beginning, player_name, score, loggingFile);
                        }
                    }else{
                        printf("There was an error in your input\n");
                        printf("Please enter values of x and y smaller than the your row and column values, but larger than -1.\n");
                        exit(1);
                    }
                    
                }
                else
                {
                    // If the user entered nothing or a string, they have exited the game and their progress will be saved
                    printf("\n");
                    printf("You exited the game\n");
                    exitGame(&beginning, player_name, score, loggingFile);
                }
            }
        } while (lives > 0 && bombs > 0);

        // If bombs end or lives end the game ends
        exitGame(&beginning, player_name, score, loggingFile);
    }
    else
    {
        printf("You must enter input to the command line in the format:\nprogram_name playername M N\n");
    }
    return 0;
}