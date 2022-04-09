#pragma once

class Tetromino {
private:
	int _block[4][4];
	int _type;
public:
	static Tetromino rotate(Tetromino& t);
	explicit Tetromino(int type);
	const int getType() const;
	const int getBlock(int x, int y) const;
	void rotate();
};