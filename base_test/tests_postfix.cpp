#include "postfix.h"
#include "polynom.h"
#include "gtest.h"

using namespace polish_notation;

//----------------------Postfix-polynom----------------------------

TEST(Postfix_polynom, can_create_postfix) {
	ASSERT_NO_THROW(Postfix<Polynom> {});
	ASSERT_NO_THROW(Postfix<Polynom> {"34*(-jf+5xy)"});
	ASSERT_NO_THROW(Postfix<Polynom> {Postfix<Polynom>{}});
	ASSERT_NO_THROW(Postfix<Polynom> {std::move(Postfix<Polynom>{})});
}

TEST(Postfix_polynom, cant_copy_postfix) {
	Postfix<Polynom> p("34*(-jf+5xy)"), c(p);
	EXPECT_EQ(c, p);
}

TEST(Postfix_polynom, cant_move_postfix) {
	Postfix<Polynom> p("34*(jf+5xy)"), c(move(p));
	EXPECT_NE(c, p);
	EXPECT_EQ("34*(jf+5xy)", c.get_infix());
}

TEST(Postfix_polynom, cant_convert_wrong_expression){
	ASSERT_ANY_THROW(Postfix<Polynom> p("((1+2)-3)*5)"));
}

TEST(Postfix_polynom, can_get_infix){
	Postfix<Polynom> p;
	EXPECT_EQ("", p.get_infix());
}

TEST(Postfix_polynom, can_get_postfix){
	Postfix<Polynom> p("1+2-3*5");
	EXPECT_EQ("1 2+ 3 5*-", p.get_postfix());
}

TEST(Postfix_polynom, can_calculate_value) {
	Postfix<Polynom> p("Polynom * (Pol2 + 4y5)");
	map<string, Polynom> m = { {"Polynom", Polynom{"34xyz"}}, {"Pol2", Polynom{"78z2"}}, {"4y5", Polynom{"4y5"}} };
	ASSERT_NO_THROW(p.calculate());
	EXPECT_EQ(p(m), Polynom{ "136xy6z+2652xyz3" });
}

//----------------------Postfix-double----------------------------

TEST(Postfix_double, can_create_postfix) {
	ASSERT_NO_THROW(Postfix<double> {});
	ASSERT_NO_THROW(Postfix<double> {"34*(-jf+5xy)"});
	ASSERT_NO_THROW(Postfix<double> {Postfix<double>{}});
	ASSERT_NO_THROW(Postfix<double> {std::move(Postfix<double>{})});
}

TEST(Postfix_double, cant_copy_postfix) {
	Postfix<double> p("34*(-jf+5xy)"), c(p);
	EXPECT_EQ(c, p);
}

TEST(Postfix_double, cant_move_postfix) {
	Postfix<double> p("34*(jf+5xy)"), c(move(p));
	EXPECT_NE(c, p);
	EXPECT_EQ("34*(jf+5xy)", c.get_infix());
}

TEST(Postfix_double, cant_convert_wrong_expression) {
	ASSERT_ANY_THROW(Postfix<double> p("((1+2)-3)*5)"));
}

TEST(Postfix_double, can_get_infix) {
	Postfix<double> p;
	EXPECT_EQ("", p.get_infix());
}

TEST(Postfix_double, can_get_postfix) {
	Postfix<double> p("1+2-3*5");
	EXPECT_EQ("1 2+ 3 5*-", p.get_postfix());
}

TEST(Postfix_double, can_calculate_value) {
	Postfix<double> p("1+2-3*5");
	ASSERT_NO_THROW(p.calculate());
	EXPECT_EQ(-12, p.calculate());
}
