#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

class Node{
	public:
		string letters;
		int index = 0;
		vector <Node *> edges;
		Node *backedge = NULL;
};

class WordDice{
	public:
		vector<Node *> dice;
		vector<Node *> word;
};

void *read_dice(istream &input, WordDice &WD);
void *read_word(istream &input, WordDice &WD);

int main(int argc, char *argv[]){

	if(argc != 3)
		return -1;

	Node *source = new Node;
	Node *sink = new Node;

	source->index = 0;

	WordDice WD;

	ifstream FileDice(argv[1]);
	read_dice(FileDice, WD);
	FileDice.close();

	ifstream FileWord(argv[2]);
	string word;

	while(FileWord >> word){

		int index = WD.dice.size()+1;

		//reading words

		for(int i = 0; i < word.size(); i++){

			Node *nLetter = new Node;

			nLetter->letters = word[i];
			nLetter->index = index;
			WD.word.push_back(nLetter);
			index++;
		}

		//making the bipartite graph
		
		for(int dice = 0; dice < WD.dice.size(); dice++)
			source->edges.push_back(WD.dice[dice]);

		for(int numWord = 0; numWord < WD.word.size(); numWord++){

			WD.word[numWord]->edges.push_back(sink);

			if(numWord == WD.word.size()-1)
				sink->index = WD.word[numWord]->index+1;

			for(int dice = 0; dice < WD.dice.size(); dice++){

				if(WD.dice[dice]->letters.find(WD.word[numWord]->letters) != string::npos){
					WD.dice[dice]->edges.push_back(WD.word[numWord]);

				}
			}
		}

		//checking connections

		cout << "NODE 0: SOURCE Edges to ";

		for(int i = 0; i < source->edges.size(); i++)
			cout << source->edges[i]->index << ' ';

		cout << '\n';

		for(int i = 0; i < WD.dice.size(); i++){

			cout << "Node " << WD.dice[i]->index << ": " << WD.dice[i]->letters << " Edges to ";

			for(int j = 0; j < WD.dice[i]->edges.size(); j++)
				cout << WD.dice[i]->edges[j]->index << ' ';

			cout << '\n';

		}

		for(int i = 0; i < WD.word.size(); i++){
			cout << "Node " << WD.word[i]->index << ": " << WD.word[i]->letters << " Edges to " << WD.word[i]->edges[0]->index << '\n';
		}

		cout << "Node " << sink->index << ": Edges to\n";

		cout << '\n';

		//need to delete the nodes for the current word's letters to make connections for the next word

		source->edges.clear();

		for(int i = 0; i < WD.word.size(); i++)
			delete WD.word[i];

		WD.word.clear();

		for(int i = 0; i < WD.dice.size(); i++)
			WD.dice[i]->edges.clear();

	}
	FileWord.close();

	return 0;

}

void *read_dice(istream &input, WordDice &WD){

	string die;
	int i = 1;

	while(input >> die){

		Node *nDice = new Node;

		nDice->letters = die;
		nDice->index = i;
		WD.dice.push_back(nDice);
		i++;

	}

}
