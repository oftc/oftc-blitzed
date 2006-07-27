/*
 * SHA1 hash functions.
 *
 * This came from GnuPG:
 *
 * Copyright (C) 1998, 1999, 2000, 2001 Free Software Foundation, Inc.
 *
 * This program is free but copyrighted software; see the file COPYING for
 * details.
 */

#include "services.h"

RCSID("$Id$");

#if defined(__GNUC__) && defined(__i386__)
static inline uint32 rol(uint32 x, int n);
#endif
static void burn_stack(int bytes);
static void transform(SHA1_CONTEXT *hd, const unsigned char *data);
static void sha1_write(SHA1_CONTEXT *hd, const unsigned char *inbuf,
    size_t inlen);
static void sha1_final(SHA1_CONTEXT *hd);
static unsigned char *sha1_read(SHA1_CONTEXT *hd);


/*
 * Rotate a 32 bit integer by n bytes.
 */
#if defined(__GNUC__) && defined(__i386__)
static inline uint32 rol(uint32 x, int n)
{
	__asm__("roll %%cl,%0"
		:"=r" (x)
		:"0" (x),"c" (n));
	return(x);
}
#else
#	define rol(x,n) ( ((x) << (n)) | ((x) >> (32-(n))) )
#endif

static void burn_stack(int bytes)
{
	char buf[128];
    
	memset(buf, 0, sizeof(buf));
	bytes -= sizeof(buf);
	
	if (bytes > 0)
		burn_stack(bytes);
}


void sha1_init(SHA1_CONTEXT *hd)
{
	hd->h0 = 0x67452301;
	hd->h1 = 0xefcdab89;
	hd->h2 = 0x98badcfe;
	hd->h3 = 0x10325476;
	hd->h4 = 0xc3d2e1f0;
	hd->nblocks = 0;
	hd->count = 0;
}


/*
 * Transform the message X which consists of 16 32-bit-words.
 */
static void transform(SHA1_CONTEXT *hd, const unsigned char *data)
{
	uint32 a, b, c, d, e, tm;
	uint32 x[16];

	/* get values from the chaining vars */
	a = hd->h0;
	b = hd->h1;
	c = hd->h2;
	d = hd->h3;
	e = hd->h4;

#ifdef WORDS_BIGENDIAN
	memcpy(x, data, 64);
#else
	{
		int i;
		unsigned char *p2;
		
		for (i = 0, p2 = (unsigned char *)x; i < 16; i++, p2 += 4) {
			p2[3] = *data++;
			p2[2] = *data++;
			p2[1] = *data++;
			p2[0] = *data++;
		}
	}
#endif


#define K1 0x5A827999L
#define K2 0x6ED9EBA1L
#define K3 0x8F1BBCDCL
#define K4 0xCA62C1D6L
#define F1(x,y,z) ( z ^ ( x & ( y ^ z ) ) )
#define F2(x,y,z) ( x ^ y ^ z )
#define F3(x,y,z) ( ( x & y ) | ( z & ( x | y ) ) )
#define F4(x,y,z) ( x ^ y ^ z )


#define M(i) ( tm = x[i&0x0f] ^ x[(i-14)&0x0f] \
    ^ x[(i-8)&0x0f] ^ x[(i-3)&0x0f] \
    , (x[i&0x0f] = rol(tm,1)) )

#define R(a,b,c,d,e,f,k,m)  do { e += rol( a, 5 )     \
				      + f( b, c, d )  \
				      + k	      \
				      + m;	      \
				 b = rol( b, 30 );    \
			       } while(0)

	R(a, b, c, d, e, F1, K1, x[ 0]);
	R(e, a, b, c, d, F1, K1, x[ 1]);
	R(d, e, a, b, c, F1, K1, x[ 2]);
	R(c, d, e, a, b, F1, K1, x[ 3]);
	R(b, c, d, e, a, F1, K1, x[ 4]);
	R(a, b, c, d, e, F1, K1, x[ 5]);
	R(e, a, b, c, d, F1, K1, x[ 6]);
	R(d, e, a, b, c, F1, K1, x[ 7]);
	R(c, d, e, a, b, F1, K1, x[ 8]);
	R(b, c, d, e, a, F1, K1, x[ 9]);
	R(a, b, c, d, e, F1, K1, x[10]);
	R(e, a, b, c, d, F1, K1, x[11]);
	R(d, e, a, b, c, F1, K1, x[12]);
	R(c, d, e, a, b, F1, K1, x[13]);
	R(b, c, d, e, a, F1, K1, x[14]);
	R(a, b, c, d, e, F1, K1, x[15]);
	R(e, a, b, c, d, F1, K1, M(16));
	R(d, e, a, b, c, F1, K1, M(17));
	R(c, d, e, a, b, F1, K1, M(18));
	R(b, c, d, e, a, F1, K1, M(19));
	R(a, b, c, d, e, F2, K2, M(20));
	R(e, a, b, c, d, F2, K2, M(21));
	R(d, e, a, b, c, F2, K2, M(22));
	R(c, d, e, a, b, F2, K2, M(23));
	R(b, c, d, e, a, F2, K2, M(24));
	R(a, b, c, d, e, F2, K2, M(25));
	R(e, a, b, c, d, F2, K2, M(26));
	R(d, e, a, b, c, F2, K2, M(27));
	R(c, d, e, a, b, F2, K2, M(28));
	R(b, c, d, e, a, F2, K2, M(29));
	R(a, b, c, d, e, F2, K2, M(30));
	R(e, a, b, c, d, F2, K2, M(31));
	R(d, e, a, b, c, F2, K2, M(32));
	R(c, d, e, a, b, F2, K2, M(33));
	R(b, c, d, e, a, F2, K2, M(34));
	R(a, b, c, d, e, F2, K2, M(35));
	R(e, a, b, c, d, F2, K2, M(36));
	R(d, e, a, b, c, F2, K2, M(37));
	R(c, d, e, a, b, F2, K2, M(38));
	R(b, c, d, e, a, F2, K2, M(39));
	R(a, b, c, d, e, F3, K3, M(40));
	R(e, a, b, c, d, F3, K3, M(41));
	R(d, e, a, b, c, F3, K3, M(42));
	R(c, d, e, a, b, F3, K3, M(43));
	R(b, c, d, e, a, F3, K3, M(44));
	R(a, b, c, d, e, F3, K3, M(45));
	R(e, a, b, c, d, F3, K3, M(46));
	R(d, e, a, b, c, F3, K3, M(47));
	R(c, d, e, a, b, F3, K3, M(48));
	R(b, c, d, e, a, F3, K3, M(49));
	R(a, b, c, d, e, F3, K3, M(50));
	R(e, a, b, c, d, F3, K3, M(51));
	R(d, e, a, b, c, F3, K3, M(52));
	R(c, d, e, a, b, F3, K3, M(53));
	R(b, c, d, e, a, F3, K3, M(54));
	R(a, b, c, d, e, F3, K3, M(55));
	R(e, a, b, c, d, F3, K3, M(56));
	R(d, e, a, b, c, F3, K3, M(57));
	R(c, d, e, a, b, F3, K3, M(58));
	R(b, c, d, e, a, F3, K3, M(59));
	R(a, b, c, d, e, F4, K4, M(60));
	R(e, a, b, c, d, F4, K4, M(61));
	R(d, e, a, b, c, F4, K4, M(62));
	R(c, d, e, a, b, F4, K4, M(63));
	R(b, c, d, e, a, F4, K4, M(64));
	R(a, b, c, d, e, F4, K4, M(65));
	R(e, a, b, c, d, F4, K4, M(66));
	R(d, e, a, b, c, F4, K4, M(67));
	R(c, d, e, a, b, F4, K4, M(68));
	R(b, c, d, e, a, F4, K4, M(69));
	R(a, b, c, d, e, F4, K4, M(70));
	R(e, a, b, c, d, F4, K4, M(71));
	R(d, e, a, b, c, F4, K4, M(72));
	R(c, d, e, a, b, F4, K4, M(73));
	R(b, c, d, e, a, F4, K4, M(74));
	R(a, b, c, d, e, F4, K4, M(75));
	R(e, a, b, c, d, F4, K4, M(76));
	R(d, e, a, b, c, F4, K4, M(77));
	R(c, d, e, a, b, F4, K4, M(78));
	R(b, c, d, e, a, F4, K4, M(79));

	/* Update chaining vars. */
	hd->h0 += a;
	hd->h1 += b;
	hd->h2 += c;
	hd->h3 += d;
	hd->h4 += e;
}


/*
 * Update the message digest with the contents of INBUF with length INLEN.
 */
static void sha1_write(SHA1_CONTEXT *hd, const unsigned char *inbuf,
    size_t inlen)
{
	if (hd->count == 64) {
		/* Flush the buffer. */
		transform( hd, hd->buf );
		burn_stack(88 + 4 * sizeof(void *));
		hd->count = 0;
		hd->nblocks++;
	}
	
	if (!inbuf)
		return;
	
	if (hd->count) {
		for( ; inlen && hd->count < 64; inlen--)
			hd->buf[hd->count++] = *inbuf++;

		sha1_write(hd, NULL, 0);

		if (!inlen)
			return;
	}

	while (inlen >= 64) {
		transform(hd, inbuf);
		hd->count = 0;
		hd->nblocks++;
		inlen -= 64;
		inbuf += 64;
	}
	
	burn_stack(88 + 4 * sizeof(void *));

	for( ; inlen && hd->count < 64; inlen--)
		hd->buf[hd->count++] = *inbuf++;
}


/*
 * The routine final terminates the computation and returns the digest.  The
 * handle is prepared for a new cycle, but adding bytes to the handle will the
 * destroy the returned buffer.
 *
 * Returns: 20 bytes representing the digest.
 */

static void sha1_final(SHA1_CONTEXT *hd)
{
	uint32 t, msb, lsb;
	unsigned char *p;

	/* Flush. */
	sha1_write(hd, NULL, 0);

	t = hd->nblocks;
	
	/* Multiply by 64 to make a byte count. */
	lsb = t << 6;
	msb = t >> 26;

	/* Add the count. */
	t = lsb;
	if ((lsb += hd->count) < t)
		msb++;

	/* Multiply by 8 to make a bit count. */
	t = lsb;
	lsb <<= 3;
	msb <<= 3;
	msb |= t >> 29;

	if (hd->count < 56) {
		/* Enough room. */
		/* Pad. */
		hd->buf[hd->count++] = 0x80;

		while (hd->count < 56) {
			/* Pad. */
			hd->buf[hd->count++] = 0;
		}
	} else {
		/* Need one extra block. */
		/* Pad. */
		hd->buf[hd->count++] = 0x80;

		while (hd->count < 64)
			hd->buf[hd->count++] = 0;

		/* Flush. */		
		sha1_write(hd, NULL, 0);

		/* Fill next block with zeroes. */
		memset(hd->buf, 0, 56);
	}

	/* Append the 64 bit count. */
	hd->buf[56] = msb >> 24;
	hd->buf[57] = msb >> 16;
	hd->buf[58] = msb >>  8;
	hd->buf[59] = msb      ;
	hd->buf[60] = lsb >> 24;
	hd->buf[61] = lsb >> 16;
	hd->buf[62] = lsb >>  8;
	hd->buf[63] = lsb      ;

	transform(hd, hd->buf);
	burn_stack(88 + 4 * sizeof(void *));

	p = hd->buf;

#ifdef WORDS_BIGENDIAN
#	define X(a) do { *(uint32*)p = hd->h##a ; p += 4; } while(0)
#else
	/* Little endian. */
#	define X(a) do { *p++ = hd->h##a >> 24; *p++ = hd->h##a >> 16;	 \
		      *p++ = hd->h##a >> 8; *p++ = hd->h##a; } while(0)
#endif

	X(0);
	X(1);
	X(2);
	X(3);
	X(4);
#undef X
}

static unsigned char *sha1_read(SHA1_CONTEXT *hd)
{
	return(hd->buf);
}

/*
 * Return some information about the algorithm.  We need algo here to
 * distinguish different flavors of the algorithm.
 *
 * Returns: A pointer to string describing the algorithm or NULL if the ALGO is
 *          invalid.
 */
const char *sha1_get_info(int algo, size_t *contextsize,
    unsigned char **r_asnoid, int *r_asnlen, int *r_mdlen,
    void (**r_init)(void *c),
    void (**r_write)(void *c, unsigned char *buf, size_t nbytes),
    void (**r_final)(void *c), unsigned char *(**r_read)(void *c))
{
	/* Object ID is 1.3.14.3.2.26 */
	static unsigned char asn[15] = {
	    0x30, 0x21, 0x30, 0x09, 0x06, 0x05, 0x2b, 0x0e, 0x03, 0x02,
	    0x1a, 0x05, 0x00, 0x04, 0x14
	};
	
	if (algo != 2)
		return NULL;

	*contextsize = sizeof(SHA1_CONTEXT);
	*r_asnoid = asn;
	*r_asnlen = lenof(asn);
	*r_mdlen = 20;
	*(void (**)(SHA1_CONTEXT *))r_init                                 = sha1_init;
	*(void (**)(SHA1_CONTEXT *, const unsigned char *, size_t))r_write = sha1_write;
	*(void (**)(SHA1_CONTEXT *))r_final                                = sha1_final;
	*(unsigned char *(**)(SHA1_CONTEXT *))r_read                       = sha1_read;

	return("SHA1");
}

static char bin2hex[] = {
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c',
	'd', 'e', 'f'
};

/*
 * SHA-1 returns 160 bits of digest (20 bytes), which we turn into 40 hex
 * characters and a terminating \0.  Therefore buffer must be a preallocated
 * buffer of at least 41 bytes.
 */ 
void make_sha_hash(const char *passwd, char *buffer)
{
	SHA1_CONTEXT ctx;
	int i, j, high, low;
	unsigned char *shadigest;

	sha1_init(&ctx);
	sha1_write(&ctx, passwd, strlen(passwd));
	sha1_final(&ctx);
	shadigest = sha1_read(&ctx);

        for (i = 0, j = 0; i < 20; i++, j += 2) {
                high = shadigest[i] >> 4;
                low = shadigest[i] - (high << 4);
                buffer[j] = bin2hex[high];
                buffer[j + 1] = bin2hex[low];
        }

	buffer[40] = 0;
}

