

#include "../stdafx.h"
#include "ioEncrypted.h"
#include "seed.h"

namespace ioEncrypted
{

	bool Encode15(const char* szPlain, const char* szUserKey, char* szCipher, NationType eNationType /*= NT_KOREA */)
	{
		// TODO: Add your implementation code here
		SEED_ALG_INFO	AlgInfo;
		BYTE IV[NT_MAX][SEED_BLOCK_LEN] =
		{
			56,170,255,3,4,78,6,54,8,222,10,123,19,88,14,1,       // NT_KOREA
			57,171,215,31,14,88,7,4,48,122,30,153,39,98,64,10     // NT_TAIWAN
		};
		BYTE UserKey[SEED_USER_KEY_LEN] = "";
		BYTE Cipher[DATA_LEN + 32] = "";
		BYTE Plain[DATA_LEN + 32] = "";
		DWORD cipherLen;

		int plainLen = strlen(szPlain);

		if (plainLen >= DATA_LEN)
		{
			printf("Encode15 - Error -1\n");
			return false;
		}

		strcpy_s((char*)Plain, DATA_LEN + 32, szPlain);

		SEED_SetAlgInfo(AI_CFB, AI_NO_PADDING, IV[eNationType], &AlgInfo);

		int LenUserKey = strlen(szUserKey);

		if (LenUserKey >= SEED_USER_KEY_LEN)
		{
			printf("Encode15 - Error -2\n");
			return false;
		}

		strcpy_s((char*)UserKey, SEED_USER_KEY_LEN, szUserKey);

		if (SEED_KeySchedule(UserKey, SEED_USER_KEY_LEN, &AlgInfo) != CTR_SUCCESS)
		{
			printf("Encode15 - Error -3\n");
			return false;
		}

		if (SEED_EncInit(&AlgInfo) != CTR_SUCCESS)
		{
			printf("Encode15 - Error -4\n");
			return false;
		}

		cipherLen = 0;
		if (SEED_EncUpdate(&AlgInfo, Plain, DATA_LEN, Cipher, &cipherLen) != CTR_SUCCESS)
		{
			printf("Encode15 - Error -5\n");
			return false;
		}

		DWORD filnalCipherLen = 0;
		if (SEED_EncFinal(&AlgInfo, Cipher + cipherLen, &filnalCipherLen) != CTR_SUCCESS)
		{
			printf("Encode15 - Error -6\n");
			return false;
		}

		cipherLen += filnalCipherLen;

		// char를 16진수로 변경
		char szTemp[MAX_PATH] = "";
		for (int i = 0; i < plainLen; i++)
		{
			char szTempHex[MAX_PATH] = "";
			wsprintf(szTempHex, "%02x", Cipher[i]);
			strcat_s(szTemp, szTempHex);
		}
		strcpy_s(szCipher, sizeof(szTemp), szTemp);
		return true;
	}

	bool Decode15(char* szCipher, char* szUserKey, char* szPlain, NationType eNationType /*= NT_KOREA */)
	{
		// TODO: Add your implementation code here
		SEED_ALG_INFO	AlgInfo;
		BYTE IV[NT_MAX][SEED_BLOCK_LEN] =
		{
			56,170,255,3,4,78,6,54,8,222,10,123,19,88,14,1,       // NT_KOREA
			57,171,215,31,14,88,7,4,48,122,30,153,39,98,64,10     // NT_TAIWAN
		};
		BYTE UserKey[SEED_USER_KEY_LEN] = "";
		BYTE Cipher[DATA_LEN] = "";
		BYTE Plain[DATA_LEN] = "";
		DWORD plainLen;

		int decodeLen = strlen(szCipher) / 2;

		if (decodeLen > DATA_LEN)
		{
			printf("Decode15 - Error -1\n");
			return false;
		}

		// 16진수 -> char
		int pos = 0;
		for (int i = 0; i < decodeLen; i++)
		{
			char szTempOneHex[MAX_PATH] = "";
			char* stopstring;
			memcpy(szTempOneHex, &szCipher[pos], 2);
			pos += 2;
			Cipher[i] = (BYTE)strtol(szTempOneHex, &stopstring, 16);
		}

		SEED_SetAlgInfo(AI_CFB, AI_NO_PADDING, IV[eNationType], &AlgInfo);

		int LenUserKey = strlen(szUserKey);

		if (LenUserKey >= SEED_USER_KEY_LEN)
		{
			printf("Decode15 - Error -2\n");
			return false;
		}

		strncpy_s((char*)UserKey, SEED_USER_KEY_LEN, szUserKey, SEED_USER_KEY_LEN);

		if (SEED_KeySchedule(UserKey, SEED_USER_KEY_LEN, &AlgInfo) != CTR_SUCCESS)
		{
			printf("Decode15 - Error -3\n");
			return false;
		}

		if (SEED_DecInit(&AlgInfo) != CTR_SUCCESS)
		{
			printf("Decode15 - Error -4\n");
			return false;
		}

		plainLen = 0;
		if (SEED_DecUpdate(&AlgInfo, Cipher, decodeLen, Plain, &plainLen) != CTR_SUCCESS)
		{
			printf("Decode15 - Error -5\n");
			return false;
		}

		DWORD finalPlainLen = 0;
		if (SEED_DecFinal(&AlgInfo, Plain + plainLen, &finalPlainLen) != CTR_SUCCESS)
		{
			printf("Decode15 - Error -6\n");
			return false;
		}
		strncpy(szPlain, (char*)Plain, decodeLen);

		return true;
	}

} // end namespace ioEncrypted