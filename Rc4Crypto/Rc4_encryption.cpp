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
		/*
		cout << "0x" << hex << (int)T[i] << ',';
		*/
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
unsigned int main()
{
	char key[] = "cookie";	//Rc4加密的Key
	//要加密的ShellCode
	unsigned char buf[] =
		"";

	unsigned char s[256];
	rc4_init(s, (unsigned char*)key, strlen(key));
	for (size_t i = 0; i < sizeof(buf); i++)
	{
		rc4_crypt(s, &buf[i], sizeof(buf[i]));
		printf("\\x%02x", buf[i]); //输出Rc4加密后的ShellCode
	}
	return 0;
}