#include <stdio.h>
#include <windows.h>
#include <locale.h>



int main(int argc, char* argv)
{
	setlocale(LC_ALL, "");
	wchar_t x[] = L"ÖÐ¹ú;";
	wprintf(L"%s\n", x);
	getchar();

	return 0;
}