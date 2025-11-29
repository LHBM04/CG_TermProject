#pragma once

// Standard
#include <array>
#include <cstddef>
#include <cstdint>
#include <format>
#include <list>
#include <map>
#include <memory>
#include <new>
#include <queue>
#include <set>
#include <stack>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#define STATIC_CLASS(type)                      \
	public:                                     \
	    type() = delete;                        \
        ~type() = delete;                       \
                                                \
        type(const type&) = delete;             \
        type& operator=(const type&) = delete;  \
                                                \
        type(type&&) = delete;                  \
        type& operator=(type&&) = delete;       \
                                                \
        void* operator new(size_t) = delete;    \
        void* operator new[](size_t) = delete;  \
        void operator delete(void*) = delete;   \
        void operator delete[](void*) = delete; \

#pragma region Standards
using i8    = std::int8_t;
using i16   = std::int16_t;
using i32   = std::int32_t;
using i64   = std::int64_t;
using u8    = std::uint8_t;
using u16   = std::uint16_t;
using u32   = std::uint32_t;
using u64   = std::uint64_t;
using f32   = float;
using f64   = double;
using usize = std::size_t;
using ssize = std::ptrdiff_t;
#pragma endregion

#pragma region Containers(Collections)
using String = std::string;
using StringView = std::string_view;

using WString = std::wstring;
using WStringView = std::wstring_view;

template <typename TItem>
using List = std::vector<TItem>;

template <typename TItem>
using Queue = std::queue<TItem>;

template <typename TItem>
using Stack = std::stack<TItem>;

template <typename TItem>
using LinkedList = std::list<TItem>;

template <typename TItem, usize Size>
using Array = std::array<TItem, Size>;

template <typename TItem>
using Set = std::set<TItem>;

template <typename TItem>
using HashSet = std::unordered_set<TItem>;

template <typename TKey, typename TValue>
using Map = std::map<TKey, TValue>;

template <typename TKey, typename TValue>
using HashMap = std::unordered_map<TKey, TValue>;
#pragma endregion

#pragma region Smart Pointer
template <typename T>
using Unique = std::unique_ptr<T>;

/**
 * @brief 스코프를 생성합니다.
 * 
 * @tparam T       생성할 인스턴스의 타입
 * @tparam ...Args 생성할 인스턴스의 인자
 * 
 * @param ...args_ 생성할 인스턴스의 인자
 * 
 * @return Unique<T> 지정한 타입의 스코프
 */
template <typename T, typename... Args>
inline constexpr Unique<T> MakeUnique(Args&& ... args_)
{
    return std::make_unique<T>(std::forward<Args>(args_)...);
}

template<typename T>
using Shared = std::shared_ptr<T>;

/**
 * @brief 레퍼런스를 생성합니다.
 *
 * @tparam T       생성할 인스턴스의 타입
 * @tparam ...Args 생성할 인스턴스의 인자
 *
 * @param ...args_ 생성할 인스턴스의 인자
 *
 * @return Shared<T> 지정한 타입의 레퍼런스
 */
template <typename T, typename... Args>
inline constexpr Shared<T> MakeShared(Args&&... args_)
{
    return std::make_shared<T>(std::forward<Args>(args_)...);
}

template <typename T>
using Weak = std::weak_ptr<T>;
#pragma endregion