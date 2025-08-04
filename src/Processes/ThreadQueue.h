#pragma once

#include <atomic>
#include <cassert>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <type_traits>
#include <vector>

// we will create a simple thread queue class for chunk generation
// the header file will only contain declarations

class ThreadQueue
{
public:
  ThreadQueue(size_t numThreads);
  ~ThreadQueue();

  template <typename F, typename... Args> void Enqueue(F &&f, Args &&...args)
  {
    auto task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
    {
      std::unique_lock<std::mutex> lock(m_QueueMutex);
      m_Tasks.emplace(std::move(task));
    }
    m_Condition.notify_one();
  }

  void Start();
  void Stop();
  void WaitForCompletion();
  bool IsEmpty();
  size_t Size();
  void Clear();

private:
  std::queue<std::function<void()>> m_Tasks;
  std::mutex m_QueueMutex;
  std::condition_variable m_Condition;
  bool m_Stop = false;
};