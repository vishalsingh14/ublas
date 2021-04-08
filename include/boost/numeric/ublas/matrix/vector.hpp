
//  Boost Competency test
//  vector.hpp Definition for the class vector
//  Copyright (c) Vishal Singh 

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include<iostream>
#include<iterator>
#include<memory>
#include<algorithm>

namespace comptest {

template<typename T, typename Allocator = std::allocator<T>>
class vector {

public:
    typedef T value_type;
    typedef Allocator allocator_type;
    typedef size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    vector ()
    : size_(0), capacity_(0) 
    {
        data_ = allocator_.allocate(capacity_);
    }

    explicit vector (size_type s, const T& value = T())
    : size_(s), capacity_(s) 
    {
        data_ =  allocator_.allocate(capacity_);
        std::fill_n(data_, s, value);
    }

    vector(iterator first, iterator last) {
        size_ = last - first;
        capacity_ = size_;
        data_ = allocator_.allocate(capacity_);
        std::copy(first, last, data_);
    }

    vector(std::initializer_list<T> init) {
        size_ = init.size();
        capacity_ = size_;
        data_ = allocator_.allocate(capacity_);
        std::copy(init.begin(), init.end(), data_);
    }

    vector(const vector& vec) {
        size_ = vec.size();
        capacity_ = vec.capacity();
        data_ = vec.allocator_.allocate(capacity_);
        std::copy(vec.begin(), vec.end(), data_);
    }

    vector(vector&& vec) {
        size_ = vec.size();
        capacity_ = vec.capacity();
        data_ = allocator_.allocate(capacity_);
        std::move(vec.begin(), vec.end(), data_);
        vec.clear();
    }

    vector& operator=(const vector& vec) {
        size_ = vec.size();
        capacity_ = vec.capacity();
        data_ = allocator_.allocate(capacity_);

        std::copy(vec.begin(), vec.end(), data_);
        return *this;
    }

    ~vector() {
        if(data_ != nullptr) {
            for(size_type i = 0; i < size(); ++i) {
                allocator_.destroy(&data_[i]);
            }
            allocator_.deallocate(data_, capacity());
            data_ = nullptr;
        }
    }

    // allocator:
    allocator_type get_allocator() const {
        return allocator_;
    }

    // Capacity:
    bool empty() const {
        return size_ == 0;
    }
    size_type size() const {
        return size_;
    }
    size_type max_size() const {
        return std::numeric_limits<size_type>::max();
    }
    size_type capacity () const {
        return capacity_;
    }
    void resize(size_type count) {
        resize(count, T());
    }
    void resize(size_type count, const value_type &value) {
        if(count > capacity()) {
            reallocate(count);
            for(size_type i = size(); i<count; ++i) {
                allocator_.construct(data() + i, value);
            }
        }
        else if(count < size()) {
            for(size_type i = count; i<size(); ++i) {
                allocator_.destroy(data() + i);
            }
        }
        size_ = count;
    }
    void shrink_to_fit() {
        for(size_type i = size(); i<capacity(); ++i) {
            allocator_.destroy(data() + i);
        }
        capacity_ = size();
    }
    void reserve(size_type newCapacity) {
        if(newCapacity > max_size()) {
            throw new std::length_error("capacity cannot be larger than max_size");
        }
        if(newCapacity > capacity()) {
            reallocate(newCapacity);
        }
    }


    // Modifiers:
    void push_back (const T &value) {
        if(size() == capacity()) {
            size_type newCapacity = capacity();
            if(newCapacity > 0)
                newCapacity *= 2;
            else
                ++newCapacity;
            reallocate(newCapacity);
        }
        allocator_.construct(end(), std::move(value));
        ++size_;
    }
    void push_back (T&& value) {
        if(size() == capacity()) {
            size_type newCapacity = capacity();
            if(newCapacity > 0)
                newCapacity *= 2;
            else
                ++newCapacity;
            reallocate(newCapacity);
        }
        allocator_.construct(end(), std::move(value));
        ++size_;
    }

    void pop_back() {
        allocator_.destroy(end() - 1);
        --size_;
    }

    void swap(vector& rhs) {
        std::swap(size_, rhs.size_);
        std::swap(capacity_, rhs.capacity_);
        std::swap(allocator_, rhs.allocator_);
        std::swap(data_, rhs.data_);
    }

    void clear() {
        for(size_type i = 0; i < size(); ++i) {
            allocator_.destroy(data() + i);
        }
        size_ = 0;
    }

    void assign(size_type count, const T& value) {
        clear();
        if(count > capacity()) {
            reallocate(count);
        }
        for(int i = 0; i<count; ++i) {
            data_[i] = value;
        }
        size_ = count;
    }
    void assign(iterator first, iterator last) {
        clear();
        const difference_type count = last - first;

        if(count > capacity()) {
            reallocate(count);
        }
        std::copy(first, last, data_);
        size_ = count;
    }

    iterator erase(const_iterator pos) {
        const difference_type index = pos - begin();
        allocator_.destroy(&data_[index]);

        for(auto i = index; i<size()-1; ++i) {
            allocator_.destroy(&data_[i+1]);
            allocator_.construct(&data_[i], data_[i+1]);
        }
        --size_;
        iterator it = &data_[index];
        return it;
    }
    iterator erase(const_iterator first, const_iterator last) {
        const difference_type startIndex = first - begin();
        const difference_type endIndex = last - begin();

        for(difference_type i = 0; i < endIndex - startIndex; ++i) {
            allocator_.destroy(&data_[startIndex + i]);
            allocator_.construct(&data_[startIndex + i], data_[endIndex + i]);
        }
        size_ -= endIndex - startIndex;
        iterator it = &data_[startIndex];
        return it;
    }
    
    iterator insert(const_iterator pos, const T& value) {
        return insert(pos, 1, value);
    }
    iterator insert(const_iterator pos, T&& value) {
        difference_type index = pos - begin();

        if(index < 0 || index > size()) {
            throw new std::out_of_range("Insert index is out of range");
        }

        if(size() == capacity()) {
            reallocate(capacity() + 1);
        }

        iterator it = &data_[index];
        std::move(it, end(), it + 1);
        *it = std::move(value);

        ++size_;
        return it;
    }
    iterator insert(const_iterator pos, size_type count, const T& value) {
        const difference_type index = pos - begin();

        if(index < 0 || index > size()) {
            throw new std::out_of_range("Insert index is out of range");
        }

        if(size() + count > capacity()) {
            reallocate(capacity() + count);
        }

        iterator it = &data_[index];
        std::move(it, end(), it + count);
        std::fill_n(it, count, value);

        size_ += count;
        return it;
    }
    iterator insert(const_iterator pos, iterator first, iterator last) {
        const difference_type count = last - first;
        const difference_type index = pos - begin();

        if(size() + count > capacity()) {
            reallocate(capacity() + count);
        }

        iterator it = &data_[index];
        std::move(it, end(), it + count);
        std::copy(first, last, it);

        size_ += count;
        return it;
    }
    iterator insert(const_iterator pos, std::initializer_list<T> ilist) {
        const difference_type count = ilist.size();
        const difference_type index = pos - begin();

        if(size() + count > capacity()) {
            reallocate(capacity() + count);
        }
        
        iterator it = &data_[index];
        std::move(it, end(), it + count);
        std::copy(ilist.begin(), ilist.end(), it);

        size_ += count;
        return it;
    }



    // Element Access:
    reference at (size_type index) {
        if (index < size() && index >= 0) {
            return data_[index];
        }
        throw std::out_of_range("index is out of range");
    }
    const_reference at (size_type index) const {
        if (index < size() && index >= 0) {
            return data_[index];
        }
        throw std::out_of_range("index is out of range");
    }

    reference front () {
        return data_[0];
    }
    const_reference front () const {
        return data_[0];
    }

    reference back () {
        return data_[size() - 1];
    }
    const_reference back () const {
        return data_[size() - 1];
    }

    T* data() noexcept {
        return data_;
    }
    const T* data() const noexcept {
        return data_;
    }

    reference operator [] (size_type index) {
        return data_[index]; 
    }
    const_reference operator [] (size_type index) const {
        return data_[index]; 
    }


    // Iterators:
    iterator begin() noexcept {
        return data_;
    }
    const_iterator begin() const noexcept {
        return data_;
    }

    iterator end() noexcept {
        return begin() + size_;
    }
    const_iterator end() const noexcept {
        return begin() + size_;
    }

    const_iterator cbegin() const noexcept {
        return begin();
    }
    const_iterator cend() const noexcept {
        return end();
    }

    reverse_iterator rbegin() noexcept {
        return reverse_iterator(data_ + size_);
    }
    const_reverse_iterator rbegin() const noexcept {
        return rbegin();
    }

    reverse_iterator rend() noexcept {
        return reverse_iterator(data_);
    }
    const_reverse_iterator rend() const noexcept {
        return rend();
    }

    const_reverse_iterator crbegin() const noexcept {
        return rbegin();
    }
    const_reverse_iterator crend() const noexcept {
        return crbegin();
    }

    friend std::ostream& operator<<(std::ostream &out, const vector &v) {
        size_t len = v.size_;
        out << "[" << len << "]" << "(";
        for(size_t i = 0; i < len; ++i) {
            out << v[i];
            if (i+1 != len) {
                out << ",";
            }
        }
        out << ")";
        return out;
    }

private:
    size_t size_;
    T *data_;
    size_t capacity_;
    Allocator allocator_;

    void reallocate(size_type newCapacity) {
        T* newData = allocator_.allocate(newCapacity);
        for (size_type i = 0; i < size_; ++i) {
            allocator_.construct(&newData[i], std::move(data_[i]));
            allocator_.destroy(&data_[i]);
        }

        allocator_.deallocate(data_, capacity());
        data_ = newData;
        capacity_ = newCapacity;
    }
    
};

// Non-member function overloads
template<class T, class Alloc>
bool operator == (const vector<T,Alloc> &lhs, const vector<T, Alloc> &rhs) {
    if(lhs.size() != rhs.size()) {
        return false;
    }
    for(size_t i = 0; i<lhs.size(); ++i) {
        if(lhs.at(i) != rhs.at(i)) {
            return false;
        }
    }
    return true;
}

template<class T, class Alloc>
bool operator != (const vector<T,Alloc> &lhs, const vector<T, Alloc> &rhs) {
    return !(lhs == rhs);
}

template<class T, class Alloc>
bool operator < (const vector<T,Alloc> &lhs, const vector<T, Alloc> &rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<class T, class Alloc>
bool operator > (const vector<T,Alloc> &lhs, const vector<T, Alloc> &rhs) {
    return rhs < lhs;
}

template<class T, class Alloc>
bool operator <= (const vector<T,Alloc> &lhs, const vector<T, Alloc> &rhs) {
    return !(rhs < lhs);
}

template<class T, class Alloc>
bool operator >= (const vector<T,Alloc> &lhs, const vector<T, Alloc> &rhs) {
    return !(lhs < rhs);
}

template<class T, class Alloc>
void swap (vector<T,Alloc>& x, vector<T,Alloc>& y) {
    x.swap(y);
}

} // namespace comtest

#endif
