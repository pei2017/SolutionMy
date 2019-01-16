#pragma once
#include <windows.h>
#include <coml2api.h>
#include <string>

class StoRangeTest
{
private:
	IStorage* m_file;

public:
	StoRangeTest()
	{
		HRESULT hr = StgCreateStorageEx(L"test.txt", STGM_READWRITE | STGM_SHARE_EXCLUSIVE, STGFMT_STORAGE, 0, NULL, NULL, IID_IStorage, (void**)&m_file);
		
		if (hr == S_OK)
		{
			IStream* stream = nullptr;
			hr = m_file->CreateStream(L"STR1", STGM_READWRITE | STGM_SHARE_EXCLUSIVE, 0, 0, &stream);

			if (hr == S_OK)
			{
				wchar_t tmp[] = L"hello world";
				ULONG size = 0;
				hr = stream->Write(tmp, sizeof(tmp) - 2, &size);

				STATSTG stg;
				hr = stream->Stat(&stg, STATFLAG_DEFAULT);
			}

		}
		else
		{
			hr = StgOpenStorageEx(L"test.txt", STGM_READWRITE | STGM_SHARE_EXCLUSIVE, STGFMT_STORAGE, 0, NULL, NULL, IID_IStorage, (void**)&m_file);
			if (hr == S_OK)
			{
				IStream* stream = nullptr;
				hr = m_file->OpenStream(L"STR1", nullptr, STGM_READWRITE | STGM_SHARE_EXCLUSIVE, 0, &stream);
				if (hr == S_OK)
				{
					STATSTG stg;
					hr = stream->Stat(&stg, STATFLAG_DEFAULT);
					DWORD size = stg.cbSize.LowPart;

					wchar_t tmp[256];
					hr = stream->Read((void*)(tmp), size, &size);
					
					int i = 0;
					i++; i--;
				}


				hr = m_file->CreateStream(L"STR1", STGM_READWRITE | STGM_SHARE_EXCLUSIVE, 0, 0, &stream);

				if (hr == S_OK)
				{
					wchar_t tmp[] = { L'H', L'e', L'l' ,L'l' ,L'o' ,L' ' ,L'W' ,L'o' ,L'r' ,L'l' ,L'd' };
					ULONG sizewrite = 0;
					hr = stream->Write(tmp, sizeof(tmp), &sizewrite);

					STATSTG stg;
					hr = stream->Stat(&stg, STATFLAG_DEFAULT);
					DWORD size = stg.cbSize.LowPart;

					if (hr == S_OK)
					{
						LARGE_INTEGER li = { 0};
						ULARGE_INTEGER uli = { 0 };
						hr = stream->Seek(li, STREAM_SEEK_CUR, &uli);

						if (hr == S_OK)
						{
							wchar_t tmp[] = L"hello world";
							ULONG size2 = 0;
							hr = stream->Write(tmp, sizeof(tmp) - 2, &size2);

							STATSTG stg;
							hr = stream->Stat(&stg, STATFLAG_DEFAULT);
							DWORD size = stg.cbSize.LowPart;
						}
					}


					m_file->Commit(STGC_DEFAULT);
				}
			}
		}
	}
};