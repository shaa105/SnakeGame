//Snake Game using BFS
#include<iostream>
#include<queue>
#include<vector>

using namespace std;

class SnakeGame {
public:
    int rows, cols;
    vector<vector<int>> board;
    pair<int, int> snakeHead;
    queue<pair<int, int>> snakeBody;
    pair<int, int> food;

    SnakeGame(int r, int c) {
        rows = r;
        cols = c;
        board.resize(rows, vector<int>(cols, 0)); // 0 means empty
        // Initialize snake head at the top-left corner
        snakeHead = {0, 0};
        snakeBody.push(snakeHead);
        // Place food at a random position
        food = {rand() % rows, rand() % cols};
    }

    void display() {
        // Display the game board
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < cols; j++) {
                if (make_pair(i, j) == snakeHead) {
                    cout << "H ";  // Snake head
                } else if (board[i][j] == 1) {
                    cout << "S ";  // Snake body
                } else if (make_pair(i, j) == food) {
                    cout << "F ";  // Food
                } else {
                    cout << ". ";  // Empty space
                }
            }
            cout << endl;
        }
    }

    bool moveSnake(char direction) {
        pair<int, int> newHead = snakeHead;
        switch (direction) {
            case 'w': newHead.first--; break; // Move up
            case 's': newHead.first++; break; // Move down
            case 'a': newHead.second--; break; // Move left
            case 'd': newHead.second++; break; // Move right
        }

        // Check for collisions
        if (newHead.first < 0 || newHead.first >= rows || newHead.second < 0 || newHead.second >= cols) {
            return false; // Out of bounds
        }

        if (newHead == food) {
            // If the snake eats food, grow the snake
            snakeBody.push(newHead);
            food = {rand() % rows, rand() % cols}; // Place new food
        } else {
            // Move the snake
            snakeBody.push(newHead);
            snakeBody.pop(); // Remove the last segment
        }

        snakeHead = newHead;
        return true;
    }

    bool isGameOver() {
        // Game over if the snake runs into itself
        queue<pair<int, int>> tempQueue = snakeBody;
        tempQueue.pop(); // Remove the head from the body queue to check the rest of the body
        while (!tempQueue.empty()) {
            if (tempQueue.front() == snakeHead) {
                return true; // Snake collided with itself
            }
            tempQueue.pop();
        }
        return false;
    }
};

int main() {
    SnakeGame game(10, 10);  // Game board 10x10
    char direction;

    while (true) {
        game.display();
        cout << "Enter move (w/a/s/d): ";
        cin >> direction;

        if (!game.moveSnake(direction)) {
            cout << "Game Over!" << endl;
            break;
        }

        if (game.isGameOver()) {
            cout << "Game Over! Snake collided with itself!" << endl;
            break;
        }
    }

    return 0;
}
