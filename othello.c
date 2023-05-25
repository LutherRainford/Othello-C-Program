#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define BOARD_SIZE 8

// Function Prototypes
int menu();
char prompt_color();
void game_pvp();
void game_pvc(char);
void displayScore(const char board[BOARD_SIZE][BOARD_SIZE]);
void initializeBoard(char board[BOARD_SIZE][BOARD_SIZE]);
void displayBoard(const char board[BOARD_SIZE][BOARD_SIZE]);
bool isValidMove(const char board[BOARD_SIZE][BOARD_SIZE], int row, int col, char currentPlayer);
void makeMove(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, char currentPlayer);
bool isGameOver(const char board[BOARD_SIZE][BOARD_SIZE]);
char getWinner(const char board[BOARD_SIZE][BOARD_SIZE]);
void listPossibleMoves(const char board[BOARD_SIZE][BOARD_SIZE], char currentPlayer, int moves[], int* numMoves);
void clearConsole();

int main(){
  printf("\n##############################\n# Othello by Luther Rainford #\n##############################\n");

  int o;
  o = menu();

  if(o == 3){
    return 0;
  }
  else if(o == 2){
    game_pvp();
    return 0;
  }
  else if(o == 1){
    char color;
    color = prompt_color();
    game_pvc(color);
    printf("%c", color);
    return 0;
  }
  return 0;
}

int menu(){
  int option;

  printf("\n (1) - Play computer\n (2) - Play another person\n (3) - Quit\n\nEnter Number: ");
  scanf("%d", &option);

  if(option < 1 || option > 3){
    printf("\n%d is not a valid option, please try again.\n\n", option);
    return menu();
  }

  return option;
}

char prompt_color(){
  char c;
  printf("Would you like to play white (w) or black (b)? ");
  scanf(" %c", &c);

  if(c == 'w' || c == 'W'){
    return 'W';
  }
  else if(c == 'b' || c == 'B'){
    return 'B';
  }
  printf("\nPlease choose either b or w.\n\n");
  return prompt_color();
}

void initializeBoard(char board[BOARD_SIZE][BOARD_SIZE]) {
    // Fill the board with empty spaces
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = ' ';
        }
    }

    // Set initial pieces
    board[3][3] = 'W';
    board[3][4] = 'B';
    board[4][3] = 'B';
    board[4][4] = 'W';
}

void displayBoard(const char board[BOARD_SIZE][BOARD_SIZE]) {
    printf("  1 2 3 4 5 6 7 8\n");
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%d ", i + 1);
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

bool isValidMove(const char board[BOARD_SIZE][BOARD_SIZE], int row, int col, char currentPlayer) {
    // Check if the specified position is empty
    if (board[row][col] != ' ') {
        return false;
    }

    // Check if the move is within the board boundaries
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
        return false;
    }

    // Check if the move is adjacent to an opponent's piece
    int deltaRow, deltaCol;
    for (deltaRow = -1; deltaRow <= 1; deltaRow++) {
        for (deltaCol = -1; deltaCol <= 1; deltaCol++) {
            if (deltaRow == 0 && deltaCol == 0) {
                continue;  // Skip the current position
            }

            int r = row + deltaRow;
            int c = col + deltaCol;

            // Check if the adjacent position is within the board boundaries
            if (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE && board[r][c] != ' ' && board[r][c] != currentPlayer) {
                // Check if there is a line of opponent's pieces that can be flipped
                while (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE && board[r][c] != ' ' && board[r][c] != currentPlayer) {
                    r += deltaRow;
                    c += deltaCol;

                    if (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE && board[r][c] == currentPlayer) {
                        return true;  // Valid move
                    }
                }
            }
        }
    }

    return false;  // Invalid move
}

void makeMove(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, char currentPlayer) {
    // Place the current player's piece on the specified position
    board[row][col] = currentPlayer;

    // Flip opponent's pieces
    int deltaRow, deltaCol;
    for (deltaRow = -1; deltaRow <= 1; deltaRow++) {
        for (deltaCol = -1; deltaCol <= 1; deltaCol++) {
            if (deltaRow == 0 && deltaCol == 0) {
                continue;  // Skip the current position
            }

            int r = row + deltaRow;
            int c = col + deltaCol;

            // Check if the adjacent position is within the board boundaries
            if (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE && board[r][c] != ' ' && board[r][c] != currentPlayer) {
                // Check if there is a line of opponent's pieces that can be flipped
                while (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE && board[r][c] != ' ' && board[r][c] != currentPlayer) {
                    r += deltaRow;
                    c += deltaCol;

                    if (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE && board[r][c] == currentPlayer) {
                        // Flip the pieces
                        int flipRow = row + deltaRow;
                        int flipCol = col + deltaCol;
                        while (flipRow != r || flipCol != c) {
                            board[flipRow][flipCol] = currentPlayer;
                            flipRow += deltaRow;
                            flipCol += deltaCol;
                        }

                        break;
                    }
                }
            }
        }
    }
}

bool isGameOver(const char board[BOARD_SIZE][BOARD_SIZE]) {
    // Check if the board is full
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == ' ') {
                return false;  // Game is not over
            }
        }
    }

    return true;  // Game is over
}

char getWinner(const char board[BOARD_SIZE][BOARD_SIZE]) {
    int countW = 0;
    int countB = 0;

    // Count the number of W and B pieces
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == 'W') {
                countW++;
            } else if (board[i][j] == 'B') {
                countB++;
            }
        }
    }

    // Determine the winner based on the piece count
    if (countW > countB) {
        return 'W';
    } else if (countB > countW) {
        return 'B';
    } else {
        return ' ';  // Draw
    }
}

void displayScore(const char board[BOARD_SIZE][BOARD_SIZE]){
    int countW = 0;
    int countB = 0;
    // Count the number of W and B pieces
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == 'W') {
                countW++;
            } else if (board[i][j] == 'B') {
                countB++;
            }
        }
    }
    printf("\nW: %d  B: %d\n\n", countW, countB);
}

void game_pvp() {
    char board[BOARD_SIZE][BOARD_SIZE];
    char currentPlayer = 'B';
    int row, col;

    initializeBoard(board);

    // Game loop
    while (true) {
        clearConsole();
        displayScore(board);
        displayBoard(board);

        // Get the current player's move
        printf("Player %c's turn.\n", currentPlayer);
        printf("Enter the row (1-8): ");
        scanf("%d", &row);
        printf("Enter the column (1-8): ");
        scanf("%d", &col);

        row--;
        col--;

        // Check if the move is valid
        if (isValidMove(board, row, col, currentPlayer)) {
            // Make the move
            makeMove(board, row, col, currentPlayer);

            // Switch to the next player
            currentPlayer = (currentPlayer == 'W') ? 'B' : 'W';

            // Check if the game is over
            if (isGameOver(board)) {
                // Display the final board
                displayBoard(board);

                // Determine the winner
                char winner = getWinner(board);

                // Declare the winner or a draw
                if (winner != ' ') {
                    printf("Player %c wins!\n", winner);
                } else {
                    printf("It's a draw!\n");
                }

                break;
            }
        } else {
            printf("Invalid move. Try again.\n");
        }
    }
}

void listPossibleMoves(const char board[BOARD_SIZE][BOARD_SIZE], char currentPlayer, int moves[], int* numMoves) {
    *numMoves = 0;

    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (isValidMove(board, row, col, currentPlayer)) {
                moves[*numMoves] = row * BOARD_SIZE + col;
                (*numMoves)++;
            }
        }
    }
}

int* getBestMove(const char board[BOARD_SIZE][BOARD_SIZE], char currentPlayer, int moves[], int numMoves){
    // Score array
    int score[8][8] = {
        {3, -2, 2, 1, 1, 2, -2, 3},
        {-2, -3, -1, -1, -1, -1, -3, -2},
        {2, -1, 0, 0, 0, 0, -1, 2},
        {1, -1, 0, 0, 0, 0, -1, 1},
        {1, -1, 0, 0, 0, 0, -1, 1},
        {2, -1, 0, 0, 0, 0, -1, 2},
        {-2, -3, -1, -1, -1, -1, -3, -2},
        {3, -2, 2, 1, 1, 2, -2, 3}
    };

    int* bestMove = (int*)malloc(2 * sizeof(int)); // Allocate memory for the best move array

    int maxScore = -4;
    int bestRow = -1;
    int bestCol = -1;

    for (int i = 0; i < numMoves; i++) {
        int row = moves[i] / BOARD_SIZE; // Extract row from moves[i]
        int col = moves[i] % BOARD_SIZE; // Extract column from moves[i]

        if (!isValidMove(board, row, col, currentPlayer)) {
            continue; // Skip this move if it's not valid
        }

        int moveScore = score[row][col];

        // Update the best move if the current move has a higher score
        if (moveScore > maxScore) {
            maxScore = moveScore;
            bestRow = row;
            bestCol = col;
        }
    }

    bestMove[0] = bestRow;
    bestMove[1] = bestCol;

    return bestMove;
}

void game_pvc(char color) {
  char board[BOARD_SIZE][BOARD_SIZE];
    char currentPlayer = 'B';
    int row, col;

    initializeBoard(board);

    // Game loop
    while (true) {
        clearConsole();
        displayScore(board);
        displayBoard(board);

        // Human player's turn
        if (currentPlayer == color) {
            // Get the human player's move
            printf("Your turn (Player %c).\n", currentPlayer);
            printf("Enter the row (1-8): ");
            scanf("%d", &row);
            printf("Enter the column (1-8): ");
            scanf("%d", &col);

            row--;
            col--;
        }
        // Computer player's turn
        else {
            printf("Computer's turn (Player %c).\n", currentPlayer);

            // Generate possible moves for the computer player
            int moves[BOARD_SIZE * BOARD_SIZE];
            int numMoves;
            listPossibleMoves(board, currentPlayer, moves, &numMoves);

            // Get the best move for the computer player
            int* bestMove = getBestMove(board, currentPlayer, moves, numMoves);

            // Assign the row and column of the best move
            row = bestMove[0];
            col = bestMove[1];

            // Free the memory allocated for the best move array
            free(bestMove);

            // Convert the row and column to one-based index
            printf("Computer chooses row %d, column %d.\n", row + 1, col + 1);
        }

        // Check if the move is valid
        if (isValidMove(board, row, col, currentPlayer)) {
            // Make the move
            makeMove(board, row, col, currentPlayer);

            // Switch to the next player
            currentPlayer = (currentPlayer == 'W') ? 'B' : 'W';

            // Check if the game is over
            if (isGameOver(board)) {
                // Display the final board
                displayBoard(board);

                // Determine the winner
                char winner = getWinner(board);

                // Declare the winner or a draw
                if (winner != ' ') {
                    printf("Player %c wins!\n", winner);
                } else {
                    printf("It's a draw!\n");
                }

                break;
            }
        } else {
            printf("Invalid move. Try again.\n");
        }
    }
}

void clearConsole() {
#ifdef _WIN32
  system("cls");
#else
  printf("\033[2J\033[1;1H");
#endif
}
