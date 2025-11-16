#pragma once
#include <iostream>
#include <fstream>
#include <algorithm>

template <class T>
class TStack
{
protected:
  size_t capacity;
  T* memory;
  size_t top;

public:
  TStack();
  TStack(size_t initialCapacity);
  TStack(const TStack& other);
  TStack(TStack&& other);
  ~TStack();

  size_t GetCapacity() const;
  size_t GetTop() const;
  T* GetMemory() const;

  void Put(const T& value);
  T Get();
  bool IsEmpty() const;
  bool IsFull() const;

  TStack& operator=(const TStack& other);
  TStack& operator=(TStack&& other);
  bool operator==(const TStack& other) const;
  bool operator!=(const TStack& other) const;

  template <class T>
  friend std::ostream& operator<<(std::ostream& os, TStack<T>& stack);

  template <class T>
  friend std::istream& operator>>(std::istream& is, TStack<T>& stack);

  // допы
  void SaveToFile(const std::string& filename) const;
  void LoadFromFile(const std::string& filename);
  T FindMin() const;

};

template <class T>
TStack<T>::TStack() : capacity(10), memory(new T[10]), top(0)
{
}

template <class T>
TStack<T>::TStack(size_t initialCapacity) : capacity(initialCapacity), memory(new T[initialCapacity]), top(0)
{
}

template <class T>
TStack<T>::TStack(const TStack& other) : capacity(other.capacity), memory(new T[other.capacity]), top(other.top)
{
  for (size_t i = 0; i < top; ++i)
  {
    memory[i] = other.memory[i];
  }
}

template <class T>
TStack<T>::TStack(TStack&& other) : capacity(other.capacity), memory(other.memory), top(other.top)
{
  other.memory = nullptr;
  other.capacity = 0;
  other.top = 0;
}

template <class T>
TStack<T>::~TStack()
{
  delete[] memory;
}

template <class T>
size_t TStack<T>::GetCapacity() const
{
  return capacity;
}

template <class T>
size_t TStack<T>::GetTop() const
{
  return top;
}

template <class T>
T* TStack<T>::GetMemory() const
{
  return memory;
}

template <class T>
void TStack<T>::Put(const T& value)
{
  if (IsFull())
  {
    size_t newCapacity = capacity * 2;
    T* newMemory = new T[newCapacity];
    for (size_t i = 0; i < top; ++i)
    {
      newMemory[i] = memory[i];
    }
    delete[] memory;
    memory = newMemory;
    capacity = newCapacity;
  }
  memory[top++] = value;
}

template <class T>
T TStack<T>::Get()
{
  if (IsEmpty())
  {
    throw ("Stack is empty");
  }
  return memory[--top];
}

template <class T>
bool TStack<T>::IsEmpty() const
{
  return top == 0;
}

template <class T>
bool TStack<T>::IsFull() const
{
  return top == capacity;
}

template <class T>
TStack<T>& TStack<T>::operator=(const TStack& other)
{
  if (this != &other)
  {
    delete[] memory;
    capacity = other.capacity;
    top = other.top;
    memory = new T[capacity];
    for (size_t i = 0; i < top; ++i)
    {
      memory[i] = other.memory[i];
    }
  }
  return *this;
}

template <class T>
TStack<T>& TStack<T>::operator=(TStack&& other)
{
  if (this != &other)
  {
    delete[] memory;
    capacity = other.capacity;
    top = other.top;
    memory = other.memory;
    other.memory = nullptr;
    other.capacity = 0;
    other.top = 0;
  }
  return *this;
}

template <class T>
bool TStack<T>::operator==(const TStack& other) const
{
  if (top != other.top)
  {
    return false;
  }
  for (size_t i = 0; i < top; ++i)
  {
    if (memory[i] != other.memory[i])
    {
      return false;
    }
  }
  return true;
}

template <class T>
bool TStack<T>::operator!=(const TStack& other) const
{
  return !(*this == other);
}

template <class T>
std::ostream& operator<<(std::ostream& os, TStack<T>& stack)
{
  for (size_t i = 0; i < stack.top; ++i)
  {
    os << stack.memory[i] << " ";
  }
  return os;
}

template <class T>
std::istream& operator>>(std::istream& is, TStack<T>& stack)
{
  size_t count;
  is >> count;
  for (size_t i = 0; i < count; ++i)
  {
    U value;
    is >> value;
    stack.Put(value);
  }
  return is;
}

template <class T>
void TStack<T>::SaveToFile(const std::string& filename) const
{
  std::ofstream file(filename, std::ios::binary);
  if (!file)
  {
    throw ("Cannot open file for writing");
  }
  file.write(reinterpret_cast<const char*>(&top), sizeof(top));
  file.write(reinterpret_cast<const char*>(&capacity), sizeof(capacity));
  for (size_t i = 0; i < top; ++i)
  {
    file.write(reinterpret_cast<const char*>(&memory[i]), sizeof(T));
  }
}

template <class T>
void TStack<T>::LoadFromFile(const std::string& filename)
{
  std::ifstream file(filename, std::ios::binary);
  if (!file)
  {
    throw ("Cannot open file for reading");
  }
  delete[] memory;
  file.read(reinterpret_cast<char*>(&top), sizeof(top));
  file.read(reinterpret_cast<char*>(&capacity), sizeof(capacity));
  memory = new T[capacity];
  for (size_t i = 0; i < top; ++i)
  {
    file.read(reinterpret_cast<char*>(&memory[i]), sizeof(T));
  }
}

template <class T>
T TStack<T>::FindMin() const
{
  if (IsEmpty())
  {
    throw ("Stack is empty");
  }
  T minElement = memory[0];
  for (size_t i = 1; i < top; ++i)
  {
    if (memory[i] < minElement)
    {
      minElement = memory[i];
    }
  }
  return minElement;
}

