#include"gtest.h"
#include"database.h"

TEST(Database, can_create_database) {
	ASSERT_NO_THROW((Database<string, int>{}));
}

TEST(Database, can_add_element) {
	Database<string, int> db;
	ASSERT_NO_THROW(db.add({{ "test", 123 }, { "test2" , 222}}));
	EXPECT_EQ(2, db.size());
}

TEST(Database, cant_add_dublicate) {
	Database<string, int> db;
	ASSERT_NO_THROW(db.add({ { "test", 123 }, { "test2" , 222} }));
	ASSERT_ANY_THROW(db.add({ "test" , 567}));
}

TEST(Database, can_remove_element) {
	Database<string, int> db;
	db.add({ { "test", 123 }, { "test2" , 222}, {"test3", 333} });
	ASSERT_NO_THROW(db.remove({ "test", "test3" }));
	EXPECT_EQ(1, db.size());
}

TEST(Database, test_operators_square_brackets) {
	Database<string, int> db;
	db.add({ { "test", 123 }, { "test2" , 222}, {"test3", 333} });
	ASSERT_NO_THROW(db["test"]);
	ASSERT_NO_THROW(db["test"] = 0);
	EXPECT_EQ(0, db["test"]);
	ASSERT_ANY_THROW(db["tt"]);
}

TEST(Database, can_emplace_and_replace_element) {
	Database<string, int> db;
	ASSERT_NO_THROW(db.emplace("test3", 333));
	ASSERT_NO_THROW(db.replace("test3", 222));
	EXPECT_EQ(1, db.size());
	EXPECT_EQ(222, db["test3"]);
}

TEST(Database, can_set_active_table) {
	Database<string, int> db;
	ASSERT_NO_THROW(db.set_active(1));
}