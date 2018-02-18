#ifndef RegistryH
#define RegistryH

#include <fmx.h>

class TRegistryManager{
	bool initialised;
    UnicodeString Company;
    UnicodeString Product;
public:
	static const int READ_SUCCESS = 0;
    static const int READ_WRONGTYPE = 1;
    static const int READ_ERROR = 2;
    static const int READ_NOTINITIALISED = 3;
    static const int READ_DOESNTEXIST = 4;
    static const int WRITE_SUCCESS = 0;
    static const int WRITE_ERROR = 1;
    static const int WRITE_NOTINITIALISED = 2;

	TRegistryManager();
    bool Initialise(UnicodeString Company,UnicodeString Product);
    bool StringExists(UnicodeString Value);
    bool DwordExists(UnicodeString Value);
    int ReadString(UnicodeString ValName,UnicodeString &Data);
    int ReadDword(UnicodeString ValName,DWORD &Data);
    int WriteString(UnicodeString ValName,UnicodeString Data);
    int WriteDword(UnicodeString ValName,DWORD Data);
};

#endif
