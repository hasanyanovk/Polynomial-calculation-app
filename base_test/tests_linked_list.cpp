#include "gtest.h"
#include "linked_list.h"

TEST(LinkedList, can_create_list)
{
	ASSERT_NO_THROW(LinkedList<int> l);
}

TEST(LinkedList, can_copy_list)
{
	LinkedList<int> list;
	list.push_back(1);
	list.push_back(2);
	list.push_back(3);
	list.push_back(4);
	list.push_back(5);

	LinkedList<int> copied = list;

	ASSERT_EQ(list.get_size(), copied.get_size());
	for (int i = 0; i < list.get_size(); i++)
	{
		EXPECT_EQ(list[i], copied[i]);
	}
}

TEST(LinkedList, method_empty_return_true_value)
{
	LinkedList<int> l;
	EXPECT_EQ(1, l.empty());
}

TEST(LinkedList, can_pop_element_to_list)
{
	LinkedList<int> l;
	l.push_back(2);
	l.push_back(6);
	ASSERT_NO_THROW(l.pop_back());
	ASSERT_NO_THROW(l.pop_front());
}

TEST(LinkedList, cant_pop_element_from_empty_list)
{
	LinkedList<int> l;
	ASSERT_ANY_THROW(l.pop_back());
}

TEST(LinkedList, cant_get_element_from_empty_list)
{
	LinkedList<int> l;
	ASSERT_ANY_THROW(l.get_back());
}

TEST(LinkedList, can_push_element_to_list)
{
	LinkedList<int> l;
	ASSERT_NO_THROW(l.push_back(3));
	ASSERT_NO_THROW(l.push_front(4));
}

TEST(LinkedList, can_get_front)
{
	LinkedList<int> l;
	l.push_front(2);
	ASSERT_NO_THROW(l.get_front());
}

TEST(LinkedList, can_get_back)
{
	LinkedList<int> l;
	l.push_back(2);
	ASSERT_NO_THROW(l.get_back());
}

TEST(LinkedList, can_erase_element)
{
	LinkedList<int> l;
	l.push_back(2);
	ASSERT_NO_THROW(l.erase(0));
}

TEST(LinkedList, can_insert_elemet)
{
	LinkedList<int> l;
	l.push_back(1);
	l.push_back(3);
	ASSERT_NO_THROW(l.insert(1, 2));
}

TEST(LinkedList, cant_insert_element_with_negative_index)
{
	LinkedList<int> l;
	ASSERT_ANY_THROW(l.insert(-1, 10));
}

TEST(LinkedList, get_back_and_front_return_true_value)
{
	LinkedList<int> l;
	l.push_front(2);
	l.push_back(5);
	EXPECT_EQ(5, l.get_back());
	EXPECT_EQ(2, l.get_front());
}

TEST(LinkedList, operator_get_from_index)
{
	LinkedList<int> l;
	l.push_back(2);
	ASSERT_NO_THROW(l[0]);
}

TEST(LinkedList, operator_get_from_index_return_true_value)
{
	LinkedList<int> l;
	l.push_back(2);
	EXPECT_EQ(2, l[0]);
}

TEST(LinkedList, check_size)
{
	LinkedList<int> l;
	l.push_back(4);
	l.push_back(4);
	l.push_back(4);
	l.pop_front();
	l.push_back(4);
	EXPECT_EQ(3, l.get_size());
}
