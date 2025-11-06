#pragma once

#include <givaro/modular-integer.h>
#include <ethash/keccak.hpp>
#include <openssl/sha.h>
#include <sstream>
#include <vector>
#include <memory>

using Givaro::Integer;

using std::string;
using std::stringstream;
using std::vector;

using std::unique_ptr;
using std::make_unique;
using std::make_pair;

template <typename T>
class unique_arr {
    unique_ptr<T[]> data_;
    size_t size_ = 0;

public:
    // --- Constructors ---
    unique_arr() = default;

    explicit unique_arr(size_t n)
        : data_(make_unique<T[]>(n)), size_(n) {}

    // --- Move semantics ---
    unique_arr(unique_arr&& other) noexcept
        : data_(std::move(other.data_)), size_(other.size_) {
        other.size_ = 0;
    }

    unique_arr& operator=(unique_arr&& other) noexcept {
        if (this != &other) {
            data_ = std::move(other.data_);
            size_ = other.size_;
            other.size_ = 0;
        }
        return *this;
    }

    // --- Disable copy semantics ---
    unique_arr(const unique_arr&) = delete;
    unique_arr& operator=(const unique_arr&) = delete;

    // --- Element access ---
    T& operator[](size_t i) noexcept { return data_[i]; }
    const T& operator[](size_t i) const noexcept { return data_[i]; }

    // --- Accessors ---
    T* get() noexcept { return data_.get(); }
    const T* get() const noexcept { return data_.get(); }
    size_t size() const noexcept { return size_; }

    explicit operator bool() const noexcept { return static_cast<bool>(data_); }

    // --- Utilities ---
    void reset() noexcept {
        data_.reset();
        size_ = 0;
    }

    T* release() noexcept {
        size_ = 0;
        return data_.release();
    }

    void fill(const T& value) {
        std::fill(data_.get(), data_.get() + size_, value);
    }

    void swap(unique_arr& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
    }

    // --- Range-based for loop support ---
    T* begin() noexcept { return data_.get(); }
    const T* begin() const noexcept { return data_.get(); }
    T* end() noexcept { return data_.get() + size_; }
    const T* end() const noexcept { return data_.get() + size_; }
};

vector<string> split(const string& list, const string& separator);

string removeCharsFromString(const string &val, const char* charsToRemove);