#include "gtest.h"
#include "hash_array_table.h"


TEST(HashArrayTable, can_create_table) {
	ASSERT_NO_THROW((HashArrayTable<string, int>{}));
	ASSERT_NO_THROW((HashArrayTable<string, int>{50}));
	ASSERT_NO_THROW((HashArrayTable<string, int>{HashArrayTable<string, int>{}}));
	ASSERT_NO_THROW((HashArrayTable<string, int>{std::move(HashArrayTable<string, int>{})}));
}

TEST(HashArrayTable, can_find_element) {
	HashArrayTable<string, int> t(10);
	t.add({ { "test string", 123 }, {"", 5678}, {"one", 111} , {"two", 222}, {"three", 333} });
	ASSERT_NO_THROW(t.find("two"));
	EXPECT_EQ(7, t.find("two"));
	EXPECT_EQ(-1, t.find("pupupu"));
}

TEST(HashArrayTable, can_add_element) {
	HashArrayTable<string, int> t;
	ASSERT_NO_THROW(t.add({ { "test string", 123 }, {"", 5678}, {"one", 111} , {"two", 222}, {"three", 333} }));
	EXPECT_EQ(5, t.get_size());
}

TEST(HashArrayTable, cant_add_dublicate) {
	HashArrayTable<string, int> t;
	t.add({ { "test string", 123 }, {"", 5678}, {"one", 111} , {"two", 222}, {"three", 333} });
	ASSERT_ANY_THROW(t.add({ "", 1 }));
}

TEST(HashArrayTable, can_remove_element) {
	HashArrayTable<string, int> t;
	t.add({ { "test string", 123 }, {"", 5678}, {"one", 111} , {"two", 222}, {"three", 333} });
	ASSERT_NO_THROW(t.remove({ "test string", "one" }));
	EXPECT_EQ(3, t.get_size());
}

TEST(HashArrayTable, can_swap_tables) {
	HashArrayTable<string, int> t, c;
	t.add({ { "test string", 123 }, {"", 5678}, {"one", 111} , {"two", 222}, {"three", 333} });
	ASSERT_NO_THROW(swap(t,c));
	EXPECT_EQ(0, t.get_size());
	EXPECT_EQ(5, c.get_size());
}

TEST(HashArrayTable, can_rehash) {
	HashArrayTable<string, int> t(5);
	t.add({ { "test string", 123 }, {"", 5678}, {"one", 111} , {"two", 222}, {"three", 333} });
	EXPECT_EQ(10, t.get_capacity());
}

TEST(HashArrayTable, can_operators_quad_brackets) {
	HashArrayTable<string, int> t(5);
	t.add({ { "test string", 123 }, {"", 5678}, {"one", 111} , {"two", 222}, {"three", 333} });
	ASSERT_NO_THROW(t["one"]);
	ASSERT_ANY_THROW(t["pupupu"]);
	EXPECT_EQ(t["one"], 111);
	ASSERT_NO_THROW(t["one"] = 101);
	EXPECT_EQ(t["one"], 101);
}

TEST(HashArrayTable, can_copy_table) {
	HashArrayTable<string, int> t(50);
	t.add({ { "test string", 123 }, {"", 5678}});
	HashArrayTable<string, int> c(t);
	EXPECT_EQ(50, c.get_capacity());
	EXPECT_EQ(2, c.get_size());
	EXPECT_EQ(29, c.hash("test string"));
}

TEST(HashArrayTable, can_move_table) {
	HashArrayTable<string, int> t(50);
	t.add({ { "test string", 123 }, {"", 5678} });
	HashArrayTable<string, int> c = std::move(t);
	EXPECT_EQ(50, c.get_capacity());
	EXPECT_EQ(2, c.get_size());
	EXPECT_EQ(0, t.get_size());
	EXPECT_NE(c, t);
}

TEST(HashArrayTable, test_assign_and_compare_operators) {
	HashArrayTable<string, int> t(50), c;
	t.add({ { "test string", 123 }, {"", 5678} });
	c = t;
	EXPECT_TRUE(t == c);
	EXPECT_FALSE(t != c);
}

TEST(HashArrayTable, can_get_hash_code) {
	HashArrayTable<string, int> t;
	ASSERT_NO_THROW(t.hash("test string"));
	EXPECT_EQ(379, t.hash("test string"));
}

