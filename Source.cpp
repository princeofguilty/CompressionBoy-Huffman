#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ListOfCharacters
{
	char data = NULL;
	int freq = NULL;
	ListOfCharacters* next = NULL, * parent = NULL;
}*root = NULL;

struct TreeNode {
	char data;
	TreeNode* left, * right, * parent;
}*node = NULL;

ListOfCharacters* add_character(ListOfCharacters* root, char x) {
	if (!x) return root;
	if (!root) {
		root = (ListOfCharacters*)malloc(sizeof(ListOfCharacters));
		root->parent = root->next = NULL;
		root->data = x;
		root->freq = 1;
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
	free(root);
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
	node->left->data = root->data;
	node->left->left = node->left->right = 0;
	root = ListRemoveTop(root);
	node = node->parent;
	if (!node)return;
	while (node->parent) {
		node->left = (TreeNode*)malloc(sizeof(TreeNode));
		node->left->data = root->data;
		node->left->left = node->left->right = 0;
		root = ListRemoveTop(root);
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
			printf("%d ", plan[i][j]);
		printf("\n");
	}
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
	char* letter = (char*)malloc(sizeof(char) * (worldcount + 1));
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
		arr[i]=(int*)realloc(arr[i], sizeof(int) * (j + 1));
		CODE[i] = (char*)malloc(sizeof(char) * (j + 3));
		storeT[i] = j + 2;
	}
	storeT[worldcount]--;
}

void fillcode() {
	for (int i = 0; i < worldcount + 1; i++) {
		CODE[i][0] = huffmanX.letter[i];
		for (int j = 1; j < storeT[i]; j++)
		{
			CODE[i][j] = plan[i][j - 1];
		}
	}
}

struct readystack
{
	int sizeint = 100, count = 0,size=0;
	int* intarr = (int*)malloc(sizeof(int) * sizeint);
	char* chararr;// = (char*)malloc(sizeof(char) * size);

	void resizeint() {
		intarr = (int*)realloc(intarr, sizeint * sizeof(int));
		//while (!x) {
		//	x = (int*)realloc(intarr, size * sizeof(int));
		//}
		//intarr = x;
	}
	void resizechar() {
		chararr=(char*)realloc(chararr, sizeint * sizeof(char));
	}

	void addint(int x) {
		if (count == sizeint) {
			sizeint += 100;
			resizeint();
		}
		size++;
		intarr[count++] = x;
	}
	void conv() {
		FILE* X = fopen("output.txt", "w");
		for (int i = 0; i < size; i += 8) {
			int tm = 0, t = 10000000;
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
			printf("%c", tm);
			fprintf(X, "%c", tm);
			t = 10000000; tm = 0;
		}
	}
}ready;

int main() {
	//read input
	FILE* input = fopen("input.txt", "r");
	char x;
	while (!feof(input))
	{
		fscanf(input, "%c", &x);
		root = add_character(root, x);
		x = NULL;
	}
	// reading complete
	// lets sort
	// test sort
	replace(root->next, root->next->next);
	node = PrepareTree(root, node);
	fillTree(node, root);
	worldcount--;
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

	storeT = (int*)malloc(sizeof(int) * worldcount + 1);

	trimPlan(plan);

	printplan();

	fillcode();
	printCODE(CODE);
	////////////////////////////////
	/////////////////////////////
	////////////////////////////
	//CODE IS READY
	fclose(input);

	FILE * reinput = fopen("input.txt", "r");

	char newchar;
	int counter = 0;
	while (!feof(reinput)) {
		fscanf(reinput, "%c", &newchar);
		if (newchar == 0)
			break;
		int i;
		for (i = 0; i < worldcount ; i++) {
			if (CODE[i][0] == newchar)
				break;
		}
		for (int j = 1; j < storeT[i]; j++) {
			printf("%d", CODE[i][j]);
			counter++;
			ready.addint(CODE[i][j]);
		}
		newchar = 0;
	}
	printf("\n%d\n", counter);
	ready.conv();
	fclose(reinput);

	// done compressing
	FILE* k = fopen("code.txt", "w");
	for (int i = 0; i < worldcount + 1; i++)
	{
		for (int j = 0; j < storeT[i]; j++)
		{
			printf("%c", CODE[i][j]);
			fprintf(k, "%c", CODE[i][j]);
		}
	}
	//done storing code
	fclose(k);
	free(root);
	free(node);
	//decompressing
	//FILE* d = fopen("output.txt", "r");
	return 0;
}
