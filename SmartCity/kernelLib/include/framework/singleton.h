/********************************************************************
(c) 2003-2005 C2217 Studio
Module:    Singleton.h
Author:     Yangjun D.
Created:    9/3/2005   23:17
Purpose:    Implement singleton pattern
History:
*********************************************************************/
#pragma once

#include <memory>
using namespace std;
#include "Interlocked.h"
//using namespace C2217::Win32;

//namespace C2217
//{
	namespace Pattern
	{
		template <class T>
		class Singleton
		{
		public:
			static inline T* Ptr();
			Singleton(void){}
			~Singleton(void){}

		private:
			Singleton(const Singleton&);
			Singleton & operator= (const Singleton &);

			static auto_ptr<T> _instance;
			static CResGuard _rs;
		};

		template <class T>
		auto_ptr<T> Singleton<T>::_instance;

		template <class T>
		CResGuard Singleton<T>::_rs;
		template <class T>
		inline T* Singleton<T>::Ptr()
		{
			if( 0 == _instance.get() )
			{
				CResGuard::CGuard gd(_rs);
				if( 0== _instance.get())
				{
					_instance.reset ( new T);
				}
			}
			return _instance.get();
		}
	}
//}