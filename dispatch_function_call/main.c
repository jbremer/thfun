#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define FORCEDINLINE __attribute__((always_inline))

FORCEDINLINE int aup(int a)
{
    return a + a;
}

int calc(int a, int b)
{
    return aup(a) + aup(b);
}

DWORD WINAPI calc_obf_helper(LPVOID args)
{
    int *a = (int *) args;
    return calc(a[0], a[1]);
}

FORCEDINLINE int calc_obf(int a, int b)
{
    DWORD return_value = 0, args[] = {a, b};
    HANDLE handle = CreateThread(NULL, 0, &calc_obf_helper, &args, 0, NULL);
    if(handle != NULL) {
        WaitForSingleObject(handle, INFINITE);
        GetExitCodeThread(handle, &return_value);
        CloseHandle(handle);
    }
    return return_value;
}

#define calc calc_obf

int main(int argc, char *argv[])
{
    int a = atoi(argv[1]), b = atoi(argv[2]);
    printf("calc(%d, %d): %d\n", a, b, calc(a, b));
}
