export module YMB.ConstevalNumber;

import std;

namespace YMB
{

export
template <typename Ratio>
struct ConstevalNumber
{
    constexpr ConstevalNumber operator+() const { return {}; }
    constexpr auto operator-() const -> ConstevalNumber<std::ratio<-Ratio::num, Ratio::den>> { return {}; }

    template <typename Ratio2>
    constexpr auto operator+(ConstevalNumber<Ratio2>) const -> ConstevalNumber<std::ratio_add<Ratio, Ratio2>>
    {
        return {};
    }
    template <typename Ratio2>
    constexpr auto operator-(ConstevalNumber<Ratio2>) const -> ConstevalNumber<std::ratio_subtract<Ratio, Ratio2>>
    {
        return {};
    }
    template <typename Ratio2>
    constexpr auto operator*(ConstevalNumber<Ratio2>) const -> ConstevalNumber<std::ratio_multiply<Ratio, Ratio2>>
    {
        return {};
    }
    template <typename Ratio2>
    constexpr auto operator/(ConstevalNumber<Ratio2>) const -> ConstevalNumber<std::ratio_divide<Ratio, Ratio2>>
    {
        return {};
    }

    template <std::floating_point Float>
    constexpr explicit operator Float() const { return static_cast<Float>(Ratio::num) / Ratio::den; }
};

consteval auto parseBase(std::string& literal)
{
    if (literal.front() != '0')
    {
        return 10;
    }
    literal.erase(literal.begin());
    if (!literal.empty())
    {
        switch (literal.front())
        {
        case 'X':
        case 'x':
            literal.erase(literal.begin());
            return 0x10;

        case 'B':
        case 'b':
            literal.erase(literal.begin());
            return 0b10;
        }
    }
    return 0'10;
}

consteval auto parseLiteral(std::string str)
{
    std::erase(str, '\'');

    const auto base = parseBase(str);
    const auto end = str.data() + str.length();
    std::intmax_t number;
    if (std::from_chars(str.data(), end, number, base).ptr != end)
    {
        throw "could not parse literal";
    }
    return number;
}

inline namespace literals
{

export
template <char... chs>
ConstevalNumber<std::ratio<parseLiteral({chs...})>> operator ""_cn()
{
    return {};
}

}

}