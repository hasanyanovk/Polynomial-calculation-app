#include"gtest.h"
#include"tree_table.h"


TEST(TreeTable, can_create_table) {
	ASSERT_NO_THROW((TreeTable<string, int>{}));
	ASSERT_NO_THROW((TreeTable<string, int>{ {"test string", 123 }, { "ert", 234 }, { "error", 404 }}));
	ASSERT_NO_THROW((TreeTable<string, int>{TreeTable<string, int>{}}));
	ASSERT_NO_THROW((TreeTable<string, int>{move(TreeTable<string, int> {})}));
}

TEST(TreeTable, can_add_element) {
	TreeTable<string, int> t;
	ASSERT_NO_THROW(t.add({ {"test string", 123 }, {"ert", 234}, {"error", 404} }));
	ASSERT_NO_THROW(t.emplace("one", 1));
	EXPECT_EQ(4, t.get_size());
}

TEST(TreeTable, cant_add_dublicate) {
	TreeTable<string, int> t;
	t.add({ {"test string", 123 }, {"ert", 234}, {"error", 404} });
	ASSERT_ANY_THROW(t.emplace("error", 1));
	EXPECT_EQ(3, t.get_size());
}

TEST(TreeTable, can_remove_element) {
	TreeTable<string, int> t;
	t.add({ {"test string", 123 }, {"ert", 234}, {"error", 404} });
	ASSERT_NO_THROW(t.remove({"test string", "ert", "error"}));
	EXPECT_EQ(0, t.get_size());
}

TEST(TreeTable, can_copy_table) {
	TreeTable<string, int> t({ {"test string", 123 }, {"ert", 234}, {"error", 404} }), c(t);
	EXPECT_EQ(3, c.get_size());
}

TEST(TreeTable, can_move_table) {
	TreeTable<string, int> t({ {"test string", 123 }, {"ert", 234}, {"error", 404} }), c(move(t));
	EXPECT_EQ(3, c.get_size());
	EXPECT_EQ(0, t.get_size());
}

TEST(TreeTable, can_get_and_set_elements) {
	TreeTable<string, int> t({ {"test string", 123 }, {"ert", 234}, {"error", 404} });
	ASSERT_NO_THROW(t["ert"]);
	ASSERT_NO_THROW(t["ert"] = 4);
	EXPECT_EQ(4, t["ert"]);
}

TEST(TreeTable, test_compare_operators) {
	TreeTable<string, int> t({ {"test string", 123 }, {"ert", 234}, {"error", 404} }), c(t);
	EXPECT_TRUE(c == t);
	EXPECT_FALSE(c != t);
}

TEST(TreeTable, test_iterator) {
	TreeTable<string, int> t({ {"test string", 123 }, {"ert", 234}, {"error", 404} }), 
		c({ {"test string", 124 }, {"ert", 235}, {"error", 405} });
	ASSERT_NO_THROW(auto it = t.begin());
	auto it = t.begin();
	ASSERT_NO_THROW(++it);
	ASSERT_NO_THROW(--it);
	ASSERT_NO_THROW(it++);
	ASSERT_NO_THROW(it--);
	ASSERT_NO_THROW(*it);
	EXPECT_EQ("error", *it);
	EXPECT_EQ(234, t[*(++it)]);
	for (auto& a : t) t[a]++;
	EXPECT_EQ(c, t);
}

TEST(TreeTable, can_chech_including_of_element) {
	Tree<int, int> t;
	t.insert({ { 0,0 }, {1,1} , {2, 2} });
	EXPECT_TRUE(t.include(0));
	EXPECT_FALSE(t.include(4));
}

TEST(TreeTable, test_assign_operators) {
	TreeTable<string, int> t({ {"test string", 123 }, {"ert", 234}, {"error", 404} }), c, v;
	ASSERT_NO_THROW(c = t);
	c = t;
	EXPECT_EQ(c, t);
	ASSERT_NO_THROW(v = move(t));
	EXPECT_EQ(c, v);
	EXPECT_NE(v, t);
}
