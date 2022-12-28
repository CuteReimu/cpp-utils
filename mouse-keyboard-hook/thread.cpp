#include "thread.h"
namespace ThreadAttach{

	Runnable::~Runnable(){
	}

	Thread::~Thread(){
		WaitForSingleObject(handle, INFINITE);
		CloseHandle(handle);
	}

	Thread::Thread(Runnable &aRunnable):aRun(aRunnable){
	}

	void Thread::start(){
		handle = (HANDLE)_beginthreadex(NULL, 0, (unsigned int (_stdcall *)(void *))*(*(void***)&aRun+1), NULL, 0, NULL);
	}

	unsigned int _stdcall Thread::run(void *p){
		aRun.run(p);
		return 0;
	}

}

