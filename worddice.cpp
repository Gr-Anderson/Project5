#include <vector>
#include <string>
#include <queue>
#include <fstream>
#include <iostream>
#include <algorithm>

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
bool bfs(Node *&source, Node *&sink, WordDice &WD);
int edmonds_karp(Node *&source, Node *&sink, WordDice &WD);
void clear_backedges(Node *&source, Node *&sink, WordDice &WD);

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

		if (edmonds_karp(source, sink, WD) == word.size())
		{
			for (int i = 0; i < WD.word.size(); i++)
			{
				cout << WD.word[i]->edges[0]->index - 1;

				if (i == WD.word.size()-1)
					cout << ": ";
				else
					cout << ",";
			}
			cout << word << '\n';
		}
		else
		{
			cout << "Cannot spell " << word << '\n';

		}

		//need to delete the nodes for the current word's letters to make connections for the next word

		source->edges.clear();
		sink->edges.clear();

		for(int i = 0; i < WD.word.size(); i++)
			delete WD.word[i];

		WD.word.clear();

		for(int i = 0; i < WD.dice.size(); i++){
			WD.dice[i]->edges.clear();
			WD.dice[i]->backedge = NULL;
		}

		sink->backedge = NULL;

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

bool bfs(Node *&source, Node *&sink, WordDice &WD)
{
	queue <Node *> bfsqueue;
	Node *curr;

	bfsqueue.push(source);

	while(bfsqueue.empty() == false)
	{
		curr = bfsqueue.front();
		bfsqueue.pop();

		//		cout << "BFS CURR = " << curr->index << '\n';

		for(int i = 0; i < curr->edges.size(); i++)
		{
			if (curr->edges[i]->backedge != NULL)
			{
				//				cout << "CURR ALREADY HAS BACKEDGE\n";
				continue;
			}

			//			cout << "setting " << curr->edges[i]->index << " backedge = " << curr->index << '\n';
			curr->edges[i]->backedge = curr;

			if (curr->edges[i]->index == sink->index)
			{
				//				cout << "BFS RETURNS TRUE\n";
				return true;
			}
			bfsqueue.push(curr->edges[i]);
		}
	}
	//cout << "BFS RETURNS FALSE\n";
	return false;
}

int edmonds_karp(Node *&source, Node *&sink, WordDice &WD)
{
	Node *curr;
	Node *prev;

	while (bfs(source, sink, WD))
	{
		for (curr = sink; curr != source; curr = curr->backedge)
		{
			//			cout << "curr->index = " << curr->index << '\n';
			//			cout << "curr->backedge = " << curr->backedge->index << '\n';

			curr->edges.push_back(curr->backedge);

			//FIXME this line needs to erase current node from the backedge's edges vector
			// but it takes the position instead of the actual node

			vector<Node *>::iterator it = find(curr->backedge->edges.begin(), curr->backedge->edges.end(), curr);

			int ind = distance(curr->backedge->edges.begin(), it);

			curr->backedge->edges.erase(curr->backedge->edges.begin()+ind);
		}
		clear_backedges(source, sink, WD);
	}
	//	cout << "Sink->edges.size() = " << sink->edges.size() << '\n';
	return sink->edges.size();
}

void clear_backedges(Node *&source, Node *&sink, WordDice &WD)
{
	source->backedge = NULL;
	sink->backedge = NULL;

	for (int i = 0; i < WD.dice.size(); i++)
		WD.dice[i]->backedge = NULL;

	for (int i = 0; i < WD.word.size(); i++)
		WD.word[i]->backedge = NULL;
}
