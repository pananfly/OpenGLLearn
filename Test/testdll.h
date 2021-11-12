#ifndef test_dll_h
#define test_dll_h
#define TEST_EXPORT extern "C" __declspec(dllexport)
TEST_EXPORT int testDll(int a);
#endif