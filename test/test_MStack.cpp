#include <gtest.h>
#include "..//build/Multistacklib/Multistack.h"


TEST(TMultiStackTest, DefaultConstructor) {
  TMultiStack<int> stack;

  EXPECT_EQ(stack.GetStackCount(), 3);
  EXPECT_EQ(stack.GetLen(), 100);
  EXPECT_TRUE(stack.IsEmpty(0));
  EXPECT_TRUE(stack.IsEmpty(1));
  EXPECT_TRUE(stack.IsEmpty(2));
}

TEST(TMultiStackTest, ParameterizedConstructor) {
  TMultiStack<int> stack(50, 2);

  EXPECT_EQ(stack.GetStackCount(), 2);
  EXPECT_EQ(stack.GetLen(), 50);
  EXPECT_TRUE(stack.IsEmpty(0));
  EXPECT_TRUE(stack.IsEmpty(1));
}

TEST(TMultiStackTest, CopyConstructor) {
  TMultiStack<int> original(30, 2);
  original.Push(0, 10);
  original.Push(0, 20);
  original.Push(1, 30);

  TMultiStack<int> copy(original);

  EXPECT_EQ(copy.GetStackCount(), 2);
  EXPECT_EQ(copy.GetLen(), 30);
  EXPECT_FALSE(copy.IsEmpty(0));
  EXPECT_FALSE(copy.IsEmpty(1));
  EXPECT_EQ(copy.Pop(0), 20);
  EXPECT_EQ(copy.Pop(0), 10);
  EXPECT_EQ(copy.Pop(1), 30);
}

TEST(TMultiStackTest, MoveConstructor) {
  TMultiStack<int> original(30, 2);
  original.Push(0, 10);
  original.Push(1, 20);

  TMultiStack<int> moved(std::move(original));

  EXPECT_EQ(moved.GetStackCount(), 2);
  EXPECT_EQ(moved.GetLen(), 30);
  EXPECT_EQ(moved.Pop(0), 10);
  EXPECT_EQ(moved.Pop(1), 20);

  EXPECT_EQ(original.GetStackCount(), 0);
  EXPECT_EQ(original.GetLen(), 0);
}

TEST(TMultiStackTest, CopyAssignment) {
  TMultiStack<int> original(30, 2);
  original.Push(0, 10);
  original.Push(1, 20);

  TMultiStack<int> copy;
  copy = original;

  EXPECT_EQ(copy.GetStackCount(), 2);
  EXPECT_EQ(copy.GetLen(), 30);
  EXPECT_EQ(copy.Pop(0), 10);
  EXPECT_EQ(copy.Pop(1), 20);
  EXPECT_TRUE(copy.IsEmpty(0));
  EXPECT_TRUE(copy.IsEmpty(1));

  EXPECT_EQ(original.Pop(0), 10);
  EXPECT_EQ(original.Pop(1), 20);
}


TEST(TMultiStackTest, EqualityOperators) {
  TMultiStack<int> stack1(30, 2);
  TMultiStack<int> stack2(30, 2);
  TMultiStack<int> stack3(40, 3);

  stack1.Push(0, 10);
  stack2.Push(0, 10);

  EXPECT_TRUE(stack1 == stack2);
  EXPECT_FALSE(stack1 != stack2);
  EXPECT_FALSE(stack1 == stack3);
  EXPECT_TRUE(stack1 != stack3);

  stack1.Push(1, 20);
  EXPECT_FALSE(stack1 == stack2);
  EXPECT_TRUE(stack1 != stack2);
}

TEST(TMultiStackTest, BasicPushPopOperations) {
  TMultiStack<int> stack(20, 2);

  stack.Push(0, 1);
  stack.Push(0, 2);
  stack.Push(0, 3);
  stack.Push(1, 4);
  stack.Push(1, 5);

  EXPECT_FALSE(stack.IsEmpty(0));
  EXPECT_FALSE(stack.IsEmpty(1));
  EXPECT_EQ(stack[0].GetTop(), 3);
  EXPECT_EQ(stack[1].GetTop(), 2);

  EXPECT_EQ(stack.Pop(0), 3);
  EXPECT_EQ(stack.Pop(0), 2);
  EXPECT_EQ(stack.Pop(0), 1);
  EXPECT_EQ(stack.Pop(1), 5);
  EXPECT_EQ(stack.Pop(1), 4);

  EXPECT_TRUE(stack.IsEmpty(0));
  EXPECT_TRUE(stack.IsEmpty(1));
}

TEST(TMultiStackTest, IsEmptyIsFull) {
  TMultiStack<int> stack(5, 2);

  EXPECT_TRUE(stack.IsEmpty(0));
  EXPECT_TRUE(stack.IsEmpty(1));
  EXPECT_FALSE(stack.IsFull(0));
  EXPECT_FALSE(stack.IsFull(1));

  stack.Push(0, 1);
  stack.Push(0, 2);

  EXPECT_FALSE(stack.IsEmpty(0));
  EXPECT_FALSE(stack.IsFull(0));

  stack.Push(0, 3); 

  EXPECT_TRUE(stack.IsFull(0));
  EXPECT_FALSE(stack.IsEmpty(0));
}


TEST(TMultiStackTest, RepackTest) {
  TMultiStack<int> stack(6, 2); 

  stack.Push(0, 1);
  stack.Push(0, 2);
  stack.Push(0, 3);

  EXPECT_TRUE(stack.IsFull(0));
  EXPECT_FALSE(stack.IsFull(1));

  stack.Push(0, 4);

  EXPECT_FALSE(stack.IsFull(0));
  EXPECT_FALSE(stack.IsFull(1));

  EXPECT_EQ(stack.Pop(0), 4);
  EXPECT_EQ(stack.Pop(0), 3);
  EXPECT_EQ(stack.Pop(0), 2);
  EXPECT_EQ(stack.Pop(0), 1);
}


TEST(TMultiStackTest, DifferentDataTypes) {
  TMultiStack<double> doubleStack(20, 2);
  doubleStack.Push(0, 1.5);
  doubleStack.Push(1, 2.7);

  EXPECT_DOUBLE_EQ(doubleStack.Pop(0), 1.5);
  EXPECT_DOUBLE_EQ(doubleStack.Pop(1), 2.7);

  TMultiStack<std::string> stringStack(20, 2);
  stringStack.Push(0, "hello");
  stringStack.Push(1, "world");

  EXPECT_EQ(stringStack.Pop(0), "hello");
  EXPECT_EQ(stringStack.Pop(1), "world");
}

TEST(TMultiStackTest, MemoryEfficiency) 
{
  TMultiStack<int> stack(100, 4); 

  for (int i = 0; i < 20; i++) 
  {
    stack.Push(0, i);
    stack.Push(1, i * 2);
  }

  EXPECT_FALSE(stack.IsFull(0));
  EXPECT_FALSE(stack.IsFull(1));
  EXPECT_TRUE(stack.IsEmpty(2));
  EXPECT_TRUE(stack.IsEmpty(3));

  stack.Push(0, 100); 

  EXPECT_EQ(stack.Pop(0), 100);
  for (int i = 19; i >= 0; i--) {
    EXPECT_EQ(stack.Pop(0), i);
    EXPECT_EQ(stack.Pop(1), i * 2);
  }
}


TEST(TMultiStackTest, SequentialOperations) 
{
  TMultiStack<int> stack(50, 3);
 
  for (int i = 0; i < 10; i++) {
    stack.Push(0, i);
    stack.Push(1, i * 10);
    stack.Push(2, i * 100);
  }

  for (int i = 9; i >= 0; i--) {
    EXPECT_EQ(stack.Pop(0), i);
    EXPECT_EQ(stack.Pop(1), i * 10);
    EXPECT_EQ(stack.Pop(2), i * 100);
  }

  EXPECT_TRUE(stack.IsEmpty(0));
  EXPECT_TRUE(stack.IsEmpty(1));
  EXPECT_TRUE(stack.IsEmpty(2));
}

