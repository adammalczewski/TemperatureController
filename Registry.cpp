#pragma hdrstop

#include "Registry.h"

TRegistryManager::TRegistryManager(){
	initialised = false;
}

bool TRegistryManager::Initialise(UnicodeString Company,UnicodeString Product){

	HKEY key;
    LONG res;

    res = RegOpenCurrentUser(KEY_READ,&key);

    if (res != ERROR_SUCCESS) return false;

    UnicodeString SubKey = "Software\\";
    SubKey += Company;
	SubKey += "\\";
	SubKey += Product;

    res = RegCreateKeyEx(key,SubKey.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE
      , KEY_READ, NULL, &key,NULL);

    if (res != ERROR_SUCCESS) return false;

    this->Company = Company;
    this->Product = Product;

    initialised = true;
    return true;

}

bool TRegistryManager::StringExists(UnicodeString Value){

	if (!initialised) return false;

	HKEY key;
    LONG res;

    res = RegOpenCurrentUser(KEY_READ,&key);

    if (res != ERROR_SUCCESS) return false;

    UnicodeString SubKey = "Software\\";
    SubKey += Company;
	SubKey += "\\";
	SubKey += Product;

    res = RegCreateKeyEx(key,SubKey.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE
      , KEY_READ, NULL, &key,NULL);

    if (res != ERROR_SUCCESS) return false;

    res = RegGetValue(key,L"",Value.c_str(),RRF_RT_REG_SZ,NULL,NULL,NULL);

    if (res != ERROR_SUCCESS) return false;

    return true;

}

bool TRegistryManager::DwordExists(UnicodeString Value){

	if (!initialised) return false;

	HKEY key;
    LONG res;

    res = RegOpenCurrentUser(KEY_READ,&key);

    if (res != ERROR_SUCCESS) return false;

    UnicodeString SubKey = "Software\\";
    SubKey += Company;
	SubKey += "\\";
	SubKey += Product;

    res = RegCreateKeyEx(key,SubKey.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE
      , KEY_READ, NULL, &key,NULL);

    if (res != ERROR_SUCCESS) return false;

    res = RegGetValue(key,L"",Value.c_str(),RRF_RT_REG_DWORD,NULL,NULL,NULL);

    if (res != ERROR_SUCCESS) return false;

    return true;

}

	static const int READ_SUCCESS = 0;
    static const int READ_WRONGTYPE = 1;
    static const int READ_ERROR = 2;
    static const int READ_NOTINITIALISED = 3;
    static const int READ_DOESNTEXIST = 4;

int TRegistryManager::ReadString(UnicodeString ValName,UnicodeString &Data){

	if (!initialised) return READ_NOTINITIALISED;

	HKEY key;
	LONG res;

	res = RegOpenCurrentUser(KEY_READ,&key);

    if (res != ERROR_SUCCESS) return READ_ERROR;

    UnicodeString SubKey = "Software\\";
	SubKey += Company;
	SubKey += "\\";
	SubKey += Product;

	res = RegCreateKeyEx(key,SubKey.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE
	  , KEY_READ, NULL, &key,NULL);

    if (res != ERROR_SUCCESS) return READ_ERROR;

    DWORD type,size;

    res = RegGetValue(key,L"",ValName.c_str(),RRF_RT_ANY,&type,NULL,&size);

	if (res != ERROR_SUCCESS) return READ_DOESNTEXIST;
	if (type != REG_SZ) return READ_WRONGTYPE;

	wchar_t *Buffer = new wchar_t[size+1];

	res = RegGetValue(key,L"",ValName.c_str(),RRF_RT_REG_SZ,&type
	  ,reinterpret_cast<void*>(Buffer),&size);

	Data = Buffer;

	delete Buffer;

	if (res != ERROR_SUCCESS) return READ_ERROR;

    return READ_SUCCESS;
}

int TRegistryManager::ReadDword(UnicodeString ValName,DWORD &Data){

	if (!initialised) return READ_NOTINITIALISED;

	HKEY key;
    LONG res;

    res = RegOpenCurrentUser(KEY_READ,&key);

    if (res != ERROR_SUCCESS) return READ_ERROR;

    UnicodeString SubKey = "Software\\";
    SubKey += Company;
	SubKey += "\\";
	SubKey += Product;

    res = RegCreateKeyEx(key,SubKey.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE
	  , KEY_READ, NULL, &key,NULL);

    if (res != ERROR_SUCCESS) return READ_ERROR;

    DWORD type,size;

    res = RegGetValue(key,L"",ValName.c_str(),RRF_RT_ANY,&type,NULL,NULL);

    if (res != ERROR_SUCCESS) return READ_DOESNTEXIST;
	if (type != REG_DWORD) return READ_WRONGTYPE;

	unsigned long DwordSize = sizeof(DWORD);

	res = RegGetValue(key,L"",ValName.c_str(),RRF_RT_REG_DWORD,NULL
	  ,reinterpret_cast<void*>(&Data),&DwordSize);

	if (res != ERROR_SUCCESS) return READ_ERROR;

    return READ_SUCCESS;
}

int TRegistryManager::WriteString(UnicodeString ValName,UnicodeString Data){

	if (!initialised) return WRITE_NOTINITIALISED;

	HKEY key;
    LONG res;

    res = RegOpenCurrentUser(KEY_READ,&key);

    if (res != ERROR_SUCCESS) return WRITE_ERROR;

    UnicodeString SubKey = "Software\\";
    SubKey += Company;
	SubKey += "\\";
	SubKey += Product;

    res = RegCreateKeyEx(key,SubKey.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE
      , KEY_ALL_ACCESS, NULL, &key,NULL);

    if (res != ERROR_SUCCESS) return WRITE_ERROR;

    res = RegSetValueEx(key,ValName.c_str(),0,REG_SZ
	  ,reinterpret_cast<BYTE*>(Data.c_str()),(Data.Length()+1)*sizeof(wchar_t));

    if (res != ERROR_SUCCESS) return WRITE_ERROR;

    return WRITE_SUCCESS;
}

int TRegistryManager::WriteDword(UnicodeString ValName,DWORD Data){

	if (!initialised) return WRITE_NOTINITIALISED;

	HKEY key;
    LONG res;

    res = RegOpenCurrentUser(KEY_READ,&key);

    if (res != ERROR_SUCCESS) return WRITE_ERROR;

    UnicodeString SubKey = "Software\\";
    SubKey += Company;
	SubKey += "\\";
	SubKey += Product;

    res = RegCreateKeyEx(key,SubKey.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE
      , KEY_ALL_ACCESS, NULL, &key,NULL);

    if (res != ERROR_SUCCESS) return WRITE_ERROR;

    res = RegSetValueEx(key,ValName.c_str(),0,REG_DWORD
      ,reinterpret_cast<BYTE*>(&Data),sizeof(DWORD));

    if (res != ERROR_SUCCESS) return WRITE_ERROR;

    return WRITE_SUCCESS;

}

#pragma package(smart_init)
