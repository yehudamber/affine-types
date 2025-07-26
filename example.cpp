import std;
import YMB.AffineTypes;

using namespace YMB::literals;

using P = YMB::AffineTypes::Point<double>;

int main()
{
    auto point1 = P(13);
    auto point2 = P(42);
    auto midpoint = (point1 + point2) / 2_cn;
    auto twoThirdsWay = (point1 + 2_cn * point2) / 3_cn;
    auto incompleteComputation = point1 / 2_cn + point2 / 3_cn;
    
    static_assert(std::is_same_v<decltype(midpoint), P>);
    static_assert(std::is_same_v<decltype(twoThirdsWay), P>);
    static_assert(!std::is_same_v<decltype(incompleteComputation), P>);
    
    std::println("point1: {}, point2: {}", point1.get(), point2.get());
    std::println("midpoint: {}", midpoint.get());
    std::println("2/3 way: {}", twoThirdsWay.get());
}
