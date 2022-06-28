#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctime> // random 


bool print = false;
int ListOfCharacters_Size = 1;
struct ListOfCharacters
{
	char data = NULL;
	int freq = NULL;
	int* code;
	ListOfCharacters* next = NULL, * parent = NULL;
	void addcode(int i, int v) {
		code[i] = v;
	}
}*root = NULL;

struct TreeNode {
	char data;
	TreeNode* left, * right, * parent;
}*node = NULL;

int getfilength(FILE* file) {
	int x;
	fseek(file, 0L, SEEK_END);
	x = ftell(file);
	rewind(file);
	return x;
}
int global_File_Length;

ListOfCharacters* add_character(ListOfCharacters* root, char x) {
	if (!x) return root;
	if (!root) {
		root = (ListOfCharacters*)malloc(sizeof(ListOfCharacters));
		root->next = root->parent = NULL;
		root->data = x;
		root->freq = 1;
		root->code = NULL;
	}
	else if (root->data == x)
		root->freq++;
	else {
		root->next = add_character(root->next, x);
		root->next->parent = root;
	}
	return root;
}

void replace(ListOfCharacters* first, ListOfCharacters* second) {
	if (!second)return;
	ListOfCharacters* temp = (ListOfCharacters*)malloc(sizeof(ListOfCharacters));
	temp->data = first->data;
	temp->freq = first->freq;
	first->data = second->data;
	first->freq = second->freq;
	second->freq = temp->freq;
	second->data = temp->data;
	free(temp);
}

void sort_list(ListOfCharacters* root) {
	ListOfCharacters* main = root, * main2 = root;
	while (main) {
		while (main2)
		{
			if (main->freq > main2->freq)
				replace(main, main2);
			main2 = main2->next;
		}
		main = main->next;
		main2 = main;
	}
}

ListOfCharacters* ListRemoveTop(ListOfCharacters* root) {
	ListOfCharacters* x = root->next;
	//free(root); // later, for the sake of linked list
	return x;
}

int worldcount;

TreeNode* PrepareTree(ListOfCharacters* root, TreeNode* node) {
	//count
	sort_list(root);
	int count = 0;
	while (root) {
		count++;
		root = root->next;
	}
	TreeNode* tmp = NULL;
	worldcount = count;
	while (--count) {
		node = (TreeNode*)malloc(sizeof(TreeNode));
		if (tmp) {
			tmp->right = node;
			node->parent = tmp;
		}
		else
			node->parent = NULL;
		node->data = '*';
		node->left = node->right = NULL;
		tmp = node;
		node = node->right;
	}
	node = (TreeNode*)malloc(sizeof(TreeNode));
	node->parent = tmp;
	tmp->right = node;
	tmp->left = (TreeNode*)malloc(sizeof(TreeNode));
	return tmp;
}

void fillTree(TreeNode* node, ListOfCharacters* root) {
	node->right->data = root->data;
	node->right->left = node->right->right = 0;
	root = ListRemoveTop(root);
	if (root)
		root->parent = NULL;
	node->left->data = root->data;
	node->left->left = node->left->right = 0;
	root = ListRemoveTop(root);
	if (root)
		root->parent = NULL;
	node = node->parent;
	if (!node)return;
	while (node->parent) {
		node->left = (TreeNode*)malloc(sizeof(TreeNode));
		node->left->data = root->data;
		node->left->left = node->left->right = 0;
		root = ListRemoveTop(root);
		root->parent = NULL;
		node = node->parent;
	}
	node->left = (TreeNode*)malloc(sizeof(TreeNode));
	node->left->data = root->data;
	node->left->left = node->left->right = 0;
	ListRemoveTop(root);
}

int** plan;
int* storeT;


void add_to_front(int** arr, int x, int i) {
	int j = 0;
	if (x == 0) {
		while (arr[i][j])
			j++;
		arr[i][j] = x;
	}
	else
	{
		while (i <= worldcount) {
			while (arr[i][j])
				j++;
			arr[i][j] = x;
			j = 0;
			i++;
		}
	}
}

void printplan() {
	for (int i = 0; i < worldcount + 1; i++) {
		for (int j = 0; j < worldcount && plan[i][j] != 253; j++)
			if (print)
				printf("%d ", plan[i][j]);
		if (print)
			printf("\n");
	}
	if (print)
		printf("\n");
}

void printCODE(char** plan) {
	for (int i = 0; i < worldcount + 1; i++) {
		printf("%c ", plan[i][0]);
		for (int j = 1; j < storeT[i]; j++)
			printf("%d ", plan[i][j]);
		printf("\n");
	}
	printf("\n");
}

struct huffman
{
	//char** letter=(char **)malloc(sizeof(char*)*(worldcount+1));
	char* letter;
	//for(int i=0; i<)
	int count = 0;
	void add(char a) {
		letter[count++] = a;
		letter[count] = '\0';
	}

}huffmanX;

int trash = 0;
void planTree(TreeNode* node, int** arr, int i = 0) {
	trash++;
	if (!node->left && !node->right) {
		add_to_front(arr, 0, i);
		huffmanX.add(node->data);
	}
	if (node->left) {
		add_to_front(arr, 0, i);
		planTree(node->left, arr, i);
	}
	if (node->right) {
		add_to_front(arr, 1, ++i);
		planTree(node->right, arr, i);
	}
	printplan();
	return;
}

char** CODE;

void trimPlan(int** arr) {
	int j;
	for (int i = 0; i < worldcount + 1; i++)
	{
		for (j = 0; j < worldcount && arr[i][j]; j++);
		arr[i][j] = '\0'; // for easier tracing / debuging
		arr[i] = (int*)realloc(arr[i], sizeof(int) * (j + 2));
		CODE[i] = (char*)malloc(sizeof(char) * (j + 3));
		storeT[i] = j + 2;
	}
	storeT[worldcount]--;
}

//void printToList(LinkedList * plan) {
//
//	plan->data = (char*)malloc(sizeof(char) * storeT[i]);
//}

void fillcode(ListOfCharacters* notessential) {
	int newi = 0;
	for (int i = 0; i < worldcount + 1; i++) {
		CODE[i][0] = huffmanX.letter[i];
		if (notessential->next)
			notessential->code = (int*)malloc(sizeof(int) * ListOfCharacters_Size++);
		else
			notessential->code = (int*)malloc(sizeof(int) * (ListOfCharacters_Size - 1));
		for (int j = 1; j < storeT[i]; j++)
		{
			CODE[i][j] = plan[i][j - 1];
			notessential->addcode(newi++, plan[i][j - 1]);
		}
		newi = 0;
		notessential = notessential->next;
	}
}

void decrypt();

int main();
int binaryToDecimal(int num);

struct readystack
{
	int sizeint = 10000, count = 0;
	long size = 0, tt = 0;
	int* intarr = (int*)malloc(sizeof(int) * sizeint);
	void resizeint() {
		intarr = (int*)realloc(intarr, (sizeint + 1) * sizeof(int));
		if (intarr == NULL) {
			printf("realloc error, trying to fix the situation");
			sizeint -= 100;
			if (sizeint == 0) main();
			resizeint();
		}
	}

	void addint(int x, bool print) {
		size++;
		intarr[count++] = x;
		if (count == sizeint) {
			conv(print);
			count = 0;
		}
	}
	void conv(bool print) {
		int g = 0;
		FILE* X;
		X = fopen("output.txt", "a");
		if (print) {
			for (int i = 0; i < count; i += 8) {
				int tm = 0, t = 10000000;
				g++;
				if (i - size > 0) {
					for (int j = i; j < i + 8; j++)
					{
						tm += intarr[j] * t;
						t /= 10;
					}
					tt += i - size;
				}
				else {
					for (int j = i; j < i + 8 && j % size == j; j++)
					{
						tm += intarr[j] * t;
						t /= 10;
					}
				}
				tm = binaryToDecimal(tm);
				printf("%c", tm);
				fprintf(X, "%c", tm);
				tt += 8;
				t = 10000000; tm = 0;
				if (g == sizeint) {
					fflush(X);
					g = 0;
				}
			}
		} ///////////// only to skip that if print==true statement everytime.. storage for performance
		else {
			for (int i = 0; i < count; i += 8) {
				int tm = 0, t = 10000000;
				g++;
				if (i - size > 0) {
					for (int j = i; j < i + 8; j++)
					{
						tm += intarr[j] * t;
						t /= 10;
					}
				}
				else {
					for (int j = i; j < i + 8 && j % size == j; j++)
					{
						tm += intarr[j] * t;
						t /= 10;
					}
				}
				tm = binaryToDecimal(tm);
				fprintf(X, "%c", tm);
				tt++;
				t = 10000000; tm = 0;
				if (g == sizeint) {
					fflush(X);
					g = 0;
				}
			}
		}
		fclose(X);
	}
	~readystack() {
		free(intarr);
	}
}ready;

void cleanTree(TreeNode* child) {
	if (child->right)
		cleanTree(child->right);
	if (child->left)
		cleanTree(child->left);
	free(child);
}

void logo() {
	FILE* x = fopen("logo.cpp", "r");
	if (x) {
		srand(time(NULL));
		int r;
		r = rand() % 9;
		char c;
		while (!feof(x)) {
			c = fgetc(x);
			if (c == r + 48 + 1)
				break;
		}
		c = 0;
		while (!feof(x) && c <= '0' + 1 || c > '9' + 1 || c == '8' + 1) {
			printf("%c", c - 1);
			c = fgetc(x);
		}
	}
	else
	{
		printf("");
		printf("                                                     ___\n");
		printf("                                                  ,o88888\n");
		printf("                                               ,o8888888\'\n");
		printf("                         ,:o:o:oooo.        ,8O88Pd8888\"\n");
		printf("                     ,.::.::o:ooooOoOoO. ,oO8O8Pd888'\"\n");
		printf("                   ,.:.::o:ooOoOoOO8O8OOo.8OOPd8O8O\"\n");
		printf("                  , ..:.::o:ooOoOOOO8OOOOo.FdO8O8\"\n");
		printf("                 , ..:.::o:ooOoOO8O888O8O,COCOO\"\n");
		printf("                , . ..:.::o:ooOoOOOO8OOOOCOCO\"\n");
		printf("                 . ..:.::o:ooOoOoOO8O8OCCCC\"o\n");
		printf("                    . ..:.::o:ooooOoCoCCC\"o:o\n");
		printf("                    . ..:.::o:o:,cooooCo\"oo:o:\n");
		printf("                 `   . . ..:.:cocoooo\"'o:o:::'\n");
		printf("                 .`   . ..::ccccoc\"'o:o:o:::'\n");
		printf("                :.:.    ,c:cccc\"':.:.:.:.:.'\n");
		printf("              ..:.:\"'`::::c:\"'..:.:.:.:.:.'\n");
		printf("            ...:.'.:.::::\"'    . . . . .'\n");
		printf("           .. . ....:.\"' `   .  . . ''\n");
		printf("         . . . ....\"'\n");
		printf("         .. . .\"'     -AHMED SAMIR-\n");
		printf("        .         -HUFFMAN COMPRESSION- LOGO file missing!! optional but worths effort\n");
		printf("\n");
		printf("\n");
	}
}

float CR(int input_size, int w) {
	return (float)input_size / ((float)ready.tt + w + 1);
}

int main() {
	//read input
	//header();

	logo();
	printf("print every detail possible y/n ? printing is bad for performance\nbetter print details for small files only\npress e to exit\nso, (y/n/e) _");
	char x = getch();
	if (x == 'e')
		return 0;
	else if (x == 'y' || x == 'p') {
		print = 1;
		printf("\nOkay cool, as u wish");
	}
	else {
		print = 0;
		printf("Not printing, nice choice");
	}
	printf("\n\ncompress or decompress c/d ? \n");
	x = getch();
	if (x == 'c') {
		printf("\nprocessing..");
		FILE* input = fopen("input.txt", "rb");
		char x;
		global_File_Length = getfilength(input);
		while (global_File_Length--)
		{
			fscanf(input, "%c", &x);
			root = add_character(root, x);
			x = NULL;
		}

		node = PrepareTree(root, node);
		fillTree(node, root);

		//clean root manually

		worldcount--;

		huffmanX.letter = (char*)malloc(sizeof(char) * (worldcount + 2));

		plan = (int**)malloc(sizeof(int*) * (worldcount + 1));
		for (int i = 0; i < worldcount + 1; i++)
			plan[i] = (int*)malloc(sizeof(int) * (worldcount));

		for (int i = 0; i < worldcount + 1; i++)
			for (int j = 0; j < worldcount; j++)
				plan[i][j] = 0;

		ready.resizeint();
		printplan();

		while (node->parent)
			node = node->parent;

		planTree(node, plan);

		CODE = (char**)malloc(sizeof(char*) * (worldcount + 1));

		storeT = (int*)malloc(sizeof(int) * (worldcount + 1));

		trimPlan(plan);

		printplan();

		fillcode(root);

		ListOfCharacters* kh;
		while (root) {
			kh = root;
			free(root->code);
			root = root->next;
			free(kh);
		}

		if (print)
			printCODE(CODE);
		////////////////////////////////
		/////////////////////////////
		////////////////////////////
		//CODE IS READY
		fclose(input);

		input = fopen("input.txt", "rb");

		FILE* X = fopen("output.txt", "w");
		fclose(X); // clean files 'D

		char newchar;
		int counter = 0;
		int input_size;
		input_size = global_File_Length = getfilength(input);
		if (print)
			while (global_File_Length--) {
				newchar = fgetc(input);
				if (newchar == 0)
					break;
				int i;
				for (i = 0; i < worldcount; i++) {
					if (CODE[i][0] == newchar)
						break;
				}
				for (int j = 1; j < storeT[i]; j++) {
					printf("%d", CODE[i][j]);
					counter++;
					ready.addint(CODE[i][j], print);
				}
				newchar = 0;
			}
		else
			while (global_File_Length--) {
				newchar = fgetc(input);
				if (newchar == 0)
					break;
				int i;
				for (i = 0; i < worldcount; i++) {
					if (CODE[i][0] == newchar)
						break;
				}
				for (int j = 1; j < storeT[i]; j++) {
					counter++;
					ready.addint(CODE[i][j], print);
				}
				newchar = 0;
			}
		if (print)
			printf("\n%d\n", counter);
		FILE* k = fopen("codes.dat", "w");
		fprintf(k, "%c", 8 - (counter % 8));
		ready.conv(print);
		fclose(input);

		// done compressing
		//fputc(ready.tt%8, k);
		int w = 0;
		if (print)
			for (int i = 0; i < worldcount + 1; i++)
			{
				for (int j = 0; j < storeT[i]; j++)
				{
					printf("%c", CODE[i][j]);
					fprintf(k, "%c", CODE[i][j]);
					w++;
				}
			}
		else
			for (int i = 0; i < worldcount + 1; i++)
			{
				for (int j = 0; j < storeT[i]; j++)
				{
					fprintf(k, "%c", CODE[i][j]);
					w++;
				}
			}
		//done storing code
		fclose(k);

		printf("\n\n\n\ncompression ratio is (input/output) %f, a value higher than 1 means compression was useful. However, the higher, the better\n*output includes output.txt & codes.dat files*\n", CR(input_size,w));

		cleanTree(node);
		//decompressing
		//FILE* d = fopen("output.txt", "r");
		//free(k);

		for (int i = 0; i < worldcount + 1; i++) {
			free(CODE[i]);
			free(plan[i]);
		}
		free(CODE);
		free(storeT);
		free(plan);

		printf("\nCOMPRESSING COMPLETED\n\n.");
		main();
	}
	else if (x == 'd')
	{
		decrypt();
		printf("\n\n\nDECOMPRESSING COMPLETED\n\n.");
		main();
	}
	else {
		main();
	}
	return 0;
}

int binaryToDecimal(int num) {
	int binary_val, decimal_val = 0, base = 1, rem;
	binary_val = num;
	while (num > 0)
	{
		rem = num % 10;
		decimal_val = decimal_val + rem * base;
		num = num / 10;
		base = base * 2;
	}
	return decimal_val;
}

int DecimalToBinary(int n) {
	bool flag = false;
	if (n < 0) {
		n += 128;
		flag = true;
	}
	int bin = 0;
	int rem, i = 1, step = 1;

	while (n != 0) {
		rem = n % 2;
		n /= 2;
		bin += rem * i;
		i *= 10;
	}
	if (flag)
		bin += 10000000;
	return bin;
}
int globalsize = 2;
struct decryptool {
	char Letter;
	char* code;
	//char code[128];
	int loc = 0;
	decryptool* next = NULL, * parent = NULL;
	void NewLetter(char x) {
		Letter = x;
		code = (char*)malloc(sizeof(char) * globalsize++);
		decryptCode->loc = 0;
		decryptCode->next = NULL;
	}
	void NewCode(int x) {
		//code=(char*)realloc(code, sizeof(char) * (this->size++));
		code[loc++] = x + 48;
		code[loc] = '\0';
	}
	void removelastCode() {
		code[loc - 1] = '\0';
	}
}*decryptCode;

struct test
{
	int loc = 0;
	char* code = (char*)malloc(sizeof(char) * worldcount);
	void add(char x) {
		code[loc++] = x;
		code[loc] = '\0';
	}
	void reset() {
		loc = 0;
	}
}comp;

bool comparestrings(char* a, char* b) {
	//length
	int count_a = 0, count_b = 0;
	for (int i = 0; a[i] != '\0'; i++)
	{
		count_a++;
	}
	for (int i = 0; b[i] != '\0'; i++)
	{
		count_b++;
	}
	if (count_a != count_b) return false;
	//chars
	for (int i = 0; i < count_a; i++)
	{
		if (a[i] != b[i])
			return false;
	}
	return true;
}

bool m = false;

bool compare(decryptool* x, test cmp) {
	if (strcmp(x->code, cmp.code) == 0) {
		printf("%c", x->Letter);
		m = false;
		return true;
	}
	else {
		while (x->parent) {
			x = x->parent;
			return compare(x, cmp);
		}
	}
	return m;
}

void cleandecryptool(decryptool* start) {
	if (start->next)
		cleandecryptool(start->next);
	free(start->code);
	free(start);
}

void decrypt() {
	printf("\n\n\n\n");
	FILE* input = fopen("output.txt", "rb");
	FILE* code = fopen("codes.dat", "rb");
	char x;
	decryptool* start=NULL;
	decryptCode = (decryptool*)malloc(sizeof(decryptool));
	start = decryptCode;
	int minus = fgetc(code);
	fscanf(code, "%c", &x);
	decryptCode->NewLetter(x);
	decryptCode->loc = 0;
	//decryptCode->code = (char*)malloc(sizeof(char));
	decryptCode->next = decryptCode->parent = NULL;
	while (!feof(code)) {
		fscanf(code, "%c", &x);
		if (x != 0 && x != 1) {
			//printf("%c", x);
			decryptCode->next = (decryptool*)malloc(sizeof(decryptool));
			decryptCode->next->parent = decryptCode;
			decryptCode = decryptCode->next;
			decryptCode->NewLetter(x);
			decryptCode->next = NULL;
		}
		else {
			//printf("%d", x);
			decryptCode->NewCode(x);
		}
		x = NULL;
	}
	decryptCode->removelastCode();
	//printf("\b ");
	//printf("\n");
	FILE* k = fopen(".tmp", "w");
	int g = 0;
	global_File_Length = getfilength(input);
	while (global_File_Length-- && !ferror(input)) {
		g++;
		fscanf(input, "%c", &x);
		//printf("%08d", DecimalToBinary(x));
		fprintf(k, "%08d", DecimalToBinary(x));
		if (g == 100000) {
			fflush(k);
			g = 0;
		}
	}

	fclose(k);
	FILE* j = fopen(".tmp", "rb");
	char xf = 0;
	if (minus == 8) minus = 0;
	minus = getfilength(j) - minus;
	while (minus--) {
		fscanf(j, "%c", &xf);

		if (xf == 48 || xf == 49) {
			comp.add(xf);
			if (compare(decryptCode, comp)) // printing decompressed
				comp.reset();
		}
		if (minus == 1)
			continue;
	}

	while (start) {
		decryptool* x = start;
		free(start->code);
		if (start->next)
			start = start->next;
		else
			start = NULL;
		free(x);
	}
	fclose(j);
	fclose(input);
	fclose(code);
	printf("\n");
	j = fopen(".tmp", "w");
	fclose(j);
	printf("\n\npress any key to continue ...");
	getch();
}