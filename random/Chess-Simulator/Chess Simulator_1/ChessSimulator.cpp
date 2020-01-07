#include <cstdlib>
#include <ctime>
#include "GameManager.h"
#include "ChessBoard.h"
#include "ChessMove.h"
#include "ChessPiece.h"
#include "ChessPosition.h"

using namespace std;

GameManager *gameManager = new GameManager();

void seed() {srand((unsigned)time(NULL));}
int rnd(int range) {return rand()%range;}

class ChessSimulator {
public:
	ChessSimulator() {
		board = new ChessBoard();
		gameActive = true;
	}
	void processMove(ChessMove *move) {
		board->movePiece(move);

		bool whiteAlive = false, blackAlive = false;
		for(int d=0;d<board->pieces.size();d++) {
			if(board->pieces[d]->type == K && board->pieces[d]->color == WHITE)whiteAlive = true;
			if(board->pieces[d]->type == K && board->pieces[d]->color == BLACK)blackAlive = true;
		}
		if(!whiteAlive || !blackAlive) {
			gameActive = false;
			string winner = (whiteAlive)?"White":"Black";
			cout << "Winner: " << winner << endl;
		}
		board->render();
	}
	ChessMove *findBestMove(int color) {
		ChessPiece *chosen;
		ChessPosition *destination;
		bool found = false;
		while(!found) {
			int index = rnd(board->pieces.size());
			vector<ChessPosition *> possibles = board->pieces[index]->possibleMovementPositionsOnBoard(board);
			if(possibles.size() > 0 && board->pieces[index]->color == board->activeColor) {
				chosen = board->pieces[index];
				int possibleIndex = rnd(possibles.size());
				destination = possibles[possibleIndex];
				found = true;
			}
		}
		ChessMove *retVal = new ChessMove(chosen->color, chosen->type, chosen->pos, destination);

		return retVal;
	}
	void playMove() {
		ChessMove *bestMove = findBestMove(board->activeColor);
		gameManager->annotateMove(bestMove);
	}

	ChessBoard *board;
	bool gameActive;
}*simulator;

void processCommand(string command) {
	cout << "Received command: " << command << endl;
	// Possible input commands are:
	//	Simulator's name to go first, this will only occur on turn 0
	//	Move of a piece on the board
	if(command == gameManager->simulatorName) {
		gameManager->playerColor = WHITE;
		simulator->playMove();
	}
	else {
		ChessMove *move = new ChessMove(command);
		simulator->processMove(move);

		//if(move->color != gameManager->playerColor)simulator->playMove();
		simulator->playMove();
	}
}

void gameSetup() {
	simulator = new ChessSimulator();
	gameManager->processCommand_callback = processCommand;
	gameManager->writeCommand(gameManager->simulatorName);

	//simulator->board->pieces.clear();

	//ChessPiece *testA = new ChessPiece(new ChessPosition("d4"), R, WHITE);
	//ChessPiece *testB = new ChessPiece(new ChessPosition("g4"), P, BLACK);
	//ChessPiece *testC = new ChessPiece(new ChessPosition("e3"), P, BLACK);
	//simulator->board->pieces.push_back(testA);
	//simulator->board->pieces.push_back(testB);
	//simulator->board->pieces.push_back(testC);
	//simulator->board->setOverlayTo(testA);

	simulator->board->render();
}

int main() {
	seed();
	gameSetup();

	while(simulator->gameActive) {
		gameManager->tick();
		cout << simulator->board->FENNotation() << endl;
		sleep(100);
	}
	cout << "End" << endl;
}
