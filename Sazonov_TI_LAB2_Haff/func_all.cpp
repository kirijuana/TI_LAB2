#include "stdafx.h"
#include "lib.h"

using namespace std;

ofstream fout("E://уч/3 курс/2й семестр/Теория информации/lab 2/out.txt");
ifstream fin("E://уч/3 курс/2й семестр/Теория информации/lab 2/in.txt");

class Node
{
public:
	int count;
	char symbol;
	Node *left;
	Node *right;
	Node *rightright;

	Node() { }

	Node(char __symbol, int __count)
	{
		symbol = __symbol;
		count = __count;
	}

	Node(Node *l, Node *r, Node *rr) // create parent
	{
		symbol = 0;
		left = l;
		right = r;
		rightright = rr;
		count = l->count + r->count + rr->count;
	}

	static void Print(Node *root, int depth = 0)
	{
		if (!root) return;

		if (root->symbol)
		{
			for (int i = 0; i < depth; i++)
				fout << ".";
			fout << root->symbol << endl;
		}
		else
			depth++;
		Print(root->left, depth);
		Print(root->right, depth);
		Print(root->rightright, depth);
	}
};

string raw;

map<char, int> frequencies;

list<Node*> trees;

map<char, int>::iterator itr;
Node *root;

void BuildTable(Node *root, vector<int> &code, map<char, vector<int>> &table) 
{
	if (root->left)
	{
		code.push_back(0); // left
		BuildTable(root->left, code, table);
	}

	if (root->right)
	{
		code.push_back(1); // right
		BuildTable(root->right, code, table);
	}

	if (root->rightright)
	{
		code.push_back(2); // right->rigth
		BuildTable(root->rightright, code, table);
	}


	if (root->symbol)
		table[root->symbol] = code;
	if (code.size())
		code.pop_back();
}

bool SortNode(const Node *a, const Node *b)
{
	return a->count < b->count;
}

string Decode(string &str, map<vector<int>, char> &table) 
{
	string out = "";
	vector<int> code;
	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] == '0')
		{
			code.push_back(0);
		}
		if (str[i] == '1')
		{
			code.push_back(1);
		}
		if (str[i] == '2')
		{
			code.push_back(2);
		}
	
		if (table[code])
		{
			out += table[code];
			code.clear();
		}
	}
	return out;
}

void count_frequencies()
{
	getline(fin, raw);
	for (int i = 0; i < raw.length(); i++)
	frequencies[raw[i]]++;
}

void convert_to_tree()
{
	for (itr = frequencies.begin(); itr != frequencies.end(); itr++)
	{
		Node *p = new Node(itr->first, itr->second);
		trees.push_back(p);
	}
}

void code_decode_func()
{

	while (trees.size() != 1)
	{
		trees.sort(SortNode);

		Node *l = trees.front();
		trees.pop_front();
		if(trees.size() == 0)
		{
			Node *r = new Node('\0', 0);
			trees.push_back(r);
		}	

			Node *r = trees.front();
			trees.pop_front();

		if (trees.size() == 0)
		{
			 Node *rr = new Node('\0', 0);
			 trees.push_back(rr);
		}
		
			Node *rr = trees.front();
			trees.pop_front();

		Node *parent = new Node(l, r, rr);
		trees.push_back(parent);

	}

	root = trees.front();
	root->Print(root);

	vector<int> code; // buffer
	map<char, vector<int> > table;
	BuildTable(root, code, table); // generate symbol-code

								   // print symbol & its code
	for (itr = frequencies.begin(); itr != frequencies.end(); itr++)
	{
		fout << itr->first << " - ";
		for (int j = 0; j < table[itr->first].size(); j++)
			fout << table[itr->first][j];
		fout << endl;
	}

	string out = "";
	// print coded string
	for (int i = 0; i < raw.length(); i++)
		for (int j = 0; j < table[raw[i]].size(); j++)
		{
			out += table[raw[i]][j] + '0';
			fout << table[raw[i]][j];
		}
	fout << endl;
	
	map<vector<int>, char> ftable;
	for (auto i = table.begin(); i != table.end(); i++)
		ftable[i->second] = i->first;
	fout << Decode(out, ftable) << endl;
}

