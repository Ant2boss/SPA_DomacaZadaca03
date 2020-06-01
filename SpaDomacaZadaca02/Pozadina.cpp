#include "Pozadina.h"
#include <cmath>
#include <queue>

struct UsporedbaPretrage {
	bool operator() (AlgoritamPretrage A1, AlgoritamPretrage A2) {
		return A1.UdaljenostOdPocetka > A2.UdaljenostOdPocetka;
	}
};

int Pozadina::indexOf(int x, int y) {
	return y * this->GetSize().GetX() + x;
}

int Pozadina::indexOf(Tocka T) {
	return this->indexOf(T.GetX(), T.GetY());
}

int Pozadina::Udaljenost(Tocka T, int x, int y) {
	return 10 * std::sqrt(std::pow(T.GetX() - x, 2) + std::pow(T.GetY() - y, 2));
}

Pozadina::Pozadina(sf::RenderWindow* RenWin, int xPolje, int yPolje, int xKocka, int yKocka) {
	this->MyWindow = RenWin;
	this->VelicinaPolja.SetXY(xPolje, yPolje);
	this->Origin.SetXY(0,0);

	this->Kocka.setSize(sf::Vector2f(xKocka, yKocka));
	this->Kocka.setOutlineColor(sf::Color::Black);
	this->Kocka.setOutlineThickness(2);

	this->PoljeBoja.resize(yPolje * xPolje);
	this->PoljePrepreka.resize(xPolje * yPolje);

	//Postavi primjer
	this->PocetakPretrage.SetXY(4, 3);
	this->KrajPretrage.SetXY(10, 3);

	this->PoljePrepreka[this->indexOf(7, 2)] = true;
	this->PoljePrepreka[this->indexOf(7, 3)] = true;
	this->PoljePrepreka[this->indexOf(7, 4)] = true;

	this->Provedi_Dijkstra();
}

void Pozadina::SetOrigin(int x, int y) {
	this->Origin.SetXY(x, y);
}

sf::Vector2f Pozadina::GetGridSize() {
	return this->Kocka.getSize();
}

Tocka Pozadina::GetSize() {
	return this->VelicinaPolja;
}

Tocka Pozadina::GetSelectedGrid() {
	sf::Vector2i MousePos = sf::Mouse::getPosition(*this->MyWindow);

	MousePos.x -= this->Origin.GetX();
	MousePos.y -= this->Origin.GetY();

	Tocka Poz;
	Poz.SetX(MousePos.x / this->Kocka.getSize().x);
	Poz.SetY(MousePos.y / this->Kocka.getSize().y);

	if (Poz.GetX() >= this->VelicinaPolja.GetX()) Poz.SetX(this->VelicinaPolja.GetX() - 1);
	if (Poz.GetY() >= this->VelicinaPolja.GetY()) Poz.SetY(this->VelicinaPolja.GetY() - 1);

	return Poz;
}

void Pozadina::PostaviBojuNa(BojaKocke Boja, int x, int y) {
	this->PoljeBoja[this->indexOf(x, y)] = Boja;
}

BojaKocke& Pozadina::at(int x, int y) {
	return this->PoljeBoja[this->indexOf(x, y)];
}

BojaKocke& Pozadina::operator()(int x, int y) {
	return this->at(x, y);
}

void Pozadina::Provedi_Dijkstra() {
	this->PutDoKraja.clear();
	std::vector<AlgoritamPretrage> PoljePretrage(this->VelicinaPolja.GetX() * this->VelicinaPolja.GetY());

	//Inicijalizacija polja za pretragu
	for (int y = 0; y < this->VelicinaPolja.GetY(); ++y) {
		for (int x = 0; x < this->VelicinaPolja.GetX(); ++x) {
			PoljePretrage[this->indexOf(x, y)].MojaPozicija.SetXY(x, y);
		}
	}

	Tocka TrenutnaPozicija = this->PocetakPretrage;
	PoljePretrage[this->indexOf(TrenutnaPozicija)].ProslaPozicija = this->PocetakPretrage;
	PoljePretrage[this->indexOf(TrenutnaPozicija)].UdaljenostOdPocetka = 0;

	bool Uspjeh = true;

	std::priority_queue<AlgoritamPretrage, std::vector<AlgoritamPretrage>, UsporedbaPretrage> pq;

	while (TrenutnaPozicija != this->KrajPretrage) {
		int xs = TrenutnaPozicija.GetX() - 1;
		int xe = TrenutnaPozicija.GetX() + 1;

		int ys = TrenutnaPozicija.GetY() - 1;
		int ye = TrenutnaPozicija.GetY() + 1;

		AlgoritamPretrage& Trenutno = PoljePretrage[this->indexOf(TrenutnaPozicija)];

		for (int y = ys; y <= ye; ++y) {
			for (int x = xs; x <= xe; ++x) {
				if (x == TrenutnaPozicija.GetX() && y == TrenutnaPozicija.GetY()) continue;
				else if (x < 0 || x >= this->VelicinaPolja.GetX()) continue;
				else if (y < 0 || y >= this->VelicinaPolja.GetY()) continue;
				else if (PoljePretrage[this->indexOf(x, y)].Posjecen) continue;
				else if (this->PoljePrepreka[this->indexOf(x, y)]) continue;
				
				AlgoritamPretrage& Susjed = PoljePretrage[this->indexOf(x, y)];

				//Izracuna se tenative distance, ako je manja ubaci se
				int t = this->Udaljenost(TrenutnaPozicija, x, y) + Trenutno.UdaljenostOdPocetka;
				if (t < Susjed.UdaljenostOdPocetka) {
					Susjed.UdaljenostOdPocetka = t;
					Susjed.ProslaPozicija = TrenutnaPozicija;
					pq.push(Susjed);
				}

			}
		}

		Trenutno.Posjecen = true;
		for (int i = 0; i < PoljePretrage.size(); ++i) {
			if(PoljePretrage[i].Posjecen && PoljePretrage[i].Postojim >= -100) PoljePretrage[i].Postojim--;
		}

		if (pq.empty()) {
			Uspjeh = false;
			break;
		}
		TrenutnaPozicija = pq.top().MojaPozicija;
		pq.pop();
	}
	
	while (TrenutnaPozicija != this->PocetakPretrage && Uspjeh) {
		this->PutDoKraja.push_back(TrenutnaPozicija);
		TrenutnaPozicija = PoljePretrage[this->indexOf(TrenutnaPozicija)].ProslaPozicija;
	}
	if(Uspjeh) this->PutDoKraja.push_back(TrenutnaPozicija);

	//Azuriramo boje
	for (int y = 0; y < this->VelicinaPolja.GetY(); ++y) {
		for (int x = 0; x < this->VelicinaPolja.GetX(); ++x) {
			int index = this->indexOf(x, y);
			if (PoljePretrage[index].Posjecen) {
				if (PoljePretrage[index].Postojim > 0)
					this->PoljeBoja[index].SetBoje(255, PoljePretrage[index].Postojim, PoljePretrage[index].Postojim, 255);
				else {
					this->PoljeBoja[index].SetBoje(221 + PoljePretrage[index].Postojim, 0, 0, 255);
				}
			}
			else this->PoljeBoja[index].SetBoje(242, 242, 242, 255);
		}
	}
}

void Pozadina::Append() {
	
	sf::Vector2f PozicijaKocke(0,0);

	Tocka Mis = this->GetSelectedGrid();

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		if (Mis == this->PocetakPretrage && this->KrajOznacen == false) {
			this->PocetakOznacen = true;
		}
		else if (Mis == this->KrajPretrage && this->PocetakOznacen == false) {
			this->KrajOznacen = true;
		}
		else if(this->KrajOznacen == false && this->PocetakOznacen == false) {
			this->PoljePrepreka[this->indexOf(Mis.GetX(), Mis.GetY())] = true;
			this->Provedi_Dijkstra();
		}
	}
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
		if (this->KrajOznacen == false && this->PocetakOznacen == false) {
			this->PoljePrepreka[this->indexOf(Mis.GetX(), Mis.GetY())] = false;
			this->Provedi_Dijkstra();
		}
	}
	else {
		if (this->PocetakOznacen == true) {
			if (this->PoljePrepreka[this->indexOf(Mis.GetX(), Mis.GetY())]) {
				this->PocetakOznacen = false;
			}
			else {
				this->PocetakPretrage = Mis;
				this->PocetakOznacen = false;

				this->Provedi_Dijkstra();
			}
		}
		if (this->KrajOznacen == true) {
			if (this->PoljePrepreka[this->indexOf(Mis.GetX(), Mis.GetY())]) {
				this->KrajOznacen = false;
			}
			else {
				this->KrajPretrage = Mis;
				this->KrajOznacen = false;

				this->Provedi_Dijkstra();
			}
		}
	}

	bool preklapanje = (this->PocetakPretrage == this->KrajPretrage) && (this->PocetakOznacen == false && this->KrajOznacen == false);

	for (int y = 0; y < this->VelicinaPolja.GetY(); ++y) {
		for (int x = 0; x < this->VelicinaPolja.GetX(); ++x) {
			
			PozicijaKocke.x = (this->Kocka.getSize().x * x) + this->Origin.GetX();
			PozicijaKocke.y = (this->Kocka.getSize().y * y) + this->Origin.GetY();
			this->Kocka.setPosition(PozicijaKocke);
			
			BojaKocke C = this->at(x,y);
			bool ZabranjenaPozicija = this->PoljePrepreka[this->indexOf(Mis.GetX(), Mis.GetY())];

			//Boja kada se drzi pocetak iznad zida
			if (Mis.GetX() == x && Mis.GetY() == y && this->PocetakOznacen == true && ZabranjenaPozicija == true) {
				this->Kocka.setFillColor(sf::Color::Color(255, 77, 77));
			}
			//Boja kada se drzi pocetak i nije iznad zida
			else if (Mis.GetX() == x && Mis.GetY() == y && this->PocetakOznacen == true && ZabranjenaPozicija == false) {
				this->Kocka.setFillColor(sf::Color::Color(121, 210, 166));
			}
			//Boja kada se drzi kraj  iznad zida
			else if (Mis.GetX() == x && Mis.GetY() == y && this->KrajOznacen == true && ZabranjenaPozicija == true) {
				this->Kocka.setFillColor(sf::Color::Color(255, 77, 77));
			}
			//Boja kada se drzi kraj  i nije iznad zida
			else if (Mis.GetX() == x && Mis.GetY() == y && this->KrajOznacen == true && ZabranjenaPozicija == false) {
				this->Kocka.setFillColor(sf::Color::Color(255, 204, 153));
			}
			//Boja pocetka kada se pocetak i kraj preklapaju
			else if (this->PocetakPretrage.GetX() == x && this->PocetakPretrage.GetY() == y && preklapanje) {
				this->Kocka.setFillColor(sf::Color::Color(255, 0, 0));
			}
			//Boja pocetka (Normalna)
			else if (this->PocetakPretrage.GetX() == x && this->PocetakPretrage.GetY() == y && this->PocetakOznacen == false) {
				this->Kocka.setFillColor(sf::Color::Color(51, 153, 102, 255));
			}
			//Boja kraja kada se preklapaju
			else if (this->KrajPretrage.GetX() == x && this->KrajPretrage.GetY() == y && preklapanje) {
				this->Kocka.setFillColor(sf::Color::Color(255, 0, 0));
			}
			//Boja kraja (Normalna)
			else if (this->KrajPretrage.GetX() == x && this->KrajPretrage.GetY() == y && this->KrajOznacen == false) {
				this->Kocka.setFillColor(sf::Color::Color(255, 153, 51));
			}
			//Mis, ali ima prepreka
			else if (Mis.GetX() == x && Mis.GetY() == y && this->PoljePrepreka[this->indexOf(x, y)]) {
				this->Kocka.setFillColor(sf::Color::Color(179, 179, 204));
			}
			//Nema misa, ali se nalazi prepreka
			else if (this->PoljePrepreka[this->indexOf(x, y)]) {
				this->Kocka.setFillColor(sf::Color::Color(102, 102, 153));
			}
			//Nije nikakv poseban tile, ali se mis nalazi na njemu
			else if (Mis.GetX() == x && Mis.GetY() == y && this->PocetakOznacen == false) {
				this->Kocka.setFillColor(sf::Color::Color(217, 217, 217));
			}
			//Sve ostalo
			else {
				this->Kocka.setFillColor(sf::Color::Color(C.R, C.G, C.B, C.A));
			}


			this->MyWindow->draw(this->Kocka);
		}
	}

	if (this->PocetakOznacen) {
		sf::Vector2i Poz = sf::Mouse::getPosition(*this->MyWindow);
		Poz.x -= this->Kocka.getSize().x / 2;
		Poz.y -= this->Kocka.getSize().y / 2;
		this->Kocka.setFillColor(sf::Color::Color(51, 153, 102));
		this->Kocka.setPosition(Poz.x, Poz.y);
		this->MyWindow->draw(this->Kocka);
	}

	if (this->KrajOznacen) {
		sf::Vector2i Poz = sf::Mouse::getPosition(*this->MyWindow);
		Poz.x -= this->Kocka.getSize().x / 2;
		Poz.y -= this->Kocka.getSize().y / 2;
		this->Kocka.setFillColor(sf::Color::Color(255, 153, 51));
		this->Kocka.setPosition(Poz.x, Poz.y);
		this->MyWindow->draw(this->Kocka);
	}

	//Stavi Put do kraja
	sf::RectangleShape FollowNode;
	FollowNode.setSize(sf::Vector2f(this->Kocka.getSize().x / 3, this->Kocka.getSize().y / 3));
	FollowNode.setOrigin(sf::Vector2f(FollowNode.getSize().x / 2, FollowNode.getSize().y / 2));
	FollowNode.setFillColor(sf::Color::Color(51, 204, 255));
	FollowNode.setOutlineColor(sf::Color::Color(0, 102, 255));
	FollowNode.setOutlineThickness(5);
	for (int i = 0; i < this->PutDoKraja.size(); ++i) {
		sf::Vector2f Poz;
		Poz.x = (this->PutDoKraja[i].GetX() * this->Kocka.getSize().x) + this->Origin.GetX() + (this->Kocka.getSize().x/2);
		Poz.y = (this->PutDoKraja[i].GetY() * this->Kocka.getSize().y) + this->Origin.GetY() + (this->Kocka.getSize().y/2);

		FollowNode.setPosition(Poz);
		this->MyWindow->draw(FollowNode);
	}
}

void BojaKocke::SetBoje(int R, int G, int B, int A) {
	this->R = R;
	this->G = G;
	this->B = B;
	this->A = A;
}
