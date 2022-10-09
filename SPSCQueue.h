// SPSCQueue.h : Single producer, single consumer queue. Implementation is based on std::queue with support of multithreading
//

#pragma once

#include <iostream>

// TODO: Reference additional headers your program requires here.
#include <queue>
#include <mutex>

template <class T>
class SPSCQueue {
	std::queue<T> q;
	std::mutex _mut_queue;
public:
	//template <class T>
	//SPSCQueue() {}

	//template <class T>
	size_t size() {
		std::lock_guard<std::mutex> lock(_mut_queue);
		return this->q.size();
	}

	//template <class T>
	void push(const T& entry) {
		std::lock_guard<std::mutex> lock(_mut_queue);
		this->q.push(entry);
	}

	//template <class T>
	void emplace(const T&& entry) {
		std::lock_guard<std::mutex> lock(_mut_queue);
		this->q.emplace(entry);
	}

	//template <class T>
	T& front() {
		std::lock_guard<std::mutex> lock(_mut_queue);
		return this->q.front();
	}

	//template <class T>
	void pop() {
		std::lock_guard<std::mutex> lock(_mut_queue);
		this->q.pop();
	}

	//template <class T>
	T& peek_and_pop() {
		std::lock_guard<std::mutex> lock(_mut_queue);
		auto& ret = this->q.front();
		this->q.pop();
		return ret;
	}
};