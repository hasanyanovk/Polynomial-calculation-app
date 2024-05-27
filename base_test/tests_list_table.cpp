#include"gtest.h"
#include"list_table.h"

TEST(ListTable, can_create_table) {
	ASSERT_NO_THROW((ListTable<string, short>{}));
	ASSERT_NO_THROW((ListTable<string, short>{std::move(ListTable<string, short>{})}));
}

TEST(ListTable, can_swap_table) {
	ListTable<string, int> a{ { "1", 1 } ,{ "2", 2 }, { "3", 3 } }, b(std::move(a));
	EXPECT_EQ(0, a.get_size());
	EXPECT_EQ(3, b.get_size());
	EXPECT_EQ((ListTable<string, int> { { "1", 1 } ,{ "2", 2 }, { "3", 3 } }), b);
}

TEST(ListTable, can_copy_table) {
	ListTable<string, int> a{ { "1", 1 } ,{ "2", 2 }, { "3", 3 } }, b(a);
	EXPECT_EQ(a, b);
}

TEST(ListTable, can_change_value_of_element) {
	ListTable<string, int> t;
	t.add({ "1", 1 });
	t.emplace("2", 2);
	ASSERT_NO_THROW(t.replace("2", 0));
	ASSERT_ANY_THROW(t.replace("3", 0));
	EXPECT_EQ(0, t["2"]);
}

TEST(ListTable, test_operators_square_brackets) {
	ListTable<string, int> t{ { "1", 1 } ,{ "2", 2 }, { "3", 3 } };
	std::pair<string, int> p;
	ASSERT_NO_THROW(t["1"]);
	EXPECT_EQ(2, t["2"]);
}

TEST(ListTable, test_iterators) {
	ListTable<string, int> t{ { "1", 1 } ,{ "2", 2 }, { "3", 3 } };
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

TEST(ListTable, can_add_element) {
	ListTable<string, int> t;
	ASSERT_NO_THROW(t.add({ "one", 1 }));
	EXPECT_EQ(1, t.get_size());
	EXPECT_EQ(1, t["one"]);
}

TEST(ListTable, cant_add_element_with_duplicate_key) {
	ListTable<string, int> t;
	ASSERT_NO_THROW(t.add({ { "one", 1 }, {"four", 4} }));
	EXPECT_ANY_THROW(t.add({ "one", 23 }));
}

TEST(ListTable, can_delete_element) {
	ListTable<string, int> t{ { "1", 1 } ,{ "2", 2 }, { "3", 3 } };
	ASSERT_NO_THROW(t.remove({ "2", "3" }));
	EXPECT_EQ(1, t.get_size());
	EXPECT_EQ(1, t["1"]);
}

TEST(ListTable, can_find_element) {
	ListTable<string, int> t{ { "1", 1 } ,{ "2", 2 }, { "3", 3 } };
	ASSERT_NO_THROW(t.find("2"));
	EXPECT_EQ(1, t.find("2"));
}