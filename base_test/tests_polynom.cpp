#include "gtest.h"
#include "polynom.h"


TEST(Polynom, can_create_polynom) {
	ASSERT_NO_THROW(Polynom());
}

TEST(Polynom, can_create_polynom_from_string) {
	ASSERT_NO_THROW(Polynom{ "-2x3y + yz2 - 4xyz" });
}

TEST(Polynom, can_copy_polynom) {
	Polynom p("2xy+4y4z2");
	ASSERT_NO_THROW(Polynom{p});
	EXPECT_EQ(p, Polynom{ p });
}

TEST(Polynom, test_add_monom) {
	Polynom p("3x2y2z3 + xy");
	Monom p1("xy2z2");
	Polynom res("3x2y2z3+xy2z2 + xy");
	ASSERT_NO_THROW(p.add(p1));
	EXPECT_EQ(res, p);
}

TEST(Polynom, test_multiply_operators) {
	Polynom p("3x2y2z3 + xy");
	Monom p1("xy2z2");
	Polynom res("3x3y4z5+x2y3z2");
	ASSERT_NO_THROW(p * p1);
	p  = p * p1;
	EXPECT_EQ(res, p);
}

TEST(Polynom, test_subtract_operators) {
	Polynom p("3x2y2z3 + xy");
	Polynom p1("xy2z2");
	Polynom res("3x2y2z3 + xy - xy2z2");
	ASSERT_NO_THROW(p - p1);
	p -= p1;
	EXPECT_EQ(res, p);
}

TEST(Polynom, test_plus_operators) {
	Polynom p("3x2y2z3 + xy");
	Polynom p1("xy2z2");
	Polynom res("3x2y2z3 + xy + xy2z2");
	ASSERT_NO_THROW(p + p1);
	p += p1;
	EXPECT_EQ(res, p);
}

TEST(Polynom, test_divide_operators) {
	Polynom p("x3-12x2-42");
	Polynom m("x-3");
	Polynom res("x2-9x-27");
	ASSERT_NO_THROW(p / m);
	p /= m;
	EXPECT_EQ(res, p);
}

TEST(Polynom, test_calculate) {
	Polynom p("3x2y2z3 + xy");
	ASSERT_NO_THROW(p.calculate(2,1,2));
	EXPECT_EQ(98, p.calculate(2,1,2));
}

TEST(Polynom, test_compare_operators) {
	Polynom p("3x2y2z3 + xy");
	Polynom p1("3x3y2z3 + xy");
	EXPECT_TRUE(p1 != p);
	EXPECT_TRUE(p1 > p);
	EXPECT_FALSE(p1 < p);
	EXPECT_TRUE(p1 >= p);
	EXPECT_FALSE(p1 <= p);
	EXPECT_FALSE(p1 == p);
}

TEST(Polynom, test_differential) {
	Polynom p("3x2z + xy");
	ASSERT_NO_THROW(p.differential('x'));
	EXPECT_EQ(Polynom{"6xz+y"}, p.differential('x'));
}

TEST(Polynom, test_primitive) {
	Polynom p("2xz+3x2 -5");
	ASSERT_NO_THROW(p.primitive('x'));
	EXPECT_EQ(Polynom{ "x2z+x3-5x" }, p.primitive('x'));
}

TEST(Polynom, test_point) {
	Polynom p("3x2y2z3 + xy");
	Point point{ 2,1,2 };
	ASSERT_NO_THROW(p.calculate(point));
	EXPECT_EQ(98, p.calculate(point));
}