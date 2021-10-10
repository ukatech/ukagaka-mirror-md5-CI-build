#pragma once
#include <string>
#include <stdexcept>
#include <fstream>
#include <Windows.h>
namespace cool
{
	class MD5
	{
		struct MD5_CTX
		{
			ULONG num[2];
			ULONG buf[4];
			BYTE input[64];
			BYTE digest[16];
		};
		typedef void (WINAPI *MD5_INIT)(MD5_CTX*);
		typedef void (WINAPI *MD5_UPDATE)(MD5_CTX*,const void*,unsigned int);
		const char* MODULE_NAME = "Cryptdll.dll";
	public:
		MD5() 
			:mInit(nullptr)
			,mFinal(nullptr)
			,mUpdate(nullptr)
			,mModule(NULL)
		{
			
			mModule = ::LoadLibraryA(MODULE_NAME);
			if (!mModule)
			{
				throw std::runtime_error("module load fail");
			}

			mInit = (MD5_INIT)::GetProcAddress(mModule, "MD5Init");
			mFinal = (MD5_INIT)::GetProcAddress(mModule, "MD5Final");
			mUpdate = (MD5_UPDATE)::GetProcAddress(mModule, "MD5Update");
			if (!mInit || !mFinal || !mUpdate)
			{
				throw std::runtime_error("Get Function Address fail");
			}
		}
		~MD5() 
		{
			if (mModule)
			{
				::FreeLibrary(mModule);
				mModule = NULL;
			}
		}
		std::string get_str_md5(const std::string& str)
		{
			std::string str_md5;
			MD5_CTX ctx = { 0 };
			mInit(&ctx);
			mUpdate(&ctx, str.c_str(), str.length());
			mFinal(&ctx);
			str_md5 = to_string(&ctx);
			return str_md5;
		}
		std::string get_file_md5(const std::string& filename)
		{
			std::string str_md5;
			MD5_CTX ctx = { 0 };
			std::ifstream file(filename, std::ios::binary | std::ios::in);
			if (file.is_open())
			{
				char data[1024] = { 0 };
				long long len = 0;
				mInit(&ctx);
				while (!file.eof())
				{
					file.read(data, 1024);
					mUpdate(&ctx, data, file.gcount());
					len += file.gcount();
				}
				mFinal(&ctx);
				str_md5 = to_string(&ctx);
				file.close();
			}
			return str_md5;
		}
		std::string get_buffer_md5(void* buffer, ULONG len)
		{
			std::string str_md5;
			MD5_CTX ctx = { 0 };
			mInit(&ctx);
			mUpdate(&ctx, buffer, len);
			mFinal(&ctx);
			str_md5 = to_string(&ctx);
			return str_md5;
		}
	private:
		std::string to_string(MD5_CTX* ctx)
		{
			char szBuffer[33] = { 0 };
			for (size_t i = 0; i < 16; i++)
			{
				sprintf(szBuffer + i * 2, "%02x", ctx->digest[i]);
			}
			return std::string().assign(szBuffer);
		}
	private:
		MD5_INIT mInit;
		MD5_INIT mFinal;
		MD5_UPDATE mUpdate;
		HMODULE mModule;

	};
}
