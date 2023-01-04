
#include <iostream>
#include <concepts>
#include <optional>
#include <tuple>
#include <memory>

#define Some1 [](int val) {if(val == 5){std::cout<<"IS FIVE\n";} }
#define Some2 [](int val) {if(val == 10){return val + 10;} }
#define None3 [](int val) {if(val == 69){std::cerr<<"ERROR69\n";} }

template<typename T, typename E>
class Result{
private:

    T m_Some;
    E m_Err;
    enum class set{
        Some,
        Err
    };
    set m_set;
public:
    Result() = delete;
    Result(E error): m_Err{error}, m_set{set::Err} {}
    Result(T some): m_Some{some}, m_set{set::Some} {}

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
auto match2(Res<T, E> opt, Funcs &&f){
    return f(opt);
}

std::optional<int> fun(bool c){
    if (c)
        return 69;
    return std::nullopt;
}
Result<int, const char*> res_check(bool val){
    if(val) return (420);
    return ("Error in foo");
}

int main(int argc, char const *argv[]){
    std::optional<int> obj = fun(true);
    int val = match(obj, [](decltype(obj) val){
                if(val.has_value()){
                    std::cout<<"Eval: "<<val.value()<<'\n';
                    return val.value();
                }else{
                    std::cout<<"\nRetardium achieved";
                    exit(-1);
                } 
            });
    match2(res_check(false), [](auto val){
        if(val.Some().has_value()){
            std::cout<<"has_val\n";
        }else{
            std::cout<<"has_not_val\n";
        }
    });
    std::cout << std::endl;
}
