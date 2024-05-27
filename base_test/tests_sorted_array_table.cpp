#include"gtest.h"
#include"sorted_array_table.h"

TEST(SortedArrayTable, can_create_table) {
	ASSERT_NO_THROW((SortedArrayTable<string, short> {}));
	ASSERT_NO_THROW((SortedArrayTable<string, short>{10}));
	ASSERT_NO_THROW((SortedArrayTable<string, short>{SortedArrayTable<string, short>{5}}));
	ASSERT_NO_THROW((SortedArrayTable<string, short>{std::move(SortedArrayTable<string, short>{5})}));
}

TEST(SortedArrayTable, can_copy_table) {
	SortedArrayTable<string, int> a{ { "1", 1 } ,{ "2", 2 }, { "3", 3 } }, b(a);
	EXPECT_EQ(a, b);
}


TEST(SortedArrayTable, can_swap_table) {
	SortedArrayTable<string, int> a{ { "1", 1 } ,{ "2", 2 }, { "3", 3 } }, b(std::move(a));
	EXPECT_EQ(0, a.get_size());
	EXPECT_EQ(3, b.get_size());
	EXPECT_EQ((SortedArrayTable<string, int> { { "1", 1 }, { "2", 2 }, { "3", 3 } }), b);
}

TEST(SortedArrayTable, can_change_value_of_element) {
	SortedArrayTable<string, int> t;
	t.add({ "1", 1 });
	t.emplace("2", 2);
	ASSERT_NO_THROW(t.replace("2", 0));
	ASSERT_ANY_THROW(t.replace("3", 0));
	EXPECT_EQ(0, t["2"]);
}

TEST(SortedArrayTable, cant_create_table_with_invalid_capacity) {
	ASSERT_ANY_THROW((SortedArrayTable<string, int>{ 0 }));
}

TEST(SortedArrayTable, test_operators_square_brackets) {
	SortedArrayTable<string, int> t{ { "1", 1 } ,{ "2", 2 }, { "3", 3 } };
	ASSERT_NO_THROW(t["1"]);
	EXPECT_EQ(2, t["2"]);
}

TEST(SortedArrayTable, test_iterators) {
	SortedArrayTable<string, int> t{ { "1", 1 } ,{ "2", 2 }, { "3", 3 } };
	int i = 1;
	auto it = t.begin();
	for (auto& a : t) {
		EXPECT_EQ(i, t[a]);
		i++;
	}
	ASSERT_NO_THROW(++it);
	ASSERT_NO_THROW(it++);
	ASSERT_NO_THROW(--it);
	ASSERT_NO_THROW(it--);
	ASSERT_NO_THROW(*it);

}

TEST(SortedArrayTable, can_add_element) {
	SortedArrayTable<string, int> t;
	ASSERT_NO_THROW(t.add({ { "one", 1 }, {"four", 4} }));
	EXPECT_EQ(2, t.get_size());
	EXPECT_EQ(1, t["one"]);
}

TEST(SortedArrayTable, cant_add_element_with_duplicate_key) {
	SortedArrayTable<string, int> t;
	ASSERT_NO_THROW(t.add({ { "one", 1 }, {"four", 4} }));
	EXPECT_ANY_THROW(t.add({"one", 23}));
}

TEST(SortedArrayTable, can_delete_element) {
	SortedArrayTable<string, int> t{ { "1", 1 } ,{ "2", 2 }, { "3", 3 } };
	ASSERT_NO_THROW(t.remove({ "2","1" }));
	EXPECT_EQ(1, t.get_size());
	EXPECT_EQ(3, t["3"]);
}

TEST(SortedArrayTable, can_find_element) {
	SortedArrayTable<string, int> t{ { "1", 1 } ,{ "2", 2 }, { "3", 3 } };
	ASSERT_NO_THROW(t.find("2"));
	EXPECT_EQ(1, t.find("2"));
}

