//============================================================================
// Name        : minimax.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

//0 is human, 1 is comp, 2 is free space
//human: best score = -10, comp: best score = 10;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char board[9];
long int recursions = 0;

int score();

int gameOver() {
	int iscore = score();
	if (iscore != 0) //win - game over
		return iscore;
	for (int i = 0; i < 9; i++) {
		if (board[i] == 2)
			return 0; //game is not over
	}
	return 1; //board is full - game over
}

int minimax(bool player, int alpha, int beta) {
	recursions++;
	int iscore, bestScore;
	if (player == 0) //human
		bestScore = 10;
	else
		//comp
		bestScore = -10;
	for (int move = 0; move < 9; move++) { //try possible moves
		if (board[move] != 2)
			continue;
		board[move] = player;
		if (gameOver() == 0) {
			iscore = minimax(!player, alpha, beta);
		} else {
			iscore = score();
		}
		board[move] = 2;
		if (player == 0) { //human
			if (iscore < bestScore) bestScore = iscore;
			if (bestScore < beta) beta = bestScore;
			if (beta <= alpha) break;
		} else { //comp
			if (iscore > bestScore) bestScore = iscore;
			if (bestScore > alpha) alpha = bestScore;
			if (beta <= alpha) break;
		}
	}
	return bestScore;
}

int bestMove() {
	//returns bestMove for comp
	int iscore, bestScore = -10;
	int move;
	for (int i = 0; i < 9; i++) {
		if (board[i] == 2) {
			move = i; //initialize move
			break;
		}
	}
	int bestMove = move;
	for (; move < 9; move++) { //try possible moves
		if (board[move] != 2)
			continue;
		board[move] = 1;
		if (gameOver() == 0) {
			iscore = minimax(0, -10, 10);
		} else {
			iscore = score();
		}
		board[move] = 2;
		if (iscore > bestScore) {
			bestScore = iscore;
			bestMove = move;
			if (bestScore == 10)
				return bestMove;
		}
	}
	return bestMove;
}

void printBoard(char board[9], char first);

int main() {
	memset(board, 2, 9);
	int move;
	int igameOver = gameOver();

	char example[9];
	memset(example, 3, 9);
	printBoard(example, 0);

	printf("do you want to go first? (y/n): ");
	char first = getchar();
	getchar();
	int i;
	if (first == 'y')
		i = 0;
	else
		i = 1;

	srand((unsigned) time(NULL));

	for (; igameOver == 0; i++, igameOver = gameOver()) {
		if (i % 2 == 0) {
			printf("enter move (1-9): ");
			move = getchar() - '0' - 1;
			getchar();
			board[move] = 0;
		} else if ((i == 1) && (first != 'y'))
			board[rand() % 9] = 1;
		else
			board[bestMove()] = 1;
		printBoard(board, first);
	}

	if (igameOver == 10)
		printf("computer wins\n");
	else if (igameOver == -10)
		printf("human wins\n");
	else
		printf("tie\n");

	printf("%ld\n", recursions);

	return 0;
}

void printBoard(char board[9], char first) {
	for (int i = 0; i < 9; i++) {
		if (i % 3 == 0)
			printf("\n");
		if (i % 3 == 1 || i % 3 == 2)
			printf("|");
		switch (board[i]) {
		case 0:
			if (first == 'y')
				printf(" X ");
			else
				printf(" O ");
			break;
		case 1:
			if (first == 'y')
				printf(" O ");
			else
				printf(" X ");
			break;
		case 2:
			printf("   ");
			break;
		default:
			printf(" %d ", i + 1);
		}
	}
	printf("\n\n");
}

int score() {
//human: best score = -10, computer: best score = 10;
//check for horizontal
	for (int i = 0; i <= 6; i += 3) {
		if ((board[i] == board[i + 1]) && (board[i + 1] == board[i + 2])
				&& (board[i + 2] != 2)) {
			return ((board[i]) * 20) - 10;
		}
	}
//check for vertical
	for (int i = 0; i <= 2; i++) {
		if ((board[i] == board[i + 3]) && (board[i + 3] == board[i + 6])
				&& (board[i + 6] != 2)) {
			return ((board[i]) * 20) - 10;
		}
	}
//check for diagonal
	if ((board[0] == board[4]) && (board[4] == board[8]) && (board[8] != 2)) {
		return ((board[0]) * 20) - 10;
	}
	if ((board[2] == board[4]) && (board[4] == board[6]) && (board[6] != 2)) {
		return ((board[2]) * 20) - 10;
	}
	return 0;
}
