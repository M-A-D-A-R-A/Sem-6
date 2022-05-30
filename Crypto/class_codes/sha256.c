#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef unsigned char byte;
typedef unsigned int uint32;


#define ROTR(x,n) ((x>>n) | (x<<(32-n)))

#define BIG_ENDIAN(b) ((b[0]<<24) | (b[1]<<16) | (b[2]<<8) | b[3])

// computes the hash of the data buffer
void sha256_hash(const void* data, size_t len, byte output[32]);
// computers the hash of whatever file is passed
void sha256_hash_file(FILE* file, byte output[32]);

#define Ch(x, y, z) ((x & y) ^ (~x & z))
#define Maj(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
#define SIGMA_0(x) (ROTR(x,  2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define SIGMA_1(x) (ROTR(x,  6) ^ ROTR(x, 11) ^ ROTR(x, 25))
#define sigma_0(x) (ROTR(x,  7) ^ ROTR(x, 18) ^ (x >> 3))
#define sigma_1(x) ((ROTR(x, 17) ^ ROTR(x, 19)) ^ (x >> 10))

typedef struct sha256_context
{
    uint32 block[16];
    uint32 hash[8];
    size_t dataLength;
} sha256_context;

static uint32 K[] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};
static uint32 pad(byte buffer[128], size_t totalLength)
{
    uint32 n = totalLength & 0x3f;
    buffer[n] = 0x80;
    int i;
    for(i=n+1; (i & 63) != 56; i++)
        buffer[i] = 0;
    totalLength <<= 3;
    buffer += i;
    for(int j=7; j>=0; j--)
    {
        buffer[j] = (totalLength & 0xff);
        totalLength >>= 8;
    }
    return i<64? 64:128; 
}
void parse(sha256_context* ctx, byte buffer[64])
{
    for(int i=0; i< 16; i++)
        ctx->block[i] = BIG_ENDIAN((buffer+4*i));
}
void init(sha256_context* ctx, size_t length)
{
    // initial internal state
    ctx->hash[0] = 0x6a09e667;
    ctx->hash[1] = 0xbb67ae85;
    ctx->hash[2] = 0x3c6ef372;
    ctx->hash[3] = 0xa54ff53a;
    ctx->hash[4] = 0x510e527f;
    ctx->hash[5] = 0x9b05688c;
    ctx->hash[6] = 0x1f83d9ab;
    ctx->hash[7] = 0x5be0cd19;
    ctx->dataLength = length;
}
void update(sha256_context* ctx)
{
    uint32 W[64];
    for(int i=0; i<16; i++)
        W[i] = ctx->block[i];
    for(int i=16; i< 64; i++)
        W[i] = sigma_1(W[i-2]) + W[i-7] + sigma_0(W[i-15]) + W[i-16];
    
    uint32 a,b,c,d,e,f,g,h;

    a = ctx->hash[0];
    b = ctx->hash[1];
    c = ctx->hash[2];
    d = ctx->hash[3];
    e = ctx->hash[4];
    f = ctx->hash[5];
    g = ctx->hash[6];
    h = ctx->hash[7];

    uint32 T1, T2;

    for (int t = 0; t < 64; t++)
    {
        T1 = h + SIGMA_1(e) + Ch(e, f, g) + K[t] + W[t];
        T2 = SIGMA_0(a) + Maj(a,b,c);
        h = g;
        g = f;
        f = e;
        e = d + T1;
        d = c;
        c = b;
        b = a;
        a = T1 + T2;
    }
    ctx->hash[0] += a;
    ctx->hash[1] += b;
    ctx->hash[2] += c;
    ctx->hash[3] += d;
    ctx->hash[4] += e;
    ctx->hash[5] += f;
    ctx->hash[6] += g;
    ctx->hash[7] += h;
}
void hash(sha256_context* ctx, byte* data)
{
    uint32 n = ctx->dataLength & 0x3f;
    for(size_t i=0; i< ctx->dataLength - n; i+=64)
    {
        parse(ctx, data + i);
        update(ctx);
    }
    byte buffer[128] = {0};
    memcpy(buffer, data + ctx->dataLength - n, n);
    uint32 N = pad(buffer, ctx->dataLength);
    for (int i = 0; i < N; i+=64)
    {
        parse(ctx, buffer+i);
        update(ctx);
    }
}
void hash_file(sha256_context* ctx, FILE* file)
{
    byte buffer[128] = {0};
    uint32 n = fread(buffer, 1, 64, file);
    while(n == 64)
    {
        parse(ctx, buffer);
        update(ctx);
        ctx->dataLength += n;
        n = fread(buffer, 1, 64, file);
    }
    ctx->dataLength += n;
    uint32 N = pad(buffer, ctx->dataLength);
    for (int i = 0; i < N; i+=64)
    {
        parse(ctx, buffer+i);
        update(ctx);
    }
}
void finish(sha256_context* ctx, byte output[32])
{
    for(int i=0; i<8; i++)
    {
        for(int j=3; j>=0; j--)
        {
            output[4*i + j] = ctx->hash[i] & 0xff;
            ctx->hash[i] >>= 8;
        }
    }
}
void sha256_hash(const void* data, size_t len, byte output[32])
{
    sha256_context ctx;
    init(&ctx, len);
    hash(&ctx, (byte*)data);
    finish(&ctx, output);
}
void sha256_hash_file(FILE* file, byte output[32])
{
    sha256_context ctx;
    init(&ctx, 0);
    hash_file(&ctx, file);
    finish(&ctx, output);
}
static uint32 pad(byte buffer[128], size_t totalLength)
{
    uint32 n = totalLength & 0x3f;
    buffer[n] = 0x80;
    int i;
    for(i=n+1; (i & 63) != 56; i++)
        buffer[i] = 0;
    totalLength <<= 3;
    buffer += i;
    for(int j=7; j>=0; j--)
    {
        buffer[j] = (totalLength & 0xff);
        totalLength >>= 8;
    }
    return i<64? 64:128; 
}
void parse(sha256_context* ctx, byte buffer[64])
{
    for(int i=0; i< 16; i++)
        ctx->block[i] = BIG_ENDIAN((buffer+4*i));
}
void init(sha256_context* ctx, size_t length)
{
    // initial internal state
    ctx->hash[0] = 0x6a09e667;
    ctx->hash[1] = 0xbb67ae85;
    ctx->hash[2] = 0x3c6ef372;
    ctx->hash[3] = 0xa54ff53a;
    ctx->hash[4] = 0x510e527f;
    ctx->hash[5] = 0x9b05688c;
    ctx->hash[6] = 0x1f83d9ab;
    ctx->hash[7] = 0x5be0cd19;
    ctx->dataLength = length;
}
void update(sha256_context* ctx)
{
    uint32 W[64];
    for(int i=0; i<16; i++)
        W[i] = ctx->block[i];
    for(int i=16; i< 64; i++)
        W[i] = sigma_1(W[i-2]) + W[i-7] + sigma_0(W[i-15]) + W[i-16];
    
    uint32 a,b,c,d,e,f,g,h;

    a = ctx->hash[0];
    b = ctx->hash[1];
    c = ctx->hash[2];
    d = ctx->hash[3];
    e = ctx->hash[4];
    f = ctx->hash[5];
    g = ctx->hash[6];
    h = ctx->hash[7];

    uint32 T1, T2;

    for (int t = 0; t < 64; t++)
    {
        T1 = h + SIGMA_1(e) + Ch(e, f, g) + K[t] + W[t];
        T2 = SIGMA_0(a) + Maj(a,b,c);
        h = g;
        g = f;
        f = e;
        e = d + T1;
        d = c;
        c = b;
        b = a;
        a = T1 + T2;
    }
    ctx->hash[0] += a;
    ctx->hash[1] += b;
    ctx->hash[2] += c;
    ctx->hash[3] += d;
    ctx->hash[4] += e;
    ctx->hash[5] += f;
    ctx->hash[6] += g;
    ctx->hash[7] += h;
}
void hash(sha256_context* ctx, byte* data)
{
    uint32 n = ctx->dataLength & 0x3f;
    for(size_t i=0; i< ctx->dataLength - n; i+=64)
    {
        parse(ctx, data + i);
        update(ctx);
    }
    byte buffer[128] = {0};
    memcpy(buffer, data + ctx->dataLength - n, n);
    uint32 N = pad(buffer, ctx->dataLength);
    for (int i = 0; i < N; i+=64)
    {
        parse(ctx, buffer+i);
        update(ctx);
    }
}
void hash_file(sha256_context* ctx, FILE* file)
{
    byte buffer[128] = {0};
    uint32 n = fread(buffer, 1, 64, file);
    while(n == 64)
    {
        parse(ctx, buffer);
        update(ctx);
        ctx->dataLength += n;
        n = fread(buffer, 1, 64, file);
    }
    ctx->dataLength += n;
    uint32 N = pad(buffer, ctx->dataLength);
    for (int i = 0; i < N; i+=64)
    {
        parse(ctx, buffer+i);
        update(ctx);
    }
}
void finish(sha256_context* ctx, byte output[32])
{
    for(int i=0; i<8; i++)
    {
        for(int j=3; j>=0; j--)
        {
            output[4*i + j] = ctx->hash[i] & 0xff;
            ctx->hash[i] >>= 8;
        }
    }
}
void sha256_hash(const void* data, size_t len, byte output[32])
{
    sha256_context ctx;
    init(&ctx, len);
    hash(&ctx, (byte*)data);
    finish(&ctx, output);
}
void sha256_hash_file(FILE* file, byte output[32])
{
    sha256_context ctx;
    init(&ctx, 0);
    hash_file(&ctx, file);
    finish(&ctx, output);
}

void fromFile(const char* path, byte hash[32])
{
    FILE* f = fopen(path, "rb");
    if(!f)
    {
        printf("Error opening %s: %s\n", path, strerror(errno));
        exit(EXIT_FAILURE);
    }
    sha256_hash_file(f, hash);
}
void fromString(const char* string, byte hash[32])
{
    sha256_hash(string, strlen(string), hash);
}
void help()
{
    printf("Usage: \n");
    printf("sha256 [options] input\n\n");
    printf("Options:\n");
    printf("-h: Show this help\n");
    printf("-f: input is a file to read from\n");
    printf("-U: Use UPPERCASE format\n");

    exit(EXIT_SUCCESS);
}
void (*options[])(const char*, byte*) = {fromString, fromFile};

int main(int argc, char const *argv[])
{
    int format = 0;
    int func = 0;
    byte hash[32];

    if(argc < 2) help();
    for (int i = 1; i < argc && *argv[i] == '-'; i++)
    {
        for (int j = 1; j < strlen(argv[i]); j++)
        {
            if(argv[i][j] == 'h') help();
            format += argv[i][j] == 'U';
            func |= argv[i][j] == 'f';
        }
    }
    options[func](argv[argc-1], hash);
    // sha256_hash(argv[argc-1], strlen(argv[argc-1]), hash);

    for (int i = 0; i < 32; i++)
        printf(format?"%02X":"%02x", hash[i]);
    printf("\n");
    
    return 0;
}