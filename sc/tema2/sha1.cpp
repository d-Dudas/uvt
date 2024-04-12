
//	*********************************************************************
//	*  sha1.cpp : cpp source for the hash function sha-1 implemntation  *
//	*  the implementation is based on the specification doc fips 180-1  *
//	*********************************************************************

#include "sha1.h"
#include <string.h>
#include <stdio.h>

//	==================================================================
//	methods to be implemented
//	------------------------------------------------------------------

void initHs() {
	// load the initial values for H's
	H[0] = 0x67452301;
	H[1] = 0xefcdab89;
	H[2] = 0x98badcfe;
	H[3] = 0x10325476;
	H[4] = 0xc3d2e1f0;
}

//	------------------------------------------------------------------

void initKs() {
	// load the initial values for K's
	int i;
	for (i=00; i<20 ; i++)   K[i] = 0x5a827999;
	for (i=20; i<40 ; i++)   K[i] = 0x6ed9eba1;
	for (i=40; i<60 ; i++)   K[i] = 0x8f1bbcdc;
	for (i=60; i<80 ; i++)   K[i] = 0xca62c1d6;
}

//	------------------------------------------------------------------

int getMessage(int k) {
	// copy message k from msg[k] to binmsg
    int msgLen = strlen(msg[k]);
    memcpy(binmsg, msg[k], msgLen);
    return msgLen;
}

//	------------------------------------------------------------------

int paddMessage(int messLen) {
	// pad the binmsg array according to the specification
	// return the number of blocks of the padded message
}

//	------------------------------------------------------------------

void getWsfromM(int currentBlock) {
	// fill out the W[] from the current block
}

//	------------------------------------------------------------------

void getAsfromHs() {
	// initialize A, B, C, D, E from H's
}

//	------------------------------------------------------------------

void displayDigest(uint32 H[]) {
	printf(" digest value - %9x %9x %9x %9x %9x\n\n",
		H[0], H[1], H[2], H[3], H[4]);
}

//	==================================================================

int main(int argc, char* argv[]) {
	int i, j, k;
	int messLen;
	int numBlocks;

	// parse all 3 test messages
	for (k=0; k<3; k++) {
		initKs();
		initHs();

		if (k!=2) {
            messLen = getMessage(k);
		} else {
            // third message is much bigger
            memset((void *)(binmsg), 'a', 1000000);
            mesLen = 1000000;
		}
		numBlocks = paddMessage(messLen);

		for (i=0; i<numBlocks; i++) {
			getWsfromM(i);

			for (j=16; j<80; j++) {
				W[j] = W[j-3]^W[j-8]^W[j-14]^W[j-16];
				W[j] = S(W[j], 1);
			}

			getAsfromHs();

			for (j=0; j<80; j++) {
				TEMP = S(A,5);
				int cit = j / 20;

				switch (cit) {
				case 0:
					TEMP += F0(B,C,D);
					break;
				case 1:
					TEMP += F1(B,C,D);
					break;
				case 2:
					TEMP += F2(B,C,D);
					break;
				case 3:
					TEMP += F3(B,C,D);
					break;
				}

				TEMP = TEMP + E + W[j] + K[j];

				E=D;
				D=C;
				C=S(B,30);
				B=A;
				A=TEMP;
//				printf("%9x %9x %9x %9x %9x\n", A, B, C, D, E);
			}

			H[0] = H[0] + A;
			H[1] = H[1] + B;
			H[2] = H[2] + C;
			H[3] = H[3] + D;
			H[4] = H[4] + E;
		}
        displayDigest(H);
	}
	return 0;
}

//	******************************************************************
