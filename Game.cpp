#include "pch.h"
#include "Game.h"

Game::Game(int width, int height) :
	_width(width), _height(height),
	_tLocX(0), _tLocY(0),
	_tetromino(0), _nextTetromino(0),
	_state(GAME_STATE::STOP),
	_field(height, std::vector<int>(width, 0)) {}

void Game::newTetromino() {
	std::swap(_tetromino, _nextTetromino);
	_nextTetromino = Tetromino{ rand() % 7 + 1 };

	_tLocX = _width / 2 - 2; // the size of the tetromino is 4
	_tLocY = 0;

}

const Tetromino& Game::getTetromino() const {
	return _tetromino;
}

const Tetromino& Game::getNextTetromino() const {
	return _nextTetromino;
}

std::pair<int, int> Game::getFallingDownPostion() const {
	int x = _tLocX;
	int y = _tLocY;

	while (!checkCollision(x, y + 1, _tetromino)) {
		y++;
	}

	return std::make_pair(x, y);
}

void Game::start() {
	srand(static_cast<int>(time(0)));

	_state = GAME_STATE::GO;

	newTetromino();
	newTetromino();
}

bool Game::checkCollision(int x, int y, const Tetromino& tetromino) const {
	// check if the tetromino is out of the field 
	// or collide with other tetromino
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (tetromino.getBlock(i, j) == 1) {
				if (x + i < 0 || x + i >= _width || y + j >= _height) {
					return true;
				}
				if (_field[y + j][x + i] != 0) {
					return true;
				}
			}
		}
	}
	return false;
}

bool Game::moveWithDirection(int dx, int dy) {
	if (checkCollision(_tLocX + dx, _tLocY + dy, _tetromino)) {
		return false;
	}
	else {
		_tLocX += dx;
		_tLocY += dy;
		return true;
	}
}

bool Game::moveDown() {
	return moveWithDirection(0, 1);
}

bool Game::moveLeft() {
	return moveWithDirection(-1, 0);
}

bool Game::moveRight() {
	return moveWithDirection(1, 0);
}

bool Game::rotate() {
	Tetromino tmp = Tetromino::rotate(_tetromino);

	if (checkCollision(_tLocX, _tLocY, tmp)) {
		return false;
	}
	else {
		_tetromino = tmp;
		return true;
	}
}

void Game::fixTetromino() {
	// This function should be called 
	// when check collision returns false
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (_tetromino.getBlock(i, j) == 1) {
				_field[_tLocY + j][_tLocX + i] = _tetromino.getType();
			}
		}
	}

	newTetromino();
}

std::vector<std::vector<int>> Game::getCurrentScreen() const {
	std::vector<std::vector<int>> screen{ _field };

	if (_tetromino.getType() != 0) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (_tetromino.getBlock(i, j) == 1) {
					screen[_tLocY + j][_tLocX + i] = _tetromino.getType();
				}
			}
		}

		auto pair = getFallingDownPostion();

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				int x = i + pair.first;
				int y = j + pair.second;
				if (_tetromino.getBlock(i, j) == 1 && screen[y][x] == 0) {
					screen[y][x] = 8;
				}
			}
		}
	}

	return screen;
}

void Game::removeLines() {
	std::vector<bool> mark;

	for (auto& line : _field) {
		if (std::all_of(line.begin(), line.end(), [](int i) {return i != 0; })) {
			line = std::vector<int>(_width, 0);
		}
	}

	for (const auto& line : _field) {
		mark.push_back(std::any_of(line.begin(), line.end(),
			[](int i) { return i != 0; }));
	}

	int it1 = _height - 1;
	int it2 = _height - 1;

	while (it1 >= 0) {
		while (it1 >= 0 && !mark[it1]) {
			// find the lowest full line
			it1--;
		}

		if (it1 < 0) break;

		if (it1 != it2) {
			std::swap(_field[it1], _field[it2]);
			std::swap(mark[it1], mark[it2]);
		}

		it2--; it1--;
	}

	for (; it2 >= 0; it2--) {
		_field[it2] = std::vector<int>(_width, 0);
	}
}

void Game::input(UINT key) {
	switch (key) {
	case 'A': case VK_LEFT: moveLeft(); break;
	case 'D': case VK_RIGHT: moveRight(); break;
	case 'W': case VK_UP: rotate(); break;
	case 'S': case VK_DOWN: moveDown(); break;
	case VK_RETURN:
		while (moveDown());
		fixTetromino();
		removeLines();
		break;
	default:
		break;
	}
}

bool Game::go() {
	if (!moveDown()) {
		fixTetromino(); // this function will generate a new tetromino
		removeLines();

		if (std::any_of(_field[0].begin(), _field[0].end(),
			[](int i) { return i != 0; })) {
			_state = GAME_STATE::STOP;
			return false;
		}
	}

	return true;
}

void Game::switchPauseAndContinue() {
	switch (_state) {
	case GAME_STATE::GO:    _state = GAME_STATE::PAUSE; break;
	case GAME_STATE::PAUSE: _state = GAME_STATE::GO;    break;
	}
}

GAME_STATE Game::getState() const {
	return _state;
}

int Game::getHeight() const {
	return _height;
}

int Game::getWidth() const {
	return _width;
}

int Game::getLocX() const {
	return _tLocX;
}

int Game::getLocY() const {
	return _tLocY;
}

int Game::getBlock(int x, int y) const {
	return _field[x][y];
}
