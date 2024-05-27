#include"gtest.h"
#include"tree.h"

TEST(Tree, can_create_tree) {
	ASSERT_NO_THROW((Tree<int, int>{}));
	ASSERT_NO_THROW((Tree<int, int>(Tree<int, int> {})));
	ASSERT_NO_THROW((Tree<int, int>(move(Tree<int, int>{}))));
}

TEST(Tree, can_add_element) {
	Tree<int, int> t;
	ASSERT_NO_THROW(t.insert({ { 0,0 }, {1,1} }));
	ASSERT_NO_THROW(t.emplace(2,2));
	EXPECT_EQ(3, t.get_size());
}

TEST(Tree, cant_add_dublicate) {
	Tree<int, int> t;
	ASSERT_NO_THROW(t.insert({ { 0,0 }, {1,1} }));
	ASSERT_ANY_THROW(t.insert({ 1, 2 }));
}

TEST(Tree, can_remove_element) {
	Tree<int, int> t;
	t.insert({ { 0,0 }, {1,1} , {2, 2} });
	ASSERT_NO_THROW(t.remove({ 0 , 2 , 9}));
	EXPECT_EQ(1, t.get_size());
}

TEST(Tree, can_copy_tree) {
	Tree<int, int> t;
	t.insert({ { 0,0 }, {1,1} , {2, 2} });
	Tree<int, int> c(t);
	EXPECT_EQ(3, c.get_size());
}

TEST(Tree, can_move_tree) {
	Tree<int, int> t;
	t.insert({ { 0,0 }, {1,1} , {2, 2} });
	Tree<int, int> c(move(t));
	EXPECT_EQ(3, c.get_size());
	EXPECT_EQ(0, t.get_size());
}

TEST(Tree, can_get_and_set_element_value) {
	Tree<int, int> t;
	t.insert({ { 0,0 }, {1,1} , {2, 2} });
	ASSERT_NO_THROW(t[1]);
	ASSERT_NO_THROW(t[1] = 4);
	EXPECT_EQ(4, t[1]);
}

TEST(Tree, test_compare_operators) {
	Tree<int, int> t;
	t.insert({ { 0,0 }, {1,1} , {2, 2} });
	Tree<int, int> c(t);
	EXPECT_TRUE(c == t);
	EXPECT_FALSE( c != t);
}

TEST(Tree, can_chech_including_of_element) {
	Tree<int, int> t;
	t.insert({ { 0,0 }, {1,1} , {2, 2} });
	EXPECT_TRUE(t.include(0));
	EXPECT_FALSE(t.include(4));
}

TEST(Tree, test_iterator) {
	Tree<int, int> t, c;
	t.insert({ { 0,0 }, {1,1} , {2, 2} });
	c.insert({ { 0,1 }, {1,2} , {2, 3} });
	ASSERT_NO_THROW(auto it = t.begin());
	auto it = t.begin();
	ASSERT_NO_THROW(++it);
	EXPECT_EQ(1,*it);
	ASSERT_NO_THROW(--it);
	ASSERT_NO_THROW(it++);
	ASSERT_NO_THROW(it--);
	ASSERT_NO_THROW(*it);
	EXPECT_EQ(0, *it);
	EXPECT_EQ(1, t[*(++it)]);
	for (auto& a : t) t[a]++;
	EXPECT_EQ(c, t);
}

TEST(Tree, test_assign_operators) {
	Tree<int, int> t, c, v;
	t.insert({ { 0,0 }, {1,1} , {2, 2} });
	ASSERT_NO_THROW(c = t);
	EXPECT_EQ(c, t);
	ASSERT_NO_THROW(v = move(t));
	EXPECT_EQ(c, v);
	EXPECT_NE(v, t);
}

TEST(Tree, can_clear_tree) {
	Tree<int, int> t;
	t.insert({ { 0,0 }, {1,1} , {2, 2} });
	ASSERT_NO_THROW(t.clear());
	EXPECT_EQ(0, t.get_size());
	t.emplace(123, 123);
	EXPECT_EQ(1, t.get_size());
}


