#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<stack>
using namespace std;
class production {
public:
	char left;
	string right;
	production(char a=' ', string b=" ") {
		left = a;
		right = b;
	}
	~production() {
	}
};
ostream& operator << (ostream &os, production& p) {
	if (p.right != " ")
		os << p.left << "->" << p.right;
	else
		os << p.left << "->ε";
	return os;
}
struct node {
	char type='n';
	int num=0;
};
node action[16][8];			//action表
int goTo[16][3];			//goto表
production G[9];			//文法产生式
map<char, int> terminator = {
	{'+',0},
	{'-',1},
	{'*',2},
	{'/',3},
	{'(',4},
	{')',5},
	{'n',6},
	{'$',7},
};
map<char, int> nonterminator = {
	{'E',0},
	{'T',1},
	{'F',2},
};
void init() {
	G[0] = production('S', "E");
	G[1] = production('E', "E+T");
	G[2] = production('E', "E-T");
	G[3] = production('E', "T");
	G[4] = production('T', "T*F");
	G[5] = production('T', "T/F");
	G[6] = production('T', "F");
	G[7] = production('F', "(E)");
	G[8] = production('F', "num");
	action[0][terminator['(']] = { 'S',4 };
	action[0][terminator['n']] = { 'S',5 };
	goTo[0][nonterminator['E']] = 1;
	goTo[0][nonterminator['T']] = 2;
	goTo[0][nonterminator['F']] = 3;
	action[1][terminator['+']] = { 'S',6 };
	action[1][terminator['-']] = { 'S',7 };
	action[1][terminator['$']] = { 'A' };
	action[2][terminator['+']] = { 'R',3 };
	action[2][terminator['-']] = { 'R',3 };
	action[2][terminator[')']] = { 'R',3 };
	action[2][terminator['$']] = { 'R',3 };
	action[2][terminator['*']] = { 'S',8 };
	action[2][terminator['/']] = { 'S',9 };
	action[3][terminator['+']] = { 'R',6 };
	action[3][terminator['-']] = { 'R',6 };
	action[3][terminator['*']] = { 'R',6 };
	action[3][terminator['/']] = { 'R',6 };
	action[3][terminator[')']] = { 'R',6 };
	action[3][terminator['$']] = { 'R',6 };
	action[4][terminator['(']] = { 'S',4 };
	action[4][terminator['n']] = { 'S',5 };
	goTo[4][nonterminator['E']] = 10;
	goTo[4][nonterminator['T']] = 2;
	goTo[4][nonterminator['F']] = 3;
	action[5][terminator['+']] = { 'R',8 };
	action[5][terminator['-']] = { 'R',8 };
	action[5][terminator['*']] = { 'R',8 };
	action[5][terminator['/']] = { 'R',8 };
	action[5][terminator[')']] = { 'R',8 };
	action[5][terminator['$']] = { 'R',8 };
	action[6][terminator['(']] = { 'S',4 };
	action[6][terminator['n']] = { 'S',5 };
	goTo[6][nonterminator['T']] = 11;
	goTo[6][nonterminator['F']] = 3;
	action[7][terminator['(']] = { 'S',4 };
	action[7][terminator['n']] = { 'S',5 };
	goTo[7][nonterminator['T']] = 12;
	goTo[7][nonterminator['F']] = 3;
	action[8][terminator['(']] = { 'S',4 };
	action[8][terminator['n']] = { 'S',5 };
	goTo[8][nonterminator['F']] = 13;
	action[9][terminator['(']] = { 'S',4 };
	action[9][terminator['n']] = { 'S',5 };
	goTo[9][nonterminator['F']] = 14;
	action[10][terminator['+']] = { 'S',6 };
	action[10][terminator['-']] = { 'S',7 };
	action[10][terminator[')']] = { 'S',15 };
	action[11][terminator['+']] = { 'R',1 };
	action[11][terminator['-']] = { 'R',1 };
	action[11][terminator['*']] = { 'S',8 };
	action[11][terminator['/']] = { 'S',9 };
	action[11][terminator[')']] = { 'R',1 };
	action[11][terminator['$']] = { 'R',1 };
	action[12][terminator['+']] = { 'R',2 };
	action[12][terminator['-']] = { 'R',2 };
	action[12][terminator['*']] = { 'S',8 };
	action[12][terminator['/']] = { 'S',9 };
	action[12][terminator[')']] = { 'R',2 };
	action[12][terminator['$']] = { 'R',2 };
	action[13][terminator['+']] = { 'R',4 };
	action[13][terminator['-']] = { 'R',4 };
	action[13][terminator['*']] = { 'R',4 };
	action[13][terminator['/']] = { 'R',4 };
	action[13][terminator[')']] = { 'R',4 };
	action[13][terminator['$']] = { 'R',4 };
	action[14][terminator['+']] = { 'R',5 };
	action[14][terminator['-']] = { 'R',5 };
	action[14][terminator['*']] = { 'R',5 };
	action[14][terminator['/']] = { 'R',5 };
	action[14][terminator[')']] = { 'R',5 };
	action[14][terminator['$']] = { 'R',5 };
	action[15][terminator['+']] = { 'R',7 };
	action[15][terminator['-']] = { 'R',7 };
	action[15][terminator['*']] = { 'R',7 };
	action[15][terminator['/']] = { 'R',7 };
	action[15][terminator[')']] = { 'R',7 };
	action[15][terminator['$']] = { 'R',7 };
}
void analysis(string s) {
	s.push_back('$');
	int pos = 0;
	stack<int> state;
	state.push(0);
	stack<char>symbol;
	while (1) {
		if (isdigit(s[pos])) {
			pos++;
			while (isdigit(s[pos])) {
				pos++;
			}
			pos--;
			s[pos] = 'n';
		}
		if (action[state.top()][terminator[s[pos]]].type == 'A') {
			break;
		}
		else if (action[state.top()][terminator[s[pos]]].type =='S') {
			state.push(action[state.top()][terminator[s[pos]]].num);
			symbol.push(s[pos]);
			pos++;
		}
		else if (action[state.top()][terminator[s[pos]]].type == 'R') {
			cout << G[action[state.top()][terminator[s[pos]]].num] << endl;
			production temp(G[action[state.top()][terminator[s[pos]]].num]);
			int count = temp.right.size();	
			if (temp.right == "num")
				count = 1;
			for (int i = 0; i < count; i++) {
				symbol.pop();
				state.pop();
			}
			symbol.push(temp.left);
			state.push(goTo[state.top()][nonterminator[symbol.top()]]);

		}
		else {
			cout << "error!!!" << endl;
			break;
		}
	}
	
}
int main(void) {
	init();
	string s;
	cout << "Please input your string:" << endl;
	cin >> s;
	analysis(s);
	return 0;
}