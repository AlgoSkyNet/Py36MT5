// Test.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "Python.h"
#include <string>

using namespace std;

PyObject *pModule;
PyObject *pFunc;
PyObject *pArgs;
PyObject *pValue;
PyObject *global_dict;


void LoadScript(wchar_t* path, const wchar_t* name)
{
	wstring w(name);
	string st(w.begin(), w.end());
	const char * s = st.c_str();
	wstring w1(path);
	string st1(w1.begin(), w1.end());
	string st2 = st1 + st;
	const char * s1 = st2.c_str();
	const char * s3 = st1.c_str();
	Py_Initialize();
	PyObject* sysPath = PySys_GetObject((char*)"path");
	PyObject* curDir = PyUnicode_FromString((char*)s3);
	PyList_Append(sysPath, curDir);
	Py_DECREF(curDir);
	Py_DECREF(sysPath);
	FILE *file = _Py_fopen(s1, "r+");
	if (file != NULL) {
		PyRun_SimpleFile(file, s);
	}
	PyObject *pName;
	pName = PyUnicode_FromString("__main__");
	pModule = PyImport_Import(pName);
	global_dict = PyModule_GetDict(pModule);
}

void Finalize()
{
	Py_DECREF(global_dict);
	Py_DECREF(pModule);
	Py_Finalize();
}

void NewArg(int i)
{
	pArgs = PyTuple_New(i);
}

void CallFunc(const wchar_t* name)
{
	wstring w(name);
	string st(w.begin(), w.end());
	const char * s = st.c_str();
	pFunc = PyDict_GetItemString(global_dict, s);
	if (pFunc && PyCallable_Check(pFunc))
	{
		pValue = PyObject_CallObject(pFunc, pArgs);
		Py_DECREF(pArgs);
		Py_DECREF(pFunc);
	}
}

long GetRezLong()
{
	long rez = 0;
	if (pValue != NULL)
	{
		rez = PyLong_AsLong(pValue);
		Py_DECREF(pValue);
	}
	return rez;
}

int main()
{
	auto fn = L"test.py";
	wchar_t* p = (wchar_t *)L"C:\\local\\Scripts\\";
	LoadScript(p, fn);
	NewArg(0);
	auto f = L"tickcount";
	CallFunc(f);
	long r = GetRezLong();
	return 0;
}

