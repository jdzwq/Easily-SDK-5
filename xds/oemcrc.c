
/*
* Copyright 2001-2010 Georges Menie (www.menie.org)
* Copyright 2010-2012 Salvatore Sanfilippo (adapted to Redis coding style)
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of the University of California, Berkeley nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/* CRC16 implementation according to CCITT standards.
*
* Note by @antirez: this is actually the XMODEM CRC 16 algorithm, using the
* following parameters:
*
* Name                       : "XMODEM", also known as "ZMODEM", "CRC-16/ACORN"
* Width                      : 16 bit
* Poly                       : 1021 (That is actually x^16 + x^12 + x^5 + 1)
* Initialization             : 0000
* Reflect Input byte         : False
* Reflect Output CRC         : False
* Xor constant to output CRC : 0000
* Output for "123456789"     : 31C3
*/

#include "oemcrc.h"

static const uint16_t crc16tab[256] = {
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
	0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
	0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
	0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
	0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
	0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
	0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
	0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
	0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
	0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
	0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
	0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
	0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
	0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
	0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
	0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
	0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
	0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
	0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
	0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
	0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
	0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
	0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
	0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
	0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
	0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
	0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
	0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
	0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
	0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
	0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
	0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};

uint16_t crc16(const unsigned char *s, uint16_t l) {
	int counter;
	uint16_t crc = 0;
	for (counter = 0; counter < l; counter++)
		crc = (crc << 8) ^ crc16tab[((crc >> 8) ^ *s++) & 0x00FF];
	return crc;
}

static const uint32_t crc32tab[256] = {
	0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
	0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
	0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
	0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
	0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
	0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
	0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
	0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
	0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
	0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
	0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
	0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
	0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
	0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
	0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
	0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
	0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
	0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
	0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
	0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
	0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
	0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
	0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
	0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
	0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
	0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
	0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
	0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
	0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
	0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
	0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
	0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
	0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
	0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
	0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
	0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
	0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
	0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
	0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
	0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
	0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
	0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
	0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
	0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
	0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
	0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
	0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
	0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
	0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
	0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
	0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
	0x2d02ef8dL
};


/* ========================================================================= */
#define DO1(buf) crc = crc32tab[((int)crc ^ (*buf++)) & 0xff] ^ (crc >> 8);
#define DO2(buf)  DO1(buf); DO1(buf);
#define DO4(buf)  DO2(buf); DO2(buf);
#define DO8(buf)  DO4(buf); DO4(buf);

uint32_t crc32(uint32_t crc, const unsigned char *s, uint32_t l) {
	crc = crc ^ 0xffffffffL;
	while (l >= 8)
	{
		DO8(s);
		l -= 8;
	}
	if (l) do {
		DO1(s);
	} while (--l);
	return crc ^ 0xffffffffL;
}

static const uint64_t crc64_tab[256] = {
	UINT64_C(0x0000000000000000), UINT64_C(0x7ad870c830358979),
	UINT64_C(0xf5b0e190606b12f2), UINT64_C(0x8f689158505e9b8b),
	UINT64_C(0xc038e5739841b68f), UINT64_C(0xbae095bba8743ff6),
	UINT64_C(0x358804e3f82aa47d), UINT64_C(0x4f50742bc81f2d04),
	UINT64_C(0xab28ecb46814fe75), UINT64_C(0xd1f09c7c5821770c),
	UINT64_C(0x5e980d24087fec87), UINT64_C(0x24407dec384a65fe),
	UINT64_C(0x6b1009c7f05548fa), UINT64_C(0x11c8790fc060c183),
	UINT64_C(0x9ea0e857903e5a08), UINT64_C(0xe478989fa00bd371),
	UINT64_C(0x7d08ff3b88be6f81), UINT64_C(0x07d08ff3b88be6f8),
	UINT64_C(0x88b81eabe8d57d73), UINT64_C(0xf2606e63d8e0f40a),
	UINT64_C(0xbd301a4810ffd90e), UINT64_C(0xc7e86a8020ca5077),
	UINT64_C(0x4880fbd87094cbfc), UINT64_C(0x32588b1040a14285),
	UINT64_C(0xd620138fe0aa91f4), UINT64_C(0xacf86347d09f188d),
	UINT64_C(0x2390f21f80c18306), UINT64_C(0x594882d7b0f40a7f),
	UINT64_C(0x1618f6fc78eb277b), UINT64_C(0x6cc0863448deae02),
	UINT64_C(0xe3a8176c18803589), UINT64_C(0x997067a428b5bcf0),
	UINT64_C(0xfa11fe77117cdf02), UINT64_C(0x80c98ebf2149567b),
	UINT64_C(0x0fa11fe77117cdf0), UINT64_C(0x75796f2f41224489),
	UINT64_C(0x3a291b04893d698d), UINT64_C(0x40f16bccb908e0f4),
	UINT64_C(0xcf99fa94e9567b7f), UINT64_C(0xb5418a5cd963f206),
	UINT64_C(0x513912c379682177), UINT64_C(0x2be1620b495da80e),
	UINT64_C(0xa489f35319033385), UINT64_C(0xde51839b2936bafc),
	UINT64_C(0x9101f7b0e12997f8), UINT64_C(0xebd98778d11c1e81),
	UINT64_C(0x64b116208142850a), UINT64_C(0x1e6966e8b1770c73),
	UINT64_C(0x8719014c99c2b083), UINT64_C(0xfdc17184a9f739fa),
	UINT64_C(0x72a9e0dcf9a9a271), UINT64_C(0x08719014c99c2b08),
	UINT64_C(0x4721e43f0183060c), UINT64_C(0x3df994f731b68f75),
	UINT64_C(0xb29105af61e814fe), UINT64_C(0xc849756751dd9d87),
	UINT64_C(0x2c31edf8f1d64ef6), UINT64_C(0x56e99d30c1e3c78f),
	UINT64_C(0xd9810c6891bd5c04), UINT64_C(0xa3597ca0a188d57d),
	UINT64_C(0xec09088b6997f879), UINT64_C(0x96d1784359a27100),
	UINT64_C(0x19b9e91b09fcea8b), UINT64_C(0x636199d339c963f2),
	UINT64_C(0xdf7adabd7a6e2d6f), UINT64_C(0xa5a2aa754a5ba416),
	UINT64_C(0x2aca3b2d1a053f9d), UINT64_C(0x50124be52a30b6e4),
	UINT64_C(0x1f423fcee22f9be0), UINT64_C(0x659a4f06d21a1299),
	UINT64_C(0xeaf2de5e82448912), UINT64_C(0x902aae96b271006b),
	UINT64_C(0x74523609127ad31a), UINT64_C(0x0e8a46c1224f5a63),
	UINT64_C(0x81e2d7997211c1e8), UINT64_C(0xfb3aa75142244891),
	UINT64_C(0xb46ad37a8a3b6595), UINT64_C(0xceb2a3b2ba0eecec),
	UINT64_C(0x41da32eaea507767), UINT64_C(0x3b024222da65fe1e),
	UINT64_C(0xa2722586f2d042ee), UINT64_C(0xd8aa554ec2e5cb97),
	UINT64_C(0x57c2c41692bb501c), UINT64_C(0x2d1ab4dea28ed965),
	UINT64_C(0x624ac0f56a91f461), UINT64_C(0x1892b03d5aa47d18),
	UINT64_C(0x97fa21650afae693), UINT64_C(0xed2251ad3acf6fea),
	UINT64_C(0x095ac9329ac4bc9b), UINT64_C(0x7382b9faaaf135e2),
	UINT64_C(0xfcea28a2faafae69), UINT64_C(0x8632586aca9a2710),
	UINT64_C(0xc9622c4102850a14), UINT64_C(0xb3ba5c8932b0836d),
	UINT64_C(0x3cd2cdd162ee18e6), UINT64_C(0x460abd1952db919f),
	UINT64_C(0x256b24ca6b12f26d), UINT64_C(0x5fb354025b277b14),
	UINT64_C(0xd0dbc55a0b79e09f), UINT64_C(0xaa03b5923b4c69e6),
	UINT64_C(0xe553c1b9f35344e2), UINT64_C(0x9f8bb171c366cd9b),
	UINT64_C(0x10e3202993385610), UINT64_C(0x6a3b50e1a30ddf69),
	UINT64_C(0x8e43c87e03060c18), UINT64_C(0xf49bb8b633338561),
	UINT64_C(0x7bf329ee636d1eea), UINT64_C(0x012b592653589793),
	UINT64_C(0x4e7b2d0d9b47ba97), UINT64_C(0x34a35dc5ab7233ee),
	UINT64_C(0xbbcbcc9dfb2ca865), UINT64_C(0xc113bc55cb19211c),
	UINT64_C(0x5863dbf1e3ac9dec), UINT64_C(0x22bbab39d3991495),
	UINT64_C(0xadd33a6183c78f1e), UINT64_C(0xd70b4aa9b3f20667),
	UINT64_C(0x985b3e827bed2b63), UINT64_C(0xe2834e4a4bd8a21a),
	UINT64_C(0x6debdf121b863991), UINT64_C(0x1733afda2bb3b0e8),
	UINT64_C(0xf34b37458bb86399), UINT64_C(0x8993478dbb8deae0),
	UINT64_C(0x06fbd6d5ebd3716b), UINT64_C(0x7c23a61ddbe6f812),
	UINT64_C(0x3373d23613f9d516), UINT64_C(0x49aba2fe23cc5c6f),
	UINT64_C(0xc6c333a67392c7e4), UINT64_C(0xbc1b436e43a74e9d),
	UINT64_C(0x95ac9329ac4bc9b5), UINT64_C(0xef74e3e19c7e40cc),
	UINT64_C(0x601c72b9cc20db47), UINT64_C(0x1ac40271fc15523e),
	UINT64_C(0x5594765a340a7f3a), UINT64_C(0x2f4c0692043ff643),
	UINT64_C(0xa02497ca54616dc8), UINT64_C(0xdafce7026454e4b1),
	UINT64_C(0x3e847f9dc45f37c0), UINT64_C(0x445c0f55f46abeb9),
	UINT64_C(0xcb349e0da4342532), UINT64_C(0xb1eceec59401ac4b),
	UINT64_C(0xfebc9aee5c1e814f), UINT64_C(0x8464ea266c2b0836),
	UINT64_C(0x0b0c7b7e3c7593bd), UINT64_C(0x71d40bb60c401ac4),
	UINT64_C(0xe8a46c1224f5a634), UINT64_C(0x927c1cda14c02f4d),
	UINT64_C(0x1d148d82449eb4c6), UINT64_C(0x67ccfd4a74ab3dbf),
	UINT64_C(0x289c8961bcb410bb), UINT64_C(0x5244f9a98c8199c2),
	UINT64_C(0xdd2c68f1dcdf0249), UINT64_C(0xa7f41839ecea8b30),
	UINT64_C(0x438c80a64ce15841), UINT64_C(0x3954f06e7cd4d138),
	UINT64_C(0xb63c61362c8a4ab3), UINT64_C(0xcce411fe1cbfc3ca),
	UINT64_C(0x83b465d5d4a0eece), UINT64_C(0xf96c151de49567b7),
	UINT64_C(0x76048445b4cbfc3c), UINT64_C(0x0cdcf48d84fe7545),
	UINT64_C(0x6fbd6d5ebd3716b7), UINT64_C(0x15651d968d029fce),
	UINT64_C(0x9a0d8ccedd5c0445), UINT64_C(0xe0d5fc06ed698d3c),
	UINT64_C(0xaf85882d2576a038), UINT64_C(0xd55df8e515432941),
	UINT64_C(0x5a3569bd451db2ca), UINT64_C(0x20ed197575283bb3),
	UINT64_C(0xc49581ead523e8c2), UINT64_C(0xbe4df122e51661bb),
	UINT64_C(0x3125607ab548fa30), UINT64_C(0x4bfd10b2857d7349),
	UINT64_C(0x04ad64994d625e4d), UINT64_C(0x7e7514517d57d734),
	UINT64_C(0xf11d85092d094cbf), UINT64_C(0x8bc5f5c11d3cc5c6),
	UINT64_C(0x12b5926535897936), UINT64_C(0x686de2ad05bcf04f),
	UINT64_C(0xe70573f555e26bc4), UINT64_C(0x9ddd033d65d7e2bd),
	UINT64_C(0xd28d7716adc8cfb9), UINT64_C(0xa85507de9dfd46c0),
	UINT64_C(0x273d9686cda3dd4b), UINT64_C(0x5de5e64efd965432),
	UINT64_C(0xb99d7ed15d9d8743), UINT64_C(0xc3450e196da80e3a),
	UINT64_C(0x4c2d9f413df695b1), UINT64_C(0x36f5ef890dc31cc8),
	UINT64_C(0x79a59ba2c5dc31cc), UINT64_C(0x037deb6af5e9b8b5),
	UINT64_C(0x8c157a32a5b7233e), UINT64_C(0xf6cd0afa9582aa47),
	UINT64_C(0x4ad64994d625e4da), UINT64_C(0x300e395ce6106da3),
	UINT64_C(0xbf66a804b64ef628), UINT64_C(0xc5bed8cc867b7f51),
	UINT64_C(0x8aeeace74e645255), UINT64_C(0xf036dc2f7e51db2c),
	UINT64_C(0x7f5e4d772e0f40a7), UINT64_C(0x05863dbf1e3ac9de),
	UINT64_C(0xe1fea520be311aaf), UINT64_C(0x9b26d5e88e0493d6),
	UINT64_C(0x144e44b0de5a085d), UINT64_C(0x6e963478ee6f8124),
	UINT64_C(0x21c640532670ac20), UINT64_C(0x5b1e309b16452559),
	UINT64_C(0xd476a1c3461bbed2), UINT64_C(0xaeaed10b762e37ab),
	UINT64_C(0x37deb6af5e9b8b5b), UINT64_C(0x4d06c6676eae0222),
	UINT64_C(0xc26e573f3ef099a9), UINT64_C(0xb8b627f70ec510d0),
	UINT64_C(0xf7e653dcc6da3dd4), UINT64_C(0x8d3e2314f6efb4ad),
	UINT64_C(0x0256b24ca6b12f26), UINT64_C(0x788ec2849684a65f),
	UINT64_C(0x9cf65a1b368f752e), UINT64_C(0xe62e2ad306bafc57),
	UINT64_C(0x6946bb8b56e467dc), UINT64_C(0x139ecb4366d1eea5),
	UINT64_C(0x5ccebf68aecec3a1), UINT64_C(0x2616cfa09efb4ad8),
	UINT64_C(0xa97e5ef8cea5d153), UINT64_C(0xd3a62e30fe90582a),
	UINT64_C(0xb0c7b7e3c7593bd8), UINT64_C(0xca1fc72bf76cb2a1),
	UINT64_C(0x45775673a732292a), UINT64_C(0x3faf26bb9707a053),
	UINT64_C(0x70ff52905f188d57), UINT64_C(0x0a2722586f2d042e),
	UINT64_C(0x854fb3003f739fa5), UINT64_C(0xff97c3c80f4616dc),
	UINT64_C(0x1bef5b57af4dc5ad), UINT64_C(0x61372b9f9f784cd4),
	UINT64_C(0xee5fbac7cf26d75f), UINT64_C(0x9487ca0fff135e26),
	UINT64_C(0xdbd7be24370c7322), UINT64_C(0xa10fceec0739fa5b),
	UINT64_C(0x2e675fb4576761d0), UINT64_C(0x54bf2f7c6752e8a9),
	UINT64_C(0xcdcf48d84fe75459), UINT64_C(0xb71738107fd2dd20),
	UINT64_C(0x387fa9482f8c46ab), UINT64_C(0x42a7d9801fb9cfd2),
	UINT64_C(0x0df7adabd7a6e2d6), UINT64_C(0x772fdd63e7936baf),
	UINT64_C(0xf8474c3bb7cdf024), UINT64_C(0x829f3cf387f8795d),
	UINT64_C(0x66e7a46c27f3aa2c), UINT64_C(0x1c3fd4a417c62355),
	UINT64_C(0x935745fc4798b8de), UINT64_C(0xe98f353477ad31a7),
	UINT64_C(0xa6df411fbfb21ca3), UINT64_C(0xdc0731d78f8795da),
	UINT64_C(0x536fa08fdfd90e51), UINT64_C(0x29b7d047efec8728),
};

uint64_t crc64(uint64_t crc, const unsigned char *s, uint64_t l) {
	uint64_t j;

	for (j = 0; j < l; j++) {
		uint8_t byte = s[j];
		crc = crc64_tab[(uint8_t)crc ^ byte] ^ (crc >> 8);
	}
	return crc;
}
