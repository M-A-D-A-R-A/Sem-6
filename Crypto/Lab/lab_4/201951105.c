#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

// Basic point structure.
typedef struct
{
  int x;
  int y;
} point;

// Variables

#define CHUNK_SIZE 64
#define TOTAL_LEN_LEN 8

// Curve's coefficients.
int a = 25, b = 31;

// Prime defining the field we are working on.
int prime = 101;

// Constants

// No of points on the curve
int points_count = 0;

// Array of points on the curve
point *array_points;

// Point at infinity
const point PAI = {-1, -1};

// Functions
int in_curve(point);
int equal(point, point);
int inv(int, int);
int multiply(int, int);
int modulo(int, int);
point inverse(point);
point add(point, point);
point fast_addition(int, point);
point slow_addition(int, point);
void print_points(point[], int);
point random_points_in_EC();

#define AES_BLOCKLEN 16
#define AES_KEYLEN 32
#define AES_keyExpSize 240
#define Nb 4
#define Nk 8
#define Nr 14

#define IV_LEN 16
#define KEY_LEN 32 // 256 bit
#define DATA_LEN 64

#if MULTIPLY_AS_A_FUNCTION
static unsigned char Multiply(unsigned char x, unsigned char y)
{
  return (((y & 1) * x) ^
          ((y >> 1 & 1) * xtime(x)) ^
          ((y >> 2 & 1) * xtime(xtime(x))) ^
          ((y >> 3 & 1) * xtime(xtime(xtime(x)))) ^
          ((y >> 4 & 1) * xtime(xtime(xtime(xtime(x)))))); /* this last call to xtime() can be omitted */
}
#else
#define Multiply(x, y)                       \
  (((y & 1) * x) ^                           \
   ((y >> 1 & 1) * xtime(x)) ^               \
   ((y >> 2 & 1) * xtime(xtime(x))) ^        \
   ((y >> 3 & 1) * xtime(xtime(xtime(x)))) ^ \
   ((y >> 4 & 1) * xtime(xtime(xtime(xtime(x))))))

#endif
struct AES_contex
{
  unsigned char RoundKey[AES_keyExpSize];
  unsigned char Iv[AES_BLOCKLEN];
};

void AES256CBC_init(struct AES_contex *contex, const unsigned char *key, const unsigned char *iv);

void AES256CBC_encrypt(struct AES_contex *contex, unsigned char *buf, unsigned int length);
void AES256CBC_decrypt(struct AES_contex *contex, unsigned char *buf, unsigned int length);

typedef unsigned char state_t[4][4];

// The lookup-tables
static const unsigned char sbox[256] = {
    // 0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16};

static const unsigned char rsbox[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d};

static const unsigned char Rcon[11] = {
    0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};

#define getSBoxValue(num) (sbox[(num)])

#define getSBoxInvert(num) (rsbox[(num)])

// This function produces Nb(Nr+1) round keys. The round keys are used in each round to decrypt the states.
static void KeyExpansion(unsigned char *RoundKey, const unsigned char *Key)
{
  unsigned i, j, k;
  unsigned char tempa[4]; // Used for the column/row operations

  // The first round key is the key itself.
  for (i = 0; i < Nk; ++i)
  {
    RoundKey[(i * 4) + 0] = Key[(i * 4) + 0];
    RoundKey[(i * 4) + 1] = Key[(i * 4) + 1];
    RoundKey[(i * 4) + 2] = Key[(i * 4) + 2];
    RoundKey[(i * 4) + 3] = Key[(i * 4) + 3];
  }

  // All other round keys are found from the previous round keys.
  for (i = Nk; i < Nb * (Nr + 1); ++i)
  {
    {
      k = (i - 1) * 4;
      tempa[0] = RoundKey[k + 0];
      tempa[1] = RoundKey[k + 1];
      tempa[2] = RoundKey[k + 2];
      tempa[3] = RoundKey[k + 3];
    }

    if (i % Nk == 0)
    {

      {
        const unsigned char u8tmp = tempa[0];
        tempa[0] = tempa[1];
        tempa[1] = tempa[2];
        tempa[2] = tempa[3];
        tempa[3] = u8tmp;
      }

      {
        tempa[0] = getSBoxValue(tempa[0]);
        tempa[1] = getSBoxValue(tempa[1]);
        tempa[2] = getSBoxValue(tempa[2]);
        tempa[3] = getSBoxValue(tempa[3]);
      }

      tempa[0] = tempa[0] ^ Rcon[i / Nk];
    }
    if (i % Nk == 4)
    {
      // Function Subword()
      {
        tempa[0] = getSBoxValue(tempa[0]);
        tempa[1] = getSBoxValue(tempa[1]);
        tempa[2] = getSBoxValue(tempa[2]);
        tempa[3] = getSBoxValue(tempa[3]);
      }
    }
    j = i * 4;
    k = (i - Nk) * 4;
    RoundKey[j + 0] = RoundKey[k + 0] ^ tempa[0];
    RoundKey[j + 1] = RoundKey[k + 1] ^ tempa[1];
    RoundKey[j + 2] = RoundKey[k + 2] ^ tempa[2];
    RoundKey[j + 3] = RoundKey[k + 3] ^ tempa[3];
  }
}

void AES256CBC_init(struct AES_contex *contex, const unsigned char *key, const unsigned char *iv)
{
  KeyExpansion(contex->RoundKey, key);
  memcpy(contex->Iv, iv, AES_BLOCKLEN);
}

static void AddRoundKey(unsigned char round, state_t *state, const unsigned char *RoundKey)
{
  unsigned char i, j;
  for (i = 0; i < 4; ++i)
  {
    for (j = 0; j < 4; ++j)
    {
      (*state)[i][j] ^= RoundKey[(round * Nb * 4) + (i * Nb) + j];
    }
  }
}

static void SubBytes(state_t *state)
{
  unsigned char i, j;
  for (i = 0; i < 4; ++i)
  {
    for (j = 0; j < 4; ++j)
    {
      (*state)[j][i] = getSBoxValue((*state)[j][i]);
    }
  }
}

static void ShiftRows(state_t *state)
{
  unsigned char temp;

  // Rotate first row 1 columns to left
  temp = (*state)[0][1];
  (*state)[0][1] = (*state)[1][1];
  (*state)[1][1] = (*state)[2][1];
  (*state)[2][1] = (*state)[3][1];
  (*state)[3][1] = temp;

  // Rotate second row 2 columns to left
  temp = (*state)[0][2];
  (*state)[0][2] = (*state)[2][2];
  (*state)[2][2] = temp;

  temp = (*state)[1][2];
  (*state)[1][2] = (*state)[3][2];
  (*state)[3][2] = temp;

  // Rotate third row 3 columns to left
  temp = (*state)[0][3];
  (*state)[0][3] = (*state)[3][3];
  (*state)[3][3] = (*state)[2][3];
  (*state)[2][3] = (*state)[1][3];
  (*state)[1][3] = temp;
}

static unsigned char xtime(unsigned char x)
{
  return ((x << 1) ^ (((x >> 7) & 1) * 0x1b));
}

// MixColumns function mixes the columns of the state matrix
static void MixColumns(state_t *state)
{
  unsigned char i;
  unsigned char Tmp, Tm, t;
  for (i = 0; i < 4; ++i)
  {
    t = (*state)[i][0];
    Tmp = (*state)[i][0] ^ (*state)[i][1] ^ (*state)[i][2] ^ (*state)[i][3];
    Tm = (*state)[i][0] ^ (*state)[i][1];
    Tm = xtime(Tm);
    (*state)[i][0] ^= Tm ^ Tmp;
    Tm = (*state)[i][1] ^ (*state)[i][2];
    Tm = xtime(Tm);
    (*state)[i][1] ^= Tm ^ Tmp;
    Tm = (*state)[i][2] ^ (*state)[i][3];
    Tm = xtime(Tm);
    (*state)[i][2] ^= Tm ^ Tmp;
    Tm = (*state)[i][3] ^ t;
    Tm = xtime(Tm);
    (*state)[i][3] ^= Tm ^ Tmp;
  }
}

static void InvMixColumns(state_t *state)
{
  int i;
  unsigned char a, b, c, d;
  for (i = 0; i < 4; ++i)
  {
    a = (*state)[i][0];
    b = (*state)[i][1];
    c = (*state)[i][2];
    d = (*state)[i][3];

    (*state)[i][0] = Multiply(a, 0x0e) ^ Multiply(b, 0x0b) ^ Multiply(c, 0x0d) ^ Multiply(d, 0x09);
    (*state)[i][1] = Multiply(a, 0x09) ^ Multiply(b, 0x0e) ^ Multiply(c, 0x0b) ^ Multiply(d, 0x0d);
    (*state)[i][2] = Multiply(a, 0x0d) ^ Multiply(b, 0x09) ^ Multiply(c, 0x0e) ^ Multiply(d, 0x0b);
    (*state)[i][3] = Multiply(a, 0x0b) ^ Multiply(b, 0x0d) ^ Multiply(c, 0x09) ^ Multiply(d, 0x0e);
  }
}

static void InvSubBytes(state_t *state)
{
  unsigned char i, j;
  for (i = 0; i < 4; ++i)
  {
    for (j = 0; j < 4; ++j)
    {
      (*state)[j][i] = getSBoxInvert((*state)[j][i]);
    }
  }
}

static void InvShiftRows(state_t *state)
{
  unsigned char temp;

  // Rotate first row 1 columns to right
  temp = (*state)[3][1];
  (*state)[3][1] = (*state)[2][1];
  (*state)[2][1] = (*state)[1][1];
  (*state)[1][1] = (*state)[0][1];
  (*state)[0][1] = temp;

  // Rotate second row 2 columns to right
  temp = (*state)[0][2];
  (*state)[0][2] = (*state)[2][2];
  (*state)[2][2] = temp;

  temp = (*state)[1][2];
  (*state)[1][2] = (*state)[3][2];
  (*state)[3][2] = temp;

  // Rotate third row 3 columns to right
  temp = (*state)[0][3];
  (*state)[0][3] = (*state)[1][3];
  (*state)[1][3] = (*state)[2][3];
  (*state)[2][3] = (*state)[3][3];
  (*state)[3][3] = temp;
}

static void Cipher(state_t *state, const unsigned char *RoundKey)
{

  unsigned char round = 0;

  AddRoundKey(0, state, RoundKey);

  for (round = 1; round < Nr; ++round)
  {
    SubBytes(state);
    ShiftRows(state);
    MixColumns(state);
    AddRoundKey(round, state, RoundKey);
  }

  SubBytes(state);
  ShiftRows(state);
  AddRoundKey(Nr, state, RoundKey);
}

static void Cipher_inverse(state_t *state, const unsigned char *RoundKey)
{
  unsigned char round = 0;

  AddRoundKey(Nr, state, RoundKey);

  for (round = (Nr - 1); round > 0; --round)
  {
    InvShiftRows(state);
    InvSubBytes(state);
    AddRoundKey(round, state, RoundKey);
    InvMixColumns(state);
  }

  InvShiftRows(state);
  InvSubBytes(state);
  AddRoundKey(0, state, RoundKey);
}

static void XorWithIv(unsigned char *buf, const unsigned char *Iv)
{
  unsigned char i;
  for (i = 0; i < AES_BLOCKLEN; ++i) // The block in AES is always 128bit no matter the key size
  {
    buf[i] ^= Iv[i];
  }
}

void AES256CBC_encrypt(struct AES_contex *contex, unsigned char *buf, unsigned int length)
{
  unsigned int i;
  unsigned char *Iv = contex->Iv;
  for (i = 0; i < length; i += AES_BLOCKLEN)
  {
    XorWithIv(buf, Iv);
    Cipher((state_t *)buf, contex->RoundKey);
    Iv = buf;

    buf += AES_BLOCKLEN;
  }

  memcpy(contex->Iv, Iv, AES_BLOCKLEN);
}

void AES256CBC_decrypt(struct AES_contex *contex, unsigned char *buf, unsigned int length)
{
  unsigned int i;
  unsigned char storeNextIv[AES_BLOCKLEN];
  for (i = 0; i < length; i += AES_BLOCKLEN)
  {
    memcpy(storeNextIv, buf, AES_BLOCKLEN);
    Cipher_inverse((state_t *)buf, contex->RoundKey);
    XorWithIv(buf, contex->Iv);
    memcpy(contex->Iv, storeNextIv, AES_BLOCKLEN);
    buf += AES_BLOCKLEN;
  }
}

// Returns 1 if the point is in the EC, 0 otherwise.
int in_curve(point point)
{

  // Left side of the equality.
  int left = multiply(point.y, point.y);

  // Right side of the equality.
  int right = multiply(multiply(point.x, point.x), point.x);

  right += multiply(a, point.x);
  right += b;
  // Was missing modulo here. Important in case it goes over prime.
  right = modulo(right, prime);

  if (left == right)
    return 1;

  return 0;
}

// Returns 1 if points are equal, false otherwise.
int equal(point p1, point p2)
{
  if ((p1.x == p2.x) && (p1.y == p2.y))
    return 1;
  return 0;
}

// Returns the inverse modulo a prime p of the number.
int inv(int x, int p)
{
  x = x % p;
  if (x < 0)
    x = p + x;
  // Remainders and elements of the linear combination in the extended Euclidean algorithm
  int r0, r1, r_temp, t0, t1, t_temp;
  r0 = p, r1 = x, t0 = 0, t1 = 1;
  r_temp = r1, t_temp = t1;
  while (r1 != 1)
  {
    // Quotient of division
    int q = r0 / r1;
    r_temp = r0 % r1;
    t_temp = t0 - q * t1;
    r0 = r1, r1 = r_temp;
    t0 = t1, t1 = t_temp;
  }
  return t1 < 0 ? p + t1 : t1;
}

// Multiplies modulo prime
int multiply(int x, int y)
{
  return modulo(x * y, prime);
}

// Modulo operation
int modulo(int x, int y)
{
  // Remainder of division
  int mod = x % y;
  return mod < 0 ? y + mod : mod;
}

// Returns the inverse of the given point.
point inverse(point p)
{
  if (equal(PAI, p))
    return PAI;
  // The inverse.
  point q = {p.x, modulo((prime - p.y), prime)};
  return q;
}

// Performs modular addition of p1 and p2
point add(point p1, point p2)
{
  if (equal(PAI, p1))
    return p2;
  else if (equal(PAI, p2))
    return p1;
  else if (equal(p1, inverse(p2)))
    return PAI;
  else
  {
    // Slope of the line p1p2
    int alpha;
    if (!equal(p1, p2))
      alpha = multiply(p1.y - p2.y, inv(p1.x - p2.x, prime));
    else
    {
      // Temporary variable
      int temp = multiply(p1.x, p1.x);
      alpha = 3 * temp;
      alpha = modulo(alpha, prime);
      alpha += a;
      alpha = modulo(alpha, prime);
      alpha = multiply(alpha, inv(p1.y + p1.y, prime));
    }
    // Result coordinates.
    int x3, y3;
    x3 = multiply(alpha, alpha) - (p1.x + p2.x);
    x3 = modulo(x3, prime);
    y3 = multiply(alpha, p1.x - x3) - p1.y;
    y3 = modulo(y3, prime);
    // The resulting point.
    point sum = {x3, y3};
    return sum;
  }
}

// Performs fast multiplication algorithm to add p to itself d times.
point fast_addition(int d, point p)
{
  // Base 2 logarithm of d.
  int log2d = log(d) / log(2);
  // Second Highest power of 2 smaller than d
  int h_power = 1 << (log2d - 1);
  // Auxiliary value
  point y = {p.x, p.y};
  if (d == 0)
    return PAI;
  int coef = 1;
  while (h_power > 0)
  {
    if ((h_power & d) > 0)
    {
      y = add(y, y);
      y = add(y, p);
      coef = 2 * coef + 1;
    }
    else
    {
      y = add(y, y);
      coef = 2 * coef;
    }
    h_power >>= 1;
  }
  return y;
}

// Performs multiplication by scalar slowly.
point slow_addition(int d, point p)
{
  // Auxiliary value
  point y = PAI;
  if (d == 0)
    return y;
  // Counter
  int count;
  for (count = 0; count < d; ++count)
    y = add(y, p);
  return y;
}

// Prints an array of points.
void print_points(point points[], int size)
{
  // Counter
  int i;
  for (i = 0; i < size; ++i)
    printf("(%d, %d) ", points[i].x, points[i].y);
  printf("\n");
}

point random_points_in_EC()
{
  time_t t;
  srand((unsigned)time(&t));

  int rand_index = rand() % 10;
  point y = PAI;
  point *array_points = malloc(points_count * sizeof *array_points);
  for (int i = 0; i < prime; i++)
  {
    for (int j = 0; j < prime; j++)
    {
      point p = {i, j};
      if (in_curve(p))
      {
        array_points[points_count].x = p.x;
        array_points[points_count].y = p.y;
        points_count++;
      }
    }
  }

  y.x = array_points[rand_index].x;
  y.y = array_points[rand_index].y;

  return y;
}

static const uint32_t k[] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

struct buffer_state
{
  const uint8_t *p;
  size_t len;
  size_t total_len;
  int single_one_delivered;
  int total_len_delivered;
};

static inline uint32_t right_rot(uint32_t value, unsigned int count)
{
  return value >> count | value << (32 - count);
}

static void initstate_buf(struct buffer_state *state, const void *input, size_t len)
{
  state->p = input;
  state->len = len;
  state->total_len = len;
  state->single_one_delivered = 0;
  state->total_len_delivered = 0;
}

static int chunk_cal(uint8_t chunk[CHUNK_SIZE], struct buffer_state *state)
{
  size_t space_in_chunk;

  if (state->total_len_delivered)
  {
    return 0;
  }

  if (state->len >= CHUNK_SIZE)
  {
    memcpy(chunk, state->p, CHUNK_SIZE);
    state->p += CHUNK_SIZE;
    state->len -= CHUNK_SIZE;
    return 1;
  }

  memcpy(chunk, state->p, state->len);
  chunk += state->len;
  space_in_chunk = CHUNK_SIZE - state->len;
  state->p += state->len;
  state->len = 0;

  if (!state->single_one_delivered)
  {
    *chunk++ = 0x80;
    space_in_chunk -= 1;
    state->single_one_delivered = 1;
  }

  if (space_in_chunk >= TOTAL_LEN_LEN)
  {
    const size_t left = space_in_chunk - TOTAL_LEN_LEN;
    size_t len = state->total_len;
    int i;
    memset(chunk, 0x00, left);
    chunk += left;

    // Storing of len * 8 as a big endian 64-bit without overflow.

    chunk[7] = (uint8_t)(len << 3);
    len >>= 5;
    for (i = 6; i >= 0; i--)
    {
      chunk[i] = (uint8_t)len;
      len >>= 8;
    }
    state->total_len_delivered = 1;
  }
  else
  {
    memset(chunk, 0x00, space_in_chunk);
  }

  return 1;
}

void sha_256(uint8_t hash[32], const void *input, size_t len)
{

  uint32_t h[] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};
  unsigned i, j;

  uint8_t chunk[64];

  struct buffer_state state;

  initstate_buf(&state, input, len);

  while (chunk_cal(chunk, &state))
  {
    uint32_t ah[8];

    const uint8_t *p = chunk;

    for (i = 0; i < 8; i++)
      ah[i] = h[i];

    for (i = 0; i < 4; i++)
    {

      uint32_t w[16];

      for (j = 0; j < 16; j++)
      {
        if (i == 0)
        {
          w[j] = (uint32_t)p[0] << 24 | (uint32_t)p[1] << 16 |
                 (uint32_t)p[2] << 8 | (uint32_t)p[3];
          p += 4;
        }
        else
        {

          const uint32_t s0 = right_rot(w[(j + 1) & 0xf], 7) ^ right_rot(w[(j + 1) & 0xf], 18) ^ (w[(j + 1) & 0xf] >> 3);
          const uint32_t s1 = right_rot(w[(j + 14) & 0xf], 17) ^ right_rot(w[(j + 14) & 0xf], 19) ^ (w[(j + 14) & 0xf] >> 10);
          w[j] = w[j] + s0 + w[(j + 9) & 0xf] + s1;
        }
        const uint32_t s1 = right_rot(ah[4], 6) ^ right_rot(ah[4], 11) ^ right_rot(ah[4], 25);
        const uint32_t ch = (ah[4] & ah[5]) ^ (~ah[4] & ah[6]);
        const uint32_t temp1 = ah[7] + s1 + ch + k[i << 4 | j] + w[j];
        const uint32_t s0 = right_rot(ah[0], 2) ^ right_rot(ah[0], 13) ^ right_rot(ah[0], 22);
        const uint32_t maj = (ah[0] & ah[1]) ^ (ah[0] & ah[2]) ^ (ah[1] & ah[2]);
        const uint32_t temp2 = s0 + maj;

        ah[7] = ah[6];
        ah[6] = ah[5];
        ah[5] = ah[4];
        ah[4] = ah[3] + temp1;
        ah[3] = ah[2];
        ah[2] = ah[1];
        ah[1] = ah[0];
        ah[0] = temp1 + temp2;
      }
    }

    for (i = 0; i < 8; i++)
      h[i] += ah[i];
  }

  for (i = 0, j = 0; i < 8; i++)
  {
    hash[j++] = (uint8_t)(h[i] >> 24);
    hash[j++] = (uint8_t)(h[i] >> 16);
    hash[j++] = (uint8_t)(h[i] >> 8);
    hash[j++] = (uint8_t)h[i];
  }
}

static void hashstring(char string[65], const uint8_t hash[32])
{
  size_t i;
  for (i = 0; i < 32; i++)
  {
    string += sprintf(string, "%02x", hash[i]);
  }
}

static void decode_sha(char input[])
{
  uint8_t hash[32];
  char hash_string[65];
  sha_256(hash, input, strlen(input));
  hashstring(hash_string, hash);
  memcpy(input, hash_string, 65);
}

void int2bin(unsigned integer, char *binary, int n)
{
  for (int i = 0; i < n; i++)
    binary[i] = (integer & (int)1 << (n - i - 1)) ? '1' : '0';
  binary[n] = '\0';
}

void hex2bin(const char *in, size_t len, unsigned char *out)
{

  static const unsigned char TBL[] = {
      0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 58, 59, 60, 61,
      62, 63, 64, 10, 11, 12, 13, 14, 15, 71, 72, 73, 74, 75,
      76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
      90, 91, 92, 93, 94, 95, 96, 10, 11, 12, 13, 14, 15};

  static const unsigned char *LOOKUP = TBL - 48;

  const char *end = in + len;

  while (in < end)
    *(out++) = LOOKUP[*(in++)] << 4 | LOOKUP[*(in++)];
}

int main()
{
  printf("\nCurve Taken \ny^2 = x^3 + 25x +31 under mod 101 \n");
  point alpha, np, mq, nmr;
  char x1[33], y1[33];
  unsigned char a = 1, b = 2;
  unsigned char message1[96];
  unsigned int ma[96];

  // unsigned char *ciphertext;
  char temp;
  int ctr = 0;
  // unsigned char data[DATA_LEN];

  unsigned char iv[IV_LEN];
  unsigned char key[KEY_LEN];
  unsigned char xorkey2[KEY_LEN];
  memset(iv, 0x00, sizeof(iv));

  alpha = random_points_in_EC();
  printf("alpha = (%d, %d) ", alpha.x, alpha.y);

  char alicenumber[2];

  char bobnumber[2];

  printf("\nEnter a the value alice number: ");
  scanf("%s", &alicenumber);
  int x = atoi(alicenumber);
  // printf("\nThe value of Alice Number: %d",x);

  np = fast_addition(x, alpha);
  // printf("n time p  = (%d, %d) ", np.x, np.y);

  printf("\nEnter a the value bob number: ");
  scanf("%s", &bobnumber);
  int y = atoi(bobnumber);
  // printf("\nThe value of bob Number: %d",y);

  mq = fast_addition(y, alpha);
  // printf("n time p  = (%d, %d) ", mq.x, mq.y);
  // printf("n time p  = (%d, %d) ", mq.x, mq.y);

  nmr = fast_addition(x * y, alpha);
  printf("shared key  = (%d, %d) \n", nmr.x, nmr.y);

  int2bin(nmr.x, x1, 8);
  int2bin(nmr.y, y1, 8);

  strcat(x1, y1);
  decode_sha(x1);
  unsigned char key_256[32];
  hex2bin(x1, 64, key_256);
  printf("Ka =");
  for (int i = 0; i < 32; i++)
  {
    printf("%02X ", key_256[i]);
  }
  printf("\nKb =");
  for (int i = 0; i < 32; i++)
  {
    printf("%02X ", key_256[i]);
  }
  printf("\nKey =");
  for (int i = 0; i < 32; i++)
  {
    key[i] = key_256[i];
    printf("%02X", key[i]);
  }

  printf("\nEnter MA: ");
  scanf("%c", &temp); // temp statement to clear buffer
  fgets(message1, 96, stdin);
  message1[95] = '\0';
  unsigned char whatever[] = "";
  unsigned char whatever2[] = "";
  ctr = 0;
  for (size_t i = 0; i < 96; i++)
  {
    char output[3];
    output[2] = '\0';
    if (i % 3 == 0)
    {
      output[0] = message1[i];
      output[1] = message1[i + 1];
      whatever[ctr++] = strtol(output, NULL, 16);
    }
  }
  whatever[32] = '\0';

  struct AES_contex contex;
  AES256CBC_init(&contex, key, iv);
  AES256CBC_encrypt(&contex, whatever, DATA_LEN);
  printf("\n Ca: ");
  for (int i = 0; i < 32; i++)
  {
    printf("%02X", whatever[i]);
  }
  AES256CBC_init(&contex, key, iv);
  AES256CBC_decrypt(&contex, whatever, DATA_LEN);
  printf("\n MB = ");
  for (int i = 0; i < 32; i++)
  {
    printf("%02X", whatever[i]);
  }

  memcpy(ma, key, 32);
  ma[31] = ma[31] ^ 1;
  memcpy(xorkey2, key, 32);
  xorkey2[31] = xorkey2[31] ^ 2;

  decode_sha(xorkey2);
  hex2bin(xorkey2, 64, xorkey2);
  memcpy(ma + 32, xorkey2, 64);
  memcpy(ma + 64, whatever, 96);

  printf("\n MacA =");
  decode_sha(ma);
  hex2bin(ma, 96, ma);
  for (int i = 0; i < 32; i++)
  {

    printf("%02X", ma[i]);
  }

  printf("\n MacB =");
  for (int i = 0; i < 32; i++)
  {

    printf("%02X", ma[i]);
  }

  printf("\n Ka = kb \n MA= Mb \n MACa =MACb \n");

  return 0;
}