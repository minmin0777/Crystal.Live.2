#pragma once
#ifndef UTILS_BASE_SAFE_MAP_H
#define UTILS_BASE_SAFE_MAP_H

#include <map>
#include <mutex>

namespace Common {

    template <typename K, typename V>
    class ThreadSafeMap {
    public:
        ThreadSafeMap() {}

        ~ThreadSafeMap() {}

        ThreadSafeMap(const ThreadSafeMap& rhs)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            map_ = rhs.map_;
        }

        ThreadSafeMap& operator=(const ThreadSafeMap& rhs)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (&rhs != this) {
                map_ = rhs.map_;
            }

            return *this;
        }

        V& operator[](const K& key)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            return map_[key];
        }

        // when multithread calling size() return a tmp status, some threads may insert just after size() call
        int size()
        {
            std::lock_guard<std::mutex> lock(mutex_);
            return map_.size();
        }

        // when multithread calling Empty() return a tmp status, some threads may insert just after Empty() call
        bool isEmpty()
        {
            std::lock_guard<std::mutex> lock(mutex_);
            return map_.empty();
        }

        bool insert(const K& key, const V& value)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            auto ret = map_.insert(std::pair<K, V>(key, value));
            return ret.second;
        }

        void ensureInsert(const K& key, const V& value)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            auto ret = map_.insert(std::pair<K, V>(key, value));
            // find key and cannot insert
            if (!ret.second) {
                map_.erase(ret.first);
                map_.insert(std::pair<K, V>(key, value));
                return;
            }
            return;
        }

        bool find(const K& key, V& value)
        {
            bool ret = false;
            std::lock_guard<std::mutex> lock(mutex_);

            auto iter = map_.find(key);
            if (iter != map_.end()) {
                value = iter->second;
                ret = true;
            }

            return ret;
        }

        bool update(const K& key, V& oldValue, const V& newValue)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            auto iter = map_.find(key);
            if (iter != map_.end()) {
                oldValue = iter->second;
                iter->second = newValue; // 直接更新值
                return true;
            }
            return false;
        }

        void erase(const K& key)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            map_.erase(key);
        }

        void clear()
        {
            std::lock_guard<std::mutex> lock(mutex_);
            map_.clear();
            map_.shrink_to_fit();
            return;
        }
        bool contains(const K& key) const {
            std::lock_guard<std::mutex> lock(mutex_);
            return map_.find(key) != map_.end();
        }

        std::vector<K> keys() const {
            std::lock_guard<std::mutex> lock(mutex_);
            std::vector<K> result;
            for (auto& pair : map_) {
                result.push_back(pair.first);
            }
            return result;
        }
    private:
        std::mutex mutex_;
        std::map<K, V> map_;
    };

} // namespace Common
#endif

