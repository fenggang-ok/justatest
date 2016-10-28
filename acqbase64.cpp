#include "acqbase64.h"

ACQ_NAMESPACE_START

char* encode_3_bytes(const uint8* p_cursor, char encode_buf[5], const char* index)
{
	uint8 b0 = *p_cursor++;
	uint8 b1 = *p_cursor++;
	uint8 b2 = *p_cursor++;
	encode_buf[0] = index[b0 >> 2];
	encode_buf[1] = index[((b0 & 0x03) << 4) + (b1 >> 4)];
	encode_buf[2] = index[((b1 & 0x0F) << 2) + (b2 >> 6)];
	encode_buf[3] = index[b2 & 0x3F];
	encode_buf[4] = 0;
	return encode_buf;
}

int base64_encode(const buffer_t& src, string_t& encoded, const char* index)
{
	if (!src.size())
	{
		return 0;
	}
	char encode_buf[5] = {0};
	size_type src_len = src.size();
	int remain_num = src_len % 3;
	const uint8* p_end = src.end() - remain_num;
	encoded.erase();
	for (const uint8* p_cursor = src.begin(); p_cursor != p_end; p_cursor += 3)
	{
		encoded.append(encode_3_bytes(p_cursor, encode_buf, index));
	}
	if (remain_num)
	{
		uint8 remain[] = {*p_end, 0, 0};
		if (remain_num == 2)
		{
			remain[1] = *(p_end + 1);
		}
		encoded.append(encode_3_bytes(remain, encode_buf, index));
		encoded[-1] = NORMAL_PAD64;
		if (remain_num == 1)
		{
			encoded[-2] = NORMAL_PAD64;
		}
	}
	return 1;
}

int base64_decode(const string_t& encoded, buffer_t& decoded, const uint8* index)
{
	const char* p_cursor = encoded.begin();
	uint8 decode_buf[3] = {0};
	uint8 tmp[4];
	int i = 0;
	do
	{
		char  c0 = *p_cursor++;
		uint8 v0 = index[c0];
		if (find_char(c0, STR_CLOTHES) == nopos && c0 != NORMAL_PAD64)
		{
			tmp[i++] = v0;
			if (i == 4)
			{
				decode_buf[0] = (tmp[0] << 2) + ((tmp[1] & 0x30) >> 4);
				decode_buf[1] = ((tmp[1] & 0xF) << 4) + ((tmp[2] & 0x3C) >> 2);
				decode_buf[2] = ((tmp[2] & 0x3) << 6) + tmp[3];
				decoded.append(decode_buf, 3, 1.5 * encoded.size());
				i = 0;
			}
		}
		else if (c0 == NORMAL_PAD64)
		{
			switch (i)
			{
			case 0:
				break;
			case 1:
				decode_buf[0] = tmp[0] << 2;
				decoded.append(decode_buf, 1);
				break;
			case 2:
				decode_buf[0] = (tmp[0] << 2) + ((tmp[1] & 0x30) >> 4);
				decoded.append(decode_buf, 1);
				break;
			case 3:
				decode_buf[0] = (tmp[0] << 2) + ((tmp[1] & 0x30) >> 4);
				decode_buf[1] = ((tmp[1] & 0xF) << 4) + ((tmp[2] & 0x3C) >> 2);
				decoded.append(decode_buf, 2);
				break;
			}
			return 1;
		}
	}while (p_cursor != encoded.end());
	return 1;
}

ACQ_NAMESPACE_END