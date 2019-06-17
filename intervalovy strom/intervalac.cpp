#include <iostream>
#include <fstream>
#include <utility>
#include "funkcie.cpp"

using namespace std;

void vypisMoznosti() {
	cout << "\n_________________________________________" << endl;
	cout << "Vyberte moznost: " << endl;
	cout << "-1 ... koniec" << endl;
	cout << " 1 ... zmenit hodnotu i-teho prvku" << endl;
	cout << " 2 ... zistit sucet na intervale [l, r)" << endl;
	cout << " 3 ... zistit minimum na intervale [l, r)" << endl;
	cout << " 4 ... zistit maximum na intervale [l, r)" << endl;
	cout << " 5 ... prenasob interval [l, r)" << endl;
	cout << " 6 ... nacitat dalsi vstup" << endl;
}

void zmenPrvok(int index) {
	int pozicia, hodnota;

	cout << "\nZMENA CISLA" << endl;

	cout << "pozicia: ";
	while(!(cin >> pozicia) || pozicia < 0 || pozicia > vstupy[index].velkost-1){
	    cout << "Neplatna hodnota - {0, ..., " << vstupy[index].velkost-1 << "}" << endl;
	    cin.clear();
	    cin.ignore(numeric_limits<streamsize>::max(), '\n');
	    cout << "pozicia: "; 
	}

	cout << "Hodnota: ";
	while(!(cin >> hodnota)){
	    cout << "Neplatna hodnota - zadajte cele cislo" << endl;
	    cin.clear();
	    cin.ignore(numeric_limits<streamsize>::max(), '\n');
	    cout << "pozicia: "; 
	}

	zmen(index, pozicia, hodnota, 1);

	cout << "Zmenene!" << endl;
}

pair<int,int> getInterval(int index) {
	int l,r;

	cout << "l: ";
	while(!(cin >> l) || l < 0 || l > vstupy[index].velkost-1){
	    cout << "Neplatna hodnota - {0, ..., " << vstupy[index].velkost-1 << "}" << endl;
	    cin.clear();
	    cin.ignore(numeric_limits<streamsize>::max(), '\n');
	    cout << "l: "; 
	}

	cout << "r: ";
	while(!(cin >> r) || r < 1 || r > vstupy[index].velkost){
	    cout << "Neplatna hodnota - {1, ..., " << vstupy[index].velkost << "}" << endl;
	    cin.clear();
	    cin.ignore(numeric_limits<streamsize>::max(), '\n');
	    cout << "r: ";
	}

	return make_pair(l,r);
}

void sucet(int index) {
	cout << "\nSUCET NA INTERVALE [l, r)" << endl;

	pair<int,int> hranice = getInterval(index);

	cout << "SUCET NA DANOM INTERVALE JE: " << zistiSucet(index, hranice.first, hranice.second, 1) << endl;
}

void minimum(int index) {
	cout << "\nMINUMUM NA INTERVALE [l, r)" << endl;

	pair<int,int> hranice = getInterval(index);

	cout << "MINIMUM NA DANOM INTERVALE JE: " << zistiMinimum(index, hranice.first, hranice.second, 1) << endl;
}

void maximum(int index) {
	cout << "\nMAXIMUM NA INTERVALE [l, r)" << endl;

	pair<int,int> hranice = getInterval(index);

	cout << "MAXIMUM NA DANOM INTERVALE JE: " << zistiMaximum(index, hranice.first, hranice.second, 1) << endl;
}

void prenasob(int index) {
	int h;

	cout << "\nPRENASOBENIE INTERVALU [l, r)" << endl;

	pair<int,int> hranice = getInterval(index);

	cout << "hodnota: ";
	while(!(cin >> h)){
	    cout << "Neplatna hodnota - zadajte cele cislo" << endl;
	    cin.clear();
	    cin.ignore(numeric_limits<streamsize>::max(), '\n');
	    cout << "h: ";
	}

	prenasobLazy(index, hranice.first, hranice.second, 1, h);

	cout << "Prenasobene!" << endl;
}

void getInput() {

	vstupy.resize(vstupy.size()+1);
	int aktI = vstupy.size() - 1;

	int typVstupu;
	cout << "VSTUP zo:" << endl;
	cout << "1 ... suboru" << endl;
	cout << "2 ... stdin (klavesnica)" << endl;

	while(!(cin >> typVstupu) || typVstupu < 1 || typVstupu > 2){
	    cout << "Neplatna hodnota\n1 - subor, 2 - klavesnica" << endl;
	    cin.clear();
	    cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	// nacitanie vstupu
	fstream F;
	if(typVstupu == 1) {

		string nazov;
		cout << "Zadajte nazov suboru" << endl;
		cout << "Subor musi byt ulozeny v rovnakom priecinku ako program!" << endl;
		
		cin >> nazov;
		F.open(nazov, ifstream::in);
		while(!F.good()) {
			cout << "Subor neexistuje!\nZadajte nazov znova:" << endl;
			cin >> nazov;
			F.open(nazov, ifstream::in);
		}

		F >> vstupy[aktI].velkost;

	} else {
		
		cout << "Pocet cisel: ";

		while(!(cin >> vstupy[aktI].velkost) || vstupy[aktI].velkost < 1){
		    cout << "Neplatna hodnota - Zadajte cele kladne cislo!" << endl;
		    cin.clear();
		    cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}

	// inicializovanie potrebnych premennych
	init(aktI);

	// nacitanie hodnot postupnosti
	int a;
	if(typVstupu == 1) {
		for(int i = 0; i < vstupy[aktI].velkost; i++) {
			F >> a;
			vstupy[aktI].sumTree[vstupy[aktI].n+i] = a;
			vstupy[aktI].minTree[vstupy[aktI].n+i] = a;
			vstupy[aktI].maxTree[vstupy[aktI].n+i] = a;
		}

		F.close();
	} else {
		cout << "Cisla: " << endl;
		for(int i = 0; i < vstupy[aktI].velkost; i++) {
			cin >> a;
			vstupy[aktI].sumTree[vstupy[aktI].n+i] = a;
			vstupy[aktI].minTree[vstupy[aktI].n+i] = a;
			vstupy[aktI].maxTree[vstupy[aktI].n+i] = a;
		}
	}

	// dopocitanie hodnot v strome
	dopocitaj(aktI);
}

int main() {

	cout << "_________________________" << endl;
	cout << "|     INTERVALAC        |" << endl;
	cout << "ˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆ" << endl;


	getInput();
	int vstupCislo = 0;

	// odpovede na dotazy
	vypisMoznosti();

	int vyber;
	cin >> vyber;

	while(vyber != -1) {
		
		if(vyber == 1) {
			zmenPrvok(vstupCislo);
		} else if(vyber == 2) {
			sucet(vstupCislo);
		} else if(vyber == 3) {
			minimum(vstupCislo);
		} else if(vyber == 4) {
			maximum(vstupCislo);
		} else if(vyber == 5) {
			prenasob(vstupCislo);
		} else if(vyber == 6) {
			getInput();
		} else {
			cout << "neplatna hodnota" << endl;
		}

		if(vstupy.size() == 1) {
			vstupCislo = 0;
		} else {
			cout << "Cislo vstupu na ktory chcete odpovede: ";
			while(!(cin >> vstupCislo) || vstupCislo < 0 || vstupCislo > vstupy.size()-1){
			    cout << "Neplatna hodnota - zadajte cele cislo z rozsahu 0 .. " << vstupy.size()-1 << endl;
			    cin.clear();
			    cin.ignore(numeric_limits<streamsize>::max(), '\n');
			    cout << "Cislo vstupu: ";
			}
		}

		vypisMoznosti();
		cin >> vyber;

	}

	cout << "\nProgram skoncil!" << endl;

	return 0;
}
