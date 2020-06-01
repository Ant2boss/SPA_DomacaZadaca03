#include "Tocka.h"

Tocka::Tocka() {
	this->SetXY(0, 0);
}

Tocka::Tocka(int x, int y) {
	this->SetXY(x, y);
}

Tocka& Tocka::SetX(int x) {
	if (x < 0) x = 0;
	this->x = x;
	return *this;
}

Tocka& Tocka::SetY(int y) {
	if (y < 0)  y = 0;
	this->y = y;
	return *this;
}

Tocka& Tocka::SetXY(int x, int y) {
	this->SetX(x);
	this->SetY(y);
	return *this;
}

int Tocka::GetX() {
	return this->x;
}

int Tocka::GetY() {
	return this->y;
}

Tocka Tocka::operator+(Tocka T) {
	Tocka Priv;
	Priv.SetX(this->GetX() + T.GetX());
	Priv.SetY(this->GetY() + T.GetY());
	return Priv;
}

Tocka Tocka::operator-(Tocka T) {
	Tocka Priv;
	Priv.SetX(this->GetX() - T.GetX());
	Priv.SetY(this->GetY() - T.GetY());
	return Priv;
}

void Tocka::operator+=(Tocka T) {
	this->SetX(this->GetX() + T.GetX());
	this->SetY(this->GetY() + T.GetY());
}

void Tocka::operator-=(Tocka T) {
	this->SetX(this->GetX() - T.GetX());
	this->SetY(this->GetY() - T.GetY());
}

bool Tocka::operator==(Tocka T) {
	if (this->GetX() != T.GetX()) return false;
	if (this->GetY() != T.GetY()) return false;
	return true;
}

bool Tocka::operator!=(Tocka T) {
	return !this->operator==(T);
}
