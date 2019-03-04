#include <iostream>
#include <fstream>
#include <vector>
#include "interval-tree.cpp"

using namespace std;

vector<interval_tree*> trees;

void print_options() {
	cout << "\n_________________________________________" << endl;
	cout << "Make a choice: " << endl;
	cout << "-1 ... Quit" << endl;
	cout << " 1 ... Change value" << endl;
	cout << " 2 ... Sum on interval [l, r)" << endl;
	cout << " 3 ... Change interval [l, r)" << endl;
	cout << " 4 ... Load next input" << endl;
}

void change_value(int index) {
	int position, value;

	cout << "\nCHANGE VALUE" << endl;

	cout << "position: ";
	while(!(cin >> position) || position < 0 || position > trees[index]->size-1){
	    cout << "Invalid value - {0, ..., " << trees[index]->size-1 << "}" << endl;
	    cin.clear();
	    cin.ignore(numeric_limits<streamsize>::max(), '\n');
	    cout << "position: "; 
	}

	cout << "Value: ";
	while(!(cin >> value)){
	    cout << "Invalid value - enter a whole number" << endl;
	    cin.clear();
	    cin.ignore(numeric_limits<streamsize>::max(), '\n');
	    cout << "position: "; 
	}

	trees[index]->change(position, value);

	cout << "Done!" << endl;
}

pair<int,int> getInterval(int index) {
	int l,r;

	cout << "l: ";
	while(!(cin >> l) || l < 0 || l > trees[index]->size-1){
	    cout << "Invalid value - {0, ..., " << trees[index]->size-1 << "}" << endl;
	    cin.clear();
	    cin.ignore(numeric_limits<streamsize>::max(), '\n');
	    cout << "l: "; 
	}

	cout << "r: ";
	while(!(cin >> r) || r < 1 || r > trees[index]->size){
	    cout << "Invalid value - {1, ..., " << trees[index]->size << "}" << endl;
	    cin.clear();
	    cin.ignore(numeric_limits<streamsize>::max(), '\n');
	    cout << "r: ";
	}

	return make_pair(l,r);
}

void get_sum(int index) {
	cout << "\nSUM ON INTERVAL [l, r)" << endl;

	pair<int,int> boundaries = getInterval(index);

	cout << "The sum is: " << trees[index]->sum(boundaries.first, boundaries.second) << endl;
}

void multiply(int index) {
	int value;

	cout << "\nMULTIPLY INTERVAL [l, r)" << endl;

	pair<int,int> boundaries = getInterval(index);

	cout << "value: ";
	while(!(cin >> value)){
	    cout << "Invalid value. Enter a whole number" << endl;
	    cin.clear();
	    cin.ignore(numeric_limits<streamsize>::max(), '\n');
	    cout << "value: ";
	}

	trees[index]->change_interval(boundaries.first, boundaries.second, value);

	cout << "Done!" << endl;
}

void getInput() {

	trees.resize(trees.size()+1);
	int index = trees.size() - 1;

	int input_type;
	cout << "Input from:" << endl;
	cout << "1 ... file" << endl;
	cout << "2 ... keyboard" << endl;

	while(!(cin >> input_type) || input_type < 1 || input_type > 2){
	    cout << "Invalid value\n1 - file, 2 - keyboard" << endl;
	    cin.clear();
	    cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	// load input size
	int input_size;

	fstream F;
	if(input_type == 1) {

		string name;
		cout << "Enter a file name" << endl;
		
		cin >> name;
		F.open(name, ifstream::in);
		while(!F.good()) {
			cout << "File does not exist!\nIt has to be in the same directory as program\nFile name:" << endl;
			cin >> name;
			F.open(name, ifstream::in);
		}

		F >> input_size;
		trees[index] = new interval_tree(input_size);

	} else {
		
		cout << "Input size: ";

		while(!(cin >> input_size) || input_size < 1){
		    cout << "Invalid value. Please entre a natural number." << endl;
		    cin.clear();
		    cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}

		trees[index] = new interval_tree(input_size);
	}

	// load input sequence
	int a;
	if(input_type == 1) {
		for(int i = 0; i < input_size; i++) {
			F >> a;
			trees[index]->change(i,a);
		}

		F.close();
	} else {
		cout << "Sequence: " << endl;
		for(int i = 0; i < input_size; i++) {
			cin >> a;
			trees[index]->change(i,a);
		}
	}
}

int main() {

	cout << "_________________________" << endl;
	cout << "|     INTERVAL TREE     |" << endl;
	cout << "ˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆ" << endl;

	getInput();

	int input_number = 0;

	// answering questions
	print_options();

	int choice;
	cin >> choice;

	while(choice != -1) {
		
		if(choice == 1) {
			change_value(input_number);
		} else if(choice == 2) {
			get_sum(input_number);
		} else if(choice == 3) {
			multiply(input_number);
		} else if(choice == 4) {
			getInput();
		} else {
			cout << "Incorrect Value" << endl;
		}

		if(trees.size() == 1) {
			input_number = 0;
		} else {
			cout << "Input number (counting from zero):";
			while(!(cin >> input_number) || input_number < 0 || input_number > trees.size()-1){
			    cout << "Invalid number. Please enter a number - 0 .. " << trees.size()-1 << endl;
			    cin.clear();
			    cin.ignore(numeric_limits<streamsize>::max(), '\n');
			    cout << "Input number: ";
			}
		}

		print_options();
		cin >> choice;

	}

	cout << "\nQuit!" << endl;

	return 0;
}
