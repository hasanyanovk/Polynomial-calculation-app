#include "gtest.h"
#include "sorted_list.h"

TEST(SortedList, can_insert_elem_in_order) {
	SortedList<int> list;
	list.push_back(1);
	list.push_back(2);
	list.push_back(4);
	ASSERT_NO_THROW(list.insert_in_order(3, [](int x, int y) {return x < y; }));
	EXPECT_EQ(3, list[2]);
}