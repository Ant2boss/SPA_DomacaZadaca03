#pragma once
class Tocka {
	int x;
	int y;

public:
	Tocka();
	Tocka(int x, int y);

	Tocka& SetX(int x);
	Tocka& SetY(int y);
	Tocka& SetXY(int x, int y);

	int GetX();
	int GetY();

	Tocka operator+ (Tocka T);
	Tocka operator- (Tocka T);
	
	void operator+= (Tocka T);
	void operator-= (Tocka T);

	bool operator== (Tocka T);
	bool operator!= (Tocka T);
};

