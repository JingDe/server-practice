#ifndef PORT_POSIX_H_
#define PORT_POSIX_H_

#ifdef POSIX
#include"common/noncopyable.h"

#include<pthread.h>
#include<string>

namespace port {

	extern int getThreadID();
	
	class Mutex {
	public:
		Mutex();
		~Mutex();

		void Lock();
		void Unlock();
		void AssertHeld();

	private:
		pthread_mutex_t mu_;

		Mutex(const Mutex&);
		void operator=(const Mutext&);
	};


	class CondVar {
	public:
		explicit CondVar(Mutex* mu);
		~CondVar();

		void Wait();
		void waitForSeconds(int secs);
		void Signal();
		void SignalAll();

	private:
		Mutex * mu_;

		pthread_cond_t cv_;
	};

	class Thread : noncopyable {
	public:
		typedef std::function<void (void)> ThreadFunc; // 线程函数类型

		explicit Thread(const ThreadFunc& func, std::string name=std::string());
		~Thread();

		void start();
		void join();

		bool started() const { return started_;  }
		const string& name() const { return name_; }

	private:
		void* startThread(void*);

		pthread_t tid_; // 线程id
				
		ThreadFunc func_; // 线程函数
		std::string name_; // 线程名

		bool started_;
		bool joined_;
	};

	class ThreadData {
	public:
		typedef std::function<void(void)> ThreadFunc; // 线程函数类型

		ThreadData(ThreadFunc func) :func_(func) {}
		~ThreadData() {}

		void runInThread()
		{

			try {
				func_();
			}
			catch (const std::exception& ex)
			{
				fprintf(stderr, "reason: %s\n", ex.what());
				abort();
			}
			catch (...)
			{
				throw;
			}
		}

		friend class Thread;

	private:
		ThreadFunc func_;
	};
}
#endif

#endif