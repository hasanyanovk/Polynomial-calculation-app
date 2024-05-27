#include "gtest.h"
#include "hash_list_table.h"

TEST(HashListTable, can_get_hash_code) {
	HashListTable<string, int> t;
	ASSERT_NO_THROW(t.hash("test string"));
	EXPECT_EQ(29,t.hash("test string"));
	EXPECT_NE(29, t.hash("teststring"));
	EXPECT_EQ(29, t.hash("test string"));
}

TEST(HashListTable, can_create_table) {
	ASSERT_NO_THROW((HashListTable<string, int>{}));
	ASSERT_NO_THROW((HashListTable<string, int>{10}));
	ASSERT_NO_THROW((HashListTable<string, int>{HashListTable<string, int>{20}}));
	ASSERT_NO_THROW((HashListTable<string, int>{std::move(HashListTable<string, int>{20})}));
}

TEST(HashListTable, can_find_elements) {
	HashListTable<string, int> t;
	ASSERT_NO_THROW(t.add({ {"test string", 123 }, {"ert", 234} }));
	ASSERT_NO_THROW(t.find("test string"));
	EXPECT_EQ(0, t.find("ert"));
}

TEST(HashListTable, can_add_elements) {
	HashListTable<string, int> t;
	ASSERT_NO_THROW(t.add({ {"test string", 123 }, {"ert", 234} }));
	EXPECT_NE(-1, t.find("test string"));
	EXPECT_EQ(27, t.hash("ert"));
}

TEST(HashListTable, cant_add_dublicate) {
	HashListTable<string, int> t;
	ASSERT_NO_THROW(t.add({ {"test string", 123 }, {"ert", 234} }));
	ASSERT_ANY_THROW(t.add({"ert", 4}));
}

TEST(HashListTable, can_remove_element) {
	HashListTable<string, int> t;
	t.add({ {"test string", 123 }, {"ert", 234}, {"error", 404}});
	ASSERT_NO_THROW(t.remove({"ert", "test string", ""}));
	EXPECT_EQ(1, t.get_size());
}

TEST(HashListTable, can_rehash_table) {
	HashListTable<string, int> t(4);
	t.add({ {"test string", 123 }, {"ert", 234}, {"error", 404} });
	EXPECT_EQ(8, t.get_capacity());
	EXPECT_EQ(3, t.get_size());
	auto it = t.begin();
}

TEST(HashListTable, test_quad_brackets) {
	HashListTable<string, int> t;
	t.add({ {"test string", 123 }, {"ert", 234} });
	ASSERT_NO_THROW(t["ert"]);
	EXPECT_EQ(234, t["ert"]);
	ASSERT_ANY_THROW(t["pupupu"]);
	ASSERT_NO_THROW(t["ert"] = 345);
	EXPECT_EQ(345, t["ert"]);
}

TEST(HashListTable, test_compare_operators) {
	HashListTable<string, int> t1, t2;
	t1.add({ {"test string", 123 }, {"ert", 234} });
	t2.add({ {"test string", 123 }, {"ert", 234} });
	EXPECT_TRUE(t1 == t2);
	EXPECT_FALSE(t1 != t2);
	t2.add({ "pupupu", 111 });
	EXPECT_NE(t1, t2);
}

TEST(HashListTable, can_create_default_table) {
	HashListTable<string, int> t, v(20);
	EXPECT_EQ(50, t.get_capacity());
	EXPECT_EQ(20, v.get_capacity());
	EXPECT_EQ(0, v.get_size());
}

TEST(HashListTable, can_copy_table) {
	HashListTable<string, int> t;
	t.add({ {"test string", 123 }, {"ert", 234} });
	HashListTable<string, int> c(t);
	EXPECT_EQ(t, c);
	EXPECT_EQ(234, c["ert"]);
}

TEST(HashListTable, can_move_table) {
	HashListTable<string, int> t;
	t.add({ {"test string", 123 }, {"ert", 234} });
	HashListTable<string, int> c(move(t));
	EXPECT_NE(t, c);
	EXPECT_EQ(2, c.get_size());
	EXPECT_EQ(0, t.get_size());
}

TEST(HashListTable, test_swap) {
	HashListTable<string, int> t, v;
	t.add({ {"test string", 123 }, {"ert", 234} });
	ASSERT_NO_THROW(swap(t, v));
	EXPECT_NE(t, v);
	EXPECT_EQ(0, t.get_size());
	EXPECT_EQ(2, v.get_size());
}