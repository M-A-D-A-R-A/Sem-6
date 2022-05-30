/*

    Nishant Andoriya
    201951105
    CS-364: Introduction to Cryptography and Network Security LAB
*/

// Importing stdander libraries
#include <stdint.h>
#include <assert.h>
#include <string.h>

#define N_ROUNDS 10 // Defining Total Number of Rounds for Aes

#define MULT_X(n) ((n)&0x80 ? ((n) << 1) ^ 0x1b : ((n) << 1)) // multiplication by x in GF(2^8)

void string2hexString(char *input, char *output); // function to convert a string into hex string

static const uint8_t rcon[11] = {0, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};

static uint8_t round_keys[176];

// AES Subbyte S-box
static const uint8_t sbox[256] = {
    0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
    0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
    0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
    0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
    0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
    0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
    0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
    0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
    0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
    0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
    0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
    0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
    0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
    0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
    0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
    0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16};

// AES Subbyte  Inverse S-box
static const uint8_t inv_sbox[256] =
    {
        0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
        0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
        0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
        0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
        0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
        0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
        0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
        0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
        0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
        0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
        0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
        0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
        0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
        0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
        0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
        0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D};

static uint8_t char_value(const char c);                           // function to give value of Charracter 'c'
static void key_schedule(const uint8_t *key, uint8_t *round_keys); // function to schedule keys for a given key

void aes128_ecb_encrypt(const uint8_t *plaintext, const uint8_t *key, uint8_t *ciphertext);   // function to encrpyt using AES
void aes128_ecb_decrypt(uint8_t *plaintext, const uint8_t *key, const uint8_t *ciphertext);   // function to decrpyt using AES

void aes128_string_parse(const char *string, uint8_t *data);                                  // parsing the string in data pointer.


// Driver code
int main(int argc, const char *argv[])
{
    uint8_t key[17];
    uint8_t plaintext[17];     // 16 byte char input for the AES
    uint8_t ciphertext[17];    // 16 byte char encrypted output for the AES
    uint8_t decrpyttext[17];   // 16 byte char decrypted output for the AES
    uint8_t m1[17];            // char input for m1
    uint8_t hex_m1[17];        // hex output for m1
    uint8_t m2[17];            // char input for m2
    uint8_t hex_m2[17];        // hex output for m2
    uint8_t arbitary_x[17];    // char input for arbitary X
    uint8_t arbitary_prex[17]; // hex output for arbitary X

    printf("Enter a 16 byte char string as input for this program\n");
    int x = 0, j = 0, y = 0;
    // Question 1 : Your code will take input m1, m2 ∈ {0, 1}^128 and print h(m1||m2).
    printf("Enter a the value of m1: ");
    scanf("%[^\n]s", m1);

    for (int i = 0; i < strlen(m1); i++)
    {
        sprintf(hex_m1 + j, "%02X", m1[i]);
        j += 2;
    }

    hex_m1[j] = '\0';

    printf("Enter a the value of m2: ");
    scanf("\n%[^\n]s", m2);
    for (int i = 0; i < strlen(m2); i++)
    {
        sprintf(hex_m2 + x, "%02X", m2[i]);
        x += 2;
    }

    hex_m2[x] = '\0';
    printf("value of m1 || m2 : %s||%s \n", hex_m1, hex_m2);
    
    // first parse the key and plaintext strings into an array of bytes
    aes128_string_parse(hex_m2, key);
    aes128_string_parse(hex_m1, plaintext);

    // encrypting the text (m1) with the key (m2) to get C1
    aes128_ecb_encrypt(plaintext, key, ciphertext);

    printf("h{m1||m2}: = C1 = ");

    int i;
    for (i = 0; i < 16; i++)
        printf("%02x", ciphertext[i]);

    // Question 2: Implement a second pre-image (m1`||m2`) ∈ {0, 1}^ 256 finding process for h corresponding to any random input (m1||m2) ∈ {0, 1}^256

    printf("\nEnter a the value of m2`: ");
    scanf("\n%[^\n]s", arbitary_x);

    for (int i = 0; i < strlen(arbitary_x); i++)
    {
        sprintf(arbitary_prex + y, "%02X", arbitary_x[i]);
        y += 2;
    }

    arbitary_x[y] = '\0';

    printf("value of m2`: %s (%s) \n", arbitary_prex, arbitary_x);

    /*
    Now h{m1||m2}: = C1 = aes128_ecb_encrypt(m1,m2)

    for any arbitary m2`:
    m1` = aes128_ecb_decrypt(C1,m2`)

    now h{m1`||m2`} = aes128_ecb_encrypt(m1`,m2`) = C1
    */

    aes128_ecb_decrypt(ciphertext, arbitary_prex, ciphertext);

    aes128_ecb_encrypt(ciphertext, arbitary_prex, ciphertext);
    // Question 3: Print the obtained second pre-image (m1`||m2`) and the compressed values h(m1`||m2`), h(m1||m2).
    printf("value of h{m1`||m2`} :");
    for (int i = 0; i < 16; i++)
        printf("%02x", ciphertext[i]);
    return 0;
}

static uint8_t char_value(const char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    else if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    else if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;

    return 255;
}

// parseing the string
void aes128_string_parse(const char *string, uint8_t *data)
{
    int i;
    for (i = 0; i < 32; i++)
        data[i] = char_value(string[i * 2]) * 16 + char_value(string[i * 2 + 1]);
}

// multiplication x * y in GF(2^8)
static uint8_t gf_mult(uint8_t x, uint8_t y)
{
    uint8_t result = 0;

    while (x != 0)
    {
        result ^= x & 1 ? y : 0;
        x >>= 1;
        y = MULT_X(y);
    }

    return result;
}

static void key_schedule(const uint8_t *key, uint8_t *round_keys)
{
    /*The first bytes of the expanded key are always equal to the key. If the key is 16 bytes long the first 16 bytes of the
     expanded key will be the same as the original key. If the key size is 32 bytes then the first 32 bytes of the expanded key
     will be the same as the original key. */
    int round, i;
    uint8_t word[4];
    uint8_t temp;

    memcpy(round_keys, key, 16);

    // loop for N_ROUNDS to generating 10 keys
    for (round = 1; round <= N_ROUNDS; round++)
    {

        /* With the exception of the first rounds each round also takes the previous rounds 4 bytes as input operates and returns 4 bytes*/
        for (i = 0; i < 4; i++)
            word[i] = round_keys[16 * round - 4 + i];

        temp = sbox[word[0]];
        word[0] = sbox[word[1]] ^ rcon[round];
        word[1] = sbox[word[2]];
        word[2] = sbox[word[3]];
        word[3] = temp;

        for (i = 0; i < 16; i++)
        {
            // 4 Rounds for a 16 byte Key
            if (i < 4)
                round_keys[16 * round + i] = word[i] ^ round_keys[16 * (round - 1) + i];
            // 6 Rounds for a 24 byte Key
            else
                round_keys[16 * round + i] = round_keys[16 * round + i - 4] ^ round_keys[16 * (round - 1) + i];
        }
    }
}

static void add_round_key(uint8_t *input, const uint8_t *rk)
{
    /* Each of the 16 bytes of the state is XORed against each of the 16 bytes of a portion of the expanded key for the
current round.*/
    int i;
    for (i = 0; i < 16; i++)
        input[i] ^= rk[i];
}

static void sub_bytes(uint8_t *input)
{
    /*During encryption each value of the state is replaced with the corresponding SBOX value */
    int i;
    for (i = 0; i < 16; i++)
        input[i] = sbox[input[i]];
}

static void shift_rows(uint8_t *input)
{

    /* Performing a circular shift for each row. This is not a bit wise shift. The
circular shift just moves each byte one space over. A byte that was in the second position may end up in the third
position after the shift. The circular part of it specifies that the byte in the last position shifted one space will end up
in the first position in the same row.*/

    // 2nd row, shift by one to the left
    uint8_t temp = input[1];
    input[1] = input[5];
    input[5] = input[9];
    input[9] = input[13];
    input[13] = temp;

    // 3rd row, shift by 2 to the left/right
    temp = input[2];
    input[2] = input[10];
    input[10] = temp;
    temp = input[6];
    input[6] = input[14];
    input[14] = temp;

    // 4th row, shift by 3 to the left (1 to the right)
    temp = input[15];
    input[15] = input[11];
    input[11] = input[7];
    input[7] = input[3];
    input[3] = temp;
}

static void mix_columns(uint8_t *input)
{
    /*The input state is arranged into a 4 row table (as described in the Shift Row function).
The multiplication is performed one column at a time (4 bytes). Each value in the column is eventually
multiplied against every value of the matrix (16 total multiplications). The results of these multiplications
are XORed together to produce only 4 result bytes for the next state. There fore 16 bytes input, 16
multiplications 12 XORs and 16 bytes output. The multiplication is performed one matrix row at a time
against each value of a state column.

s`(x) = a(x)XOR(x): where a(x) = {03}x^3 + {01}x^2 + {01}x + {02}

*/
    static uint8_t output[16];
    static uint8_t mult2[16];
    static uint8_t mult3[16];
    int i;
    for (i = 0; i < 16; i++)
    {
        output[i] = input[i];
        mult2[i] = MULT_X(input[i]);
        mult3[i] = mult2[i] ^ input[i];
    }

    for (i = 0; i < 4; i++)
    {
        output[i * 4] = mult2[i * 4] ^ mult3[i * 4 + 1] ^ input[i * 4 + 2] ^ input[i * 4 + 3];
        output[i * 4 + 1] = input[i * 4] ^ mult2[i * 4 + 1] ^ mult3[i * 4 + 2] ^ input[i * 4 + 3];
        output[i * 4 + 2] = input[i * 4] ^ input[i * 4 + 1] ^ mult2[i * 4 + 2] ^ mult3[i * 4 + 3];
        output[i * 4 + 3] = mult3[i * 4] ^ input[i * 4 + 1] ^ input[i * 4 + 2] ^ mult2[i * 4 + 3];
    }

    memcpy(input, output, 16);
}

static void inv_sub_bytes(uint8_t *input)
{
    /*During Decryption each value of the state is replaced with the corresponding Inverse SBOX value */
    int i;
    for (i = 0; i < 16; i++)
        input[i] = inv_sbox[input[i]];
}

static void inv_shift_rows(uint8_t *input)
{

    /* inv_shift_rows() is the inverse of the shift_rows() transformation. The bytes in the last
three rows of the State are cyclically shifted over different numbers of bytes (offsets). The first
row, r = 0, is not shifted */

    // 2nd row, shift by one to the right
    uint8_t temp = input[13];
    input[13] = input[9];
    input[9] = input[5];
    input[5] = input[1];
    input[1] = temp;

    // 3rd row, shift by 2 to the right/left
    temp = input[2];
    input[2] = input[10];
    input[10] = temp;
    temp = input[6];
    input[6] = input[14];
    input[14] = temp;

    // 4th row, shift by 3 to the right (1 to the left)
    temp = input[3];
    input[3] = input[7];
    input[7] = input[11];
    input[11] = input[15];
    input[15] = temp;
}

static void inv_mix_columns(uint8_t *input)
{
    /*
    inv_mix_columns() is the inverse of the mix_columns() transformation.
inv_mix_columns() operates on the State column-by-column, treating each column as a four-term polynomial under.
    s`(x) = a^-1 (x) XOR s(x) where a^-1(x) = {0b}x^3+{0d}x^2+{09}x+{0e}.

    */
    static uint8_t mult9[16];
    static uint8_t mult11[16];
    static uint8_t mult13[16];
    static uint8_t mult14[16];
    int i;
    for (i = 0; i < 16; i++)
    {
        mult9[i] = gf_mult(9, input[i]);
        mult11[i] = gf_mult(11, input[i]);
        mult13[i] = gf_mult(13, input[i]);
        mult14[i] = gf_mult(14, input[i]);
    }

    for (i = 0; i < 4; i++)
    {
        input[i * 4] = mult14[i * 4] ^ mult11[i * 4 + 1] ^ mult13[i * 4 + 2] ^ mult9[i * 4 + 3];
        input[i * 4 + 1] = mult9[i * 4] ^ mult14[i * 4 + 1] ^ mult11[i * 4 + 2] ^ mult13[i * 4 + 3];
        input[i * 4 + 2] = mult13[i * 4] ^ mult9[i * 4 + 1] ^ mult14[i * 4 + 2] ^ mult11[i * 4 + 3];
        input[i * 4 + 3] = mult11[i * 4] ^ mult13[i * 4 + 1] ^ mult9[i * 4 + 2] ^ mult14[i * 4 + 3];
    }
}

void aes128_ecb_encrypt(const uint8_t *plaintext, const uint8_t *key, uint8_t *ciphertext)
{
    int round;

    memcpy(ciphertext, plaintext, 16);

    // Performing key scheduling  to get the required keys
    key_schedule(key, round_keys);
    
    // add round key function on the first key and the paintext.
    add_round_key(ciphertext, round_keys);

    // for 9 rounds doing all 4 functions
    for (round = 1; round < N_ROUNDS; round++)
    {
        sub_bytes(ciphertext);
        shift_rows(ciphertext);
        mix_columns(ciphertext);
        add_round_key(ciphertext, round_keys + 16 * round);
    }

    // For round 10 doing only  3 functions
    sub_bytes(ciphertext);
    shift_rows(ciphertext);
    add_round_key(ciphertext, round_keys + 160);
}

void aes128_ecb_decrypt(uint8_t *plaintext, const uint8_t *key, const uint8_t *ciphertext)
{
    int round;

    memcpy(plaintext, ciphertext, 16);

    // Performing key scheduling  to get the required keys
    key_schedule(key, round_keys);

    // Performing 3 functions first for the first round in reverse order
    add_round_key(plaintext, round_keys + 160);
    inv_shift_rows(plaintext);
    inv_sub_bytes(plaintext);

    // performing all 4 functions for 9 rounds again in reverse over
    for (round = N_ROUNDS - 1; round > 0; round--)
    {
        add_round_key(plaintext, round_keys + 16 * round);
        inv_mix_columns(plaintext);
        inv_shift_rows(plaintext);
        inv_sub_bytes(plaintext);
    }

    // performing add round key function for after the last round on the ciphertext.
    add_round_key(plaintext, round_keys);
}

void string2hexString(char *input, char *output)
{
    // function to convert a string into hex string
    int loop;
    int i;

    i = 0;
    loop = 0;

    while (input[loop] != '\0')
    {
        sprintf((char *)(output + i), "%02X", input[loop]);
        loop += 1;
        i += 2;
    }
    // insert NULL at the end of the output string
    output[i++] = '\0';
}