#include <bits/stdc++.h>
using namespace std;
#define N 3

struct PuzzleNode {
    PuzzleNode* parentNode;
    vector<vector<int>> puzzleMatrix;
    int xBlank, yBlank;
    int costEstimate;
    int moveCount;
};


void printMatrix(const vector<vector<int>>& mat) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            printf("%d ", mat[i][j]);
        printf("\n");
    }
}


PuzzleNode* createNewPuzzleNode(const vector<vector<int>>& puzzleMat, int x, int y, int newX, int newY, int level, PuzzleNode* parent) {
    PuzzleNode* pNode = new PuzzleNode;

    pNode->parentNode = parent;
    pNode->puzzleMatrix = puzzleMat;

    swap(pNode->puzzleMatrix[x][y], pNode->puzzleMatrix[newX][newY]);

    pNode->costEstimate = INT_MAX;
    pNode->moveCount = level;

    pNode->xBlank = newX;
    pNode->yBlank = newY;

    return pNode;
}

int rowMoves[] = { 1, 0, -1, 0 };
int colMoves[] = { 0, -1, 0, 1 };


int calculateMisplacedTiles(const vector<vector<int>>& initialState, const vector<vector<int>>& goalState) {
    int count = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (initialState[i][j] && initialState[i][j] != goalState[i][j])
                count++;
    return count;
}

// Function to check if (x, y) is a valid matrix coordinate
int isValidCoordinate(int x, int y) {
    return (x >= 0 && x < N && y >= 0 && y < N);
}

// Function to print the path from the initial state to the goal state
void printPuzzlePath(PuzzleNode* root) {
    if (root == NULL)
        return;
    printPuzzlePath(root->parentNode);
    printMatrix(root->puzzleMatrix);
    printf("\n");
}

// Comparison object for ordering the priority queue
struct PuzzleNodeComparator {
    bool operator()(const PuzzleNode* lhs, const PuzzleNode* rhs) const {
        return (lhs->costEstimate + lhs->moveCount) > (rhs->costEstimate + rhs->moveCount);
    }
};

// Function to solve N*N - 1 puzzle algorithm using Branch and Bound
void solvePuzzle(const vector<vector<int>>& initialPuzzle, int xBlank, int yBlank, const vector<vector<int>>& finalPuzzle) {
    priority_queue<PuzzleNode*, vector<PuzzleNode*>, PuzzleNodeComparator> pq;

    PuzzleNode* rootPuzzleNode = createNewPuzzleNode(initialPuzzle, xBlank, yBlank, xBlank, yBlank, 0, NULL);
    rootPuzzleNode->costEstimate = calculateMisplacedTiles(initialPuzzle, finalPuzzle);

    pq.push(rootPuzzleNode);

    while (!pq.empty()) {
        PuzzleNode* minNode = pq.top();
        pq.pop();

        if (minNode->costEstimate == 0) {
            printPuzzlePath(minNode);
            return;
        }

        for (int i = 0; i < 4; i++) {
            int newX = minNode->xBlank + rowMoves[i];
            int newY = minNode->yBlank + colMoves[i];

            if (isValidCoordinate(newX, newY)) {
                PuzzleNode* childNode = createNewPuzzleNode(minNode->puzzleMatrix, minNode->xBlank, minNode->yBlank, newX, newY, minNode->moveCount + 1, minNode);
                childNode->costEstimate = calculateMisplacedTiles(childNode->puzzleMatrix, finalPuzzle);

                pq.push(childNode);
            }
        }
    }
}

// Driver code
int main() {
    // Initial puzzle configuration (0 represents the empty space)
    vector<vector<int>> initialPuzzle(N, vector<int>(N));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> initialPuzzle[i][j];
        }
    }

    // Solvable final puzzle configuration (0 represents the empty space)
    vector<vector<int>> finalPuzzle(N, vector<int>(N));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            finalPuzzle[i][j] = i * N + j + 1;
        }
    }
    finalPuzzle[N - 1][N - 1] = 0;

    int xBlank, yBlank;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (initialPuzzle[i][j] == 0) {
                xBlank = i;
                yBlank = j;
            }
        }
    }

    solvePuzzle(initialPuzzle, xBlank, yBlank, finalPuzzle);

    return 0;
}

