#include<iostream>
#include<vector>
#include<string>
#include<map>
using namespace std;
class production {				//文法产生式
public:
	char left;
	string right;
	production(char a, string b) {
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
vector<char>  First[5];			//first集
vector<char>  Follow[5];		//follow集
vector<production> G;			//文法产生式
int anlysisTable[5][8];			//分析表 int表示G中对应文法的下标
map<char, int> nonterminator = {
	{'E',0},
	{'A',1},
	{'T',2},
	{'B',3},
	{'F',4},	
};
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

void init() {
	First[nonterminator['E']].push_back('(');
	First[nonterminator['E']].push_back('n');
	First[nonterminator['A']].push_back('+'); 
	First[nonterminator['A']].push_back('-');
	First[nonterminator['A']].push_back(' ');
	First[nonterminator['T']].push_back('(');
	First[nonterminator['T']].push_back('n');
	First[nonterminator['B']].push_back('*');
	First[nonterminator['B']].push_back('/');
	First[nonterminator['B']].push_back(' ');
	First[nonterminator['F']].push_back('(');
	First[nonterminator['F']].push_back('n');
	Follow[nonterminator['E']].push_back('$');
	Follow[nonterminator['E']].push_back(')');
	Follow[nonterminator['A']].push_back('$');
	Follow[nonterminator['A']].push_back(')');
	Follow[nonterminator['T']].push_back('$');
	Follow[nonterminator['T']].push_back(')');
	Follow[nonterminator['T']].push_back('+');
	Follow[nonterminator['T']].push_back('-');
	Follow[nonterminator['B']].push_back('$');
	Follow[nonterminator['B']].push_back(')');
	Follow[nonterminator['B']].push_back('+');
	Follow[nonterminator['B']].push_back('-');
	Follow[nonterminator['F']].push_back('*');
	Follow[nonterminator['F']].push_back('/');
	Follow[nonterminator['F']].push_back('$');
	Follow[nonterminator['F']].push_back(')');
	Follow[nonterminator['F']].push_back('+');
	Follow[nonterminator['F']].push_back('-');
	G.push_back(production('E', "TA"));
	G.push_back(production('A', "+TA"));
	G.push_back(production('A', "-TA"));
	G.push_back(production('A', " "));
	G.push_back(production('T', "FB"));
	G.push_back(production('B', "*FB"));
	G.push_back(production('B', "/FB"));
	G.push_back(production('B', " "));
	G.push_back(production('F', "(E)"));
	G.push_back(production('F', "num"));
}

vector<char> first(string s) {
	vector<char> result;
	int empty = 1;
	int pos = 0;
	while (empty == 1 && pos < s.size()) {
		empty = 0;
		if (s[pos] == '+' || s[pos] == '-' || s[pos] == '*' || s[pos] == '/' ||
			s[pos] == '(' || s[pos] == ')' || s[pos] == '$' || s[pos] == 'n') {
			result.push_back(s[pos]);
		}
		else {
			int k = nonterminator[s[pos]];
			for (int i = 0; i < First[k].size(); i++)
				if (First[k][i] == ' ')
					empty = 1;
				else
					result.push_back(First[k][i]);

		}
		pos++;
	}
	return result;
}
void fillTable() {
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 8; j++)
			anlysisTable[i][j] = -1;
	for (int i = 0; i < G.size(); i++) {
		int rowNum = nonterminator[G[i].left];
		if (G[i].right == " ") {
			int num = Follow[rowNum].size();		//follow 集符号个数
			for (int j = 0; j < num; j++) {
				int columnNum = terminator[Follow[rowNum][j]];
				anlysisTable[rowNum][columnNum] = i;
			}
		}
		else {
			int num = first(G[i].right).size();
			for (int j = 0; j < num; j++) {
				int columnNum = terminator[first(G[i].right)[j]];
				anlysisTable[rowNum][columnNum] = i;
			}

		}
	}
}
char noToN(int x) {
	switch (x) {
	case 0:return 'E';
	case 1:return 'A';
	case 2:return 'T';
	case 3:return 'B';
	case 4:return 'F';
	}
}
void printTable() {
	cout << "	+	-	*	/	(	)	num	$" << endl;
	for (int i = 0; i < 5; i++) {
		cout << noToN(i) << "	";
		for (int j = 0; j < 8; j++)
			if (anlysisTable[i][j] != -1)
				cout << G[anlysisTable[i][j]] << "	";
			else cout << "	";
		cout << endl;
	}
}
void anlysis(string s) {
	s.push_back('$');
	vector<char> stack;
	stack.push_back('$');
	stack.push_back('E');
	int pos = 0;		//当前输入符号位置
	while (1) {
		if (stack[stack.size() - 1] == s[pos] && s[pos] == '$')
			break;
		else if (stack[stack.size() - 1] == s[pos] || (stack[stack.size() - 1] == 'n'&&isdigit(s[pos]))) {
			stack.pop_back();
			pos++;
		}
		else if ((stack[stack.size() - 1] == '+' || stack[stack.size() - 1] == '-' || stack[stack.size() - 1] == '*' || stack[stack.size() - 1] == '/' ||
			stack[stack.size() - 1] == '(' || stack[stack.size() - 1] == ')' || stack[stack.size() - 1] == '$' || stack[stack.size() - 1] == 'n') && stack[stack.size() - 1] != s[pos]) {
			cout << "error!!!" << endl;
			break;
		}
		else {
			int i = nonterminator[stack[stack.size() - 1]];
			int j;
			if (isdigit(s[pos])) {
				j = terminator['n'];
				pos++;
				while (isdigit(s[pos])) {
					pos++;
				}
				pos--;
			}
			else
				j = terminator[s[pos]];
			if (anlysisTable[i][j] == -1) {
				cout << "error！！！" << endl;
				break;
			}
			cout << G[anlysisTable[i][j]] << endl;
			if (G[anlysisTable[i][j]].right == " ") {
				stack.pop_back();
			}
			else {
				stack.pop_back();
				string temp = (G[anlysisTable[i][j]].right);
				if (temp != "num") {
					reverse(temp.begin(), temp.end());
					for (int k = 0; k < temp.size(); k++)
						stack.push_back(temp[k]);
				}
				else stack.push_back('n');
			}
		}
	}
}
int main(void) {
	init();
	fillTable();
	printTable();
	string s;
	cout << "Please input your string:" << endl;
	cin >> s;
	anlysis(s);
	return 0;
}