#include <iostream>
#include <queue>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <windows.h> // For Sleep()
#include <conio.h>   // For _kbhit() and _getch()

using namespace std;

const int width = 20;
const int height = 17;
int grid[height][width]; // 0 = empty, 1 = snake, 2 = fruit
int snakeX, snakeY, fruitX, fruitY, score;
vector<pair<int, int>> snake; // Snake's body (head is at the front)
bool gameOver;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

// Directions for BFS: Left, Right, Up, Down
int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};

void Setup() {
    gameOver = false;
    dir = STOP;
    snakeX = width / 2;
    snakeY = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;

    snake.clear();
    snake.push_back({snakeX, snakeY});
    grid[snakeY][snakeX] = 1;
    grid[fruitY][fruitX] = 2;
}

void Draw() {
    system("cls"); // Clear screen
    for (int i = 0; i < width + 2; i++) cout << "#";
    cout << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0) cout << "#";
            if (grid[i][j] == 1)
                cout << "O"; // Snake
            else if (grid[i][j] == 2)
                cout << "F"; // Fruit
            else
                cout << " ";
            if (j == width - 1) cout << "#";
        }
        cout << endl;
    }

    for (int i = 0; i < width + 2; i++) cout << "#";
    cout << endl;

    cout << "Score: " << score << endl;
}

bool isValid(int x, int y) {
    return x >= 0 && x < width && y >= 0 && y < height && grid[y][x] != 1;
}

bool isFruitReachable() {
    queue<pair<int, int>> q;
    vector<vector<bool>> visited(height, vector<bool>(width, false));
    q.push({snakeX, snakeY});
    visited[snakeY][snakeX] = true;

    while (!q.empty()) {
        auto [currX, currY] = q.front();
        q.pop();

        if (currX == fruitX && currY == fruitY) return true;

        for (int i = 0; i < 4; i++) {
            int newX = currX + dx[i];
            int newY = currY + dy[i];

            if (isValid(newX, newY) && !visited[newY][newX]) {
                visited[newY][newX] = true;
                q.push({newX, newY});
            }
        }
    }
    return false;
}

void Input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'a':
            dir = LEFT;
            break;
        case 'd':
            dir = RIGHT;
            break;
        case 'w':
            dir = UP;
            break;
        case 's':
            dir = DOWN;
            break;
        case 'x':
            gameOver = true;
            break;
        }
    }
}

void MoveSnake() {
    int newX = snakeX, newY = snakeY;

    switch (dir) {
    case LEFT:
        newX--;
        break;
    case RIGHT:
        newX++;
        break;
    case UP:
        newY--;
        break;
    case DOWN:
        newY++;
        break;
    default:
        return; // No movement if dir == STOP
    }

    // Wrap-around logic
    if (newX < 0) newX = width - 1;
    else if (newX >= width) newX = 0;
    if (newY < 0) newY = height - 1;
    else if (newY >= height) newY = 0;

    // Check collision with itself
    for (auto &[sx, sy] : snake) {
        if (newX == sx && newY == sy) {
            gameOver = true;
            return;
        }
    }

    // Update snake body
    snake.insert(snake.begin(), {newX, newY});
    grid[newY][newX] = 1;

    // Check if fruit is eaten
    if (newX == fruitX && newY == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        grid[fruitY][fruitX] = 2;
    } else {
        // Remove tail
        auto [tailX, tailY] = snake.back();
        grid[tailY][tailX] = 0;
        snake.pop_back();
    }

    snakeX = newX;
    snakeY = newY;

    // Check fruit reachability
    if (!isFruitReachable()) {
        gameOver = true;
    }
}

int main() {
    srand(static_cast<unsigned>(time(0)));
    Setup();
    while (!gameOver) {
        Draw();
        Input();
        MoveSnake();
        Sleep(100); // Control game speed
    }
    cout << "Game Over! Final Score: " << score << endl;
    return 0;
}
