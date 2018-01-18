#include "common.h"
#include "storm.h"
#include <base/hook/fp_call.h>
#include <assert.h>

namespace base { namespace warcraft3 { namespace lua_engine { namespace bignum {

#include "sha_1.h"	 
#include "sha_1.inl"

	namespace sbig {
		static void Del(HANDLE big)
		{
			static uintptr_t func = storm_s::instance().get_proc(606);
			std_call<void>(func, big);
		}

		static void FromBinary(HANDLE big, const char* buf, size_t len)
		{
			static uintptr_t func = storm_s::instance().get_proc(609);
			std_call<void>(func, big, buf, len);
		}

		static void New(HANDLE* big)
		{
			static uintptr_t func = storm_s::instance().get_proc(624);
			std_call<void>(func, big);
		}

		static void PowMod(HANDLE dst, HANDLE src, HANDLE pow, HANDLE mod)
		{
			static uintptr_t func = storm_s::instance().get_proc(628);
			std_call<void>(func, dst, src, pow, mod);
		}

		static void ToBinaryBuffer(HANDLE big, char* buf, size_t len, size_t* wlen)
		{
			static uintptr_t func = storm_s::instance().get_proc(638);
			std_call<void>(func, big, buf, len, wlen);
		}
	}

	namespace lbig {
		static HANDLE create(lua_State* L)
		{
			HANDLE* big = (HANDLE*)lua_newuserdata(L, sizeof(HANDLE));
			luaL_getmetatable(L, "jbignum_t");
			lua_setmetatable(L, -2);
			sbig::New(big);
			return *big;
		}

		static int new_(lua_State* L)
		{
			HANDLE big = create(L);
			size_t len = 0;
			const char* buf = lua_tolstring(L, 1, &len);
			sbig::FromBinary(big, buf, len);
			return 1;
		}

		static int gc(lua_State* L)
		{
			HANDLE* big = (HANDLE*)lua_touserdata(L, 1);
			sbig::Del(*big);
			*big = 0;
			return 0;
		}

		static int tostring(lua_State* L)
		{
			HANDLE big = *(HANDLE*)lua_touserdata(L, 1);
			size_t len = 0x100;
			size_t wlen = 0;
			char* buf = 0;
			for (;;) {
				buf = (char*)lua_newuserdata(L, len + 1);
				wlen = 0;
				sbig::ToBinaryBuffer(big, buf, len + 1, &wlen);
				if (wlen < len + 1) {
					break;
				}
				lua_pop(L, 1);
				len *= 2;
			} 
			lua_pushlstring(L, buf, wlen);
			return 1;
		}

		static int len(lua_State* L)
		{
			HANDLE big = *(HANDLE*)lua_touserdata(L, 1);
			size_t len = 0x100;
			size_t wlen = 0;
			char* buf = 0;
			for (;;) {
				buf = (char*)lua_newuserdata(L, len + 1);
				wlen = 0;
				sbig::ToBinaryBuffer(big, buf, len + 1, &wlen);
				if (wlen < len + 1) {
					break;
				}
				lua_pop(L, 1);
				len *= 2;
			}
			lua_pushinteger(L, wlen);
			return 1;
		}

		static int powmod(lua_State* L)
		{
			HANDLE src = *(HANDLE*)lua_touserdata(L, 1);
			HANDLE pow = *(HANDLE*)lua_touserdata(L, 2);
			HANDLE mod = *(HANDLE*)lua_touserdata(L, 3);
			HANDLE dst = create(L);
			sbig::PowMod(dst, src, pow, mod);
			return 1;
		}
	}	   

	static int lbin(lua_State* L)
	{
		static unsigned char binstr[] = {
			/*         0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F */
			/* 0x00 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			/* 0x10 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			/* 0x20 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			/* 0x30 */ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			/* 0x40 */ 0x00, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			/* 0x50 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			/* 0x60 */ 0x00, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			/* 0x70 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			/* 0x80 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			/* 0x90 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			/* 0xA0 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			/* 0xB0 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			/* 0xC0 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			/* 0xD0 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			/* 0xE0 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			/* 0xF0 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		};
		size_t hexlen = 0;
		const char* hex = lua_tolstring(L, 1, &hexlen);
		assert(hexlen % 2 == 0);
		size_t binlen = hexlen / 2;
		char* bin = (char*)lua_newuserdata(L, binlen);
		for (size_t i = 0; i < binlen; ++i)
		{
			bin[binlen - i - 1] = (binstr[hex[2 * i + 0]] << 4) + binstr[hex[2 * i + 1]];
		} 	
		lua_pushlstring(L, bin, binlen);
		return 1;
	}

	static int lhex(lua_State* L)
	{
		static char hexstr[] = "0123456789ABCDEF";
		size_t binlen = 0;
		const char* bin = lua_tolstring(L, 1, &binlen);
		size_t hexlen = binlen * 2;
		char* hex = (char*)lua_newuserdata(L, hexlen);
		for (size_t i = 0; i < binlen; ++i)
		{
			hex[hexlen - 2 * i - 2] = hexstr[(bin[i] >> 4) & 0xF];
			hex[hexlen - 2 * i - 1] = hexstr[bin[i] & 0xF];
		}
		lua_pushlstring(L, hex, hexlen);
		return 1;
	}

	static int lsha1(lua_State* L)
	{
		size_t len = 0;
		const unsigned char* buf = (const unsigned char*)lua_tolstring(L, 1, &len);
		unsigned char sha1out[SHA1HashSize] = { 0 };
		SHA1Context context;
		SHA1Reset(&context);
		SHA1Input(&context, buf, len);
		SHA1Result(&context, sha1out);
		lua_pushlstring(L, (const char*)sha1out, SHA1HashSize);
		return 1;
	}
	int open(lua_State* L)
	{
		luaL_Reg meta[] = {
			{ "__len", lbig::len },
			{ "__tostring", lbig::tostring },
			{ "__gc", lbig::gc },
			{ "powmod", lbig::powmod },
			{ NULL, NULL },
		};
		luaL_newmetatable(L, "jbignum_t");
		lua_pushvalue(L, -1);
		lua_setfield(L, -2, "__index");
		luaL_setfuncs(L, meta, 0);
		lua_pop(L, 1);

		lua_newtable(L);
		luaL_Reg lib[] = {
			{ "new", lbig::new_ },
			{ "bin", lbin },
			{ "hex", lhex },
			{ "sha1", lsha1 },
			{ NULL, NULL },
		};
		luaL_setfuncs(L, lib, 0);
		return 1;
	}

	static unsigned char rsa_e[] = { 0x01, 0x00, 0x01, 0x00 };
	static unsigned char rsa_n[] = { 
		0xB1, 0x7C, 0xA8, 0xF6, 0x08, 0xC0, 0x3D, 0x5E, 0xFC, 0x4D, 0xDA, 0x62, 0x2A, 0x31, 0x87, 0x87,
		0x46, 0xDE, 0x48, 0xF6, 0x28, 0x87, 0xBD, 0xE1, 0x91, 0x26, 0x13, 0x4A, 0x7D, 0x4E, 0x9E, 0xD6,
		0x7E, 0x5A, 0x5D, 0x7D, 0x14, 0x62, 0x52, 0x65, 0x41, 0x6E, 0x2E, 0x7B, 0xF9, 0xA7, 0xEE, 0x4C,
		0x5F, 0x89, 0x02, 0x3B, 0x4B, 0xED, 0xD3, 0x8A, 0xAD, 0x71, 0xCA, 0x8A, 0x8B, 0x0D, 0x53, 0x34,
		0x53, 0x67, 0x25, 0xAB, 0x2F, 0xA3, 0xEA, 0xAF, 0xB5, 0x6A, 0x25, 0xC4, 0x35, 0x1A, 0x4F, 0xCB,
		0xC2, 0x26, 0x14, 0xFA, 0x8E, 0xE2, 0x1D, 0x81, 0x40, 0x55, 0x20, 0x11, 0x5B, 0xA1, 0xE4, 0x6C,
		0x70, 0x1F, 0x8D, 0x67, 0xF7, 0x83, 0x7B, 0xE4, 0x51, 0x08, 0xDC, 0x3E, 0x0E, 0x85, 0xAF, 0x0A,
		0x14, 0x4E, 0x2B, 0x9D, 0xE1, 0xD6, 0x96, 0xF3, 0xF5, 0xFB, 0x5C, 0xCD, 0x16, 0x72, 0x85, 0x8B,
		0x6D, 0xEB, 0xB4, 0x2E, 0x38, 0xD4, 0x10, 0x61, 0x7B, 0x98, 0x60, 0xED, 0x22, 0x6F, 0xA4, 0x46,
		0x89, 0x19, 0x78, 0x26, 0xF9, 0xF7, 0x46, 0x87, 0x7A, 0xA8, 0x37, 0x7B, 0xDD, 0xA3, 0xC9, 0x04,
		0x76, 0x0F, 0x3D, 0x5E, 0xC7, 0xD8, 0x05, 0x88, 0x12, 0x70, 0xB0, 0x9E, 0x55, 0xC8, 0x31, 0x5D,
		0x9F, 0xBF, 0x42, 0x75, 0x30, 0x8A, 0x99, 0x2F, 0x87, 0x71, 0x3E, 0x1B, 0xBC, 0x09, 0xBB, 0x5C,
		0xD6, 0x57, 0x06, 0x9B, 0xAC, 0xB8, 0xF9, 0xE4, 0xDE, 0xC5, 0x9F, 0x11, 0xE1, 0x45, 0x8A, 0x80,
		0x90, 0xC4, 0x22, 0x24, 0x6C, 0x84, 0xA4, 0x73, 0x6B, 0x0C, 0x21, 0x06, 0x70, 0x7C, 0x42, 0x33,
		0xB9, 0x8D, 0x43, 0x62, 0xF8, 0x29, 0xB1, 0x54, 0x18, 0xE4, 0x8D, 0xBF, 0xAA, 0x51, 0xE1, 0x55,
		0x88, 0x33, 0x50, 0x5D, 0x96, 0xDF, 0xE4, 0xF2, 0xDF, 0x10, 0x75, 0x73, 0xC5, 0x0B, 0x04, 0xA6,
	};

	rsa::rsa() {
		sbig::New(&e);
		sbig::FromBinary(e, (const char*)rsa_e, sizeof rsa_e);
		sbig::New(&n);
		sbig::FromBinary(n, (const char*)rsa_n, sizeof rsa_n);
	}
	rsa::~rsa() {
		sbig::Del(e);
		sbig::Del(n);
	}
	bool rsa::check(const char* content, size_t contentlen, const char* sign, size_t signlen) {
		static unsigned char mout[SHA1HashSize+1] = { 0 };
		HANDLE m = encrypt(sign, signlen);
		size_t len = 0;
		sbig::ToBinaryBuffer(m, (char*)mout, SHA1HashSize + 1, &len);
		if (len != SHA1HashSize) {
			return false;
		}
		const unsigned char* sha1out = sha1(content, contentlen);
		bool eq = memcmp(mout, sha1out, len) == 0;
		sbig::Del(m);
		return eq;
	}
	HANDLE rsa::encrypt(const char* buf, size_t len) {
		HANDLE src;
		sbig::New(&src);
		sbig::FromBinary(src, buf, len);
		HANDLE dst;
		sbig::New(&dst);
		sbig::PowMod(dst, src, e, n);
		sbig::Del(src);
		return dst;
	}
	const unsigned char* rsa::sha1(const char* buf, size_t len) {
		static unsigned char sha1out[SHA1HashSize] = { 0 };
		SHA1Context context;
		SHA1Reset(&context);
		SHA1Input(&context, (const unsigned char*)buf, len);
		SHA1Result(&context, sha1out);
		((const char*)sha1out, SHA1HashSize);
		return sha1out;
	}
}}}}
