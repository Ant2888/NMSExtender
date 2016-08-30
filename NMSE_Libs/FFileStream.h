#pragma once
#include <fstream>

//Simple FILE wrapper used to log things and dump info

class FFileStream{

	public:
		FFileStream();
		~FFileStream();

		//attemps to open a file and can append to one as well
		//if one is already open it will close the one you currently have open
		bool OpenFile(const char* path, bool append);

		//default write (will probably mess with padding and such if not a bit careful with it)
		bool WriteToFile(const char* buffer);

		//adds an error tag
		bool WriteError(const char* errMsg, int errNum = 0);

		//adds a success tag to it
		bool WriteSuccess(const char* msg);
	private:
		FILE *m_File;
		
};

extern FFileStream global_Logger;

inline bool WRITEMSG(const char* msg){
	return global_Logger.WriteToFile(msg);
}

inline bool SUCCESSMSG(const char* msg){
	return global_Logger.WriteSuccess(msg);
}

inline bool ERRORMSG(const char* msg, int err = 0){
	return global_Logger.WriteError(msg, err);
}