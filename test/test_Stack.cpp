#include <gtest.h>
#include "..//Multistacklib/Stack.h"

TEST(TStackTest, DefaultConstructor)
{
  TStack<int> stack;
  EXPECT_EQ(stack.GetCapacity(), 10);
  EXPECT_EQ(stack.GetTop(), 0);
  EXPECT_TRUE(stack.IsEmpty());
  EXPECT_FALSE(stack.IsFull());
}

TEST(TStackTest, ParameterizedConstructor)
{
  TStack<int> stack(5);
  EXPECT_EQ(stack.GetCapacity(), 5);
  EXPECT_EQ(stack.GetTop(), 0);
  EXPECT_TRUE(stack.IsEmpty());
  EXPECT_FALSE(stack.IsFull());
}

TEST(TStackTest, CopyConstructor)
{
  TStack<int> stack1;
  stack1.Put(1);
  stack1.Put(2);
  stack1.Put(3);

  TStack<int> stack2(stack1);
  EXPECT_EQ(stack1.GetTop(), stack2.GetTop());
  EXPECT_EQ(stack1.GetCapacity(), stack2.GetCapacity());
  EXPECT_TRUE(stack1 == stack2);
}

TEST(TStackTest, MoveConstructor)
{
  TStack<int> stack1;
  stack1.Put(1);
  stack1.Put(2);

  TStack<int> stack2(std::move(stack1));
  EXPECT_EQ(stack2.GetTop(), 2);
  EXPECT_EQ(stack1.GetTop(), 0);
  EXPECT_EQ(stack1.GetMemory(), nullptr);
}

TEST(TStackTest, PutAndGet)
{
  TStack<int> stack;
  stack.Put(10);
  stack.Put(20);
  stack.Put(30);

  EXPECT_EQ(stack.GetTop(), 3);
  EXPECT_EQ(stack.Get(), 30);
  EXPECT_EQ(stack.Get(), 20);
  EXPECT_EQ(stack.Get(), 10);
  EXPECT_TRUE(stack.IsEmpty());
}

TEST(TStackTest, IsEmpty)
{
  TStack<int> stack;
  EXPECT_TRUE(stack.IsEmpty());
  stack.Put(1);
  EXPECT_FALSE(stack.IsEmpty());
  stack.Get();
  EXPECT_TRUE(stack.IsEmpty());
}

TEST(TStackTest, IsFull)
{
  TStack<int> stack(2);
  EXPECT_FALSE(stack.IsFull());
  stack.Put(1);
  EXPECT_FALSE(stack.IsFull());
  stack.Put(2);
  EXPECT_TRUE(stack.IsFull());
}

TEST(TStackTest, AutoResize)
{
  TStack<int> stack(2);
  stack.Put(1);
  stack.Put(2);
  EXPECT_TRUE(stack.IsFull());
  EXPECT_EQ(stack.GetCapacity(), 2);

  stack.Put(3);
  EXPECT_FALSE(stack.IsFull());
  EXPECT_EQ(stack.GetCapacity(), 4);
  EXPECT_EQ(stack.GetTop(), 3);
}

TEST(TStackTest, AssignmentOperator)
{
  TStack<int> stack1;
  stack1.Put(1);
  stack1.Put(2);

  TStack<int> stack2;
  stack2 = stack1;

  EXPECT_TRUE(stack1 == stack2);
  EXPECT_EQ(stack2.Get(), 2);
  EXPECT_EQ(stack2.Get(), 1);
}

TEST(TStackTest, MoveAssignmentOperator)
{
  TStack<int> stack1;
  stack1.Put(1);
  stack1.Put(2);

  TStack<int> stack2;
  stack2 = std::move(stack1);

  EXPECT_EQ(stack2.GetTop(), 2);
  EXPECT_EQ(stack1.GetTop(), 0);
  EXPECT_EQ(stack1.GetMemory(), nullptr);
}

TEST(TStackTest, EqualityOperator)
{
  TStack<int> stack1;
  stack1.Put(1);
  stack1.Put(2);

  TStack<int> stack2;
  stack2.Put(1);
  stack2.Put(2);

  EXPECT_TRUE(stack1 == stack2);

  stack2.Put(3);
  EXPECT_FALSE(stack1 == stack2);
}

TEST(TStackTest, InequalityOperator)
{
  TStack<int> stack1;
  stack1.Put(1);

  TStack<int> stack2;
  stack2.Put(2);

  EXPECT_TRUE(stack1 != stack2);
}

TEST(TStackTest, FindMin)
{
  TStack<int> stack;
  stack.Put(5);
  stack.Put(2);
  stack.Put(8);
  stack.Put(1);
  stack.Put(4);

  EXPECT_EQ(stack.FindMin(), 1);
}

TEST(TStackTest, FindMinEmptyStack)
{
  TStack<int> stack;
  EXPECT_THROW(stack.FindMin(), const char*);
}

TEST(TStackTest, GetFromEmptyStack)
{
  TStack<int> stack;
  EXPECT_THROW(stack.Get(), const char*);
}

TEST(TStackTest, ComplexOperations)
{
  TStack<int> stack;

  for (int i = 0; i < 100; ++i)
  {
    stack.Put(i);
  }

  EXPECT_EQ(stack.GetTop(), 100);
  EXPECT_GE(stack.GetCapacity(), 100);

  for (int i = 99; i >= 0; --i)
  {
    EXPECT_EQ(stack.Get(), i);
  }

  EXPECT_TRUE(stack.IsEmpty());
}

TEST(TStackTest, DoubleStack)
{
  TStack<double> stack;
  stack.Put(1.1);
  stack.Put(2.2);
  stack.Put(3.3);

  EXPECT_DOUBLE_EQ(stack.Get(), 3.3);
  EXPECT_DOUBLE_EQ(stack.Get(), 2.2);
  EXPECT_DOUBLE_EQ(stack.Get(), 1.1);
}
