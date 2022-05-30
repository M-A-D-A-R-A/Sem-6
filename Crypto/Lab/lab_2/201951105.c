/*

    Nishant Andoriya
    201951105
    You need to create a "plaintext.txt" and "key.txt" to read the plain-text and the key from those files.
    S-Boxes Refernces :- http://www.facweb.iitkgp.ac.in/~sourav/DES.pdf
*/

// Importing stdander libraries

#include <stdio.h>
#include <stdlib.h>

/* Initial Permutation */
int IP[64] =
    {
        58, 50, 42, 34, 26, 18, 10, 2,
        60, 52, 44, 36, 28, 20, 12, 4,
        62, 54, 46, 38, 30, 22, 14, 6,
        64, 56, 48, 40, 32, 24, 16, 8,
        57, 49, 41, 33, 25, 17, 9, 1,
        59, 51, 43, 35, 27, 19, 11, 3,
        61, 53, 45, 37, 29, 21, 13, 5,
        63, 55, 47, 39, 31, 23, 15, 7};

/* Permutation (Inverse Initaial Permutation, IP^-1)*/
int IP_inverse[64] =
    {
        40, 8, 48, 16, 56, 24, 64, 32,
        39, 7, 47, 15, 55, 23, 63, 31,
        38, 6, 46, 14, 54, 22, 62, 30,
        37, 5, 45, 13, 53, 21, 61, 29,
        36, 4, 44, 12, 52, 20, 60, 28,
        35, 3, 43, 11, 51, 19, 59, 27,
        34, 2, 42, 10, 50, 18, 58, 26,
        33, 1, 41, 9, 49, 17, 57, 25};

/* Expand Permutaion, Expand 32bit(right block) -> 48 bit */
int Expand_Permutation[48] =
    {
        32, 1, 2, 3, 4, 5,
        4, 5, 6, 7, 8, 9,
        8, 9, 10, 11, 12, 13,
        12, 13, 14, 15, 16, 17,
        16, 17, 18, 19, 20, 21,
        20, 21, 22, 23, 24, 25,
        24, 25, 26, 27, 28, 29,
        28, 29, 30, 31, 32, 1};

/* Permutation after S-box */
int P[32] =
    {
        16, 7, 20, 21,
        29, 12, 28, 17,
        1, 15, 23, 26,
        5, 18, 31, 10,
        2, 8, 24, 14,
        32, 27, 3, 9,
        19, 13, 30, 6,
        22, 11, 4, 25};

/* 8 S-box, each S-box map 6-to-4 bits */
int S_BOX[8][4][16] = {
    {{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
     {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
     {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
     {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}},
    {{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
     {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
     {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
     {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}},
    {{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
     {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
     {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
     {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}},
    {{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
     {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
     {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
     {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}},
    {{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
     {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
     {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
     {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}},
    {{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
     {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
     {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
     {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}},
    {{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
     {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
     {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
     {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}},
    {{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
     {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
     {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
     {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}}};

/* Key - Permutation Choice One */
int PC1[56] =
    {
        57, 49, 41, 33, 25, 17, 9,
        1, 58, 50, 42, 34, 26, 18,
        10, 2, 59, 51, 43, 35, 27,
        19, 11, 3, 60, 52, 44, 36,
        63, 55, 47, 39, 31, 23, 15,
        7, 62, 54, 46, 38, 30, 22,
        14, 6, 61, 53, 45, 37, 29,
        21, 13, 5, 28, 20, 12, 4};

/* Key - Permutation Choice two */
int PC2[48] =
    {
        14, 17, 11, 24, 1, 5,
        3, 28, 15, 6, 21, 10,
        23, 19, 12, 4, 26, 8,
        16, 7, 27, 20, 13, 2,
        41, 52, 31, 37, 47, 55,
        30, 40, 51, 45, 33, 48,
        44, 49, 39, 56, 34, 53,
        46, 42, 50, 36, 29, 32};

/* Schedule of Left Circular Shifts */
int l_shifts[] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

int key64bit[64];        // store key
int key56bit[56];        // store key after PC1
int subkey48bit[16][48]; // 16 subkey used by each round

void create16Keys();                             // function to create 16 keys from key.txt
void key64to56(int key64[]);                     // function to change the key for the first perumtation i.e *PC 1*
void key56to48(int key56[], int round);          // function to change the key for the Second perumtation i.e *PC 2*
void leftCicularShift(int bits_28[], int shift); // function to left shit the given array

void readPlainText(int plaintext[]);  // function to Read the plaintext for encrypting from the "plaintext.txt" file
void readCipherText(int plaintext[]); // function to Reading the cypher text for decrypting from the "ciphertext.txt" file

int *encrypt(int plaintext[]);  // function to encrpyt using DES
int *decrypt(int ciphertext[]); // function to decrpyt useing DES

void f(int rightBlock[], int subkey48bit[]); // final function  output
int XOR(int a, int b);                       // XOR of int a and int b

int *translate_decimal_TO_bit(int value);               // function to chnage decimal to bits
int *translate_hex_To_4bit(char ch);                    // function to change hexadecimal to 4 bits
void printBinaryBit(int *BitSequence, int size);        // print the binary bit of the given size
void printBinaryToHex(int *BitSequence, int size);      // print the binary to hex of given size
int *translate_bits_To_hex(int *BitSequence, int size); // function to chnage bits to hexadecimal

void readPlainText(int plaintext[])
{

    printf("**** Plaintext ****\n");
    FILE *fp = fopen("plaintext.txt", "rb"); // plaintext.txt format -> Ex. 02468aceeca86420 (hex).

    int index = 0;
    char ch;

    while ((ch = getc(fp)) != EOF)
    {
        int num_decimal;
        // translate char(hex format) to 4bit(binary format)
        int *fourBits_pointer = translate_hex_To_4bit(ch);
        for (int i = 0; i < 4; i++)
            plaintext[index++] = fourBits_pointer[i]; // store in bit format
    }
    // print plaintext in binary and hex
    printf("plaintext 64bit(binary): ");
    printBinaryBit(plaintext, 64);
    printf("plaintext 64bit(hex): ");
    printBinaryToHex(plaintext, 64);
}

void readCipherText(int ciphertext[])
{
    printf("**** Plaintext ****\n");
    FILE *fp = fopen("ciphertext.txt", "rb"); // plaintext.txt format -> Ex. 02468aceeca86420 (hex).

    int index = 0;
    char ch;

    while ((ch = getc(fp)) != EOF)
    {
        int num_decimal;
        // translate char(hex format) to 4bit(binary format)
        int *fourBits_pointer = translate_hex_To_4bit(ch);
        for (int i = 0; i < 4; i++)
            ciphertext[index++] = fourBits_pointer[i]; // store in bit format
    }
    // print plaintext in binary and hex
    printf("ciphertext 64bit(binary): ");
    printBinaryBit(ciphertext, 64);
    printf("ciphertext 64bit(hex): ");
    printBinaryToHex(ciphertext, 64);
}

void create16Keys()
{
    printf("**** Create 16 keys ****\n");

    FILE *fp = fopen("key.txt", "rb"); // open key file

    int index = 0;
    char ch;
    /*** read key file , key.txt format-> Ex. 0f1571c947d9e859 (hex), total 64bit. ***/
    while ((ch = getc(fp)) != EOF)
    {
        int num_decimal;
        // translate char(hex format) to 4bit(binary format)
        int *fourBits_pointer = translate_hex_To_4bit(ch);
        for (int i = 0; i < 4; i++)
            key64bit[index++] = fourBits_pointer[i]; // store in bit format
        free(fourBits_pointer);
    }
    // print Key in binary and hex
    printf("Key 64bit(binary): ");
    printBinaryBit(key64bit, 64);
    printf("Key 64bit(hex): ");
    printBinaryToHex(key64bit, 64);

    /*** map 64-to-56 bits by PC1, store in global variable key56bit[56] ***/
    key64to56(key64bit);
    // print result
    printf("Key 56bit(binary): ");
    printBinaryBit(key56bit, 56);
    printf("Key 56bit(hex): ");
    printBinaryToHex(key56bit, 56);

    /*** 16 round generate 16 subkey, each round map 56-to-48 bits by PC2 ***/
    // preprocess
    int left28[28];
    int right28[28];
    int realkey56[56];
    for (int i = 0; i < 28; i++) // first 28 bits
        left28[i] = key56bit[i];
    for (int i = 28; i < 56; i++) // last 28 bits
        right28[i - 28] = key56bit[i];
    for (int round = 0; round < 16; round++)
    {
        /** left circular shift each round **/

        // left circular shift
        leftCicularShift(left28, l_shifts[round]);
        leftCicularShift(right28, l_shifts[round]);
        // print each round result
        printf("-- Round %d --\n", round + 1);
        printf("After Cicular Shift leftBlock 28bit(binary):");
        printBinaryBit(left28, 28);
        printf("After Cicular Shift rightBlock 28bit(binary):", round);
        printBinaryBit(right28, 28);

        /** generate subkey each round by PC2 **/

        // combine left 28 bit and right 28bit
        for (int i = 0; i < 28; i++)
            realkey56[i] = left28[i];
        for (int i = 28; i < 56; i++)
            realkey56[i] = right28[i - 28];

        key56to48(realkey56, round); // generation subkey each round, subkey are stored in {subkey48bit} array
        // print all subkey
        printf("Generation subkey in this round:");
        printBinaryBit(subkey48bit[round], 48);
    }
}

// encrypt plaintext to ciphertext
int *encrypt(int plaintext[])
{
    printf("\n\n**** Encrypt ****\n");
    int *ciphertext = malloc(64 * sizeof(int));
    int currentBits[64];
    int leftBlock[32];
    int rightBlock[32];
    int newLeftBlock[32]; // tempBlock when rightBlock assign to leftBlock

    /*** Step1 : initial permutation, IP ***/
    for (int i = 0; i < 64; i++)
        currentBits[i] = plaintext[IP[i] - 1];
    // print result after IP
    printf("Result After Initial Permutation(IP) : ");
    printBinaryBit(currentBits, 64);

    /*** Step2 : divide 64bit to two 32bits block(leftBlock and rightBlock) ***/
    for (int i = 0; i < 32; i++)
        leftBlock[i] = currentBits[i];
    for (int i = 32; i < 64; i++)
        rightBlock[i - 32] = currentBits[i];

    printf("\ninital leftblock : ");
    printBinaryToHex(leftBlock, 32);
    printf("inital rightblock : ");
    printBinaryToHex(rightBlock, 32);
    /*** Step3 : Execute 16 round(f function) */
    for (int round = 0; round < 16; round++)
    {
        for (int i = 0; i < 32; i++)
            newLeftBlock[i] = rightBlock[i]; // copy rightBlock to tempBlock(it will be new leftblock)

        f(rightBlock, subkey48bit[round]); // update rightBlock
        for (int i = 0; i < 32; i++)
            rightBlock[i] = XOR(leftBlock[i], rightBlock[i]); // XOR each bit

        for (int i = 0; i < 32; i++)
            leftBlock[i] = newLeftBlock[i]; // copy tempBlock to leftBlock for following execuation

        // print leftBlock and rightBlock result each round
        printf("\nRound %d --\n", round + 1);
        printf("Key used in this round : ");
        printBinaryToHex(subkey48bit[round], 48);
        printf("new leftblock : ");
        printBinaryToHex(leftBlock, 32);
        printf("new rightblock : ");
        printBinaryToHex(rightBlock, 32);
    }

    /*** Step4 :　Conbine leftBlock and rightBlock to 64bit Ciphertext; Note: L16R16 -> R16L16 ***/
    // reuse currentBits array to store and conbine 'R16,L16'
    for (int i = 0; i < 32; i++)
        currentBits[i] = rightBlock[i];
    for (int i = 32; i < 64; i++)
        currentBits[i] = leftBlock[i - 32];

    /*** Ste5 : Inverse initial permutation IP^-1*/
    for (int i = 0; i < 64; i++)
        ciphertext[i] = currentBits[IP_inverse[i] - 1];

    // print ciphertext
    printf("ciphertext : ");
    printBinaryToHex(ciphertext, 64);
    return ciphertext;
}

int *decrypt(int ciphertext[])
{
    printf("\n\n**** Decrypt ****\n");
    int *plaintext = malloc(64 * sizeof(int));
    int currentBits[64];
    int leftBlock[32];
    int rightBlock[32];
    int newLeftBlock[32]; // tempBlock when rightBlock assign to leftBlock

    /*** Step1 : initial permutation, IP ***/
    for (int i = 0; i < 64; i++)
        currentBits[i] = ciphertext[IP[i] - 1];
    // print result after IP
    printf("Result After Initial Permutation(IP) : ");
    printBinaryBit(currentBits, 64);

    /*** Step2 : divide 64bit to two 32bits block(leftBlock and rightBlock) ***/
    for (int i = 0; i < 32; i++)
        leftBlock[i] = currentBits[i];
    for (int i = 32; i < 64; i++)
        rightBlock[i - 32] = currentBits[i];

    printf("\ninital leftblock : ");
    printBinaryToHex(leftBlock, 32);
    printf("inital rightblock : ");
    printBinaryToHex(rightBlock, 32);
    /*** Step3 : Execute 16 round(f function) */
    for (int round = 0; round < 16; round++)
    {
        for (int i = 0; i < 32; i++)
            newLeftBlock[i] = rightBlock[i]; // copy rightBlock to tempBlock(it will be new leftblock)

        f(rightBlock, subkey48bit[15 - round]); // update rightBlock *****Note : the order of subkey used in each round is the reverse of encrytion
        for (int i = 0; i < 32; i++)
            rightBlock[i] = XOR(leftBlock[i], rightBlock[i]); // XOR each bit

        for (int i = 0; i < 32; i++)
            leftBlock[i] = newLeftBlock[i]; // copy tempBlock to leftBlock for following execuation

        // print leftBlock and rightBlock result each round
        printf("\nRound %d --\n", round + 1);
        printf("Key used in this round : ");
        printBinaryToHex(subkey48bit[round], 48);
        printf("new leftblock : ");
        printBinaryToHex(leftBlock, 32);
        printf("new rightblock : ");
        printBinaryToHex(rightBlock, 32);
    }

    /*** Step4 :　Conbine leftBlock and rightBlock to 64bit Ciphertext; Note: L16R16 -> R16L16 ***/
    // reuse currentBits array to store and conbine 'R16,L16'
    for (int i = 0; i < 32; i++)
        currentBits[i] = rightBlock[i];
    for (int i = 32; i < 64; i++)
        currentBits[i] = leftBlock[i - 32];

    /*** Ste5 : Inverse initial permutation IP^-1*/
    for (int i = 0; i < 64; i++)
        ciphertext[i] = currentBits[IP_inverse[i] - 1];

    // print ciphertext
    printf("plaintext : ");
    printBinaryToHex(ciphertext, 64);
    return ciphertext;
}
void f(int rightBlock[], int subkey48bit[])
{
    int ExpansionRightBlock[48];
    /*** Step1 : Expansion/Permutation 32bit -> 48bit ***/
    for (int i = 0; i < 48; i++)
        ExpansionRightBlock[i] = rightBlock[Expand_Permutation[i] - 1];

    /*** Step2 : XOR(ExpansionRightBlock, key) ***/
    for (int i = 0; i < 48; i++)
        ExpansionRightBlock[i] = XOR(ExpansionRightBlock[i], subkey48bit[i]);

    /*** Step3 : S-box ***/
    int After_SBOX_translate_bit32[32];
    int index = 0;
    for (int i = 0; i < 48; i += 6)
    {
        int row = ExpansionRightBlock[i] * 2 + ExpansionRightBlock[i + 5];
        int col = ExpansionRightBlock[i + 1] * 8 + ExpansionRightBlock[i + 2] * 4 + ExpansionRightBlock[i + 3] * 2 + ExpansionRightBlock[i + 4];
        int findout_num = S_BOX[i / 6][row][col];
        // translate num to binary;
        for (int j = 0; j < 4; j++) // maximum findout_num value is 15(4 bit)
        {
            After_SBOX_translate_bit32[index + 3 - j] = findout_num % 2;
            findout_num /= 2;
        }
        index += 4;
    }

    /*** Step4 : Permutation, 32 -> 32 bit, >>>This output will be f function final output, so assign to rightBlock<<< ***/
    for (int i = 0; i < 32; i++)
        rightBlock[i] = After_SBOX_translate_bit32[P[i] - 1]; // update rightBlock After final permutation
}

int XOR(int a, int b)
{
    return (a ^ b);
}

void key64to56(int key64[])
{
    /* PC1 */
    for (int i = 0; i < 56; i++)
    {
        key56bit[i] = key64[PC1[i] - 1]; // The table range is  1 ~ 64, but key array index range is 0 ~ 63, so do PC1[i] - 1
    }
}

void key56to48(int key56[], int round)
{
    /* PC2 */
    for (int i = 0; i < 48; i++)
    {
        subkey48bit[round][i] = key56[PC2[i] - 1];
    }
}

void leftCicularShift(int bits_28[], int shift)
{
    int AfterShift_bits_28[28];
    for (int i = 0; i < 28; i++)
    {
        // i is represent index of new bits array
        // When shift amount is 1, if i = 27, let new[27] = old[0]
        // When shift amount is 2, if i = 26, let new[26] = old[0];if i = 27, let new[27] = old[1]
        if (i + shift >= 28)
            AfterShift_bits_28[i] = bits_28[(i + shift) % 28];
        else
            AfterShift_bits_28[i] = bits_28[i + shift];
    }

    // copy back to old array
    for (int i = 0; i < 28; i++)
        bits_28[i] = AfterShift_bits_28[i];
}

int *translate_hex_To_4bit(char ch)
{
    int *fourBits_pointer = malloc(4 * sizeof(int)); // four int to store 4 bit
    int digit = 0;
    if ((ch - 48) <= 9)
        digit = (int)ch - 48;
    else if (ch == 'a' || ch == 'A')
        digit = 10;
    else if (ch == 'b' || ch == 'B')
        digit = 11;
    else if (ch == 'c' || ch == 'C')
        digit = 12;
    else if (ch == 'd' || ch == 'D')
        digit = 13;
    else if (ch == 'e' || ch == 'E')
        digit = 14;
    else if (ch == 'f' || ch == 'F')
        digit = 15;

    for (int i = 0; i < 4; i++)
    {
        fourBits_pointer[3 - i] = digit % 2;
        digit = digit / 2;
        // Ex: if digit is 13, then fourBits_pointer => 1101
    }

    return fourBits_pointer;
}

void printBinaryBit(int *BitSequence, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (i % 4 == 0)
            printf(" ");
        printf("%d", BitSequence[i]);
    }
    printf("\n");
}

void printBinaryToHex(int *BitSequence, int size)
{
    int temp = 0;
    for (int progess = 0; progess < size; progess += 4)
    {
        temp = 0;
        temp += BitSequence[progess] * 8;
        temp += BitSequence[progess + 1] * 4;
        temp += BitSequence[progess + 2] * 2;
        temp += BitSequence[progess + 3] * 1;
        printf("%x", temp);
    }
    printf("\n");
}

// key.txt format-> 0f1571c947d9e859 (hex) 16 * 4ibt = 64bit
int main()
{
    int choice;
    printf("Choose (1)Encryption or (2)Decryption : ");
    scanf("%d", &choice);

    if (choice == 1)
    {
        /**** Encrypt Plaintext ****/
        int plaintext[64];
        printf("\n\nReading 'plaintext.txt' now...\n");
        readPlainText(plaintext); // read plaintext
        printf("\n\nGenerating all keys...\n");
        create16Keys();                       // generate all key used in each round
        int *ciphertext = encrypt(plaintext); // encrypt

        /** write out ciphertext to file **/
        printf("\n\nWriting ciphertext to 'cihpertext.txt' now...\n");
        FILE *fp = fopen("ciphertext.txt", "w");
        int temp = 0;
        for (int progess = 0; progess < 64; progess += 4)
        {
            temp = 0;
            temp += ciphertext[progess] * 8;
            temp += ciphertext[progess + 1] * 4;
            temp += ciphertext[progess + 2] * 2;
            temp += ciphertext[progess + 3] * 1;
            fprintf(fp, "%x", temp);
        }
        printf("Already write ciphertext to file 'ciphertext.txt'!");
        free(ciphertext);
    }
    else if (choice == 2)
    {
        /**** Decrypt ciphertext ****/
        int ciphertext[64];
        printf("\n\nReading 'ciphertext.txt' now...\n");
        readCipherText(ciphertext); // read plaintext
        printf("\n\nGenerating all keys...\n");
        create16Keys();                       // generate all key used in each round
        int *plaintext = decrypt(ciphertext); // dncrypt

        /** write out plaintext to file **/
        printf("\n\nWriting plaintext to 'decrypted_plaintext.txt' now...\n");
        FILE *fp = fopen("decrypted_plaintext.txt", "w");
        int temp = 0;
        for (int progess = 0; progess < 64; progess += 4)
        {
            temp = 0;
            temp += plaintext[progess] * 8;
            temp += plaintext[progess + 1] * 4;
            temp += plaintext[progess + 2] * 2;
            temp += plaintext[progess + 3] * 1;
            fprintf(fp, "%x", temp);
        }
        printf("Already write ciphertext to file 'decrypted_plaintext.txt'!");
        free(plaintext);
    }
    else
    {
        printf("No this option!");
    }
}
