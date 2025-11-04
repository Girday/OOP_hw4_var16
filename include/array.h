#pragma once

#include "figure.h"

#include <array>

template <typename T>
class Array {
public:
    Array() : data(nullptr), capacity(0), size(0) {}
    ~Array() = default;

    Array(const Array& other) = delete;
    Array& operator=(const Array& other) = delete;

    Array(Array&& other) noexcept
        : data(std::move(other.data)), capacity(other.capacity), size(other.size) {
        other.capacity = 0;
        other.size = 0;
    }

    Array& operator=(Array&& other) noexcept {
        if (this != &other) {
            data = std::move(other.data);
            capacity = other.capacity;
            size = other.size;

            other.capacity = 0;
            other.size = 0;
        }
        return *this;
    }

    void add(std::shared_ptr<T> element) {
        if (size == capacity)
            grow();
        data[size++] = std::move(element);
    }

    void remove(size_t index) {
        if (size == 0)
            throw std::out_of_range("Array is empty");
        if (index >= size)
            throw std::out_of_range("Index out of range");
        
        for (size_t i = index; i < size - 1; ++i)
            data[i] = std::move(data[i + 1]);
        
        data[size - 1].reset();
        --size;

        std::cout << "Element at index " << index << " removed.\n";
    }

    void printAll() const {
        if (size == 0)
            throw std::out_of_range("Array is empty");
        
        for (size_t i = 0; i < size; ++i)
            std::cout << i << ": " << *data[i] << " | Area: " << static_cast<double>(*data[i]) << "\n";  
    }

    void printCenters() const {
        if (size == 0)
            throw std::out_of_range("Array is empty");

        for (size_t i = 0; i < size; ++i) {
            auto c = data[i]->center();
            std::cout << i << ": Center = (" << c.x() << ", " << c.y() << ")\n";
        }
    }

    void printTotalArea() const {
        if (size == 0)
            throw std::out_of_range("Array is empty");

        double totalArea = 0.0;
        for (size_t i = 0; i < size; ++i)
            totalArea += static_cast<double>(*(data[i]));
        
        std::cout << "Total Area: " << totalArea << "\n";
    }

    int getSize() const {
        return size;
    }

private:
    void grow() {
        size_t newCapacity = (capacity == 0) ? 2 : capacity * 2;
        auto newData = std::make_unique<std::shared_ptr<T>[]>(newCapacity);
        
        for (size_t i = 0; i < size; ++i)
            newData[i] = std::move(data[i]);
        
        data = std::move(newData);
        capacity = newCapacity;
    }

    std::unique_ptr<std::shared_ptr<T>[]> data;
    size_t capacity;
    size_t size;
};
