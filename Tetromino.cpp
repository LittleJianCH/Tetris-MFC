#include "pch.h"
#include "Tetromino.h"

Tetromino::Tetromino(int type) : _type(type) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			_block[i][j] = 0;
		}
	}

	switch (type) {
	case 1:
		_block[1][1] = 1;    // OOOO
		_block[1][2] = 1;    // OXXO
		_block[2][1] = 1;    // OXXO
		_block[2][2] = 1;    // OOOO
		break;
	case 2:
		_block[1][1] = 1;    // OOOO
		_block[1][2] = 1;    // OXXO
		_block[2][1] = 1;    // OXOO
		_block[3][1] = 1;    // OXOO
		break;
	case 3:
		_block[1][1] = 1;    // OOOO
		_block[1][2] = 1;    // OXXO
		_block[2][0] = 1;    // XXOO
		_block[2][1] = 1;    // OOOO
		break;
	case 4:
		_block[1][1] = 1;    // OOOO
		_block[2][0] = 1;    // OXOO
		_block[2][1] = 1;    // XXXO
		_block[2][2] = 1;    // OOOO
		break;
	case 5:
		_block[1][0] = 1;    // OOOO
		_block[1][1] = 1;    // XXOO
		_block[2][1] = 1;    // OXOO
		_block[3][1] = 1;    // OXOO
		break;
	case 6:
		_block[1][0] = 1;    // OOOO
		_block[1][1] = 1;    // XXOO
		_block[2][1] = 1;    // OXXO
		_block[2][2] = 1;    // OOOO
		break;
	case 7:
		_block[1][0] = 1;    // OOOO
		_block[1][1] = 1;    // XXXX
		_block[1][2] = 1;    // OOOO
		_block[1][3] = 1;    // OOOO
		break;
	}
}

const int Tetromino::getType() const {
	return _type;
}

const int Tetromino::getBlock(int x, int y) const {
	return _block[x][y];
}

void Tetromino::rotate() {
	Tetromino temp = rotate(*this);

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			_block[i][j] = temp.getBlock(i, j);
		}
	}
}

Tetromino Tetromino::rotate(Tetromino& t) {
	Tetromino temp(0);

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			temp._block[i][j] = t.getBlock(3 - j, i);
		}
	}
	temp._type = t._type;
	return temp;
}