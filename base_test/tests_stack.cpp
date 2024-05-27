#include "gtest.h"
#include "stack.h"

TEST(Stack, can_create_stack) {
	ASSERT_NO_THROW(Stack<int>{});
	ASSERT_NO_THROW(Stack<int>{10});
	ASSERT_NO_THROW(Stack<int>{Stack<int>{}});
}

TEST(Stack, can_push_element) {
	Stack<int> s;
	ASSERT_NO_THROW(s.push(6));
	EXPECT_EQ(1, s.get_size());
}

TEST(Stack, can_pop_element) {
	Stack<int> s;
	s.push(1);
	s.push(3);
	s.push(6);
	ASSERT_NO_THROW(s.pop());
	EXPECT_EQ(2, s.get_size());
}

TEST(Stack, test_compare_operators) {
	Stack<int> s1, s2;
	s1.push(1);
	s1.push(3);
	s1.push(6);
	s2.push(1);
	s2.push(3);
	s2.push(6);
	EXPECT_TRUE(s1 == s2);
	EXPECT_FALSE(s1 != s2);
	s2.pop();
	EXPECT_FALSE(s1 == s2);
}

TEST(Stack, can_copy_stack) {
	Stack<int> s;
	s.push(1);
	s.push(3);
	s.push(6);
	Stack<int> cop(s);
	EXPECT_EQ(s, cop);
}

TEST(Stack, can_get_top) {
	Stack<int> s;
	s.push(1);
	s.push(3);
	s.push(6);
	ASSERT_NO_THROW(s.get_top());
	EXPECT_EQ(6, s.get_top());
}