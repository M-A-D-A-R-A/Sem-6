//Nishant Andoriya
//201951105

// Importing stdander libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define SIZE 30	  // the size of the input text
#define ROWS 5	  // the size of row of the playfair matrix
#define COLUMNS 5 // the size of columms of the playfair matrix

#define CEASER_KEY 3 // key for Ceaser cypher

#define ALPHA_AFFINE_KEY 7 // alpha key for affine cypher
#define BETA_AFFINE_KEY 5  // beta key for affine cypher

// function for playfair cypher encryptation and decryptation

void toLowerCase(char plain[], int ps);						// function for converting all plain text to lowercase
int removeSpaces(char *plain, int ps);						// function for removing all the spacces for the plain text
void generateKeyTable(char key[], int ks, char keyT[5][5]); // function for create the gide with key + left other letters
void search(char keyT[5][5], char a, char b, int arr[]);	// search letter in grid put column number and row number in to array (arr[])
int mod5(int x, int y);										// function for find modulo operation of a values(specialy for negative values)
int prepare(char str[], int ptrs);							// function to check the lenght of plain text if odd add the char "x"

void encryptByPlayfairstr(char str[], char key[]); // driven function for encrypt array
void encrypt(char str[], char keyT[5][5], int ps); // function for encrypt inputs(character array needs to convert, generated key matrix , converted plain text lenght)

void decryptByPlayfairstr(char str[], char key[]); // driven function for decrypt array
void decrypt(char str[], char keyT[5][5], int ps); // function for decrypt inputs(character array needs to convert, generated key matrix , converted plain text lenght)

// function for Ceaser cypher encryptation and decryptation
void encryptbyCeaser(char *ch, char *res, int key); // function for encrypt inputs by Ceaser Cypher(pointer to  the character needs to convert, converted plain text, key = 3)
void decryptByCeaser(char *ch, char *res, int key); // function for decrypt inputs by Ceaser Cypher (pointer to  the character needs to convert, converted plain text, key = 3)

// function for Affine cypher encryptation and decryptation
void encryptByAffinestr(char str[], int a, int b);		// function for encrypt inputs by Affine Cypher(character array needs to convert, alpha key =7, beta key =5)
void *decryptByAffinefairstr(char str[], int a, int b); // function for decrypt inputs by Affine Cypher(character array needs to convert, alpha key =7, beta key =5)

void toLowerCase(char plain[], int ps)
{
	int i;
	for (i = 0; i < ps; i++)
	{
		if (plain[i] > 64 && plain[i] < 91)
			plain[i] += 32;
	}
}
int removeSpaces(char *plain, int ps)
{
	int i, count = 0;
	for (i = 0; i < ps; i++)
		if (plain[i] != ' ')
			plain[count++] = plain[i];
	plain[count] = '\0';
	return count;
}
void generateKeyTable(char key[], int ks, char keyT[5][5])
{
	int i, j, k, flag = 0, *dicty;
	dicty = (int *)calloc(26, sizeof(int));
	for (i = 0; i < ks; i++)
	{
		if (key[i] != 'j')
			dicty[key[i] - 97] = 2;
	}
	dicty['j' - 97] = 1;
	i = 0;
	j = 0;
	for (k = 0; k < ks; k++)
	{
		if (dicty[key[k] - 97] == 2)
		{
			dicty[key[k] - 97] -= 1;
			keyT[i][j] = key[k];
			j++;
			if (j == 5)
			{
				i++;
				j = 0;
			}
		}
	}
	for (k = 0; k < 26; k++)
	{
		if (dicty[k] == 0)
		{
			keyT[i][j] = (char)(k + 97);
			j++;

			if (j == 5)
			{
				i++;
				j = 0;
			}
		}
	}
}

void search(char keyT[5][5], char a, char b, int arr[])
{
	int i, j;
	if (a == 'j')
		a = 'i';
	else if (b == 'j')
		b = 'i';
	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			if (keyT[i][j] == a)
			{
				arr[0] = i;
				arr[1] = j;
			}
			else if (keyT[i][j] == b)
			{
				arr[2] = i;
				arr[3] = j;
			}
		}
	}
}
// modulo oparation
int mod5(int x, int y)
{
	if (x >= 0)
		return (x) % y;

	return mod5(y + x, y);
}
int prepare(char str[], int ptrs)
{
	if (ptrs % 2 != 0)
	{
		str[ptrs++] = 'x';
		str[ptrs] = '\0';
	}
	return ptrs;
}
void encrypt(char str[], char keyT[5][5], int ps)
{
	int i, a[4];
	for (i = 0; i < ps; i += 2)
	{
		search(keyT, str[i], str[i + 1], a);
		if (a[0] == a[2])
		{
			str[i] = keyT[a[0]][mod5(a[1] + 1, 5)];
			str[i + 1] = keyT[a[0]][mod5(a[3] + 1, 5)];
		}
		else if (a[1] == a[3])
		{
			str[i] = keyT[mod5(a[0] + 1, 5)][a[1]];
			str[i + 1] = keyT[mod5(a[2] + 1, 5)][a[1]];
		}
		else
		{
			str[i] = keyT[a[0]][a[3]];
			str[i + 1] = keyT[a[2]][a[1]];
		}
	}
}
void encryptByPlayfairstr(char str[], char key[])
{
	char ps, ks, keyT[5][5];
	ks = strlen(key);
	ks = removeSpaces(key, ks);
	toLowerCase(key, ks);
	ps = strlen(str);
	toLowerCase(str, ps);
	ps = removeSpaces(str, ps);
	ps = prepare(str, ps);
	generateKeyTable(key, ks, keyT);
	// showMatrix(key, ks, keyT);
	printf("Delta: %s \n", str);
	printf("Generated matrix: \n");
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			printf("%c ", keyT[i][j]);
		}
		printf("\n");
	}
	encrypt(str, keyT, ps);
}
void decrypt(char str[], char keyT[5][5], int ps)
{
	int i, a[4];
	for (i = 0; i < ps; i += 2)
	{
		search(keyT, str[i], str[i + 1], a);
		if (a[0] == a[2])
		{
			str[i] = keyT[a[0]][mod5(a[1] - 1, 5)];
			str[i + 1] = keyT[a[0]][mod5(a[3] - 1, 5)];
		}
		else if (a[1] == a[3])
		{
			str[i] = keyT[mod5(a[0] - 1, 5)][a[1]];
			str[i + 1] = keyT[mod5(a[2] - 1, 5)][a[1]];
		}
		else
		{
			str[i] = keyT[a[0]][a[3]];
			str[i + 1] = keyT[a[2]][a[1]];
		}
	}
}
void decryptByPlayfairstr(char str[], char key[])
{
	char ps, ks, keyT[5][5];
	ks = strlen(key);
	ks = removeSpaces(key, ks);
	toLowerCase(key, ks);
	ps = strlen(str);
	toLowerCase(str, ps);
	ps = removeSpaces(str, ps);
	generateKeyTable(key, ks, keyT);
	decrypt(str, keyT, ps);
}
void encryptbyCeaser(char *ch, char *res, int key)
{

	int i;

	// for loop for convert characters in input char array(encrypt)
	// shift characters by value of key(in casesarstr key = 3)
	for (i = 0; i < strlen(ch); i++)
	{

		// res[i] = (ch[i]+key)mod 26
		// In here im not going to use modulo oparetion
		if (ch[i] >= 'A' && ch[i] <= 'Z')
		{
			// case 1. key + ch[i] > 'Z'
			if (ch[i] + key > 'Z')
				res[i] = 'A' - 1 + ch[i] + key - 'Z'; // formula to get converted char after exceeding 'Z'
			// case 2. key + ch[i] <= 'Z'
			else
				res[i] = ch[i] + key;
		}
		// encrypt lower case letters
		else if (ch[i] >= 'a' && ch[i] <= 'z')
		{
			if (ch[i] + key > 'z')
				res[i] = 'a' - 1 + ch[i] + key - 'z';
			else
				res[i] = ch[i] + key;
		}
		else
		{
			// if character is not in alphabet
			// will not encrypt
			res[i] = ch[i];
		}
	}
	res[i] = '\0';
}

void decryptByCeaser(char *ch, char *res, int key)
{

	int i;

	// for loop for convert characters in input char array(decrypt)
	// shift characters by value of key(in casesarcipher key = 3)
	for (i = 0; i < strlen(ch); i++)
	{
		// res[i] = (ch[i] - key)mod 26
		// In here im not going to use modulo oparetion
		//  (character - key) have 2 cases
		if (ch[i] >= 'A' && ch[i] <= 'Z')
		{
			// case 1. key - character < 'A'
			if (ch[i] - key < 'A')
				res[i] = 'Z' + 1 - ('A' - (ch[i] - key)); // formula to get converted char after exceeding 'A' (char - key)
			// case 2. key - character >= 'A'
			else
				res[i] = ch[i] - key;
		}
		else if (ch[i] >= 'a' && ch[i] <= 'z')
		{
			if (ch[i] - key < 'a')
				res[i] = 'z' + 1 - ('a' - (ch[i] - key));
			else
				res[i] = ch[i] - key;
		}
		else
		{
			res[i] = ch[i];
		}
	}
	res[i] = '\0';
}

void encryptByAffinestr(char str[], int a, int b)
{
	int ch_int;
	char *newtext;
	size_t size = strlen(str);
	newtext = (char *)malloc(sizeof(char) * size);
	for (int i = 0; str[i] != '\0'; ++i)
	{
		newtext = str[i];
		if (newtext >= 'a' && newtext <= 'z')
		{
			ch_int = newtext - 'a';
			ch_int = a * ch_int + b;
			while (ch_int >= 26)
			{
				ch_int = ch_int - 26;
			}
			newtext = ch_int + 'a';
			str[i] = newtext;
		}
		else if (newtext >= 'A' && newtext <= 'Z')
		{
			ch_int = newtext - 'A';
			ch_int = a * ch_int + b;
			while (ch_int >= 26)
			{
				ch_int = ch_int - 26;
			}
			newtext = ch_int + 'A';
			str[i] = newtext;
		}
	}
}

void *decryptByAffinefairstr(char str[], int a, int b)

{
	char *newtext;
	size_t size = strlen(str);
	newtext = (char *)malloc(sizeof(char) * size);
	int a_inv = 0;
	int flag = 0;
	for (int i = 0; i < 26; i++)
	{
		flag = (a * i) % 26;
		if (flag == 1)
		{
			a_inv = i;
		}
	}
	for (int i = 0; str[i] != '\0'; ++i)
	{
		newtext[i] = NULL;
		if (str[i] >= 'a' && str[i] <= 'z')
		{
			newtext[i] = newtext[i] + (char)(((a_inv * ((str[i] - b)) + 1) % 26) + 'a');
		}
		else if (str[i] >= 'A' && str[i] <= 'Z')
		{
			newtext[i] = newtext[i] + (char)(((a_inv * (str[i] - b) + 1) % 26) + 'A');
		}

		str[i] = newtext[i];
	}
}

void main()
{
	char str[SIZE], key[SIZE], astr[SIZE];

	printf("Enter plain text:");
	scanf("%s", &str);
	printf("Enter your key:");
	scanf("%s", &key);
	printf("--------Encrypted------------ \n ");
	encryptByPlayfairstr(str, key);
	printf("PlayFair Cypher encrpyted text with key: %s : %s\n", key, str);
	encryptbyCeaser(str, str, CEASER_KEY);
	printf("Ceaser Cypher encrpyted text with key (3): %s \n", str);
	encryptByAffinestr(str, ALPHA_AFFINE_KEY, BETA_AFFINE_KEY);
	printf("Affine Cypher encrypted text with alpha key(7) and beta key (5): %s \n", str);

	printf("\n --------Decrypting------------ \n ");

	decryptByAffinefairstr(str, ALPHA_AFFINE_KEY, BETA_AFFINE_KEY);
	printf("Affine Cypher decrypted text with alpha key(7) and beta key (5): %s \n", str);

	decryptByCeaser(str, str, CEASER_KEY);
	printf("Ceaser Cypher decrpyted text with key (3): %s \n", str);

	decryptByPlayfairstr(str, key);
	printf("Ceaser Cypher encrpyted text with key %s (Oringal text): %s \n", key, str);
}