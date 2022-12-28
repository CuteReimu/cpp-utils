#pragma once
#include <windows.h>
#include <process.h>
namespace ThreadAttach{
	class Runnable
	{
	public:
		virtual ~Runnable();
		virtual unsigned int _stdcall run(void *p) = 0;
		//void wait();
		//void notify();
	};

	class Thread:public Runnable
	{
	public:
		Thread(Runnable &aRunnable);
		Thread::~Thread();
		void start();
		unsigned int _stdcall run(void *p);
		static void sys();
		static void endsys();
	private:
		Runnable &aRun;
		HANDLE handle;
	};
}


