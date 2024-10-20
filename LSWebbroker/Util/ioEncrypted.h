
#ifndef _ioEncrypted_h_
#define _ioEncrypted_h_

#define DATA_LEN 16
#define SEED_USER_KEY_LEN 16 

namespace ioEncrypted
{
	enum NationType
	{
		NT_KOREA = 0,
		NT_TAIWAN = 1,
		NT_MAX,
	};
	bool Encode15(const char* szPlain, const char* szUserKey, char* szCipher, NationType eNationType = NT_KOREA);
	bool Decode15(char* szCipher, char* szUserKey, char* szPlain, NationType eNationType = NT_KOREA);

} // end namespace ioEncrypted
#endif