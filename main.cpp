#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

class Node{
	public:
		string letters;
		int index;
		vector <Node *> edges;
		Node *backedge = NULL;
};

class WordDice{
	public:
		Node *source;
		Node *sink;
		vector<Node *> dice;
		vector<Node *> word;
};

void *read_dice(istream &input, WordDice *WD);
void *read_word(istream &input, WordDice *WD);

int main(int argc, char *argv[]){

	if(argc != 3)
		return -1;

	WordDice *WD;

	ifstream FileDice(argv[1]);
	read_dice(FileDice, WD);
	FileDice.close();

	ifstream FileWord(argv[2]);
	read_word(FileWord, WD);
	FileWord.close();


/*	for(int i = 0; i < WD->dice.size(); i++)
		cout << WD->dice[i]->letters << '\n';

	for(int i = 0; i < WD->word.size(); i++)
		cout << WD->word[i]->letters << '\n';
*/
	return 0;

}

void *read_dice(istream &input, WordDice *WD){

	string die;
	int i = 0;
	Node *nDice = new Node;

	while(input >> die){

		cout << die << '\n';
		nDice->letters = die;
		nDice->index = i;
		//this line is causing the segfault
		WD->dice.push_back(nDice);
		i++;

	}

	return 0;
}

void *read_word(istream &input, WordDice *WD){

	string word;
	int i = 0;
	Node *nWord = new Node;

	while(input >> word){

		cout << word << '\n';
		nWord->letters = word;
		nWord->index = i;
//		WD->word.push_back(nWord);
		i++;

	}

	return 0;
}

