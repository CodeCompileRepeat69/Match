#ifndef _PATTERN_MATCHING_HPP
#define _PATTERN_MATCHING_HPP
#include <concepts>
#include <optional>
#include <stdexcept>


enum class set{
    Some,
    Err
};

template<typename T, typename E>
concept not_same = !std::is_same_v<T, E>;

template<typename T, typename E>  requires not_same<T, E>
class Result{
private:

    T m_Some;
    E m_Err;

    set m_set;
public:

    Result() = delete;
    //Result(E error): m_Err{error}, m_set{set::Err} {}
    //Result(T some): m_Some{some}, m_set{set::Some} {}
    Result(const E val): m_Err{val}  {m_set = set::Err}
    Result(const T val): m_Some{val} {m_set = set::Some}

    std::optional<T> Some(){
        if(m_set == set::Some) return (m_Some);
        return std::nullopt;
    }
    std::optional<E> Err(){
        if(m_set == set::Err) return (m_Err);
        return std::nullopt;
   }
};

template <typename E, typename T>
concept MatchableOpt = std::same_as<E, std::optional<T>>;


template <template <typename U> class Opt, typename T, class Funcs>
    requires MatchableOpt<Opt<T>, T> && std::invocable<Funcs, Opt<T>>
auto match(Opt<T> opt, Funcs &&f){
    return f(opt);
}

template <template <typename U, typename V> class Res, typename T, typename E, class Funcs>
    requires std::invocable<Funcs, Res<T, E>>
auto match(Res<T, E> opt, Funcs &&f){
    return f(opt);
}
#endif