#pragma once
#include <iostream>
#include <algorithm>
#include "..//Multistacklib/Stack.h"

template <class T>
class TMultiStack
{
private:
  size_t len;
  size_t stackCount;
  T** data;
  TStack<T>* stacks;

public:
  TMultiStack();
  TMultiStack(size_t totalSize, size_t numStacks);
  TMultiStack(const TMultiStack& other);
  TMultiStack(TMultiStack&& other);
  ~TMultiStack();

  void Push(size_t stackIndex, const T& value);
  T Pop(size_t stackIndex);
  bool IsEmpty(size_t stackIndex) const;
  bool IsFull(size_t stackIndex) const;
  void Repack(int curStack);

  TMultiStack& operator=(const TMultiStack& other);
  TMultiStack& operator=(TMultiStack&& other);
  bool operator==(const TMultiStack& other) const;
  bool operator!=(const TMultiStack& other) const;
  TStack<T>& operator[](size_t stackIndex);
  const TStack<T>& operator[](size_t stackIndex) const;

  template <class T>
  friend std::ostream& operator<<(std::ostream& os, const TMultiStack<T>& multiStack);

  template <class T>
  friend std::istream& operator>>(std::istream& is, TMultiStack<T>& multiStack);

  size_t GetLen();
  size_t GetStackCount();
  T** GetData();
  TStack<T>* GetStacks();

  void InitializeStacks();
  size_t GetStackStart(size_t stackIndex);
  void UpdateStackPointers(size_t stackIndex, size_t newStart, size_t newCapacity);
};

template <class T>
TMultiStack<T>::TMultiStack() : len(100), stackCount(3), data(new T* [3]), stacks(new TStack<T>[3])
{
  InitializeStacks();
}

template <class T>
TMultiStack<T>::TMultiStack(size_t totalSize, size_t numStacks) : len(totalSize), stackCount(numStacks),
data(new T* [numStacks]), stacks(new TStack<T>[numStacks])
{
  if (numStacks == 0)
  {
    throw ("Number of stacks must be positive");
  }
  InitializeStacks();
}

template <class T>
void TMultiStack<T>::InitializeStacks()
{
  T* commonMemory = new T[len];

  size_t baseCapacity = len / stackCount;
  size_t remainder = len % stackCount;

  size_t currentPos = 0;
  for (size_t i = 0; i < stackCount; ++i)
  {
    size_t stackCapacity = baseCapacity + (i < remainder ? 1 : 0);
    data[i] = commonMemory + currentPos;
    stacks[i] = TStack<T>(stackCapacity);
    currentPos += stackCapacity;
  }
}

template <class T>
TMultiStack<T>::TMultiStack(const TMultiStack& other) : len(other.len), stackCount(other.stackCount),
data(new T* [other.stackCount]), stacks(new TStack<T>[other.stackCount])
{
  T* commonMemory = new T[len];
  for (size_t i = 0; i < len; ++i)
  {
    commonMemory[i] = other.data[0][i];
  }

  size_t currentPos = 0;
  for (size_t i = 0; i < stackCount; ++i)
  {
    size_t stackCapacity = other.stacks[i].GetCapacity();
    data[i] = commonMemory + currentPos;
    stacks[i] = other.stacks[i];
    currentPos += stackCapacity;
  }
}

template <class T>
TMultiStack<T>::TMultiStack(TMultiStack&& other) : len(other.len), stackCount(other.stackCount),
data(other.data), stacks(other.stacks)
{
  other.data = nullptr;
  other.stacks = nullptr;
  other.len = 0;
  other.stackCount = 0;
}

template <class T>
TMultiStack<T>::~TMultiStack()
{
  if (data != nullptr && stackCount > 0)
  {
    delete[] data[0];
    delete[] data;
    delete[] stacks;
  }
}

template <class T>
void TMultiStack<T>::Push(size_t stackIndex, const T& value)
{
  if (stackIndex >= stackCount)
  {
    throw ("Stack index out of range");
  }

  if (IsFull(stackIndex))
  {
    Repack(static_cast<int>(stackIndex));
  }

  stacks[stackIndex].Put(value);
}

template <class T>
T TMultiStack<T>::Pop(size_t stackIndex)
{
  if (stackIndex >= stackCount)
  {
    throw ("Stack index out of range");
  }

  if (IsEmpty(stackIndex))
  {
    throw ("Stack is empty");
  }

  return stacks[stackIndex].Get();
}

template <class T>
bool TMultiStack<T>::IsEmpty(size_t stackIndex) const
{
  if (stackIndex >= stackCount)
  {
    throw ("Stack index out of range");
  }
  return stacks[stackIndex].IsEmpty();
}

template <class T>
bool TMultiStack<T>::IsFull(size_t stackIndex) const
{
  if (stackIndex >= stackCount)
  {
    throw ("Stack index out of range");
  }
  return stacks[stackIndex].IsFull();
}

template <class T>
void TMultiStack<T>::Repack(int curStack)
{
  size_t* newStart = new size_t[stackCount];
  size_t* newSizes = new size_t[stackCount];

  size_t busy = 0;
  for (size_t i = 0; i < stackCount; i++)
  {
    busy += stacks[i].GetTop();
  }

  size_t freeCells = len - busy;

  if (freeCells == 0)
  {
    size_t newLen = len * 2;
    T* newCommonMemory = new T[newLen];

    size_t baseCapacity = newLen / stackCount;
    size_t remainder = newLen % stackCount;

    size_t currentPos = 0;
    for (size_t i = 0; i < stackCount; i++)
    {
      size_t stackCapacity = baseCapacity + (i < remainder ? 1 : 0);
      size_t elementCount = stacks[i].GetTop();

      for (size_t j = 0; j < elementCount; j++)
      {
        newCommonMemory[currentPos + j] = stacks[i].GetMemory()[j];
      }

      newStart[i] = currentPos;
      newSizes[i] = stackCapacity;
      currentPos += stackCapacity;
    }

    delete[] data[0];
    data[0] = newCommonMemory;
    len = newLen;
  }
  else
  {
    size_t ns = freeCells / stackCount;
    size_t tail = freeCells - ns * stackCount;

    newStart[0] = 0;
    newSizes[0] = stacks[0].GetTop() + ns;

    for (size_t i = 1; i < stackCount; i++)
    {
      newSizes[i] = stacks[i].GetTop() + ns;
      if (i == curStack)
      {
        newSizes[i] += tail;
      }
      newStart[i] = newStart[i - 1] + newSizes[i - 1];
    }

    for (size_t i = 1; i < stackCount; i++)
    {
      size_t currentStart = GetStackStart(i);
      if (newStart[i] == currentStart)
      {
        continue;
      }
      else if (newStart[i] < currentStart)
      {
        for (size_t j = 0; j < stacks[i].GetTop(); j++)
        {
          data[0][newStart[i] + j] = data[0][currentStart + j];
        }
      }
      else if (newStart[i] > currentStart)
      {
        size_t q = i;
        while (q < stackCount && newStart[q] > GetStackStart(q))
        {
          q++;
        }

        for (size_t w = q - 1; w >= i; w--)
        {
          size_t wStart = GetStackStart(w);
          for (size_t n = stacks[w].GetTop(); n > 0; n--)
          {
            data[0][newStart[w] + n - 1] = data[0][wStart + n - 1];
          }
        }
        i = q - 1;
      }
    }
  }

  for (size_t i = 0; i < stackCount; i++)
  {
    UpdateStackPointers(i, newStart[i], newSizes[i]);
  }

  delete[] newStart;
  delete[] newSizes;
}

template <class T>
TMultiStack<T>& TMultiStack<T>::operator=(const TMultiStack& other)
{
  if (this != &other)
  {
    if (data != nullptr)
    {
      delete[] data[0];
      delete[] data;
      delete[] stacks;
    }

    len = other.len;
    stackCount = other.stackCount;
    data = new T * [stackCount];
    stacks = new TStack<T>[stackCount];

    T* commonMemory = new T[len];
    for (size_t i = 0; i < len; ++i)
    {
      commonMemory[i] = other.data[0][i];
    }

    size_t currentPos = 0;
    for (size_t i = 0; i < stackCount; ++i)
    {
      size_t stackCapacity = other.stacks[i].GetCapacity();
      data[i] = commonMemory + currentPos;
      stacks[i] = other.stacks[i];
      currentPos += stackCapacity;
    }
  }
  return *this;
}

template <class T>
TMultiStack<T>& TMultiStack<T>::operator=(TMultiStack&& other)
{
  if (this != &other)
  {
    if (data != nullptr)
    {
      delete[] data[0];
      delete[] data;
      delete[] stacks;
    }

    len = other.len;
    stackCount = other.stackCount;
    data = other.data;
    stacks = other.stacks;

    other.data = nullptr;
    other.stacks = nullptr;
    other.len = 0;
    other.stackCount = 0;
  }
  return *this;
}

template <class T>
bool TMultiStack<T>::operator==(const TMultiStack& other) const
{
  if (len != other.len || stackCount != other.stackCount)
  {
    return false;
  }

  for (size_t i = 0; i < stackCount; ++i)
  {
    if (stacks[i] != other.stacks[i])
    {
      return false;
    }
  }

  return true;
}

template <class T>
bool TMultiStack<T>::operator!=(const TMultiStack& other) const
{
  return !(*this == other);
}

template <class T>
TStack<T>& TMultiStack<T>::operator[](size_t stackIndex)
{
  if (stackIndex >= stackCount)
  {
    throw ("Stack index out of range");
  }
  return stacks[stackIndex];
}

template <class T>
const TStack<T>& TMultiStack<T>::operator[](size_t stackIndex) const
{
  if (stackIndex >= stackCount)
  {
    throw ("Stack index out of range");
  }
  return stacks[stackIndex];
}

template <class T>
std::ostream& operator<<(std::ostream& os, const TMultiStack<T>& multiStack)
{
  os << "MultiStack with " << multiStack.stackCount << " stacks, total size: " << multiStack.len << "\n";
  for (size_t i = 0; i < multiStack.stackCount; ++i)
  {
    os << "Stack " << i << " (capacity: " << multiStack.stacks[i].GetCapacity()
      << ", size: " << multiStack.stacks[i].GetTop() << "): " << multiStack.stacks[i] << "\n";
  }
  return os;
}

template <class T>
std::istream& operator>>(std::istream& is, TMultiStack<T>& multiStack)
{
  size_t numStacks, totalSize;
  is >> numStacks >> totalSize;

  TMultiStack<T> temp(totalSize, numStacks);

  for (size_t i = 0; i < numStacks; ++i)
  {
    size_t elementCount;
    is >> elementCount;
    for (size_t j = 0; j < elementCount; ++j)
    {
      T value;
      is >> value;
      temp.Push(i, value);
    }
  }

  multiStack = std::move(temp);
  return is;
}

template <class T>
size_t TMultiStack<T>::GetLen()
{
  return len;
}

template <class T>
size_t TMultiStack<T>::GetStackCount() 
{
  return stackCount;
}

template <class T>
T** TMultiStack<T>::GetData() 
{
  return data;
}

template <class T>
TStack<T>* TMultiStack<T>::GetStacks() 
{
  return stacks;
}

template <class T>
size_t TMultiStack<T>::GetStackStart(size_t stackIndex)
{
  return data[stackIndex] - data[0];
}

template <class T>
void TMultiStack<T>::UpdateStackPointers(size_t stackIndex, size_t newStart, size_t newCapacity)
{
  size_t elementCount = stacks[stackIndex].GetTop();
  T* temp = new T[elementCount];
  for (size_t i = 0; i < elementCount; i++)
  {
    temp[i] = stacks[stackIndex].GetMemory()[i];
  }

  data[stackIndex] = data[0] + newStart;

  for (size_t i = 0; i < elementCount; i++)
  {
    data[stackIndex][i] = temp[i];
  }

  delete[] temp;
}