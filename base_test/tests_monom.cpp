#include "gtest.h"
#include "monom.h"

TEST(Monom, can_create_monom) {
	ASSERT_NO_THROW(Monom m (1,1,1,1));
	ASSERT_NO_THROW(Monom m(Monom{}));
	ASSERT_NO_THROW(Monom m("2xy4"));
}

TEST(Monom, can_copy_monom) {
	Monom c(1,2,3,4);
	ASSERT_NO_THROW(Monom m(c));
	EXPECT_EQ(c, Monom{ c });
}

TEST(Monom, test_divide_operators) {
	Monom c(1, 2, 3, 4);
	Monom m(2, 1, 2, 4);
	Monom res(0.5, 1, 1, 0);
	ASSERT_NO_THROW(c / m);
	c /= m; 
	EXPECT_EQ(res, c);
}

TEST(Monom, test_multyply_operators) {
	Monom c(1, 2, 3, 4);
	Monom m(2, 1, 2, 4);
	Monom res(2, 3, 5, 8);
	ASSERT_NO_THROW(c * m);
	c *= m;
	EXPECT_EQ(res, c);
}

TEST(Monom, test_plus_operators) {
	Monom c(1, 2, 3, 4);
	Monom m(2, 1, 2, 4);
	Monom res(4, 2, 3, 4);
	ASSERT_ANY_THROW(c + m);
	c += Monom{3,2,3,4};
	EXPECT_EQ(res, c);
}

TEST(Monom, test_subtract_operators) {
	Monom c(3, 2, 3, 4);
	Monom m(2, 1, 2, 4);
	Monom res(2, 2, 3, 4);
	ASSERT_ANY_THROW(c - m);
	c -= Monom{ 1,2,3,4 };
	EXPECT_EQ(res, c);
}

TEST(Monom, can_parse_monom) {
	
	ASSERT_NO_THROW(Monom{ "-x5z" });
	ASSERT_ANY_THROW(Monom{ "d-43c" });
	Monom c("-x5z");
	Monom d(-1, 5, 0, 1);
	EXPECT_EQ(d, c);
}

TEST(Monom, test_calculate_monom) {
	Monom c(3, 2, 3, 4);
	ASSERT_NO_THROW(c.calculate(2,2,2));
	EXPECT_EQ(1536.0, c.calculate(2, 2, 2));
}

TEST(Monom, test_equal) {
	Monom c(3, 2, 3, 4);
	Monom d(4, 2, 3, 4);
	ASSERT_NO_THROW(c.equal(d));
	EXPECT_EQ(true, c.equal(d));
}

TEST(Monom, test_compare_operators) {
	Monom p("3x2y2z3");
	Monom p1("3x3y2z3");
	EXPECT_EQ(true, p1 != p);
	EXPECT_EQ(true, p1 > p);
	EXPECT_EQ(false, p1 < p);
	EXPECT_EQ(true, p1 >= p);
	EXPECT_EQ(false, p1 <= p);
	EXPECT_EQ(false, p1 == p);
}