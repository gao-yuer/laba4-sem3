#pragma once
#include <iostream>
#include <fstream>
#include <algorithm>

template <class T>
class TQueue
{
protected:
  size_t capacity;
  T* memory;
  size_t front;
  size_t end;
  size_t count;

public:
  TQueue();
  TQueue(size_t initialCapacity);
  TQueue(TQueue& other);
  TQueue(TQueue&& other);
  ~TQueue();

  size_t GetCapacity();
  size_t GetFront();
  size_t GetRear();
  size_t GetCount();
  T* GetMemory();

  void Put(T& value);
  T Get();
  bool IsEmpty() const;
  bool IsFull() const;

  TQueue& operator=(const TQueue& other);
  TQueue& operator=(TQueue&& other);
  bool operator==(const TQueue& other) const;
  bool operator!=(const TQueue& other) const;

  template <class T>
  friend std::ostream& operator<<(std::ostream& os, TQueue<T>& queue);

  template <class T>
  friend std::istream& operator>>(std::istream& is, TQueue<T>& queue);

  void SaveToFile(const std::string& filename) const;
  void LoadFromFile(const std::string& filename);
  T FindMin() const;
};

template <class T>
TQueue<T>::TQueue() : capacity(10), memory(new T[10]), front(0), end(0), count(0)
{
}

template <class T>
TQueue<T>::TQueue(size_t initialCapacity) : capacity(initialCapacity), memory(new T[initialCapacity]), front(0), end(0), count(0)
{
}

template <class T>
TQueue<T>::TQueue(TQueue& other) : capacity(other.capacity), memory(new T[other.capacity]), front(other.front), end(other.end), count(other.count)
{
  for (size_t i = 0; i < capacity; ++i)
  {
    memory[i] = other.memory[i];
  }
}

template <class T>
TQueue<T>::TQueue(TQueue&& other) : capacity(other.capacity), memory(other.memory), front(other.front), end(other.end), count(other.count)
{
  other.memory = nullptr;
  other.capacity = 0;
  other.front = 0;
  other.end = 0;
  other.count = 0;
}

template <class T>
TQueue<T>::~TQueue()
{
  delete[] memory;
}

template <class T>
size_t TQueue<T>::GetCapacity() 
{
  return capacity;
}

template <class T>
size_t TQueue<T>::GetFront() 
{
  return front;
}

template <class T>
size_t TQueue<T>::GetRear() 
{
  return end;
}

template <class T>
size_t TQueue<T>::GetCount() 
{
  return count;
}

template <class T>
T* TQueue<T>::GetMemory() 
{
  return memory;
}

template <class T>
void TQueue<T>::Put(T& value)
{
  if (IsFull())
  {
    size_t newCapacity = capacity * 2;
    T* newMemory = new T[newCapacity];

    for (size_t i = 0; i < count; ++i)
    {
      newMemory[i] = memory[(front + i) % capacity];
    }

    delete[] memory;
    memory = newMemory;
    capacity = newCapacity;
    front = 0;
    end = count;
  }

  memory[end] = value;
  end = (end + 1) % capacity;
  count++;
}

template <class T>
T TQueue<T>::Get()
{
  if (IsEmpty())
  {
    throw ("Queue is empty");
  }

  T value = memory[front];
  front = (front + 1) % capacity;
  count--;
  return value;
}

template <class T>
bool TQueue<T>::IsEmpty() const
{
  return count == 0;
}

template <class T>
bool TQueue<T>::IsFull() const
{
  return count == capacity;
}

template <class T>
TQueue<T>& TQueue<T>::operator=(const TQueue& other)
{
  if (this != &other)
  {
    delete[] memory;
    capacity = other.capacity;
    front = other.front;
    end = other.end;
    count = other.count;
    memory = new T[capacity];

    for (size_t i = 0; i < capacity; ++i)
    {
      memory[i] = other.memory[i];
    }
  }
  return *this;
}

template <class T>
TQueue<T>& TQueue<T>::operator=(TQueue&& other)
{
  if (this != &other)
  {
    delete[] memory;
    capacity = other.capacity;
    front = other.front;
    end = other.end;
    count = other.count;
    memory = other.memory;

    other.memory = nullptr;
    other.capacity = 0;
    other.front = 0;
    other.end = 0;
    other.count = 0;
  }
  return *this;
}

template <class T>
bool TQueue<T>::operator==(const TQueue& other) const
{
  if (count != other.count)
  {
    return false;
  }

  for (size_t i = 0; i < count; ++i)
  {
    size_t idx1 = (front + i) % capacity;
    size_t idx2 = (other.front + i) % other.capacity;
    if (memory[idx1] != other.memory[idx2])
    {
      return false;
    }
  }
  return true;
}

template <class T>
bool TQueue<T>::operator!=(const TQueue& other) const
{
  return !(*this == other);
}

template <class T>
std::ostream& operator<<(std::ostream& os, TQueue<T>& queue)
{
  for (size_t i = 0; i < queue.count; ++i)
  {
    size_t idx = (queue.front + i) % queue.capacity;
    os << queue.memory[idx] << " ";
  }
  return os;
}

template <class T>
std::istream& operator>>(std::istream& is, TQueue<T>& queue)
{
  size_t count;
  is >> count;
  for (size_t i = 0; i < count; ++i)
  {
    T value;
    is >> value;
    queue.Put(value);
  }
  return is;
}

template <class T>
void TQueue<T>::SaveToFile(const std::string& filename) const
{
  std::ofstream file(filename, std::ios::binary);
  if (!file)
  {
    throw ("Cannot open file for writing");
  }
  file.write(reinterpret_cast<const char*>(&capacity), sizeof(capacity));
  file.write(reinterpret_cast<const char*>(&front), sizeof(front));
  file.write(reinterpret_cast<const char*>(&end), sizeof(end));
  file.write(reinterpret_cast<const char*>(&count), sizeof(count));

  for (size_t i = 0; i < capacity; ++i)
  {
    file.write(reinterpret_cast<const char*>(&memory[i]), sizeof(T));
  }
}

template <class T>
void TQueue<T>::LoadFromFile(const std::string& filename)
{
  std::ifstream file(filename, std::ios::binary);
  if (!file)
  {
    throw ("Cannot open file for reading");
  }
  delete[] memory;
  file.read(reinterpret_cast<char*>(&capacity), sizeof(capacity));
  file.read(reinterpret_cast<char*>(&front), sizeof(front));
  file.read(reinterpret_cast<char*>(&end), sizeof(end));
  file.read(reinterpret_cast<char*>(&count), sizeof(count));

  memory = new T[capacity];
  for (size_t i = 0; i < capacity; ++i)
  {
    file.read(reinterpret_cast<char*>(&memory[i]), sizeof(T));
  }
}

template <class T>
T TQueue<T>::FindMin() const
{
  if (IsEmpty())
  {
    throw ("Queue is empty");
  }

  T minElement = memory[front];
  for (size_t i = 1; i < count; ++i)
  {
    size_t idx = (front + i) % capacity;
    if (memory[idx] < minElement)
    {
      minElement = memory[idx];
    }
  }
  return minElement;
}