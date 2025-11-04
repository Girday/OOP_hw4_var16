#include <gtest/gtest.h>
#include "array.h"
#include "triangle.h"
#include "square.h"
#include "octagon.h"


// Point

class PointTest : public ::testing::Test {};

TEST_F(PointTest, DefaultConstructor) {
    Point<double> p;
    EXPECT_DOUBLE_EQ(p.x(), 0.0);
    EXPECT_DOUBLE_EQ(p.y(), 0.0);
}

TEST_F(PointTest, ParameterizedConstructor) {
    Point<double> p(3.5, 4.2);
    EXPECT_DOUBLE_EQ(p.x(), 3.5);
    EXPECT_DOUBLE_EQ(p.y(), 4.2);
}

TEST_F(PointTest, EqualityOperator) {
    Point<double> p1(1.0, 2.0);
    Point<double> p2(1.0, 2.0);
    Point<double> p3(1.0, 3.0);
    EXPECT_TRUE(p1 == p2);
    EXPECT_FALSE(p1 == p3);
}


// Square

class SquareTest : public ::testing::Test {
protected:
    Square<double> sq_default{Point<double>(0, 0), Point<double>(1, 0)};
};

TEST_F(SquareTest, CenterCalculation) {
    Point<double> center = sq_default.center();
    EXPECT_NEAR(center.x(), 0.5, 1e-9);
    EXPECT_NEAR(center.y(), 0.5, 1e-9);
}

TEST_F(SquareTest, AreaCalculation) {
    Square<double> sq(Point<double>(0, 0), Point<double>(2, 0));
    EXPECT_NEAR(static_cast<double>(sq), 4.0, 1e-9);
}

TEST_F(SquareTest, EqualityOperator) {
    Square<double> sq2(Point<double>(0, 0), Point<double>(1, 0));
    const Figure<double>& f1 = sq_default;
    const Figure<double>& f2 = sq2;
    EXPECT_TRUE(f1 == f2);
}

TEST_F(SquareTest, InvalidInputHandled) {
    Square<double> sq(Point<double>(0, 0), Point<double>(0, 0));
    EXPECT_GT(static_cast<double>(sq), 0);
}


// Triangle

class TriangleTest : public ::testing::Test {
protected:
    Triangle<double> tri_default{Point<double>(0, 0), Point<double>(2, 0), 2.0};
};

TEST_F(TriangleTest, CenterCalculation) {
    Point<double> c = tri_default.center();
    EXPECT_NEAR(c.x(), 1.0, 1e-9);
    EXPECT_NEAR(c.y(), 2.0 / 3.0, 1e-9);
}

TEST_F(TriangleTest, AreaCalculation) {
    Triangle<double> tri(Point<double>(0, 0), Point<double>(4, 0), 3.0);
    EXPECT_NEAR(static_cast<double>(tri), 6.0, 1e-9);
}

TEST_F(TriangleTest, EqualityOperator) {
    Triangle<double> tri2(Point<double>(0, 0), Point<double>(2, 0), 2.0);
    const Figure<double>& f1 = tri_default;
    const Figure<double>& f2 = tri2;
    EXPECT_TRUE(f1 == f2);
}

TEST_F(TriangleTest, InvalidInputHandled) {
    Triangle<double> tri(Point<double>(0, 0), Point<double>(1, 0), 0);
    EXPECT_GT(static_cast<double>(tri), 0);
}


// Octagon

class OctagonTest : public ::testing::Test {
protected:
    Octagon<double> oct_default{Point<double>(0, 0), Point<double>(1, 0)};
};

TEST_F(OctagonTest, CenterCalculation) {
    Point<double> c = oct_default.center();
    EXPECT_NEAR(c.x(), 0.0, 1e-9);
    EXPECT_NEAR(c.y(), 0.0, 1e-9);
}

TEST_F(OctagonTest, AreaCalculation) {
    EXPECT_NEAR(static_cast<double>(oct_default), 2.8284271247461903, 1e-9);
}

TEST_F(OctagonTest, EqualityOperator) {
    Octagon<double> oct2(Point<double>(0, 0), Point<double>(1, 0));
    const Figure<double>& f1 = oct_default;
    const Figure<double>& f2 = oct2;
    EXPECT_TRUE(f1 == f2);
}


// Array

class ArrayTest : public ::testing::Test {
protected:
    void SetUp() override {
        sq = std::make_shared<Square<double>>(Point<double>(0, 0), Point<double>(1, 0));
        tri = std::make_shared<Triangle<double>>(Point<double>(0, 0), Point<double>(2, 0), 2.0);
        oct = std::make_shared<Octagon<double>>(Point<double>(0, 0), Point<double>(1, 0));
    }

    Array<Figure<double>> arr;
    std::shared_ptr<Square<double>> sq;
    std::shared_ptr<Triangle<double>> tri;
    std::shared_ptr<Octagon<double>> oct;
};

TEST_F(ArrayTest, AddFigures) {
    EXPECT_EQ(arr.getSize(), 0);
    arr.add(sq);
    arr.add(tri);
    EXPECT_EQ(arr.getSize(), 2);
}

TEST_F(ArrayTest, RemoveFigures) {
    arr.add(sq);
    arr.add(tri);
    arr.remove(0);
    EXPECT_EQ(arr.getSize(), 1);
    arr.remove(0);
    EXPECT_EQ(arr.getSize(), 0);
}

TEST_F(ArrayTest, RemoveInvalidIndex) {
    arr.add(sq);
    EXPECT_THROW(arr.remove(5), std::out_of_range);
}

TEST_F(ArrayTest, RemoveFromEmptyThrows) {
    EXPECT_THROW(arr.remove(0), std::out_of_range);
}

TEST_F(ArrayTest, EmptyArrayPrintThrows) {
    EXPECT_THROW(arr.printAll(), std::out_of_range);
    EXPECT_THROW(arr.printCenters(), std::out_of_range);
    EXPECT_THROW(arr.printTotalArea(), std::out_of_range);
}

TEST_F(ArrayTest, TotalAreaCalculationOutput) {
    arr.add(sq);
    arr.add(tri);
    testing::internal::CaptureStdout();
    arr.printTotalArea();
    std::string out = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(out.find("Total Area") != std::string::npos);
}

TEST_F(ArrayTest, MoveSemanticsWork) {
    arr.add(sq);
    arr.add(tri);
    Array<Figure<double>> moved = std::move(arr);
    EXPECT_EQ(moved.getSize(), 2);
    EXPECT_EQ(arr.getSize(), 0);
}

TEST_F(ArrayTest, MoveAssignmentWorks) {
    arr.add(sq);
    arr.add(tri);
    Array<Figure<double>> copy;
    copy = std::move(arr);
    EXPECT_EQ(copy.getSize(), 2);
    EXPECT_EQ(arr.getSize(), 0);
}


// Integration

class IntegrationTest : public ::testing::Test {};

TEST_F(IntegrationTest, FullWorkflow) {
    Array<Figure<double>> figures;
    auto sq = std::make_shared<Square<double>>(Point<double>(0, 0), Point<double>(1, 0));
    auto tri = std::make_shared<Triangle<double>>(Point<double>(0, 0), Point<double>(2, 0), 3.0);
    auto oct = std::make_shared<Octagon<double>>(Point<double>(0, 0), Point<double>(1, 0));

    figures.add(sq);
    figures.add(tri);
    figures.add(oct);
    EXPECT_EQ(figures.getSize(), 3);

    EXPECT_NO_THROW(figures.printAll());
    EXPECT_NO_THROW(figures.printCenters());
    EXPECT_NO_THROW(figures.printTotalArea());

    figures.remove(1);
    EXPECT_EQ(figures.getSize(), 2);

    EXPECT_NO_THROW(figures.printAll());
    EXPECT_NO_THROW(figures.printCenters());
    EXPECT_NO_THROW(figures.printTotalArea());
}


// Cross-Type Equality

TEST(FigureTest, DifferentTypesNotEqual) {
    Square<double> sq(Point<double>(0, 0), Point<double>(1, 0));
    Triangle<double> tri(Point<double>(0, 0), Point<double>(1, 0), 1.0);
    const Figure<double>& f1 = sq;
    const Figure<double>& f2 = tri;
    EXPECT_FALSE(f1 == f2);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
