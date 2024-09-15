#pragma once

#include <vector>
#include <mutex>
#include <stdexcept>

template<typename T>
class ThreadSafeVector {


public:
    // 向_vector中添加元素
    void push_back(const T& value) {
        std::lock_guard<std::mutex> lock(mtx);

        _vec.push_back(value);
    }

    void emplace_back(T&& value) {
        std::lock_guard<std::mutex> lock(mtx);
        _vec.emplace_back(std::move(value));
    }


    // 从_vector中移除最后一个元素
    void pop_back() {
        std::lock_guard<std::mutex> lock(mtx);
        if (!_vec.empty()) {
            _vec.pop_back();
        }
    }

    // 获取_vector的大小
    size_t size() const {
        std::lock_guard<std::mutex> lock(mtx);
        return _vec.size();
    }




    // 重载operator[]以提供对元素的访问
    T& operator[](size_t index) {
        std::lock_guard<std::mutex> lock(mtx);
        if (index >= _vec.size()) {
            throw std::out_of_range("Index out of range");
        }
        return _vec[index]; // 返回对指定元素的引用
    }

    // 为const对象提供只读访问
    const T& operator[](size_t index) const {
        std::lock_guard<std::mutex> lock(mtx);
        if (index >= _vec.size()) {
            throw std::out_of_range("Index out of range");
        }
        return _vec[index]; // 返回对指定元素的只读引用
    }

    // 清空_vector
    void clear() {
        std::lock_guard<std::mutex> lock(mtx);
        _vec.clear();
        _vec.shrink_to_fit();
    }
private:
    std::vector<T> _vec; // 内部_vector
    mutable std::mutex mtx; // 用于同步的互斥锁

};