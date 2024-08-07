/*————————————————————————————————————————————————————————————————————————————————————————
 * @Author: jason minmin0777@126.com
 * @Date: 2024-07-09 09:55:21
 * @LastEditors: jason minmin0777@126.com
 * @LastEditTime: 2024-07-09 09:55:29
 * @FilePath: \Crystal.Live.2\src\library\common\include\ThreadSafeQueue.h
 * @Description:
 * @
 * @#|----------------------------------------------------------------------------|
 * @#|  Remark         : Description                                              |
 * @#|----------------------------------------------------------------------------|
 * @#|  Change History :                                                          |
 * @#|  <Date>     | <Version> | <Author>       | <Description>                   |
 * @#|----------------------------------------------------------------------------|
 * @#|  2024/05/02 | 0.0.0.1   | jason.chen     | Create file                     |
 * @#|----------------------------------------------------------------------------|
 * @#|                                                                            |
 * @#|----------------------------------------------------------------------------|
 * @Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 ————————————————————————————————————————————————————————————————————————————————————————*/
#pragma once
#include <condition_variable>
#include <mutex>
#include <queue>
namespace Common
{
    template<typename T>
    class ThreadSafeQueue {
    public:
        // 入队
        void Enqueue(const T& value) {
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.push(value);
            condition_.notify_one(); // 通知一个等待的线程
        }

        // 尝试出队，如果队列为空则立即返回false
        bool TryDequeue(T& value) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (queue_.empty()) {
                return false;
            }
            value = queue_.front();
            queue_.pop();
            return true;
        }

        // 出队，如果队列为空则阻塞等待
        void WaitAndDequeue(T& value) {
            std::unique_lock<std::mutex> lock(mutex_);
            condition_.wait(lock, [this] { return !queue_.empty(); }); // 等待直到队列非空
            value = queue_.front();
            queue_.pop();
        }

        // 获取队列大小
        size_t Size() const {
            std::lock_guard<std::mutex> lock(mutex_);
            return queue_.size();
        }

        // 检查队列是否为空
        bool IsEmpty() const {
            std::lock_guard<std::mutex> lock(mutex_);
            return queue_.empty();
        }

    private:
        mutable std::mutex mutex_;
        std::queue<T> queue_;
        std::condition_variable condition_;
    };
}
