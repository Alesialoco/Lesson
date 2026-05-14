#include <iostream>

namespace TL_space {
    template<typename... Types>
    struct TypeList {};
    
    // Получение элемента списка по его индексу
    template<typename List, size_t Index>
    struct Get {
        static_assert(sizeof(List) == 0, "Index out of TypeList");
    };

    template<typename Head, typename... Tail>
    struct Get<TypeList<Head, Tail...>, 0> {
        using type = Head;
    };
    
    template<typename Head, typename... Tail, size_t Index>
    struct Get<TypeList<Head, Tail...>, Index> {
        using type = typename Get<TypeList<Tail...>, Index - 1>::type;
    };

    // Получение размера списка
    template<typename List>
    struct Size;

    template<>
    struct Size<TypeList<>> {
        static constexpr size_t value = 0;
    };

    template<typename Head, typename... Tail>
    struct Size<TypeList<Head, Tail...>> {
        static constexpr size_t value = 1 + Size<TypeList<Tail...>>::value;
    };
    
    // Проверка наличия типа в списке
    template<typename List, typename T>
    struct Contains;

    template<typename T, typename U>
    struct IsSame {
        static constexpr bool value = false;
    };

    template<typename T>
    struct IsSame<T, T> {
        static constexpr bool value = true;
    };
    
    template<typename T>
    struct Contains<TypeList<>, T>{
        static constexpr bool value = false;
    };
    
    template<typename Head, typename... Tail, typename T>
    struct Contains<TypeList<Head, Tail...>, T>{
        static constexpr bool value = IsSame<Head, T>::value || Contains<TypeList<Tail...>, T>::value;
    };
    
    // Получение индекса типа в списке
    template<typename List, typename T>
    struct IndexOf;

    template<typename Head, typename... Tail>
    struct IndexOf<TypeList<Head, Tail...>, Head> {
        static constexpr size_t value = 0;
    };
    
    template<typename Head, typename... Tail, typename T>
    struct IndexOf<TypeList<Head, Tail...>, T> {
        static constexpr size_t value = IndexOf<TypeList<Tail...>, T>::value + 1;
    };
    
    // Добавление типа в конец списка
    template<typename List, typename T>
    struct PushBack;
    
    template<typename... Types, typename T>
    struct PushBack<TypeList<Types...>, T> {
        using type = TypeList<Types..., T>;
    };
    
    // Добавление типа в начало списка
    template<typename List, typename T>
    struct PushFront;
    
    template<typename... Types, typename T>
    struct PushFront<TypeList<Types...>, T> {
        using type = TypeList<T, Types...>;
    };
    
} 

// Тесты
int main() {
    using Lst = TL_space::TypeList<int, double, char, float>;
    
    // Получение элемента списка по его индексу
    static_assert(TL_space::IsSame<typename TL_space::Get<Lst, 0>::type, int>::value, "First element should be int");
    static_assert(TL_space::IsSame<typename TL_space::Get<Lst, 1>::type, double>::value, "Second element should be double");
    static_assert(TL_space::IsSame<typename TL_space::Get<Lst, 3>::type, float>::value, "Fourth element should be float");

    // Получение размера списка
    static_assert(TL_space::Size<Lst>::value == 4, "Size should be 4");
    static_assert(TL_space::Size<TL_space::TypeList<>>::value == 0, "Empty list size should be 0");
    static_assert(TL_space::Size<TL_space::TypeList<int>>::value == 1, "Single element list size should be 1");
    
    // Проверка наличия тип
    static_assert(TL_space::Contains<Lst, int>::value, "int should be in the list");
    static_assert(!TL_space::Contains<Lst, long>::value, "long should not be in the list");
    static_assert(!TL_space::Contains<Lst, bool>::value, "bool should not be in the list");
    
    // Получение индекса типа
    static_assert(TL_space::IndexOf<Lst, int>::value == 0, "Index of int should be 0");
    static_assert(TL_space::IndexOf<Lst, char>::value == 2, "Index of char should be 2");
    static_assert(TL_space::IndexOf<Lst, float>::value == 3, "Index of float should be 3");
    
    // Добавление типа в конец списка
    using ListWithBack = TL_space::PushBack<Lst, long>::type;
    static_assert(TL_space::Size<ListWithBack>::value == 5, "Size after PushBack should be 5");
    static_assert(TL_space::IsSame<typename TL_space::Get<ListWithBack, 4>::type, long>::value, "Last element should be long");
    static_assert(TL_space::IsSame<typename TL_space::Get<ListWithBack, 0>::type, int>::value, "First element should still be int");
    
    // Добавление типа в начало списка
    using ListWithFront = TL_space::PushFront<Lst, bool>::type;
    static_assert(TL_space::Size<ListWithFront>::value == 5, "Size after PushFront should be 5");
    static_assert(TL_space::IsSame<typename TL_space::Get<ListWithFront, 0>::type, bool>::value, "First element should be bool");
    static_assert(TL_space::IsSame<typename TL_space::Get<ListWithFront, 1>::type, int>::value, "Second element should be int");
    
    // ошибка компиляции
    // typename TL_space::Get<Lst, 10>::type outOfRange;

    // ошибка компиляции
    // size_t idx = TL_space::IndexOf<Lst, long>::value;
    
    return 0;
}