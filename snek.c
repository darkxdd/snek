#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>

#define DEFAULT_WIDTH 40
#define DEFAULT_HEIGHT 20
#define MIN_WIDTH 20
#define MIN_HEIGHT 10

// Global variables
int gameover;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail, width, height;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
enum eDirection dir;

// Initialize the game settings
void Setup() {
    initscr();              // Initialize ncurses
    clear();                // Clear the screen
    noecho();               // Don't echo user input
    curs_set(0);            // Hide the cursor
    keypad(stdscr, TRUE);   // Enable keypad input
    timeout(0);             // Non-blocking input

    // Check if the terminal supports colors
    if (!has_colors()) {
        endwin();
        printf("Your terminal does not support color\n");
        exit(1);
    }
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK); // Borders
    init_pair(2, COLOR_GREEN, COLOR_BLACK); // Snake
    init_pair(3, COLOR_YELLOW, COLOR_BLACK); // Fruit
    init_pair(4, COLOR_WHITE, COLOR_BLACK); // Text

    gameover = 0;
    dir = STOP;
    score = 0;

    // Adjust width and height based on terminal size
    getmaxyx(stdscr, height, width);
    if (width < MIN_WIDTH + 2) width = DEFAULT_WIDTH;
    if (height < MIN_HEIGHT + 2) height = DEFAULT_HEIGHT;

    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
}

// Draw the game elements on the screen
void Draw() {
    clear(); // Clear the screen

    // Draw borders
    attron(COLOR_PAIR(1));
    for (int i = 0; i < width + 2; i++)
        mvprintw(1, i, " ");
    for (int i = 0; i < height; i++) {
        mvprintw(i + 2, 0, " ");
        mvprintw(i + 2, width + 1, " ");
    }
    for (int i = 0; i < width + 2; i++)
        mvprintw(height + 2, i, " ");
    attroff(COLOR_PAIR(1));

    // Draw snake
    attron(COLOR_PAIR(2));
    mvprintw(y + 2, x + 1, "O");
    for (int i = 0; i < nTail; i++)
        mvprintw(tailY[i] + 2, tailX[i] + 1, "o");
    attroff(COLOR_PAIR(2));

    // Draw fruit
    attron(COLOR_PAIR(3));
    mvprintw(fruitY + 2, fruitX + 1, "@");
    attroff(COLOR_PAIR(3));

    // Draw score and instructions
    attron(COLOR_PAIR(4));
    mvprintw(0, 0, "Score: %d", score);
    mvprintw(1, 0, "To quit, press 'q'.");
    attroff(COLOR_PAIR(4));

    refresh(); // Refresh the screen
}

// Handle user input
void Input() {
    int ch = wgetch(stdscr); // Get user input
    switch (ch) {
        case ERR:
            break; // No input
        case KEY_LEFT:
            dir = LEFT;
            break;
        case KEY_RIGHT:
            dir = RIGHT;
            break;
        case KEY_UP:
            dir = UP;
            break;
        case KEY_DOWN:
            dir = DOWN;
            break;
        case 'q':
            gameover = 1;
            break;
    }
}

// Update game logic
void Logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;

    // Move the tail
    tailX[0] = x;
    tailY[0] = y;
    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    // Move the snake
    switch (dir) {
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
    }

    // Wrap around the screen
    if (x >= width) x = 0; 
    if (x < 0) x = width - 1;
    if (y >= height) y = 0; 
    if (y < 0) y = height - 1;

    // Check for collisions with the tail
    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y)
            gameover = 1;
    }

    // Check if the snake has eaten the fruit
    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;
    }
}

// Main function
int main() {
    srand((unsigned int)time(NULL));
    Setup();
    nodelay(stdscr, TRUE); // Enable non-blocking input

    while (!gameover) {
        Draw();
        usleep(100000); // Adjust the sleep time to control game speed
        Input();
        Logic();
    }

    endwin(); // End ncurses mode
    return 0;
}
