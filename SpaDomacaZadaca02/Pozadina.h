#pragma once
#include "Tocka.h"
#include "SFML/Graphics.hpp"

#include<vector>

struct BojaKocke {
	int R = 242;
	int G = 242;
	int B = 242;
	int A = 255;

	void SetBoje(int R, int G, int B, int A);
};

struct AlgoritamPretrage {
	Tocka MojaPozicija;
	Tocka ProslaPozicija;

	int UdaljenostOdPocetka = 100000;
	bool Posjecen = false;

	int Postojim = 255;
};

class Pozadina {
	std::vector<BojaKocke> PoljeBoja;
	Tocka VelicinaPolja;

	Tocka Origin;

	sf::RenderWindow* MyWindow;
	sf::RectangleShape Kocka;

	Tocka PocetakPretrage;
	bool PocetakOznacen = false;

	Tocka KrajPretrage;
	bool KrajOznacen = false;

	std::vector<bool> PoljePrepreka;

	std::vector<Tocka> PutDoKraja;

	int indexOf(int x, int y);
	int indexOf(Tocka T);

	int Udaljenost(Tocka T, int x, int y);

public:
	Pozadina(sf::RenderWindow* RenWin, int xPolje, int yPolje, int xKocka, int yKocka);

	void SetOrigin(int x, int y);
	
	sf::Vector2f GetGridSize();
	Tocka GetSize();
	Tocka GetSelectedGrid();

	void PostaviBojuNa(BojaKocke Boja, int x, int y);

	BojaKocke& at(int x, int y);
	BojaKocke& operator() (int x, int y);

	void Provedi_Dijkstra();

	void Append();
};

