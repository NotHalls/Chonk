#include "ThreadQueue.h"

ThreadQueue::ThreadQueue(size_t numThreads)
{
  // Initialize the thread pool with the specified number of threads
  for(size_t i = 0; i < numThreads; ++i)
  {
    std::thread([this]() {
      while(true)
      {
        std::function<void()> task;

        {
          std::unique_lock<std::mutex> lock(m_QueueMutex);
          m_Condition.wait(lock, [this] { return m_Stop || !m_Tasks.empty(); });

          if(m_Stop && m_Tasks.empty())
          {
            return;
          }

          task = std::move(m_Tasks.front());
          m_Tasks.pop();
        }

        task();
      }
    }).detach();
  }
}

ThreadQueue::~ThreadQueue() { Stop(); }

// template <typename F, typename... Args>
// void ThreadQueue::Enqueue(F &&f, Args &&...args)
// {
//   {
//     std::unique_lock<std::mutex> lock(m_QueueMutex);
//     m_Tasks.emplace(std::bind(std::forward<F>(f),
//     std::forward<Args>(args)...));
//   }
//   m_Condition.notify_one();
// }

void ThreadQueue::Start()
{
  // No specific start logic needed since threads are created in the constructor
}
void ThreadQueue::Stop()
{
  {
    std::unique_lock<std::mutex> lock(m_QueueMutex);
    m_Stop = true;
  }
  m_Condition.notify_all();
}
void ThreadQueue::WaitForCompletion()
{
  std::unique_lock<std::mutex> lock(m_QueueMutex);
  m_Condition.wait(lock, [this] { return m_Tasks.empty(); });
}
bool ThreadQueue::IsEmpty()
{
  std::lock_guard<std::mutex> lock(m_QueueMutex);
  return m_Tasks.empty();
}
size_t ThreadQueue::Size()
{
  std::lock_guard<std::mutex> lock(m_QueueMutex);
  return m_Tasks.size();
}
void ThreadQueue::Clear()
{
  std::lock_guard<std::mutex> lock(m_QueueMutex);
  while(!m_Tasks.empty())
  {
    m_Tasks.pop();
  }
}