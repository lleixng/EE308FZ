#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

// function
void SwitchCounter();
int IfCounter(int begin);
void Pretreatment(string text);

// global
vector<int> case_num;// The num of "case" in Switch structure
vector<string> key1;// Store keyword text
static int switch_num = 0;// The num of switch
static int key_num = 0;// The num of keyword
static int if_else_num = 0;// The num of "if_else"
static int if_elseif_else_num = 0;// The num of "if_elseif_else"

int main() {
	// Input the path
	string path;
	int level;
	cout << "Please enter the path of file: " << endl;
	cin >> path;
	cout << "Please enter the level you need: " << endl;
	cin >> level;
	if (level < 1 || level > 4) {
		cout << "The level must be 0-4" << endl;
		return 0;
	}
	//Code to open the file
	bool f = true;
	string ch;
	string text;// Used to hold the text read into the file
	ifstream file(path);
	if (!file.is_open()) {
		cout << "File opening failed!" << endl;
		f = false;
	}
	while (!file.eof()) {
		ch = file.get();
		text.append(ch);
	}
	//  cout<< text<<endl;
	file.close();
	if (f) {
		Pretreatment(text);
		int i = 0;
		while (1) {
			i = IfCounter(i);
			if (i == (int)key1.size()) break;
		}
		// Four situation to four level
		if (level >= 1)
			cout << "total num: " << key_num << endl;
		if (level >= 2) {
			SwitchCounter();
		}
		if (level >= 3) {
			cout << "if-else num: " << if_else_num << endl;
		}
		if (level == 4) {
			cout << "if-elseif-else num: " << if_elseif_else_num << endl;
		}
	} else {
		cout << "Failed to open file!" << endl;
	}
	return 0;
}

//Preprocessing text
void Pretreatment(string text) {
	int flag;
	string key;
	string key_string[32] = { "auto", "break", "case", "char", "const", "continue", "default", "do",
	                          "double", "else", "enum", "extern", "float", "for", "goto", "if", "int",
							  "long", "register", "return", "short", "signed","sizeof", "static", "struct",
							  "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
	                        };
	for (int i = 0; i < (int)text.length(); i++) {
		char c = text[i];
		if (c == '\n' || c == '\t' || c == ' ' || c == '-' || c == '<' || c == '>'
		    || c == '(' || c == ')' || c == ':' || c == ';'
		    || c == '#' || c == '=' || c == '!' || c == '?') {
			flag = 0;
		} else if (c == '{' || c == '}') {
			flag = 1;
		} else {
			flag = 2;
		}
		if (i + 1 <= (int)text.length() && text[i] == '/' && text[i + 1] == '/') {
			// Determines whether the text is a comment
			for (int j = i + 2; j < (int)text.length() && text[j] != '\n'; j++) {
				i = j;
			}// Skip comment lines
		} else if (i + 1 <= (int)text.length() && text[i] == '"') {
			// Check whether the text is in double quotation marks
			for (int j = i + 1; j < (int)text.length() && text[j] != '"'; j++) {
				i = j;
			}// Skip the text in double quotes
		} else if (flag == 2) {
			// In the key
			key.push_back(text[i]);
		} else if (flag == 1) {
			string s(1, text[i]);
			key1.push_back(s);
		} else {
			if (key.length() > 1) {
				for (int i = 0; i < 32; i++) {
					if (key == key_string[i]) {
						key_num++;
						key1.push_back(key);
					}
				}
			}
			key.clear();
		}
	}
}

// Switch and case Counter
void SwitchCounter() {
	int case_num1 = 0;
	for (int i = 0; i < (int)key1.size(); i++) {
		if (key1[i] == "switch") {
			switch_num++;
			case_num1 = 0;
			for (int j = i + 1; j < (int)key1.size(); j++) {
				if (key1[j] == "switch") {
					break;
				} else {
					if (key1[j] == "case") {
						case_num1++;
					}
				}
			}
			case_num.push_back(case_num1);
		}
	}
	cout << "switch num: " << switch_num << endl;
	cout << "case num: ";
	for (int i = 0; i < (int)case_num.size(); i++) {
		cout << case_num[i] << " ";
	}
	cout << endl;
}

// If, If else, If elseif else Counter
int IfCounter(int i) {
	for (; i < (int)key1.size(); i++) {
		if (key1[i] == "if") {
			int j;
			int flag = 0;//0 as "if", 1 as "else", 2 as "if，else if", 3 as "if，else if，else"
			for (j = i + 1; j < (int)key1.size(); j++) {
				if (key1[j] == "else" && key1[j + 1] == "if") {
					if (flag == 0) {
						flag = 2;
					}
					j++;
				} else if (key1[j] == "else") {
					if (flag == 0) {
						if_else_num++;
					} else if (flag == 2) {
						if_elseif_else_num++;
					}
					break;
				} else if (key1[j] == "if") {
					j = IfCounter(j);
				}
			}
			return j;
		}
	}
	return i;
}

