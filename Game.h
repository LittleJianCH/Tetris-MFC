#pragma once

#include "Tetromino.h"
#include <vector>
#include <algorithm>

enum class GAME_STATE {
	PAUSE,
	GO,
	STOP
};

class Game {
private:
	static const int cnt2Score[5];
	std::vector<std::vector<int>> _field;

	Tetromino _tetromino;
	Tetromino _nextTetromino;

	int _width;
	int _height;
	int _tLocX;
	int _tLocY;

	int _score;

	GAME_STATE _state;

public:
	Game(int width, int height);
	GAME_STATE getState() const;
	int getHeight() const;
	int getWidth() const;
	int getLocX() const;
	int getLocY() const;
	int getScore() const;
	int getSpeed() const; // return the time of time interval
	int getBlock(int x, int y) const;
	const Tetromino& getTetromino() const;
	const Tetromino& getNextTetromino() const; 
	std::pair<int, int> Game::getFallingDownPostion() const;
	std::vector<std::vector<int>> getCurrentScreen() const;
	void start();
	void fixTetromino();
	void input(UINT key);
	bool go();
	void switchPauseAndContinue();

private:
	void newTetromino();
	bool checkCollision(int x, int y, const Tetromino& tetromino) const;
	bool moveWithDirection(int dx, int dy);
	bool moveDown();
	bool moveLeft();
	bool moveRight();
	bool rotate();
	void removeLines();
};
