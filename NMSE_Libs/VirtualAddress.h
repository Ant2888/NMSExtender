#pragma once
#include <Windows.h>

//Credit to guys at SE

class VAManager{
public:
	VAManager();
	static uintptr_t baseAddr;
};

// VA to ptr of type T
template <typename T>
class VAPtr{
	public:
		VAPtr(uintptr_t rva)
			:m_rva(rva + VAManager::baseAddr){
			//
		}

		operator T*() const{
			return GetPtr();
		}

		T* operator->() const{
			return GetPtr();
		}

		T* GetPtr() const{
			return reinterpret_cast <T *>(m_rva);
		}

		const T* GetConst() const{
			return reinterpret_cast <T *>(m_rva);
		}

		uintptr_t GetUIntPtr() const{
			return m_rva;
		}

	private:
		uintptr_t m_rva;
		VAPtr();
		VAPtr(VAPtr & rhs);
		VAPtr & operator=(VAPtr & rhs);
};

// VA to type T
template <typename T>
class VAddr{
	public:
		VAddr(uintptr_t rva)
			:m_rva(reinterpret_cast <BlockConversionType *>(rva + VAManager::baseAddr)){
			//
		}

		operator T(){
			return reinterpret_cast <T>(m_rva);
		}

		uintptr_t GetUIntPtr() const{
			return reinterpret_cast <uintptr_t>(m_rva);
		}

	private:
		struct BlockConversionType { };
		BlockConversionType* m_rva;
		VAddr();
		VAddr(VAddr& rhs);
		VAddr& operator=(VAddr& rhs);
};
