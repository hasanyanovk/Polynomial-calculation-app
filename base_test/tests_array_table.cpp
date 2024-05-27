#include"gtest.h"
#include"array_table.h"

TEST(ArrayTable, can_create_table) {
	ASSERT_NO_THROW((ArrayTable<string, short> {}));
	ASSERT_NO_THROW((ArrayTable<string, short>{10}));
	ASSERT_NO_THROW((ArrayTable<string, short>{ArrayTable<string, short>{5}}));
	ASSERT_NO_THROW((ArrayTable<string, short>{std::move(ArrayTable<string, short>{5})}));
}

TEST(ArrayTable, can_copy_table) {
	ArrayTable<string, int> a{ { "1", 1 } ,{ "2", 2 }, { "3", 3 } }, b(a);
	EXPECT_EQ(a, b);
}

TEST(ArrayTable, can_clear_table) {
	ArrayTable<string, int> a{ { "1", 1 } ,{ "2", 2 }, { "3", 3 } };
	ASSERT_NO_THROW(a.clear());
	EXPECT_EQ(0, a.get_size());
}

TEST(ArrayTable, can_swap_table) {
	ArrayTable<string, int> a{ { "1", 1 } ,{ "2", 2 }, { "3", 3 } }, b(std::move(a));
	EXPECT_EQ(0, a.get_size());
	EXPECT_EQ(3, b.get_size());
	EXPECT_EQ((ArrayTable<string, int> { { "1", 1 }, { "2", 2 }, { "3", 3 } }), b);
}

TEST(ArrayTable, cant_create_table_with_invalid_capacity) {
	ASSERT_ANY_THROW((ArrayTable<string, int>{ 0 }));
}

TEST(ArrayTable, can_change_value_of_element) {
	ArrayTable<string, int> t;
	t.add({ "1", 1 });
	t.emplace("2", 2);
	ASSERT_NO_THROW(t.replace("2", 0));
	ASSERT_ANY_THROW(t.replace("3", 0));
	EXPECT_EQ(0, t["2"]);
}

TEST(ArrayTable, test_operators_square_brackets) {
	ArrayTable<string, int> t{ { "1", 1 } ,{ "2", 2 }, { "3", 3 } };
	ASSERT_NO_THROW(t["1"]);
	EXPECT_EQ(2, t["2"]);
}

TEST(ArrayTable, test_iterators) {
	ArrayTable<string, int> t{ { "1", 1 } ,{ "2", 2 }, { "3", 3 } };
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

TEST(ArrayTable, can_add_element) {
	ArrayTable<string, int> t;
	ASSERT_NO_THROW(t.add({ { "one", 1 }, {"four", 4}}));
	ASSERT_NO_THROW(t.emplace("two", 2));
	EXPECT_EQ(3, t.get_size());
	EXPECT_EQ(1, t["one"]);
}

TEST(ArrayTable, cant_add_element_with_duplicate_key) {
	ArrayTable<string, int> t;
	ASSERT_NO_THROW(t.add({ { "one", 1 }, {"four", 4} }));
	EXPECT_ANY_THROW(t.add({ "one", 23 }));
}

TEST(ArrayTable, can_delete_element) {
	ArrayTable<string, int> t{ { "1", 1 } ,{ "2", 2 }, { "3", 3 } };
	ASSERT_NO_THROW(t.remove({ "2","1" }));
	EXPECT_EQ(1, t.get_size());
	EXPECT_EQ(3, t["3"]);
}

TEST(ArrayTable, can_find_element) {
	ArrayTable<string, int> t{ { "1", 1 } ,{ "2", 2 }, { "3", 3 } };
	ASSERT_NO_THROW(t.find("2"));
	EXPECT_EQ(1, t.find("2"));
}

