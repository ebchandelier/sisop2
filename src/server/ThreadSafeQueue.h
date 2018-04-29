#pragma once
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

template <typename T>
class ThreadSafeQueue
{
public:

	ThreadSafeQueue(size_t capacity)
	{
		this->capacity = capacity;
	}

	T consume() 
	{
		std::unique_lock<std::mutex> mlock(mutex_);
		while (queue_.empty())
		{
			cond_.wait(mlock);
		}
		auto val = queue_.front();
		queue_.pop();
		return val;
	}

	void produce(const T& item)
	{
		std::unique_lock<std::mutex> mlock(mutex_);
		queue_.push(item);
		mlock.unlock();
		cond_.notify_one();
	}
  
private:
	size_t capacity;
	std::queue<T> queue_;
	std::mutex mutex_;
	std::condition_variable cond_;
};

