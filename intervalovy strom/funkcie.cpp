#include <iostream>
#include <vector>

using namespace std;

struct data {

	vector<int> sumTree;
	vector<int> minTree;
	vector<int> maxTree;

	vector<int> lazy;

	vector<int> zaciatok;
	vector<int> koniec;

	int n = 1;
	int velkost;

};

vector<data> vstupy;

const int INF = 2147483647;
const int minusINF = -2147483648;


void zmen(int index, int p, int h, int v) {
	if(v == vstupy[index].n + p) {
		vstupy[index].sumTree[vstupy[index].n+p] = h;
		vstupy[index].minTree[vstupy[index].n+p] = h;
		vstupy[index].maxTree[vstupy[index].n+p] = h;

		vstupy[index].lazy[vstupy[index].n+p] = 1;
		return;
	}

	vstupy[index].lazy[v*2] *= vstupy[index].lazy[v];
	vstupy[index].lazy[v*2+1] *= vstupy[index].lazy[v];
	vstupy[index].lazy[v] = 1;

	if(p < vstupy[index].koniec[v*2]) {
		zmen(index, p, h, v*2);
	} else {
		zmen(index, p, h, v*2+1);
	}

	vstupy[index].sumTree[v] = vstupy[index].sumTree[v*2] + vstupy[index].sumTree[v*2+1];
	vstupy[index].minTree[v] = min(vstupy[index].minTree[v*2], vstupy[index].minTree[v*2+1]);
	vstupy[index].maxTree[v] = max(vstupy[index].maxTree[v*2], vstupy[index].maxTree[v*2+1]);
}


int zistiSucet(int index, int l, int r, int v) {

	if(vstupy[index].zaciatok[v] >= r || vstupy[index].koniec[v] <= l) {
		return 0;
	}

	if(vstupy[index].zaciatok[v] >= l && vstupy[index].koniec[v] <= r) {
		return vstupy[index].sumTree[v] * vstupy[index].lazy[v];
	}

	vstupy[index].lazy[v*2] *= vstupy[index].lazy[v];
	vstupy[index].lazy[v*2+1] *= vstupy[index].lazy[v];
	vstupy[index].lazy[v] = 1;

	return zistiSucet(index,l,r,v*2) + zistiSucet(index,l,r,v*2+1);
}

int zistiMinimum(int index, int l, int r, int v) {

	if(vstupy[index].zaciatok[v] >= r || vstupy[index].koniec[v] <= l) {
		return INF;
	}

	if(vstupy[index].zaciatok[v] >= l && vstupy[index].koniec[v] <= r) {
		return vstupy[index].minTree[v] * vstupy[index].lazy[v];
	}

	vstupy[index].lazy[v*2] *= vstupy[index].lazy[v];
	vstupy[index].lazy[v*2+1] *= vstupy[index].lazy[v];
	vstupy[index].lazy[v] = 1;

	return min(zistiMinimum(index,l,r,v*2), zistiMinimum(index,l,r,v*2+1));
}

int zistiMaximum(int index, int l, int r, int v) {

	if(vstupy[index].zaciatok[v] >= r || vstupy[index].koniec[v] <= l) {
		return minusINF;
	}

	if(vstupy[index].zaciatok[v] >= l && vstupy[index].koniec[v] <= r) {
		return vstupy[index].maxTree[v] * vstupy[index].lazy[v];
	}

	vstupy[index].lazy[v*2] *= vstupy[index].lazy[v];
	vstupy[index].lazy[v*2+1] *= vstupy[index].lazy[v];
	vstupy[index].lazy[v] = 1;

	return max(zistiMaximum(index,l,r,v*2), zistiMaximum(index,l,r,v*2+1));
}


void prenasobLazy(int index, int l, int r, int v, int h) {

	if(vstupy[index].zaciatok[v] >= r || vstupy[index].koniec[v] <= l) {
		return;
	}

	if(vstupy[index].zaciatok[v] >= l && vstupy[index].koniec[v] <= r) {
		vstupy[index].lazy[v] *= h;

		int zmena = (h-1) * vstupy[index].sumTree[v];

		for(int i = v/2; i >= 1; i/=2) {
			vstupy[index].sumTree[i] += zmena;
			vstupy[index].minTree[i] = min(vstupy[index].minTree[i*2] * vstupy[index].lazy[i*2],
				vstupy[index].minTree[i*2+1] * vstupy[index].lazy[i*2+1]);
			vstupy[index].maxTree[i] = max(vstupy[index].maxTree[i*2] * vstupy[index].lazy[i*2],
				vstupy[index].maxTree[i*2+1] * vstupy[index].lazy[i*2+1]);
		}
		
		return;
	}

	prenasobLazy(index,l,r,v*2,h);
	prenasobLazy(index,l,r,v*2+1,h);

}


void init(int index) {

	while(vstupy[index].n < vstupy[index].velkost) vstupy[index].n *= 2;

	vstupy[index].sumTree.resize(2*vstupy[index].n, 0);
	vstupy[index].minTree.resize(2*vstupy[index].n, INF);
	vstupy[index].maxTree.resize(2*vstupy[index].n, minusINF);

	vstupy[index].lazy.resize(2*vstupy[index].n, 1);

	// predpocitanie zaciatkov a koncov intervalov pod jednotlivymi vrcholmi
	vstupy[index].zaciatok.resize(2*vstupy[index].n);
	vstupy[index].koniec.resize(2*vstupy[index].n);
	
	for(int i = 0; i < vstupy[index].n; i++) {
		vstupy[index].zaciatok[vstupy[index].n + i] = i;
		vstupy[index].koniec[vstupy[index].n + i] = i+1;
	}

	for(int i = vstupy[index].n - 1; i > 0; i--) {
		vstupy[index].zaciatok[i] = vstupy[index].zaciatok[2*i];
		vstupy[index].koniec[i] = vstupy[index].koniec[2*i+1];
	}
}

void dopocitaj(int index) {
	for(int i = vstupy[index].n-1; i > 0; i--) {
		vstupy[index].sumTree[i] = vstupy[index].sumTree[2*i] + vstupy[index].sumTree[2*i+1];
		vstupy[index].minTree[i] = min(vstupy[index].minTree[2*i], vstupy[index].minTree[2*i+1]);
		vstupy[index].maxTree[i] = max(vstupy[index].maxTree[2*i], vstupy[index].maxTree[2*i+1]);
	}
}
