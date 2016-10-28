#pragma once

#include "../def.h"
#include "acqstring.h"
#include "acqpath.h"

ACQ_NAMESPACE_START

ACQ_DLL_CLASS_DECLEAR(MD5)
{
public:
	MD5();

	void update(const void*, size_type);
	void update(const buffer_t&);
	void update(const path_t&);

	buffer_t& digest(buffer_t&);
	string_t& hex_digest(string_t&, int = 0);
	void reset();

private:
	void update(const uint8* input, size_type length);
	void final();
	void transform(const uint8 block[64]);
	void encode(const ulong* input, uint8* output, size_type length);
	void decode(const uint8* input, ulong* output, size_type length);

	/* class uncopyable */
	MD5(const MD5&);
	MD5& operator = (const MD5&);

private:
	ulong _state[4];	/* state (ABCD) */
	ulong _count[2];	/* number of bits, modulo 2^64 (low-order word first) */
	uint8 _buffer[64];	/* input buffer */
	uint8 _digest[16];	/* message digest */
	bool  _finished;	/* calculate finished ? */

	static const uint8 _PADDING[64];	/* padding for calculate */
	static const size_type _BUFFER_SIZE;
};

ACQ_NAMESPACE_END