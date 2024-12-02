#ifndef DATA_HPP
#define DATA_HPP

#include <cstddef>
#include <cstdint>

struct Data {
  size_t m_size;
  uint64_t* m_data;

  Data() : m_size(0), m_data(nullptr) {}

  Data(size_t size) : m_size(size), m_data(new uint64_t[size]) {
    for (size_t i = 0; i < size; ++i) {
      m_data[i] = 0;
    }
  }

  Data(size_t size, uint64_t value) : m_size(size), m_data(new uint64_t[size]) {
    for (size_t i = 0; i < size; ++i) {
      m_data[i] = value;
    }
  }

  Data(const Data& rhs) : m_size(rhs.m_size), m_data(new uint64_t[rhs.m_size]) {
    for (size_t i = 0; i < rhs.m_size; ++i) {
      m_data[i] = rhs.m_data[i];
    }
  }

  Data& operator=(const Data& rhs) {
    if (this != &rhs) {
      delete[] m_data;
      m_size = rhs.m_size;
      m_data = new uint64_t[rhs.m_size];
      for (size_t i = 0; i < rhs.m_size; ++i) {
        m_data[i] = rhs.m_data[i];
      }
    }
    return *this;
  }

  Data(Data&& rhs) : m_size(rhs.m_size), m_data(rhs.m_data) {
    rhs.m_size = 0;
    rhs.m_data = nullptr;
  }

  Data& operator=(Data&& rhs) {
    if (this != &rhs) {
      delete[] m_data;
      m_size = rhs.m_size;
      m_data = rhs.m_data;
      rhs.m_size = 0;
      rhs.m_data = nullptr;
    }
    return *this;
  }

  ~Data() { delete[] m_data; }

  bool operator==(const Data& rhs) const {
    if (m_size != rhs.m_size) {
      return false;
    }
    for (size_t i = 0; i < m_size; i++) {
      if (m_data[i] != rhs.m_data[i]) {
        return false;
      }
    }
    return true;
  }
};

#endif  // DATA_HPP