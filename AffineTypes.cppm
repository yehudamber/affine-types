export module YMB.AffineTypes;

import std;
export import YMB.ConstevalNumber;

namespace YMB::AffineTypes
{

export
template <typename T, typename Scalar>
concept DataType = std::floating_point<Scalar> && requires(T a, T b, Scalar s) {
    { -a } -> std::convertible_to<T>;
    { a + b } -> std::convertible_to<T>;
    { a - b } -> std::convertible_to<T>;
    { a * s } -> std::convertible_to<T>;
    { s * a } -> std::convertible_to<T>;
    { a / s } -> std::convertible_to<T>;
    { a / b } -> std::convertible_to<Scalar>;
};

template <typename Ratio, typename T, typename Scalar, typename Tag> requires DataType<T, Scalar>
class WeightedValue
{
    template <typename Ratio2>
    using WV = WeightedValue<Ratio2, T, Scalar, Tag>;

public:
    using ValueType = T;
    using ScalarType = Scalar;
    using VectorType = WV<std::ratio<0>>;

    template <typename... Args> requires std::constructible_from<T, Args&&...>
    constexpr explicit WeightedValue(Args&&... args) : m_value(std::forward<Args>(args)...) {}

    template <typename Self>
    constexpr decltype(auto) get(this Self&& self)
    {
        return std::forward<Self>(self).m_value;
    }

    constexpr explicit operator T() const { return m_value; }

    constexpr WeightedValue& operator+=(const VectorType& vector) { m_value += vector.get(); return *this; }
    constexpr WeightedValue& operator-=(const VectorType& vector) { m_value -= vector.get(); return *this; }

    constexpr WeightedValue operator+() const { return *this; }
    constexpr auto operator-() const
    {
        return WV<std::ratio<-Ratio::num, Ratio::den>>{ -m_value };
    }

    template <typename Ratio2>
    constexpr auto operator+(const WV<Ratio2>& other) const
    {
        return WV<std::ratio_add<Ratio, Ratio2>>{ m_value + other.get() };
    }
    template <typename Ratio2>
    constexpr auto operator-(const WV<Ratio2>& other) const
    {
        return WV<std::ratio_subtract<Ratio, Ratio2>>{ m_value - other.get() };
    }

    template <typename Ratio2>
    constexpr auto operator*(ConstevalNumber<Ratio2> cn) const
    {
        return WV<std::ratio_multiply<Ratio, Ratio2>>{ m_value * static_cast<Scalar>(cn) };
    }
    template <typename Ratio2>
    constexpr auto operator/(ConstevalNumber<Ratio2> cn) const
    {
        return WV<std::ratio_divide<Ratio, Ratio2>>{ m_value / static_cast<Scalar>(cn) };
    }
    template <typename Ratio2>
    friend constexpr auto operator*(ConstevalNumber<Ratio2> cn, const WeightedValue& wp)
    {
        return WV<std::ratio_multiply<Ratio, Ratio2>>{ static_cast<Scalar>(cn) * wp.m_value };
    }

    void operator*(Scalar) const = delete("Use _cn literal");
    void operator/(Scalar) const = delete("Use _cn literal");
    friend void operator*(Scalar, const WeightedValue&) = delete("Use _cn literal");

    // Vector-only operators
    constexpr WeightedValue& operator*=(Scalar scalar) requires std::same_as<WeightedValue, VectorType>
    {
        m_value *= scalar;
        return *this;
    }
    constexpr WeightedValue& operator/=(Scalar scalar) requires std::same_as<WeightedValue, VectorType>
    {
        m_value /= scalar;
        return *this;
    }

    constexpr WeightedValue operator*(Scalar scalar) const requires std::same_as<WeightedValue, VectorType>
    {
        return WeightedValue{ m_value * scalar };
    }
    constexpr WeightedValue operator/(Scalar scalar) const requires std::same_as<WeightedValue, VectorType>
    {
        return WeightedValue{ m_value / scalar };
    }
    constexpr Scalar operator/(const WeightedValue& other) const requires std::same_as<WeightedValue, VectorType>
    {
        return m_value / other.m_value;
    }
    friend constexpr WeightedValue operator*(Scalar scalar, const WeightedValue& wp)
        requires std::same_as<WeightedValue, VectorType>
    {
        return WeightedValue{ scalar * wp.m_value };
    }

    constexpr bool operator==(const WeightedValue&) const = default;
    constexpr auto operator<=>(const WeightedValue&) const = default;

private:
    T m_value;
};

export
template <typename T, typename Scalar = T, typename Tag = void> requires DataType<T, Scalar>
using Vector = WeightedValue<std::ratio<0>, T, Scalar, Tag>;

export
template <typename T, typename Scalar = T, typename Tag = void> requires DataType<T, Scalar>
using Point = WeightedValue<std::ratio<1>, T, Scalar, Tag>;

}