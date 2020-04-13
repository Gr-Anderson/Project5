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

void *read_dice(istream &input, WordDice &WD);
void *read_word(istream &input, WordDice &WD);

int main(int argc, char *argv[]){

	if(argc != 3)
		return -1;

	WordDice WD;

	ifstream FileDice(argv[1]);
	read_dice(FileDice, WD);
	FileDice.close();

	ifstream FileWord(argv[2]);
	read_word(FileWord, WD);
	FileWord.close();

	//beginning of making the bipartite graph

/*
	for(int word = 0; i < WD.word.size(); i++)
		for(int wordLetter = 0; wordLetter < WD.word[word].size(); wordLetter++)
			for(int dice = 0; dice < WD.dice.size(); dice++)
				for(int diceLetter = 0; diceLetter < WD.dice[dice].size(); diceLetter++){


					if(word[wordLetter].find(dice[diceLetter]) != npos)
						WD.word[word].edges.push_back(
	
*/
	return 0;

}

void *read_dice(istream &input, WordDice &WD){

	string die;
	int i = 0;

	while(input >> die){

		Node *nDice = new Node;

		nDice->letters = die;
		nDice->index = i;
		WD.dice.push_back(nDice);
		i++;

	}

}

void *read_word(istream &input, WordDice &WD){

	string word;
	int i = 0;

	while(input >> word){
	
		Node *nWord = new Node;

		nWord->letters = word;
		nWord->index = i;	
		WD.word.push_back(nWord);
		i++;

	}

}

