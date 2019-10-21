// CPSC3770 Programming Assignment 
// Zachary Nelson
// 001196795
// This program is a working interpretation of DES

#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <vector>
#include<cmath>
#include <sstream>
#include<stdexcept>

using namespace std;

void Encode();
void Decode();

string InFile, OutFile;
ifstream file;
ofstream oFile;

//Defining the tables for the permutations and sboxes
int initialPerm[64] =
{ 58, 50, 42, 34, 26, 18, 10, 2,
60, 52, 44, 36, 28, 20, 12, 4,
62, 54, 46, 38, 30, 22, 14, 6,
64, 56, 48, 40, 32, 24, 16, 8,
57, 49, 41, 33, 25, 17, 9, 1,
59, 51, 43, 35, 27, 19, 11, 3,
61, 53, 45, 37, 29, 21, 13, 5,
63, 55, 47, 39, 31, 23, 15, 7 };

int invereseInitialPerm[64] =
{ 40, 8, 48, 16, 56, 24, 64, 32,
39, 7, 47, 15, 55, 23, 63, 31,
38, 6, 46, 14, 54, 22, 62, 30,
37, 5, 45, 13, 53, 21, 61, 29,
36, 4, 44, 12, 52, 20, 60, 28,
35, 3, 43, 11, 51, 19, 59, 27,
34, 2, 42, 10, 50, 18, 58, 26,
33, 1, 41, 9, 49, 17, 57, 25 };

int expansionPerm[48] =
{ 32, 1, 2, 3, 4, 5,
4, 5, 6, 7, 8, 9,
8, 9, 10, 11, 12, 13,
12, 13, 14, 15, 16, 17,
16, 17, 18, 19, 20, 21,
20, 21, 22, 23, 24, 25,
24, 25, 26, 27, 28, 29,
28, 29, 30, 31, 32, 1 };

int permFunc[32] =
{ 16, 7, 20, 21, 29, 12, 28, 17,
1, 15, 23, 26, 5, 18, 31, 10,
2, 8, 24, 14, 32, 27, 3, 9,
19, 13, 30, 6, 22, 11, 4, 25 };

int permChoice1[56] =
{ 57, 49, 41, 33, 25, 17, 9,
1, 58, 50, 42, 34, 26, 18,
10, 2, 59, 51, 43, 35, 27,
19, 11, 3, 60, 52, 44, 36,
63, 55, 47, 39, 31, 23, 15,
7, 62, 54, 46, 38, 30, 22,
14, 6, 61, 53, 45, 37, 29,
21, 13, 5, 28, 20, 12, 4 };

int permChoice2[48] =
{ 14, 17, 11, 24, 1, 5, 3, 28,
15, 6, 21, 10, 23, 19, 12, 4,
26, 8, 16, 7, 27, 20, 13, 2,
41, 52, 31, 37, 47, 55, 30, 40,
51, 45, 33, 48, 44, 49, 39, 56,
34, 53, 46, 42, 50, 36, 29, 32 };

int sBox1[64] =
{ 14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13 };

int sBox2[64] =
{ 15, 1, 8, 14, 16, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9 };

int sBox3[64] =
{ 10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12 };

int sBox4[64] =
{ 7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14 };

int sBox5[64] =
{ 2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3 };

int sBox6[64] =
{ 12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13 };

int sBox7[64] =
{ 4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12 };

int sBox8[64] =
{ 13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11 };

//Function to left rotate the bits
void leftRotate(string& s, int d)
{
	reverse(s.begin(), s.begin() + d);
	reverse(s.begin() + d, s.end());
	reverse(s.begin(), s.end());
}

//Function that generates the keys
vector<string> generateKeys(string s) {
	vector<string> keys;
	string s2 = "";
	string L, R;

	//Permutated choice 1
	for (int i = 0; i < 56; i++) {
		s2 = s2 + s[permChoice1[i]-1];
	}

	int j = 0;
	//Doing the permutated choice2 for 16 rounds
	for (int i = 0; i < 16; i++) {

		L = s2.substr(0, 28); 
		R = s2.substr(28, 28);

		switch(i) {
		case 0: j = 1; break;
		case 1: j = 1; break;
		case 2: j = 2; break;
		case 3: j = 2; break;
		case 4: j = 2; break;
		case 5: j = 2; break;
		case 6: j = 2; break;
		case 7: j = 2; break;
		case 8: j = 1; break;
		case 9: j = 2; break;
		case 10: j = 2; break;
		case 11: j = 2; break;
		case 12: j = 2; break;
		case 13: j = 2; break;
		case 14: j = 2; break;
		case 15: j = 1; break;
		}

		leftRotate(L,j);
		leftRotate(R, j);

		s2 = L + R;

		s = "";
		//Permutatated Choice 2
		for (int j = 0; j < 48; j++) {
			s += s2[permChoice2[j]-1];
		}

		//Adding this to the key
		keys.push_back(s);
	}
	
	//Return the vector of Keys
	return keys;

}

//Converts Text string to a string in Binary
string TextToBinaryString(string words) {
	string binaryString = "";
	for (char& _char : words) {
		binaryString += bitset<8>(_char).to_string();
	}
	return binaryString;
}

//Converts a Hex string to a string in Binary
string HexToBinaryString(string words)
{
	string fString = "";
	for (int i = 0; i < words.length(); ++i)
	{
		switch (toupper(words[i]))
		{
		case '0': fString.append("0000"); break;
		case '1': fString.append("0001"); break;
		case '2': fString.append("0010"); break;
		case '3': fString.append("0011"); break;
		case '4': fString.append("0100"); break;
		case '5': fString.append("0101"); break;
		case '6': fString.append("0110"); break;
		case '7': fString.append("0111"); break;
		case '8': fString.append("1000"); break;
		case '9': fString.append("1001"); break;
		case 'A': fString.append("1010"); break;
		case 'B': fString.append("1011"); break;
		case 'C': fString.append("1100"); break;
		case 'D': fString.append("1101"); break;
		case 'E': fString.append("1110"); break;
		case 'F': fString.append("1111"); break;
		}
	}
	return fString;
}

//Main Function
int main()
{
	char choice;

	//Main Menu
	cout << "DES Program" << endl;
	cout << "-----------" << endl;
	cout << "1: Encode" << endl;
	cout << "2: Decode" << endl;
	cout << endl;
	cout << "Please Enter Choice: ";
	cin >> choice;

	if (choice == '1')
		Encode();
	else
		Decode();

}

int binaryToInt(string s) {
	//string bin_string = "10101010";
	int number = 0;

	number = stoi(s, 0, 2);

	return number;


}

//String to Hex
string string_to_hex(const string& input)
{
	static const char* const lut = "0123456789ABCDEF";
	size_t len = input.length();

	std::string output;
	output.reserve(2 * len);
	for (size_t i = 0; i < len; ++i)
	{
		const unsigned char c = input[i];
		output.push_back(lut[c >> 4]);
		output.push_back(lut[c & 15]);
	}
	return output;
}
//////////////////////////////

string performRound(string s, string key) {
	string L, R, Placeholder, newR;

	//Separate into Left and Right
	L = s.substr(0, 32);
	R = s.substr(32, 32);

	//Left half becomes Right
	Placeholder = L;
	L = R;

	//Expanding Right
	for (int i = 0; i < 48; i++) {
		newR += R[expansionPerm[i]-1];
	}
	R = "";

	//XORing
	for (int i = 0; i < 48; i++) {
		if (newR[i] != key[i])
			R += "1";
		else
			R += "0";
	}

	//S Boxing
	int rowNum, colNum;
	string rowNumS, colNumS, final;

	//Sbox 1
	rowNumS = R.substr(0, 1) + R.substr(5, 1);
	colNumS = R.substr(1, 4);
	rowNum = binaryToInt(rowNumS);
	colNum = binaryToInt(colNumS);
	bitset<4> x(sBox1[rowNum * 16 + colNum]);
	final += x.to_string();

	//Sbox 2
	rowNumS = R.substr(6, 1) + R.substr(11, 1);
	colNumS = R.substr(7, 4);
	rowNum = binaryToInt(rowNumS);
	colNum = binaryToInt(colNumS);
	bitset<4> x2(sBox2[rowNum * 16 + colNum ]);
	final += x2.to_string();

	//Sbox 3
	rowNumS = R.substr(12, 1) + R.substr(17, 1);
	colNumS = R.substr(13, 4);
	rowNum = binaryToInt(rowNumS);
	colNum = binaryToInt(colNumS);
	bitset<4> x3(sBox3[rowNum * 16 + colNum ]);
	final += x3.to_string();

	//Sbox 4
	rowNumS = R.substr(18, 1) + R.substr(23, 1);
	colNumS = R.substr(19, 4);
	rowNum = binaryToInt(rowNumS);
	colNum = binaryToInt(colNumS);
	bitset<4> x4(sBox4[rowNum * 16 + colNum]);
	final += x4.to_string();

	//Sbox 5
	rowNumS = R.substr(24, 1) + R.substr(29, 1);
	colNumS = R.substr(25, 4);
	rowNum = binaryToInt(rowNumS);
	colNum = binaryToInt(colNumS);
	bitset<4> x5(sBox5[rowNum * 16 + colNum]);
	final += x5.to_string();

	//Sbox 6
	rowNumS = R.substr(30, 1) + R.substr(35, 1);
	colNumS = R.substr(31, 4);
	rowNum = binaryToInt(rowNumS);
	colNum = binaryToInt(colNumS);
	bitset<4> x6(sBox6[rowNum * 16 + colNum]);
	final += x6.to_string();

	//Sbox 7
	rowNumS = R.substr(36, 1) + R.substr(41, 1);
	colNumS = R.substr(37, 4);
	rowNum = binaryToInt(rowNumS);
	colNum = binaryToInt(colNumS);
	bitset<4> x7(sBox7[rowNum * 16 + colNum]);
	final += x7.to_string();

	//Sbox 8
	rowNumS = R.substr(42, 1) + R.substr(47, 1);
	colNumS = R.substr(43, 4);
	rowNum = binaryToInt(rowNumS);
	colNum = binaryToInt(colNumS);
	bitset<4> x8(sBox8[rowNum * 16 + colNum]);
	final += x8.to_string();

	//Permutation P
	R = "";
	for (int i = 0; i < 32; i++) {
		R += final[(permFunc[i] - 1)];
	}

	//XORing
	final = R;

	R = "";

	for (int i = 0; i < 32; i++) {
		if (final[i] != Placeholder[i])
			R += "1";
		else
			R += "0";
	}

	final = L + R;
	return final;

}

//Function that converts a string in binary format to a text string
string BinaryStringToText(string binaryString) {
	string text = "";
	stringstream sstream(binaryString);
	while (sstream.good())
	{
		bitset<8> bits;
		sstream >> bits;
		text += char(bits.to_ulong());
	}
	return text;
}

//Function that converts a string in Hex format to a text string
string HexStringToString(string c) {

	int length = c.length();
	string newString;
	for (int i = 0; i < length; i += 2)
	{
		string byte = c.substr(i, 2);
		char chr = (char)(int)strtol(byte.c_str(), NULL, 16);
		newString.push_back(chr);
	}
	return newString;
}

//Function that converts a string in binary format to a string in hex format
string bintohex(const string& s) {
	string out;
	for (int i = 0; i < s.size(); i += 4) {
		int8_t n = 0;
		for (int j = i; j < i + 4; ++j) {
			n <<= 1;
			if (s[j] == '1')
				n |= 1;
		}

		if (n <= 9)
			out.push_back('0' + n);
		else
			out.push_back('A' + n - 10);
	}

	return out;
}

//Encoding Function
void Encode() {

	string OriginalLine;
	string BinString;
	string HexKey, Key;

	char checker = 'N';

	//Opening File
	cout << "Encode Selected. Please enter file name for plaintext to encode: " << endl;
	cin >> InFile;
	file.open(InFile);
	cout << "Opened " << InFile << endl;

	OutFile = "Output" + InFile;
	oFile.open(OutFile);
	//File Opened
	
	//Inputting the Key
	cout << "Please enter the 16 Digit Hexadecimal Key" << endl;
	do {

		cin >> HexKey;

		//Make sure the key is 12 digits
		if (HexKey.length() > 16) {
			cout << "Error. Please enter a 12 Digit Hexadecimal Key." << endl;
			checker = 'N';
		}
		if (HexKey.length() <= 16)
			checker = 'Y';
	}while (checker == 'N');
	
	//Converting the key to binary
	Key = HexToBinaryString(HexKey);	

	//Generating the round keys
	vector<string> roundKeys;
	roundKeys = generateKeys(Key);

	string WorkingLine;
	//Loops until the end of the file
	do{
		//Gets the next line of the file
		getline(file, OriginalLine);
		OriginalLine = string_to_hex(OriginalLine);

		int charOn = 0;
		//Separates into 8 characters each string, since each char is 8 bits, 8 chars x 8 bits = 64 bits.

		while(charOn < OriginalLine.length()){

			WorkingLine = OriginalLine.substr(charOn, 16);
			charOn += 16;

			BinString = HexToBinaryString(WorkingLine);


			//If the string is less than 64 chars, just add 0s to fill out the rest of the line.
			while (BinString.size() < 64) {
				BinString = '0' + BinString;;
			}

			string InitialPerm = "";
			for (int i = 0; i < 64; i++) {
				InitialPerm += BinString[(initialPerm[i] - 1)];
			}
			BinString = InitialPerm;

			//Performing the 16 rounds
			for (int i = 0; i < 16; i++) {
				BinString = performRound(BinString, roundKeys[i]);

			}

			string placeholding = BinString.substr(0,32);
			BinString = BinString.substr(32, 32) + placeholding;

			placeholding = "";
			for (int i = 0; i < 64; i++) {
				placeholding += BinString[(invereseInitialPerm[i] - 1)];
			}

			BinString = placeholding;
			
			oFile << bintohex(BinString);
		}
	} while (!file.eof());
	cout << "Encoding complete. You can find the output at: " << OutFile << endl;
}

//Decoding Function
void Decode() {

	string OriginalLine;
	string BinString;
	string HexKey, Key;

	char checker = 'N';

	//Opening File
	cout << "Encode Selected. Please enter file name for plaintext to encode: " << endl;
	cin >> InFile;
	file.open(InFile);
	cout << "Opened " << InFile << endl;

	OutFile = "Output" + InFile;
	oFile.open(OutFile);
	//File Opened

	//Inputting the Key
	cout << "Please enter the 16 Digit Hexadecimal Key" << endl;
	do {

		cin >> HexKey;

		//Make sure the key is 12 digits
		if (HexKey.length() > 16) {
			cout << "Error. Please enter a 12 Digit Hexadecimal Key." << endl;
			checker = 'N';
		}
		if (HexKey.length() <= 16)
			checker = 'Y';
	} while (checker == 'N');

	//Converting the key to binary
	Key = HexToBinaryString(HexKey);

	//Generating the round keys
	vector<string> roundKeys;
	roundKeys = generateKeys(Key);

	string WorkingLine;
	//Loops until the end of the file
	do {
		//Gets the next line of the file
		getline(file, OriginalLine);

		int charOn = 0;
		//Separates into 8 characters each string, since each char is 8 bits, 8 chars x 8 bits = 64 bits.

		while (charOn < OriginalLine.length()) {

			WorkingLine = OriginalLine.substr(charOn, 16);
			charOn += 16;

			BinString = HexToBinaryString(WorkingLine);

			//If the string is less than 64 chars, just add 0s to fill out the rest of the line.
			while (BinString.size() < 64) {
				BinString = '0' + BinString;;
			}

			string InitialPerm = "";
			for (int i = 0; i < 64; i++) {
				InitialPerm += BinString[(initialPerm[i] - 1)];
			}
			BinString = InitialPerm;

			//Performing the 16 rounds
			for (int i = 0; i < 16; i++) {
				BinString = performRound(BinString, roundKeys[15 - i]);
			}

			string placeholding = BinString.substr(0, 32);
			BinString = BinString.substr(32, 32) + placeholding;

			placeholding = "";
			for (int i = 0; i < 64; i++) {
				placeholding += BinString[(invereseInitialPerm[i] - 1)];
			}

			BinString = placeholding;

			oFile << HexStringToString(bintohex(BinString));
		}
	} while (!file.eof());
	cout << "Decoding complete. You can find the file at " << OutFile << endl;
}
