// worddice.cpp
// Project 5
// Authors: Logan Tillman, Grant Anderson
// Date: April 15, 2020

/* This program takes two parameters, a list of word dice, and a list of words to spell.
   Using the Edmonds-Karp Algorithm, this program determines if it can spell each word with
   the given die */

#include <vector>
#include <string>
#include <queue>
#include <fstream>
#include <iostream>
#include <algorithm>
using namespace std;

// Class used to store our nodes
class Node
{
	public:
		string letters;
		int index = 0;
		vector <Node *> edges;
		Node *backedge = NULL;
};

// Class used to store the given dice and words
class WordDice
{
	public:
		vector<Node *> dice;
		vector<Node *> word;
};

// Function for reading in each dice
void *read_dice(istream &input, WordDice &WD);

// Function for reading in each word
void *read_word(istream &input, WordDice &WD);

// Function that performs the Breadth-first search
bool bfs(Node *&source, Node *&sink, WordDice &WD);

// Function that performs the Edmonds-Karp Algorithm
int edmonds_karp(Node *&source, Node *&sink, WordDice &WD);

// Functon that clears all of the backedges in the graph
void clear_backedges(Node *&source, Node *&sink, WordDice &WD);

int main(int argc, char *argv[])
{
	if(argc != 3)
		return -1;

	Node *source;
	Node *sink;
	WordDice WD;

	source = new Node;
	sink = new Node;
	source->index = 0;

	// Reading in the dice
	ifstream FileDice(argv[1]);
	read_dice(FileDice, WD);
	FileDice.close();

	// Reading in each word
	ifstream FileWord(argv[2]);
	string word;

	// Executing for each word read in
	while(FileWord >> word)
	{
		int index;

		index = WD.dice.size()+1;

		// Adding each letter to the word vector inside of the WordDice class
		for(int i = 0; i < word.size(); i++)
		{
			Node *nLetter;

			nLetter = new Node;
			nLetter->letters = word[i];
			nLetter->index = index;
			WD.word.push_back(nLetter);
			index++;
		}

		// Making the bipartite graph
		for(int dice = 0; dice < WD.dice.size(); dice++)
			source->edges.push_back(WD.dice[dice]);

		for(int numWord = 0; numWord < WD.word.size(); numWord++)
		{
			WD.word[numWord]->edges.push_back(sink);

			if(numWord == WD.word.size()-1)
				sink->index = WD.word[numWord]->index+1;

			for(int dice = 0; dice < WD.dice.size(); dice++)
				if(WD.dice[dice]->letters.find(WD.word[numWord]->letters) != string::npos)
					WD.dice[dice]->edges.push_back(WD.word[numWord]);
		}

		// Checking to see if we can spell the word by calling the Edmonds-Karp Function
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
			cout << "Cannot spell " << word << '\n';

		// Clearing the nodes to get ready for the next word
		source->edges.clear();
		sink->edges.clear();

		for(int i = 0; i < WD.word.size(); i++)
			delete WD.word[i];

		WD.word.clear();

		for(int i = 0; i < WD.dice.size(); i++)
		{
			WD.dice[i]->edges.clear();
			WD.dice[i]->backedge = NULL;
		}

		sink->backedge = NULL;
	}

	FileWord.close();
	return 0;
}

// Function for reading in each dice
void *read_dice(istream &input, WordDice &WD)
{
	string die;
	int i = 1;

	while(input >> die)
	{
		Node *nDice;

		nDice = new Node;
		nDice->letters = die;
		nDice->index = i;
		WD.dice.push_back(nDice);
		i++;
	}
}

// Function that performs the Breadth-first search
bool bfs(Node *&source, Node *&sink, WordDice &WD)
{
	// Creating a queue to store the adjacent nodes
	queue <Node *> bfsqueue;

	// Using a Node * to store the current node we're at
	Node *curr;

	bfsqueue.push(source);

	// Executes until we've visited every node
	while(bfsqueue.empty() == false)
	{
		// Starting with the first element in the queue
		curr = bfsqueue.front();
		bfsqueue.pop();

		// Exectues for every adjacent node
		for(int i = 0; i < curr->edges.size(); i++)
		{
			// Checks if we've already visited the adjacent node
			if (curr->edges[i]->backedge != NULL)
				continue;

			// If we haven't, we set the backedge to the current node
			curr->edges[i]->backedge = curr;

			// If the adjacent node is the sink, we've found a path
			if (curr->edges[i]->index == sink->index)
				return true;

			// Pushing each adjacent node onto the queue
			bfsqueue.push(curr->edges[i]);
		}
	}

	// Returns false if it doesn't find a path
	return false;
}

// Function for the Edmonds-Karp Algorithm
int edmonds_karp(Node *&source, Node *&sink, WordDice &WD)
{
	Node *curr;

	// Runs the BFS until it can't find a path
	while (bfs(source, sink, WD))
	{
		// Starts at the sink and traverses to the source using the backedges
		for (curr = sink; curr != source; curr = curr->backedge)
		{
			vector<Node *>::iterator it;

			// Adding the current node's backedge to the current node's edges
			curr->edges.push_back(curr->backedge);

			// Erasing the current node from its backedge's edge vector
			it = find(curr->backedge->edges.begin(), curr->backedge->edges.end(), curr);
			curr->backedge->edges.erase(it);
		}

		// Clearing the back edges so we can run another BFS
		clear_backedges(source, sink, WD);
	}
	
	// Returning how many paths are connected to the sink
	return sink->edges.size();
}

// Function for clearing all backedges in the graph
void clear_backedges(Node *&source, Node *&sink, WordDice &WD)
{
	source->backedge = NULL;
	sink->backedge = NULL;

	for (int i = 0; i < WD.dice.size(); i++)
		WD.dice[i]->backedge = NULL;

	for (int i = 0; i < WD.word.size(); i++)
		WD.word[i]->backedge = NULL;
}
