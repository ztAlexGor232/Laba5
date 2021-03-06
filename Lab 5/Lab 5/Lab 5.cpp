﻿#include <iostream>
#include <fstream>
#include <cmath>
#include <queue>
#include <string>
#include <map>
#include <vector>
#include <conio.h>

using namespace std;

bool isNumber(string);
float stringToFloat(string);
queue<string> parseToTokens(string);
vector<string> readFromFile(ifstream&);
void output(vector<string>);

class Node {
	string data;
public:
	Node* left;
	Node* right;
	Node(string a) {
		data = a;
		left = nullptr;
		right = nullptr;
	}
	void SetData(string data) {
		this->data = data;
	}
	bool isNumber() {
		for (int i = 0; i < data.size(); i++) {
			if (((int)data[i] < 48 || (int)data[i] > 57) && data[i] != '.' && data[i] != '-')
				return false;
		}
		return true;
	}
	string GetSymbol() {
		return data;
	}

	float GetNumber() {
		bool minus = false;
		if (data[0] == '-') {
			minus = true;
			data.erase(0,1);
		}
		float part1 = 0, part2 = 0;
		int it = 0;
		while (it < data.size() && data[it] != '.') {
			part1 *= 10;
			part1 += (int)data[it] - 48;
			it++;
		}
		if (it != data.size()) {
			for (int i = data.size() - 1; i > it; i--) {
				part2 += (int)data[i] - 48;
				part2 /= 10;
			}
		}
		if (minus)
			return (part1 + part2) * -1;
		return part1 + part2;
	}
	void Output() {
		cout << data << endl;
		if (left)
			left->Output();
		if (right)
			right->Output();
	}
};
class Tree {
public:
	virtual float Count(Node* start) = 0;
	virtual Node* GetHead() = 0;
	virtual void Output(Node* root) = 0;
};
class StatementList : public Tree {
	bool isCaptain;
	vector<Tree*> StTrees;
public:
	static map<string, float> statments;
	StatementList(vector<string> tokens, bool arg);
	float Count(Node* none) override;
	void Output(Node* none) override;
	Node* GetHead() override;
};
map<string, float> StatementList::statments;
class BinTree : public Tree {
	bool IsState;
	Node* head;
public:
	BinTree(vector<string> tokens) {
		AddNode(head, tokens);
	}
	float Count(Node* start) {
		string currSymbol = start->GetSymbol();
		if (!start->left) {
			if (start->isNumber())
				return start->GetNumber();
			if (currSymbol[0] == '-') {
				currSymbol.erase(0, 1);
				return -StatementList::statments[currSymbol];
			}
			return StatementList::statments[currSymbol];
		}
		if (currSymbol == "=") {
			StatementList::statments[start->left->GetSymbol()] = Count(start->right);
		}
		if (currSymbol == "&&")
			return Count(start->left) * Count(start->right);
		if (currSymbol == "||")
			return Count(start->left) + Count(start->right);
		if (currSymbol == "==") {
			if (Count(start->left) == Count(start->right))
				return 1;
			else
				return 0;
		}
		if (currSymbol == "!=") {
			if (Count(start->left) != Count(start->right))
				return 1;
			else
				return 0;
		}
		if (currSymbol == ">") {
			if (Count(start->left) > Count(start->right))
				return 1;
			else
				return 0;
		}
		if (currSymbol == "<") {
			if (Count(start->left) < Count(start->right))
				return 1;
			else
				return 0;
		}
		if (currSymbol == "<=") {
			if (Count(start->left) <= Count(start->right))
				return 1;
			else
				return 0;
		}
		if (currSymbol == ">=") {
			if (Count(start->left) >= Count(start->right))
				return 1;
			else
				return 0;
		}
		if (currSymbol == "+")
			return Count(start->left) + Count(start->right);
		if (currSymbol == "-")
			return Count(start->left) - Count(start->right);
		if (currSymbol == "*")
			return Count(start->left) * Count(start->right);
		if (currSymbol == "/")
			return Count(start->left) / Count(start->right);
		if (currSymbol == "^")
			return pow(Count(start->left), Count(start->right));
	}
	void AddNode(Node*& ptr, vector<string> tokens) {
		if (tokens.size() == 1) {
			ptr = new Node(tokens[0]);
		}
		else {
			//output(tokens);
			int brackets = 0;
			int ind, prior = 7;
			for (int i = tokens.size() - 1; i >= 0; i--) {
				if (tokens[i] == ")") {
					brackets++;
					continue;
				}
				if (tokens[i] == "(") {
					brackets--;
					continue;
				}
				if (brackets == 0 && prior > 1 && tokens[i] == "||") {
					prior = 1;
					ind = i;
				}
				if (brackets == 0 && prior > 2 && tokens[i] == "&&") {
					prior = 2;
					ind = i;
				}
				if (brackets == 0 && prior > 3 && (tokens[i] == "==" || tokens[i] == "!=" || tokens[i] == ">" || tokens[i] == "<" || tokens[i] == ">=" || tokens[i] == "<=" || tokens[i] == "=")) {
					prior = 3;
					ind = i;
				}
				if (brackets == 0 && prior > 4 && (tokens[i] == "+" || tokens[i] == "-")) {
					prior = 4;
					ind = i;
				}
				if (brackets == 0 && prior > 5 && (tokens[i] == "*" || tokens[i] == "/")) {
					prior = 5;
					ind = i;
				}
				if (brackets == 0 && prior > 6 && tokens[i] == "^") {
					prior = 6;
					ind = i;
				}
			}
			ptr = new Node(tokens[ind]);
			if (head == nullptr)
				head = ptr;
			vector<string> left, right;
			for (int i = 0; i < tokens.size(); i++) {
				if (i < ind)
					left.push_back(tokens[i]);
				if (i > ind)
					right.push_back(tokens[i]);
			}
			bool w = true;
			while (w) {
				if (left[0] == "(" && left[left.size() - 1] == ")") {
					left.pop_back();
					for (int i = 0; i < left.size() - 1; i++)
						left[i] = left[i + 1];
					left.pop_back();
				}
				else
					w = false;
			}
			while (!w) {
				if (right[0] == "(" && right[right.size() - 1] == ")") {
					right.pop_back();
					for (int i = 0; i < right.size() - 1; i++)
						right[i] = right[i + 1];
					right.pop_back();
				}
				else
					w = true;
			}
			tokens.clear();
			AddNode(ptr->left, left);
			AddNode(ptr->right, right);
		}
	}
	Node* GetHead() {
		return head;
	}
	void SetState(bool k) {
		IsState = k;
	}
	bool IsItState() {
		return IsState;
	}
	void Output(Node* start) override {
		start->Output();
	}
};
class IfTree : public Tree {
	Node* head;
	BinTree* Condition;
	StatementList* True;
	StatementList* False;
public:
	IfTree(vector<string> cond, vector<string> yes, vector<string> no) {
		head = nullptr;
		Condition = new BinTree(cond);
		True = new StatementList(yes, 0);
		if (!no.empty()) {
			False = new StatementList(no, 0);
		}
		else False = nullptr;
	}
	float Count(Node* none) override {
		if (Condition->Count(Condition->GetHead()) != 0)
			return True->Count(True->GetHead());
		else if (False)
			return False->Count(False->GetHead());
	}
	void Output(Node* start) override {
		Condition->Output(Condition->GetHead());
		cout << endl << "/*/" << endl;
		if (True)
			True->Output(True->GetHead());
		cout << endl << "/*/" << endl;
		if (False)
			False->Output(False->GetHead());
	}
	Node* GetHead() {
		return head;
	}
};

int main() {
	ifstream input;
	input.open("D:\\Учёба\\Файлы общего доступа\\KOD.txt");
	vector<string> kod;
	kod = readFromFile(input);
	/*for (auto i: kod){
		cout << i << endl << "//////////////////////////////////" << endl;
	}*/
	StatementList Lab(kod, 1);
	Lab.Output(nullptr);
	Lab.Count(nullptr);
	_getch();
}

StatementList::StatementList(vector<string> tokens, bool arg) {
	isCaptain = arg;
	while (!tokens.empty()) {
		string curr = tokens.front();//curr token
		tokens.erase(tokens.begin());

		if (curr == "if") {//poimavsay if
			vector<string> cond, yes, no;
			string ifToken;
			tokens.erase(tokens.begin());//if ( a ) { dkjsasdakasdkll }
			while (ifToken != ")") {
				ifToken = tokens.front();
				tokens.erase(tokens.begin());
				if (ifToken != ")")cond.push_back(ifToken);
			}
			tokens.erase(tokens.begin());
			int count = 1;
			while (ifToken != "}" || count != 0) {//dkjsasdakasdkll }
				ifToken = tokens.front();
				tokens.erase(tokens.begin());
				if (ifToken == "{")count++;
				if (ifToken == "}")count--;
				if (ifToken != "}" || count > 0)yes.push_back(ifToken);
			}
			//string s = tokens.front();

			if (!tokens.empty() && tokens.front() == "else") {//else condition
				while (ifToken != "{") {
					ifToken = tokens.front();
					tokens.erase(tokens.begin());
				}
				ifToken = "";
				while (ifToken != "}") {
					if (ifToken != "")no.push_back(ifToken);
					ifToken = tokens.front();
					tokens.erase(tokens.begin());
				}
			}
			/*output(cond);
			output(yes);
			output(no);*/

			Tree* New = new IfTree(cond, yes, no);
			StTrees.push_back(New);
		}
		else {
			vector<string> rayd;
			while (curr != ";") {
				rayd.push_back(curr);
				curr = tokens.front();
				tokens.erase(tokens.begin());
			}
			Tree* New = new BinTree(rayd);
			StTrees.push_back(New);
		}
	}
}
float StatementList::Count(Node* none) {
	float result;
	for (int i = 0; i < StTrees.size(); i++) {
		if (isCaptain == 1 && i == StTrees.size() - 1) {
			result = StTrees[i]->Count(StTrees[i]->GetHead());
			cout << "Result = " << result << ";";
			return result;
		}
		else
			StTrees[i]->Count(StTrees[i]->GetHead());
	}
	return 0;
}
void StatementList::Output(Node* none) {
	for (int i = 0; i < StTrees.size(); i++) {
		StTrees[i]->Output(StTrees[i]->GetHead());
		cout << endl << "/////////////" << endl;
	}
}
Node* StatementList::GetHead() {
	return nullptr;
}


bool isNumber(string s) {
	for (int i = 0; i < s.size(); i++) {
		if (((int)s[i] < 48 || (int)s[i] > 57) && s[i] != '.' && s[i] != '-')return false;
	}
	return true;
}
float stringToFloat(string s) {
	bool minus = false;
	if (s[0] == '-') {
		minus = true;
		s.erase(0);
	}
	float part1 = 0, part2 = 0;
	int it = 0;
	while (it < s.size() && s[it] != '.') {
		part1 *= 10;
		part1 += (int)s[it] - 48;
		it++;
	}
	if (it != s.size()) {
		for (int i = s.size() - 1; i > it; i--) {
			part2 += (int)s[i] - 48;
			part2 /= 10;
		}
	}
	if(minus)
		return (part1 + part2)*-1;
	return part1 + part2;
}
queue<string> parseToTokens(string s) {
	queue<string> res;
	string token = "";
	bool minus = false;
	for (int i = 0; i < s.size(); i++) {
		string curr = "";
		curr += s[i];
		if (curr == "-" || curr == "+" || curr == "*" || curr == "/" || curr == "^" || curr == "=" || curr == "(" || curr == ")" || curr == " " || curr == "}" || curr == "{" || curr == ";" || curr == ">" || curr == "<" || curr == "|" || curr == "&" || curr == "!") {
			if (token != "") {
				res.push(token);
			}
			if (curr != " ") {
				if (curr == "-" && (res.back() == "-" || res.back() == "+" || res.back() == "*" || res.back() == "/" || res.back() == "=" || res.back() == "^" || res.back() == "&&" || res.back() == "||" || res.back() == "==" || res.back() == "!=" || res.back() == "<" || res.back() == ">" || res.back() == "<=" || res.back() == ">=")) {
					minus = true;
				}
				else
				if (curr == "=" && s[i + 1] == '=') {
					i++;
					res.push("==");
				}
				else if (curr == "&" && s[i + 1] == '&') {
					i++;
					res.push("&&");
				}
				else if (curr == "!" && s[i + 1] == '=') {
					i++;
					res.push("!=");
				}
				else if (curr == "|" && s[i + 1] == '|') {
					i++;
					res.push("||");
				}
				else if (curr == ">") {
					if (s[i + 1] == '=') {
						res.push(">=");
						i++;
					}
					else {
						res.push(">");
					}
				}
				else if (curr == "<") {
					if (s[i + 1] == '=') {
						res.push("<=");
						i++;
					}
					else {
						res.push("<");
					}
				}else res.push(curr);
			}
			token = "";
		}
		else {
			if (minus) {
				minus = false;
				token += "-";
			}
			token += curr;
		}
	}
	res.push(token);
	if (token != ";" && token != "}" && token != "{" && token != "if" && token != "else" && token != ")" && token != "")res.push(";");
	return res;
}
vector<string> readFromFile(ifstream& inp) {
	vector<string> kod;
	queue<string> temp;
	string s;
	while (!inp.eof()) {
		getline(inp, s);
		temp = parseToTokens(s);
		while (!temp.empty()) {
			if (temp.front() != "")kod.push_back(temp.front());
			temp.pop();
		}
	}
	return kod;
}
void output(vector<string> a) {
	for (auto i : a) {
		cout << i << " ";
	}
	cout << endl;
}