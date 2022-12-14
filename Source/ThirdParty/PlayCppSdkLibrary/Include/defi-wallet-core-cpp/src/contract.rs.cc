#include "lib.rs.h"
#include "uint.rs.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <initializer_list>
#include <iterator>
#include <new>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

namespace rust {
inline namespace cxxbridge1 {
// #include "rust/cxx.h"

#ifndef CXXBRIDGE1_PANIC
#define CXXBRIDGE1_PANIC
template <typename Exception> void panic [[noreturn]] (const char *msg);
#endif // CXXBRIDGE1_PANIC

struct unsafe_bitcopy_t;

namespace {
template <typename T> class impl;
} // namespace

class Opaque;

template <typename T>::std::size_t size_of();
template <typename T>::std::size_t align_of();

#ifndef CXXBRIDGE1_RUST_STRING
#define CXXBRIDGE1_RUST_STRING
class String final {
  public:
    String() noexcept;
    String(const String &) noexcept;
    String(String &&) noexcept;
    ~String() noexcept;

    String(const std::string &);
    String(const char *);
    String(const char *, std::size_t);
    String(const char16_t *);
    String(const char16_t *, std::size_t);

    static String lossy(const std::string &) noexcept;
    static String lossy(const char *) noexcept;
    static String lossy(const char *, std::size_t) noexcept;
    static String lossy(const char16_t *) noexcept;
    static String lossy(const char16_t *, std::size_t) noexcept;

    String &operator=(const String &) &noexcept;
    String &operator=(String &&) &noexcept;

    explicit operator std::string() const;

    const char *data() const noexcept;
    std::size_t size() const noexcept;
    std::size_t length() const noexcept;
    bool empty() const noexcept;

    const char *c_str() noexcept;

    std::size_t capacity() const noexcept;
    void reserve(size_t new_cap) noexcept;

    using iterator = char *;
    iterator begin() noexcept;
    iterator end() noexcept;

    using const_iterator = const char *;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    bool operator==(const String &) const noexcept;
    bool operator!=(const String &) const noexcept;
    bool operator<(const String &) const noexcept;
    bool operator<=(const String &) const noexcept;
    bool operator>(const String &) const noexcept;
    bool operator>=(const String &) const noexcept;

    void swap(String &) noexcept;

    String(unsafe_bitcopy_t, const String &) noexcept;

  private:
    struct lossy_t;
    String(lossy_t, const char *, std::size_t) noexcept;
    String(lossy_t, const char16_t *, std::size_t) noexcept;
    friend void swap(String &lhs, String &rhs) noexcept { lhs.swap(rhs); }

    std::array<std::uintptr_t, 3> repr;
};
#endif // CXXBRIDGE1_RUST_STRING

#ifndef CXXBRIDGE1_RUST_SLICE
#define CXXBRIDGE1_RUST_SLICE
namespace detail {
template <bool> struct copy_assignable_if {};

template <> struct copy_assignable_if<false> {
    copy_assignable_if() noexcept = default;
    copy_assignable_if(const copy_assignable_if &) noexcept = default;
    copy_assignable_if &
    operator=(const copy_assignable_if &) &noexcept = delete;
    copy_assignable_if &operator=(copy_assignable_if &&) &noexcept = default;
};
} // namespace detail

template <typename T>
class Slice final
    : private detail::copy_assignable_if<std::is_const<T>::value> {
  public:
    using value_type = T;

    Slice() noexcept;
    Slice(T *, std::size_t count) noexcept;

    Slice &operator=(const Slice<T> &) &noexcept = default;
    Slice &operator=(Slice<T> &&) &noexcept = default;

    T *data() const noexcept;
    std::size_t size() const noexcept;
    std::size_t length() const noexcept;
    bool empty() const noexcept;

    T &operator[](std::size_t n) const noexcept;
    T &at(std::size_t n) const;
    T &front() const noexcept;
    T &back() const noexcept;

    Slice(const Slice<T> &) noexcept = default;
    ~Slice() noexcept = default;

    class iterator;
    iterator begin() const noexcept;
    iterator end() const noexcept;

    void swap(Slice &) noexcept;

  private:
    class uninit;
    Slice(uninit) noexcept;
    friend impl<Slice>;
    friend void sliceInit(void *, const void *, std::size_t) noexcept;
    friend void *slicePtr(const void *) noexcept;
    friend std::size_t sliceLen(const void *) noexcept;

    std::array<std::uintptr_t, 2> repr;
};

template <typename T> class Slice<T>::iterator final {
  public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = typename std::add_pointer<T>::type;
    using reference = typename std::add_lvalue_reference<T>::type;

    reference operator*() const noexcept;
    pointer operator->() const noexcept;
    reference operator[](difference_type) const noexcept;

    iterator &operator++() noexcept;
    iterator operator++(int) noexcept;
    iterator &operator--() noexcept;
    iterator operator--(int) noexcept;

    iterator &operator+=(difference_type) noexcept;
    iterator &operator-=(difference_type) noexcept;
    iterator operator+(difference_type) const noexcept;
    iterator operator-(difference_type) const noexcept;
    difference_type operator-(const iterator &) const noexcept;

    bool operator==(const iterator &) const noexcept;
    bool operator!=(const iterator &) const noexcept;
    bool operator<(const iterator &) const noexcept;
    bool operator<=(const iterator &) const noexcept;
    bool operator>(const iterator &) const noexcept;
    bool operator>=(const iterator &) const noexcept;

  private:
    friend class Slice;
    void *pos;
    std::size_t stride;
};

template <typename T> Slice<T>::Slice() noexcept {
    sliceInit(this, reinterpret_cast<void *>(align_of<T>()), 0);
}

template <typename T> Slice<T>::Slice(T *s, std::size_t count) noexcept {
    assert(s != nullptr || count == 0);
    sliceInit(this,
              s == nullptr && count == 0
                  ? reinterpret_cast<void *>(align_of<T>())
                  : const_cast<typename std::remove_const<T>::type *>(s),
              count);
}

template <typename T> T *Slice<T>::data() const noexcept {
    return reinterpret_cast<T *>(slicePtr(this));
}

template <typename T> std::size_t Slice<T>::size() const noexcept {
    return sliceLen(this);
}

template <typename T> std::size_t Slice<T>::length() const noexcept {
    return this->size();
}

template <typename T> bool Slice<T>::empty() const noexcept {
    return this->size() == 0;
}

template <typename T> T &Slice<T>::operator[](std::size_t n) const noexcept {
    assert(n < this->size());
    auto ptr = static_cast<char *>(slicePtr(this)) + size_of<T>() * n;
    return *reinterpret_cast<T *>(ptr);
}

template <typename T> T &Slice<T>::at(std::size_t n) const {
    if (n >= this->size()) {
        panic<std::out_of_range>("rust::Slice index out of range");
    }
    return (*this)[n];
}

template <typename T> T &Slice<T>::front() const noexcept {
    assert(!this->empty());
    return (*this)[0];
}

template <typename T> T &Slice<T>::back() const noexcept {
    assert(!this->empty());
    return (*this)[this->size() - 1];
}

template <typename T>
typename Slice<T>::iterator::reference
Slice<T>::iterator::operator*() const noexcept {
    return *static_cast<T *>(this->pos);
}

template <typename T>
typename Slice<T>::iterator::pointer
Slice<T>::iterator::operator->() const noexcept {
    return static_cast<T *>(this->pos);
}

template <typename T>
typename Slice<T>::iterator::reference Slice<T>::iterator::operator[](
    typename Slice<T>::iterator::difference_type n) const noexcept {
    auto ptr = static_cast<char *>(this->pos) + this->stride * n;
    return *reinterpret_cast<T *>(ptr);
}

template <typename T>
typename Slice<T>::iterator &Slice<T>::iterator::operator++() noexcept {
    this->pos = static_cast<char *>(this->pos) + this->stride;
    return *this;
}

template <typename T>
typename Slice<T>::iterator Slice<T>::iterator::operator++(int) noexcept {
    auto ret = iterator(*this);
    this->pos = static_cast<char *>(this->pos) + this->stride;
    return ret;
}

template <typename T>
typename Slice<T>::iterator &Slice<T>::iterator::operator--() noexcept {
    this->pos = static_cast<char *>(this->pos) - this->stride;
    return *this;
}

template <typename T>
typename Slice<T>::iterator Slice<T>::iterator::operator--(int) noexcept {
    auto ret = iterator(*this);
    this->pos = static_cast<char *>(this->pos) - this->stride;
    return ret;
}

template <typename T>
typename Slice<T>::iterator &Slice<T>::iterator::operator+=(
    typename Slice<T>::iterator::difference_type n) noexcept {
    this->pos = static_cast<char *>(this->pos) + this->stride * n;
    return *this;
}

template <typename T>
typename Slice<T>::iterator &Slice<T>::iterator::operator-=(
    typename Slice<T>::iterator::difference_type n) noexcept {
    this->pos = static_cast<char *>(this->pos) - this->stride * n;
    return *this;
}

template <typename T>
typename Slice<T>::iterator Slice<T>::iterator::operator+(
    typename Slice<T>::iterator::difference_type n) const noexcept {
    auto ret = iterator(*this);
    ret.pos = static_cast<char *>(this->pos) + this->stride * n;
    return ret;
}

template <typename T>
typename Slice<T>::iterator Slice<T>::iterator::operator-(
    typename Slice<T>::iterator::difference_type n) const noexcept {
    auto ret = iterator(*this);
    ret.pos = static_cast<char *>(this->pos) - this->stride * n;
    return ret;
}

template <typename T>
typename Slice<T>::iterator::difference_type
Slice<T>::iterator::operator-(const iterator &other) const noexcept {
    auto diff = std::distance(static_cast<char *>(other.pos),
                              static_cast<char *>(this->pos));
    return diff / this->stride;
}

template <typename T>
bool Slice<T>::iterator::operator==(const iterator &other) const noexcept {
    return this->pos == other.pos;
}

template <typename T>
bool Slice<T>::iterator::operator!=(const iterator &other) const noexcept {
    return this->pos != other.pos;
}

template <typename T>
bool Slice<T>::iterator::operator<(const iterator &other) const noexcept {
    return this->pos < other.pos;
}

template <typename T>
bool Slice<T>::iterator::operator<=(const iterator &other) const noexcept {
    return this->pos <= other.pos;
}

template <typename T>
bool Slice<T>::iterator::operator>(const iterator &other) const noexcept {
    return this->pos > other.pos;
}

template <typename T>
bool Slice<T>::iterator::operator>=(const iterator &other) const noexcept {
    return this->pos >= other.pos;
}

template <typename T>
typename Slice<T>::iterator Slice<T>::begin() const noexcept {
    iterator it;
    it.pos = slicePtr(this);
    it.stride = size_of<T>();
    return it;
}

template <typename T>
typename Slice<T>::iterator Slice<T>::end() const noexcept {
    iterator it = this->begin();
    it.pos = static_cast<char *>(it.pos) + it.stride * this->size();
    return it;
}

template <typename T> void Slice<T>::swap(Slice &rhs) noexcept {
    std::swap(*this, rhs);
}
#endif // CXXBRIDGE1_RUST_SLICE

#ifndef CXXBRIDGE1_RUST_BITCOPY_T
#define CXXBRIDGE1_RUST_BITCOPY_T
struct unsafe_bitcopy_t final {
    explicit unsafe_bitcopy_t() = default;
};
#endif // CXXBRIDGE1_RUST_BITCOPY_T

#ifndef CXXBRIDGE1_RUST_VEC
#define CXXBRIDGE1_RUST_VEC
template <typename T> class Vec final {
  public:
    using value_type = T;

    Vec() noexcept;
    Vec(std::initializer_list<T>);
    Vec(const Vec &);
    Vec(Vec &&) noexcept;
    ~Vec() noexcept;

    Vec &operator=(Vec &&) &noexcept;
    Vec &operator=(const Vec &) &;

    std::size_t size() const noexcept;
    bool empty() const noexcept;
    const T *data() const noexcept;
    T *data() noexcept;
    std::size_t capacity() const noexcept;

    const T &operator[](std::size_t n) const noexcept;
    const T &at(std::size_t n) const;
    const T &front() const noexcept;
    const T &back() const noexcept;

    T &operator[](std::size_t n) noexcept;
    T &at(std::size_t n);
    T &front() noexcept;
    T &back() noexcept;

    void reserve(std::size_t new_cap);
    void push_back(const T &value);
    void push_back(T &&value);
    template <typename... Args> void emplace_back(Args &&...args);
    void truncate(std::size_t len);
    void clear();

    using iterator = typename Slice<T>::iterator;
    iterator begin() noexcept;
    iterator end() noexcept;

    using const_iterator = typename Slice<const T>::iterator;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    void swap(Vec &) noexcept;

    Vec(unsafe_bitcopy_t, const Vec &) noexcept;

  private:
    void reserve_total(std::size_t new_cap) noexcept;
    void set_len(std::size_t len) noexcept;
    void drop() noexcept;

    friend void swap(Vec &lhs, Vec &rhs) noexcept { lhs.swap(rhs); }

    std::array<std::uintptr_t, 3> repr;
};

template <typename T> Vec<T>::Vec(std::initializer_list<T> init) : Vec{} {
    this->reserve_total(init.size());
    std::move(init.begin(), init.end(), std::back_inserter(*this));
}

template <typename T> Vec<T>::Vec(const Vec &other) : Vec() {
    this->reserve_total(other.size());
    std::copy(other.begin(), other.end(), std::back_inserter(*this));
}

template <typename T> Vec<T>::Vec(Vec &&other) noexcept : repr(other.repr) {
    new (&other) Vec();
}

template <typename T> Vec<T>::~Vec() noexcept { this->drop(); }

template <typename T> Vec<T> &Vec<T>::operator=(Vec &&other) &noexcept {
    this->drop();
    this->repr = other.repr;
    new (&other) Vec();
    return *this;
}

template <typename T> Vec<T> &Vec<T>::operator=(const Vec &other) & {
    if (this != &other) {
        this->drop();
        new (this) Vec(other);
    }
    return *this;
}

template <typename T> bool Vec<T>::empty() const noexcept {
    return this->size() == 0;
}

template <typename T> T *Vec<T>::data() noexcept {
    return const_cast<T *>(const_cast<const Vec<T> *>(this)->data());
}

template <typename T>
const T &Vec<T>::operator[](std::size_t n) const noexcept {
    assert(n < this->size());
    auto data = reinterpret_cast<const char *>(this->data());
    return *reinterpret_cast<const T *>(data + n * size_of<T>());
}

template <typename T> const T &Vec<T>::at(std::size_t n) const {
    if (n >= this->size()) {
        panic<std::out_of_range>("rust::Vec index out of range");
    }
    return (*this)[n];
}

template <typename T> const T &Vec<T>::front() const noexcept {
    assert(!this->empty());
    return (*this)[0];
}

template <typename T> const T &Vec<T>::back() const noexcept {
    assert(!this->empty());
    return (*this)[this->size() - 1];
}

template <typename T> T &Vec<T>::operator[](std::size_t n) noexcept {
    assert(n < this->size());
    auto data = reinterpret_cast<char *>(this->data());
    return *reinterpret_cast<T *>(data + n * size_of<T>());
}

template <typename T> T &Vec<T>::at(std::size_t n) {
    if (n >= this->size()) {
        panic<std::out_of_range>("rust::Vec index out of range");
    }
    return (*this)[n];
}

template <typename T> T &Vec<T>::front() noexcept {
    assert(!this->empty());
    return (*this)[0];
}

template <typename T> T &Vec<T>::back() noexcept {
    assert(!this->empty());
    return (*this)[this->size() - 1];
}

template <typename T> void Vec<T>::reserve(std::size_t new_cap) {
    this->reserve_total(new_cap);
}

template <typename T> void Vec<T>::push_back(const T &value) {
    this->emplace_back(value);
}

template <typename T> void Vec<T>::push_back(T &&value) {
    this->emplace_back(std::move(value));
}

template <typename T>
template <typename... Args>
void Vec<T>::emplace_back(Args &&...args) {
    auto size = this->size();
    this->reserve_total(size + 1);
    ::new (reinterpret_cast<T *>(reinterpret_cast<char *>(this->data()) +
                                 size * size_of<T>()))
        T(std::forward<Args>(args)...);
    this->set_len(size + 1);
}

template <typename T> void Vec<T>::clear() { this->truncate(0); }

template <typename T> typename Vec<T>::iterator Vec<T>::begin() noexcept {
    return Slice<T>(this->data(), this->size()).begin();
}

template <typename T> typename Vec<T>::iterator Vec<T>::end() noexcept {
    return Slice<T>(this->data(), this->size()).end();
}

template <typename T>
typename Vec<T>::const_iterator Vec<T>::begin() const noexcept {
    return this->cbegin();
}

template <typename T>
typename Vec<T>::const_iterator Vec<T>::end() const noexcept {
    return this->cend();
}

template <typename T>
typename Vec<T>::const_iterator Vec<T>::cbegin() const noexcept {
    return Slice<const T>(this->data(), this->size()).begin();
}

template <typename T>
typename Vec<T>::const_iterator Vec<T>::cend() const noexcept {
    return Slice<const T>(this->data(), this->size()).end();
}

template <typename T> void Vec<T>::swap(Vec &rhs) noexcept {
    using std::swap;
    swap(this->repr, rhs.repr);
}

template <typename T>
Vec<T>::Vec(unsafe_bitcopy_t, const Vec &bits) noexcept : repr(bits.repr) {}
#endif // CXXBRIDGE1_RUST_VEC

#ifndef CXXBRIDGE1_RUST_ERROR
#define CXXBRIDGE1_RUST_ERROR
class Error final : public std::exception {
  public:
    Error(const Error &);
    Error(Error &&) noexcept;
    ~Error() noexcept override;

    Error &operator=(const Error &) &;
    Error &operator=(Error &&) &noexcept;

    const char *what() const noexcept override;

  private:
    Error() noexcept = default;
    friend impl<Error>;
    const char *msg;
    std::size_t len;
};
#endif // CXXBRIDGE1_RUST_ERROR

#ifndef CXXBRIDGE1_IS_COMPLETE
#define CXXBRIDGE1_IS_COMPLETE
namespace detail {
namespace {
template <typename T, typename = std::size_t>
struct is_complete : std::false_type {};
template <typename T>
struct is_complete<T, decltype(sizeof(T))> : std::true_type {};
} // namespace
} // namespace detail
#endif // CXXBRIDGE1_IS_COMPLETE

#ifndef CXXBRIDGE1_LAYOUT
#define CXXBRIDGE1_LAYOUT
class layout {
    template <typename T> friend std::size_t size_of();
    template <typename T> friend std::size_t align_of();
    template <typename T>
    static typename std::enable_if<std::is_base_of<Opaque, T>::value,
                                   std::size_t>::type
    do_size_of() {
        return T::layout::size();
    }
    template <typename T>
    static typename std::enable_if<!std::is_base_of<Opaque, T>::value,
                                   std::size_t>::type
    do_size_of() {
        return sizeof(T);
    }
    template <typename T>
    static typename std::enable_if<detail::is_complete<T>::value,
                                   std::size_t>::type
    size_of() {
        return do_size_of<T>();
    }
    template <typename T>
    static typename std::enable_if<std::is_base_of<Opaque, T>::value,
                                   std::size_t>::type
    do_align_of() {
        return T::layout::align();
    }
    template <typename T>
    static typename std::enable_if<!std::is_base_of<Opaque, T>::value,
                                   std::size_t>::type
    do_align_of() {
        return alignof(T);
    }
    template <typename T>
    static typename std::enable_if<detail::is_complete<T>::value,
                                   std::size_t>::type
    align_of() {
        return do_align_of<T>();
    }
};

template <typename T> std::size_t size_of() { return layout::size_of<T>(); }

template <typename T> std::size_t align_of() { return layout::align_of<T>(); }
#endif // CXXBRIDGE1_LAYOUT

#ifndef CXXBRIDGE1_RELOCATABLE
#define CXXBRIDGE1_RELOCATABLE
namespace detail {
template <typename... Ts> struct make_void { using type = void; };

template <typename... Ts> using void_t = typename make_void<Ts...>::type;

template <typename Void, template <typename...> class, typename...>
struct detect : std::false_type {};
template <template <typename...> class T, typename... A>
struct detect<void_t<T<A...>>, T, A...> : std::true_type {};

template <template <typename...> class T, typename... A>
using is_detected = detect<void, T, A...>;

template <typename T> using detect_IsRelocatable = typename T::IsRelocatable;

template <typename T>
struct get_IsRelocatable
    : std::is_same<typename T::IsRelocatable, std::true_type> {};
} // namespace detail

template <typename T>
struct IsRelocatable
    : std::conditional<
          detail::is_detected<detail::detect_IsRelocatable, T>::value,
          detail::get_IsRelocatable<T>,
          std::integral_constant<
              bool, std::is_trivially_move_constructible<T>::value &&
                        std::is_trivially_destructible<T>::value>>::type {};
#endif // CXXBRIDGE1_RELOCATABLE

namespace repr {
struct PtrLen final {
    void *ptr;
    ::std::size_t len;
};
} // namespace repr

namespace detail {
template <typename T, typename = void *> struct operator_new {
    void *operator()(::std::size_t sz) { return ::operator new(sz); }
};

template <typename T>
struct operator_new<T, decltype(T::operator new(sizeof(T)))> {
    void *operator()(::std::size_t sz) { return T::operator new(sz); }
};
} // namespace detail

template <typename T> union ManuallyDrop {
    T value;
    ManuallyDrop(T &&value) : value(::std::move(value)) {}
    ~ManuallyDrop() {}
};

template <typename T> union MaybeUninit {
    T value;
    void *operator new(::std::size_t sz) {
        return detail::operator_new<T>{}(sz);
    }
    MaybeUninit() {}
    ~MaybeUninit() {}
};

namespace {
template <> class impl<Error> final {
  public:
    static Error error(repr::PtrLen repr) noexcept {
        Error error;
        error.msg = static_cast<char const *>(repr.ptr);
        error.len = repr.len;
        return error;
    }
};
} // namespace
} // namespace cxxbridge1
} // namespace rust

namespace org {
namespace defi_wallet_core {
struct Erc20;
struct Erc721;
struct Erc1155;
} // namespace defi_wallet_core
} // namespace org

namespace org {
namespace defi_wallet_core {
#ifndef CXXBRIDGE1_STRUCT_org$defi_wallet_core$Erc20
#define CXXBRIDGE1_STRUCT_org$defi_wallet_core$Erc20
struct Erc20 final {
    ::rust::String contract_address;
    ::rust::String web3api_url;
    bool inner_legacy;
    ::std::uint64_t inner_polling_interval_ms;
    ::std::uint64_t chain_id;

    /// Returns the decimal amount of tokens owned by `account_address`.
    /// # Examples
    /// ```
    /// Erc20 erc20 = new_erc20("0xf0307093f23311FE6776a7742dB619EB3df62969",
    ///    "https://evm-dev-t3.cronos.org", 338)
    ///  .legacy();
    /// U256 = erc20.balance_of("0xf0307093f23311FE6776a7742dB619EB3df62969");
    /// cout << balance.to_string() << endl;
    /// ```
    ::org::defi_wallet_core::U256
    balance_of(::rust::String account_address) const;

    /// Returns the name of the token
    /// ```
    /// Erc20 erc20 = new_erc20("0xf0307093f23311FE6776a7742dB619EB3df62969",
    ///    "https://evm-dev-t3.cronos.org", 338);
    /// String name = erc20.name();
    /// assert(name == "USDC");
    /// ```
    ::rust::String name() const;

    /// Returns the symbol of the token
    /// ```
    /// Erc20 erc20 = new_erc20("0xf0307093f23311FE6776a7742dB619EB3df62969",
    ///    "https://evm-dev-t3.cronos.org", 338);
    /// String symbol = erc20.symbol();
    /// assert(symbol == "USDC");
    /// ```
    ::rust::String symbol() const;

    /// Returns the number of decimals the token uses
    /// ```
    /// Erc20 erc20 = new_erc20("0xf0307093f23311FE6776a7742dB619EB3df62969",
    ///    "https://evm-dev-t3.cronos.org", 338)
    ///  .legacy();
    /// uint8_t decimals = erc20.decimals();
    /// assert(decimals == 6);
    /// ```
    ::std::uint8_t decimals() const;

    /// Makes a legacy transaction instead of an EIP-1559 one
    /// ```
    /// Erc20 erc20 = new_erc20("0xf0307093f23311FE6776a7742dB619EB3df62969",
    ///    "https://evm-dev-t3.cronos.org", 338);
    /// erc20 = erc20.legacy();
    /// ```
    ::org::defi_wallet_core::Erc20 legacy() noexcept;

    /// Sets the default polling interval for event filters and pending
    /// transactions
    /// ```
    /// Erc20 erc20 = new_erc20("0xf0307093f23311FE6776a7742dB619EB3df62969",
    ///    "https://evm-dev-t3.cronos.org", 338);
    /// erc20 = erc20.interval(3000);
    /// ```
    ::org::defi_wallet_core::Erc20
    interval(::std::uint64_t polling_interval_ms) noexcept;

    /// Moves `amount` tokens from the caller’s account to `to_address`.
    /// # Transfer 100 tokens (devnet)
    /// ```
    /// String mycronosrpc = getEnv("MYCRONOSRPC");
    /// char hdpath[100];
    /// int cointype = 60;
    /// int chainid = 777;
    /// snprintf(hdpath, sizeof(hdpath), "m/44'/%d'/0'/0/0", cointype);
    ///
    /// String signer1_mnemonics = getEnv("SIGNER1_MNEMONIC");
    /// Box<Wallet> signer1_wallet = createWallet(signer1_mnemonics);
    /// String signer1_address = signer1_wallet->get_eth_address(0);
    /// Box<PrivateKey> signer1_privatekey = signer1_wallet->get_key(hdpath);
    ///
    /// String signer2_mnemonics = getEnv("SIGNER2_MNEMONIC");
    /// Box<Wallet> signer2_wallet = createWallet(signer2_mnemonics);
    /// String signer2_address = signer2_wallet->get_eth_address(0);
    /// Box<PrivateKey> signer2_privatekey = signer2_wallet->get_key(hdpath);
    ///
    /// Erc20 erc20 = new_erc20("0x5003c1fcc043D2d81fF970266bf3fa6e8C5a1F3A",
    ///                         mycronosrpc, chainid)
    ///                 .legacy();
    /// erc20.transfer(signer2_address, "100", *privatekey);
    /// ```
    ::org::defi_wallet_core::CronosTransactionReceiptRaw
    transfer(::rust::String to_address, ::rust::String amount,
             ::org::defi_wallet_core::PrivateKey const &private_key) const;

    /// Moves `amount` tokens from `from_address` to `to_address` using the
    /// allowance mechanism. # Transfer from signer1 to validator1 using the
    /// allowance mechanism (devnet)
    /// ```
    /// String mycronosrpc = getEnv("MYCRONOSRPC");
    /// char hdpath[100];
    /// int cointype = 60;
    /// int chainid = 777;
    /// snprintf(hdpath, sizeof(hdpath), "m/44'/%d'/0'/0/0", cointype);
    ///
    /// String signer1_mnemonics = getEnv("SIGNER1_MNEMONIC");
    /// Box<Wallet> signer1_wallet = createWallet(signer1_mnemonics);
    /// String signer1_address = signer1_wallet->get_eth_address(0);
    /// Box<PrivateKey> signer1_privatekey = signer1_wallet->get_key(hdpath);
    ///
    /// String signer2_mnemonics = getEnv("SIGNER2_MNEMONIC");
    /// Box<Wallet> signer2_wallet = createWallet(signer2_mnemonics);
    /// String signer2_address = signer2_wallet->get_eth_address(0);
    /// Box<PrivateKey> signer2_privatekey = signer2_wallet->get_key(hdpath);
    ///
    /// Erc20 erc20 = new_erc20("0x5003c1fcc043D2d81fF970266bf3fa6e8C5a1F3A",
    ///                         mycronosrpc, chainid)
    ///                 .legacy();
    /// erc20.transfer_from(signer1_address, validator1_address, "100",
    ///                 *signer2_privatekey);
    /// ```
    ::org::defi_wallet_core::CronosTransactionReceiptRaw
    transfer_from(::rust::String from_address, ::rust::String to_address,
                  ::rust::String amount,
                  ::org::defi_wallet_core::PrivateKey const &private_key) const;

    /// Allows `approved_address` to withdraw from your account multiple times,
    /// up to the `amount` amount.
    /// ## approves 1000 allowance (devnet)
    /// ```
    /// String mycronosrpc = getEnv("MYCRONOSRPC");
    /// char hdpath[100];
    /// int cointype = 60;
    /// int chainid = 777;
    /// snprintf(hdpath, sizeof(hdpath), "m/44'/%d'/0'/0/0", cointype);
    ///
    /// String signer1_mnemonics = getEnv("SIGNER1_MNEMONIC");
    /// Box<Wallet> signer1_wallet = createWallet(signer1_mnemonics);
    /// String signer1_address = signer1_wallet->get_eth_address(0);
    /// Box<PrivateKey> signer1_privatekey = signer1_wallet->get_key(hdpath);
    ///
    /// String signer2_mnemonics = getEnv("SIGNER2_MNEMONIC");
    /// Box<Wallet> signer2_wallet = createWallet(signer2_mnemonics);
    /// String signer2_address = signer2_wallet->get_eth_address(0);
    /// Box<PrivateKey> signer2_privatekey = signer2_wallet->get_key(hdpath);
    ///
    /// Erc20 erc20 = new_erc20("0x5003c1fcc043D2d81fF970266bf3fa6e8C5a1F3A",
    ///                         mycronosrpc, chainid)
    ///                 .legacy();
    /// erc20.interval(3000).approve(signer2_address, "1000",
    /// *signer1_privatekey);
    /// ```
    ::org::defi_wallet_core::CronosTransactionReceiptRaw
    approve(::rust::String approved_address, ::rust::String amount,
            ::org::defi_wallet_core::PrivateKey const &private_key) const;

    /// Returns the amount which `spender` is still allowed to withdraw from
    /// `owner`.
    /// ```
    /// Erc20 erc20 = new_erc20("0x5003c1fcc043D2d81fF970266bf3fa6e8C5a1F3A",
    ///                         mycronosrpc, chainid)
    ///                 .legacy();
    /// erc20.allowance(owner, spender);
    /// ```
    ::rust::String allowance(::rust::String owner,
                             ::rust::String spender) const;

    /// Returns the amount of tokens in existence.
    /// ```
    /// Erc20 erc20 = new_erc20("0xf0307093f23311FE6776a7742dB619EB3df62969",
    ///                         "https://evm-dev-t3.cronos.org", 338)
    ///                 .legacy();
    /// U256 total_supply = erc20.total_supply();
    /// ```
    ::org::defi_wallet_core::U256 total_supply() const;

    using IsRelocatable = ::std::true_type;
};
#endif // CXXBRIDGE1_STRUCT_org$defi_wallet_core$Erc20

#ifndef CXXBRIDGE1_STRUCT_org$defi_wallet_core$Erc721
#define CXXBRIDGE1_STRUCT_org$defi_wallet_core$Erc721
struct Erc721 final {
    ::rust::String contract_address;
    ::rust::String web3api_url;
    bool inner_legacy;
    ::std::uint64_t inner_polling_interval_ms;
    ::std::uint64_t chain_id;

    /// Returns the number of tokens in owner's `account_address`.
    ///
    ::org::defi_wallet_core::U256
    balance_of(::rust::String account_address) const;

    /// Returns the owner of the `token_id` token.
    ///
    ::rust::String owner_of(::rust::String token_id) const;

    /// Get the descriptive name for a collection of NFTs in this contract
    ///
    ::rust::String name() const;

    /// Get the abbreviated name for NFTs in this contract
    ///
    ::rust::String symbol() const;

    /// Get the distinct Uniform Resource Identifier (URI) for a given asset
    ///
    ::rust::String token_uri(::rust::String token_id) const;

    /// Makes a legacy transaction instead of an EIP-1559 one
    ///
    ::org::defi_wallet_core::Erc721 legacy() noexcept;

    /// Sets the default polling interval for event filters and pending
    /// transactions
    ///
    ::org::defi_wallet_core::Erc721
    interval(::std::uint64_t polling_interval_ms) noexcept;

    /// Transfers `token_id` token from `from_address` to `to_address`.
    ///
    ::org::defi_wallet_core::CronosTransactionReceiptRaw
    transfer_from(::rust::String from_address, ::rust::String to_address,
                  ::rust::String token_id,
                  ::org::defi_wallet_core::PrivateKey const &private_key) const;

    /// Safely transfers `token_id` token from `from_address` to `to_address`.
    ///
    ::org::defi_wallet_core::CronosTransactionReceiptRaw safe_transfer_from(
        ::rust::String from_address, ::rust::String to_address,
        ::rust::String token_id,
        ::org::defi_wallet_core::PrivateKey const &private_key) const;

    /// Safely transfers `token_id` token from `from_address` to `to_address`
    /// with `additional_data`.
    ::org::defi_wallet_core::CronosTransactionReceiptRaw
    safe_transfer_from_with_data(
        ::rust::String from_address, ::rust::String to_address,
        ::rust::String token_id, ::rust::Vec<::std::uint8_t> additional_data,
        ::org::defi_wallet_core::PrivateKey const &private_key) const;

    /// Gives permission to `approved_address` to transfer `token_id` token to
    /// another account. The approval is cleared when the token is transferred.
    /// Only a single account can be approved at a time, so approving the zero
    /// address clears previous approvals.
    ::org::defi_wallet_core::CronosTransactionReceiptRaw
    approve(::rust::String approved_address, ::rust::String token_id,
            ::org::defi_wallet_core::PrivateKey const &private_key) const;

    /// Enable or disable approval for a third party `approved_address` to
    /// manage all of sender's assets
    ::org::defi_wallet_core::CronosTransactionReceiptRaw set_approval_for_all(
        ::rust::String approved_address, bool approved,
        ::org::defi_wallet_core::PrivateKey const &private_key) const;

    /// Get the approved address for a single NFT by `token_id`
    ///
    ::rust::String get_approved(::rust::String token_id) const;

    /// Query if an address is an authorized `approved_address` for `owner`
    ///
    bool is_approved_for_all(::rust::String owner,
                             ::rust::String approved_address) const;

    /// Returns the total amount of tokens stored by the contract.
    ///
    /// From IERC721Enumerable, an optional extension of the standard ERC721
    ::org::defi_wallet_core::U256 total_supply() const;

    /// Returns a token ID at a given index of all the tokens stored by the
    /// contract. Use along with totalSupply to enumerate all tokens.
    ///
    /// From IERC721Enumerable, an optional extension of the standard ERC721
    ::rust::String token_by_index(::rust::String index) const;

    /// Returns a token ID owned by owner at a given index of its token list.
    /// Use along with balanceOf to enumerate all of owner's tokens.
    ///
    /// From IERC721Enumerable, an optional extension of the standard ERC721
    ::rust::String token_of_owner_by_index(::rust::String owner,
                                           ::rust::String index) const;

    using IsRelocatable = ::std::true_type;
};
#endif // CXXBRIDGE1_STRUCT_org$defi_wallet_core$Erc721

#ifndef CXXBRIDGE1_STRUCT_org$defi_wallet_core$Erc1155
#define CXXBRIDGE1_STRUCT_org$defi_wallet_core$Erc1155
struct Erc1155 final {
    ::rust::String contract_address;
    ::rust::String web3api_url;
    bool inner_legacy;
    ::std::uint64_t inner_polling_interval_ms;
    ::std::uint64_t chain_id;

    /// Returns the amount of tokens of `token_id` owned by `account_address`.
    ///
    ::org::defi_wallet_core::U256 balance_of(::rust::String account_address,
                                             ::rust::String token_id) const;

    /// Batched version of balance_of.
    /// Get the balance of multiple account/token pairs
    ::rust::Vec<::rust::String>
    balance_of_batch(::rust::Vec<::rust::String> account_addresses,
                     ::rust::Vec<::rust::String> token_ids) const;

    /// Get distinct Uniform Resource Identifier (URI) for a given token
    ///
    ::rust::String uri(::rust::String token_id) const;

    /// Makes a legacy transaction instead of an EIP-1559 one
    ///
    ::org::defi_wallet_core::Erc1155 legacy() noexcept;

    /// Sets the default polling interval for event filters and pending
    /// transactions
    ///
    ::org::defi_wallet_core::Erc1155
    interval(::std::uint64_t polling_interval_ms) noexcept;

    /// Transfers `amount` tokens of `token_id` from `from_address` to
    /// `to_address` with `additional_data`.
    ::org::defi_wallet_core::CronosTransactionReceiptRaw safe_transfer_from(
        ::rust::String from_address, ::rust::String to_address,
        ::rust::String token_id, ::rust::String amount,
        ::rust::Vec<::std::uint8_t> additional_data,
        ::org::defi_wallet_core::PrivateKey const &private_key) const;

    /// Batched version of safeTransferFrom.
    ///
    ::org::defi_wallet_core::CronosTransactionReceiptRaw
    safe_batch_transfer_from(
        ::rust::String from_address, ::rust::String to_address,
        ::rust::Vec<::rust::String> token_ids,
        ::rust::Vec<::rust::String> amounts,
        ::rust::Vec<::std::uint8_t> additional_data,
        ::org::defi_wallet_core::PrivateKey const &private_key) const;

    /// Enable or disable approval for a third party `approved_address` to
    /// manage all of sender's assets
    ::org::defi_wallet_core::CronosTransactionReceiptRaw set_approval_for_all(
        ::rust::String approved_address, bool approved,
        ::org::defi_wallet_core::PrivateKey const &private_key) const;

    /// Query if an address is an authorized `approved_address` for `owner`
    ///
    bool is_approved_for_all(::rust::String owner,
                             ::rust::String approved_address) const;

    using IsRelocatable = ::std::true_type;
};
#endif // CXXBRIDGE1_STRUCT_org$defi_wallet_core$Erc1155
} // namespace defi_wallet_core
} // namespace org

static_assert(
    ::rust::IsRelocatable<
        ::org::defi_wallet_core::CronosTransactionReceiptRaw>::value,
    "type org::defi_wallet_core::CronosTransactionReceiptRaw should be "
    "trivially move constructible and trivially destructible in C++ to be used "
    "as a return value of `transfer`, `transfer_from`, `approve` in Rust");
static_assert(::rust::IsRelocatable<::org::defi_wallet_core::U256>::value,
              "type org::defi_wallet_core::U256 should be trivially move "
              "constructible and trivially destructible in C++ to be used as a "
              "return value of `balance_of`, `total_supply` in Rust");

namespace org {
namespace defi_wallet_core {
extern "C" {
void org$defi_wallet_core$cxxbridge1$new_erc20(
    ::rust::String *address, ::rust::String *web3api_url,
    ::std::uint64_t chian_id, ::org::defi_wallet_core::Erc20 *return$) noexcept;

::rust::repr::PtrLen org$defi_wallet_core$cxxbridge1$Erc20$balance_of(
    ::org::defi_wallet_core::Erc20 const &self, ::rust::String *account_address,
    ::org::defi_wallet_core::U256 *return$) noexcept;

::rust::repr::PtrLen org$defi_wallet_core$cxxbridge1$Erc20$name(
    ::org::defi_wallet_core::Erc20 const &self,
    ::rust::String *return$) noexcept;

::rust::repr::PtrLen org$defi_wallet_core$cxxbridge1$Erc20$symbol(
    ::org::defi_wallet_core::Erc20 const &self,
    ::rust::String *return$) noexcept;

::rust::repr::PtrLen org$defi_wallet_core$cxxbridge1$Erc20$decimals(
    ::org::defi_wallet_core::Erc20 const &self,
    ::std::uint8_t *return$) noexcept;

void org$defi_wallet_core$cxxbridge1$Erc20$legacy(
    ::org::defi_wallet_core::Erc20 &self,
    ::org::defi_wallet_core::Erc20 *return$) noexcept;

void org$defi_wallet_core$cxxbridge1$Erc20$interval(
    ::org::defi_wallet_core::Erc20 &self, ::std::uint64_t polling_interval_ms,
    ::org::defi_wallet_core::Erc20 *return$) noexcept;

::rust::repr::PtrLen org$defi_wallet_core$cxxbridge1$Erc20$transfer(
    ::org::defi_wallet_core::Erc20 const &self, ::rust::String *to_address,
    ::rust::String *amount,
    ::org::defi_wallet_core::PrivateKey const &private_key,
    ::org::defi_wallet_core::CronosTransactionReceiptRaw *return$) noexcept;

::rust::repr::PtrLen org$defi_wallet_core$cxxbridge1$Erc20$transfer_from(
    ::org::defi_wallet_core::Erc20 const &self, ::rust::String *from_address,
    ::rust::String *to_address, ::rust::String *amount,
    ::org::defi_wallet_core::PrivateKey const &private_key,
    ::org::defi_wallet_core::CronosTransactionReceiptRaw *return$) noexcept;

::rust::repr::PtrLen org$defi_wallet_core$cxxbridge1$Erc20$approve(
    ::org::defi_wallet_core::Erc20 const &self,
    ::rust::String *approved_address, ::rust::String *amount,
    ::org::defi_wallet_core::PrivateKey const &private_key,
    ::org::defi_wallet_core::CronosTransactionReceiptRaw *return$) noexcept;

::rust::repr::PtrLen org$defi_wallet_core$cxxbridge1$Erc20$allowance(
    ::org::defi_wallet_core::Erc20 const &self, ::rust::String *owner,
    ::rust::String *spender, ::rust::String *return$) noexcept;

::rust::repr::PtrLen org$defi_wallet_core$cxxbridge1$Erc20$total_supply(
    ::org::defi_wallet_core::Erc20 const &self,
    ::org::defi_wallet_core::U256 *return$) noexcept;

void org$defi_wallet_core$cxxbridge1$new_erc721(
    ::rust::String *address, ::rust::String *web3api_url,
    ::std::uint64_t chian_id,
    ::org::defi_wallet_core::Erc721 *return$) noexcept;

::rust::repr::PtrLen org$defi_wallet_core$cxxbridge1$Erc721$balance_of(
    ::org::defi_wallet_core::Erc721 const &self,
    ::rust::String *account_address,
    ::org::defi_wallet_core::U256 *return$) noexcept;

::rust::repr::PtrLen org$defi_wallet_core$cxxbridge1$Erc721$owner_of(
    ::org::defi_wallet_core::Erc721 const &self, ::rust::String *token_id,
    ::rust::String *return$) noexcept;

::rust::repr::PtrLen org$defi_wallet_core$cxxbridge1$Erc721$name(
    ::org::defi_wallet_core::Erc721 const &self,
    ::rust::String *return$) noexcept;

::rust::repr::PtrLen org$defi_wallet_core$cxxbridge1$Erc721$symbol(
    ::org::defi_wallet_core::Erc721 const &self,
    ::rust::String *return$) noexcept;

::rust::repr::PtrLen org$defi_wallet_core$cxxbridge1$Erc721$token_uri(
    ::org::defi_wallet_core::Erc721 const &self, ::rust::String *token_id,
    ::rust::String *return$) noexcept;

void org$defi_wallet_core$cxxbridge1$Erc721$legacy(
    ::org::defi_wallet_core::Erc721 &self,
    ::org::defi_wallet_core::Erc721 *return$) noexcept;

void org$defi_wallet_core$cxxbridge1$Erc721$interval(
    ::org::defi_wallet_core::Erc721 &self, ::std::uint64_t polling_interval_ms,
    ::org::defi_wallet_core::Erc721 *return$) noexcept;

::rust::repr::PtrLen org$defi_wallet_core$cxxbridge1$Erc721$transfer_from(
    ::org::defi_wallet_core::Erc721 const &self, ::rust::String *from_address,
    ::rust::String *to_address, ::rust::String *token_id,
    ::org::defi_wallet_core::PrivateKey const &private_key,
    ::org::defi_wallet_core::CronosTransactionReceiptRaw *return$) noexcept;

::rust::repr::PtrLen org$defi_wallet_core$cxxbridge1$Erc721$safe_transfer_from(
    ::org::defi_wallet_core::Erc721 const &self, ::rust::String *from_address,
    ::rust::String *to_address, ::rust::String *token_id,
    ::org::defi_wallet_core::PrivateKey const &private_key,
    ::org::defi_wallet_core::CronosTransactionReceiptRaw *return$) noexcept;

::rust::repr::PtrLen
org$defi_wallet_core$cxxbridge1$Erc721$safe_transfer_from_with_data(
    ::org::defi_wallet_core::Erc721 const &self, ::rust::String *from_address,
    ::rust::String *to_address, ::rust::String *token_id,
    ::rust::Vec<::std::uint8_t> *additional_data,
    ::org::defi_wallet_core::PrivateKey const &private_key,
    ::org::defi_wallet_core::CronosTransactionReceiptRaw *return$) noexcept;

::rust::repr::PtrLen org$defi_wallet_core$cxxbridge1$Erc721$approve(
    ::org::defi_wallet_core::Erc721 const &self,
    ::rust::String *approved_address, ::rust::String *token_id,
    ::org::defi_wallet_core::PrivateKey const &private_key,
    ::org::defi_wallet_core::CronosTransactionReceiptRaw *return$) noexcept;

::rust::repr::PtrLen
org$defi_wallet_core$cxxbridge1$Erc721$set_approval_for_all(
    ::org::defi_wallet_core::Erc721 const &self,
    ::rust::String *approved_address, bool approved,
    ::org::defi_wallet_core::PrivateKey const &private_key,
    ::org::defi_wallet_core::CronosTransactionReceiptRaw *return$) noexcept;

::rust::repr::PtrLen org$defi_wallet_core$cxxbridge1$Erc721$get_approved(
    ::org::defi_wallet_core::Erc721 const &self, ::rust::String *token_id,
    ::rust::String *return$) noexcept;

::rust::repr::PtrLen org$defi_wallet_core$cxxbridge1$Erc721$is_approved_for_all(
    ::org::defi_wallet_core::Erc721 const &self, ::rust::String *owner,
    ::rust::String *approved_address, bool *return$) noexcept;

::rust::repr::PtrLen org$defi_wallet_core$cxxbridge1$Erc721$total_supply(
    ::org::defi_wallet_core::Erc721 const &self,
    ::org::defi_wallet_core::U256 *return$) noexcept;

::rust::repr::PtrLen org$defi_wallet_core$cxxbridge1$Erc721$token_by_index(
    ::org::defi_wallet_core::Erc721 const &self, ::rust::String *index,
    ::rust::String *return$) noexcept;

::rust::repr::PtrLen
org$defi_wallet_core$cxxbridge1$Erc721$token_of_owner_by_index(
    ::org::defi_wallet_core::Erc721 const &self, ::rust::String *owner,
    ::rust::String *index, ::rust::String *return$) noexcept;

void org$defi_wallet_core$cxxbridge1$new_erc1155(
    ::rust::String *address, ::rust::String *web3api_url,
    ::std::uint64_t chian_id,
    ::org::defi_wallet_core::Erc1155 *return$) noexcept;

::rust::repr::PtrLen org$defi_wallet_core$cxxbridge1$Erc1155$balance_of(
    ::org::defi_wallet_core::Erc1155 const &self,
    ::rust::String *account_address, ::rust::String *token_id,
    ::org::defi_wallet_core::U256 *return$) noexcept;

::rust::repr::PtrLen org$defi_wallet_core$cxxbridge1$Erc1155$balance_of_batch(
    ::org::defi_wallet_core::Erc1155 const &self,
    ::rust::Vec<::rust::String> *account_addresses,
    ::rust::Vec<::rust::String> *token_ids,
    ::rust::Vec<::rust::String> *return$) noexcept;

::rust::repr::PtrLen org$defi_wallet_core$cxxbridge1$Erc1155$uri(
    ::org::defi_wallet_core::Erc1155 const &self, ::rust::String *token_id,
    ::rust::String *return$) noexcept;

void org$defi_wallet_core$cxxbridge1$Erc1155$legacy(
    ::org::defi_wallet_core::Erc1155 &self,
    ::org::defi_wallet_core::Erc1155 *return$) noexcept;

void org$defi_wallet_core$cxxbridge1$Erc1155$interval(
    ::org::defi_wallet_core::Erc1155 &self, ::std::uint64_t polling_interval_ms,
    ::org::defi_wallet_core::Erc1155 *return$) noexcept;

::rust::repr::PtrLen org$defi_wallet_core$cxxbridge1$Erc1155$safe_transfer_from(
    ::org::defi_wallet_core::Erc1155 const &self, ::rust::String *from_address,
    ::rust::String *to_address, ::rust::String *token_id,
    ::rust::String *amount, ::rust::Vec<::std::uint8_t> *additional_data,
    ::org::defi_wallet_core::PrivateKey const &private_key,
    ::org::defi_wallet_core::CronosTransactionReceiptRaw *return$) noexcept;

::rust::repr::PtrLen
org$defi_wallet_core$cxxbridge1$Erc1155$safe_batch_transfer_from(
    ::org::defi_wallet_core::Erc1155 const &self, ::rust::String *from_address,
    ::rust::String *to_address, ::rust::Vec<::rust::String> *token_ids,
    ::rust::Vec<::rust::String> *amounts,
    ::rust::Vec<::std::uint8_t> *additional_data,
    ::org::defi_wallet_core::PrivateKey const &private_key,
    ::org::defi_wallet_core::CronosTransactionReceiptRaw *return$) noexcept;

::rust::repr::PtrLen
org$defi_wallet_core$cxxbridge1$Erc1155$set_approval_for_all(
    ::org::defi_wallet_core::Erc1155 const &self,
    ::rust::String *approved_address, bool approved,
    ::org::defi_wallet_core::PrivateKey const &private_key,
    ::org::defi_wallet_core::CronosTransactionReceiptRaw *return$) noexcept;

::rust::repr::PtrLen
org$defi_wallet_core$cxxbridge1$Erc1155$is_approved_for_all(
    ::org::defi_wallet_core::Erc1155 const &self, ::rust::String *owner,
    ::rust::String *approved_address, bool *return$) noexcept;
} // extern "C"

/// Construct an Erc20 struct
/// ```
/// Erc20 erc20 = new_erc20("0xf0307093f23311FE6776a7742dB619EB3df62969",
///    "https://evm-dev-t3.cronos.org", 338);
/// ```
::org::defi_wallet_core::Erc20 new_erc20(::rust::String address,
                                         ::rust::String web3api_url,
                                         ::std::uint64_t chian_id) noexcept {
    ::rust::MaybeUninit<::org::defi_wallet_core::Erc20> return$;
    org$defi_wallet_core$cxxbridge1$new_erc20(&address, &web3api_url, chian_id,
                                              &return$.value);
    return ::std::move(return$.value);
}

::org::defi_wallet_core::U256
Erc20::balance_of(::rust::String account_address) const {
    ::rust::MaybeUninit<::org::defi_wallet_core::U256> return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$Erc20$balance_of(
            *this, &account_address, &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

::rust::String Erc20::name() const {
    ::rust::MaybeUninit<::rust::String> return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$Erc20$name(*this, &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

::rust::String Erc20::symbol() const {
    ::rust::MaybeUninit<::rust::String> return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$Erc20$symbol(*this, &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

::std::uint8_t Erc20::decimals() const {
    ::rust::MaybeUninit<::std::uint8_t> return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$Erc20$decimals(*this, &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

::org::defi_wallet_core::Erc20 Erc20::legacy() noexcept {
    ::rust::MaybeUninit<::org::defi_wallet_core::Erc20> return$;
    org$defi_wallet_core$cxxbridge1$Erc20$legacy(*this, &return$.value);
    return ::std::move(return$.value);
}

::org::defi_wallet_core::Erc20
Erc20::interval(::std::uint64_t polling_interval_ms) noexcept {
    ::rust::MaybeUninit<::org::defi_wallet_core::Erc20> return$;
    org$defi_wallet_core$cxxbridge1$Erc20$interval(*this, polling_interval_ms,
                                                   &return$.value);
    return ::std::move(return$.value);
}

::org::defi_wallet_core::CronosTransactionReceiptRaw
Erc20::transfer(::rust::String to_address, ::rust::String amount,
                ::org::defi_wallet_core::PrivateKey const &private_key) const {
    ::rust::MaybeUninit<::org::defi_wallet_core::CronosTransactionReceiptRaw>
        return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$Erc20$transfer(
            *this, &to_address, &amount, private_key, &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

::org::defi_wallet_core::CronosTransactionReceiptRaw Erc20::transfer_from(
    ::rust::String from_address, ::rust::String to_address,
    ::rust::String amount,
    ::org::defi_wallet_core::PrivateKey const &private_key) const {
    ::rust::MaybeUninit<::org::defi_wallet_core::CronosTransactionReceiptRaw>
        return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$Erc20$transfer_from(
            *this, &from_address, &to_address, &amount, private_key,
            &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

::org::defi_wallet_core::CronosTransactionReceiptRaw
Erc20::approve(::rust::String approved_address, ::rust::String amount,
               ::org::defi_wallet_core::PrivateKey const &private_key) const {
    ::rust::MaybeUninit<::org::defi_wallet_core::CronosTransactionReceiptRaw>
        return$;
    ::rust::repr::PtrLen error$ = org$defi_wallet_core$cxxbridge1$Erc20$approve(
        *this, &approved_address, &amount, private_key, &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

::rust::String Erc20::allowance(::rust::String owner,
                                ::rust::String spender) const {
    ::rust::MaybeUninit<::rust::String> return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$Erc20$allowance(*this, &owner, &spender,
                                                        &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

::org::defi_wallet_core::U256 Erc20::total_supply() const {
    ::rust::MaybeUninit<::org::defi_wallet_core::U256> return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$Erc20$total_supply(*this,
                                                           &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

/// Construct an Erc721 struct
///
::org::defi_wallet_core::Erc721 new_erc721(::rust::String address,
                                           ::rust::String web3api_url,
                                           ::std::uint64_t chian_id) noexcept {
    ::rust::MaybeUninit<::org::defi_wallet_core::Erc721> return$;
    org$defi_wallet_core$cxxbridge1$new_erc721(&address, &web3api_url, chian_id,
                                               &return$.value);
    return ::std::move(return$.value);
}

::org::defi_wallet_core::U256
Erc721::balance_of(::rust::String account_address) const {
    ::rust::MaybeUninit<::org::defi_wallet_core::U256> return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$Erc721$balance_of(
            *this, &account_address, &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

::rust::String Erc721::owner_of(::rust::String token_id) const {
    ::rust::MaybeUninit<::rust::String> return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$Erc721$owner_of(*this, &token_id,
                                                        &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

::rust::String Erc721::name() const {
    ::rust::MaybeUninit<::rust::String> return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$Erc721$name(*this, &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

::rust::String Erc721::symbol() const {
    ::rust::MaybeUninit<::rust::String> return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$Erc721$symbol(*this, &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

::rust::String Erc721::token_uri(::rust::String token_id) const {
    ::rust::MaybeUninit<::rust::String> return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$Erc721$token_uri(*this, &token_id,
                                                         &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

::org::defi_wallet_core::Erc721 Erc721::legacy() noexcept {
    ::rust::MaybeUninit<::org::defi_wallet_core::Erc721> return$;
    org$defi_wallet_core$cxxbridge1$Erc721$legacy(*this, &return$.value);
    return ::std::move(return$.value);
}

::org::defi_wallet_core::Erc721
Erc721::interval(::std::uint64_t polling_interval_ms) noexcept {
    ::rust::MaybeUninit<::org::defi_wallet_core::Erc721> return$;
    org$defi_wallet_core$cxxbridge1$Erc721$interval(*this, polling_interval_ms,
                                                    &return$.value);
    return ::std::move(return$.value);
}

::org::defi_wallet_core::CronosTransactionReceiptRaw Erc721::transfer_from(
    ::rust::String from_address, ::rust::String to_address,
    ::rust::String token_id,
    ::org::defi_wallet_core::PrivateKey const &private_key) const {
    ::rust::MaybeUninit<::org::defi_wallet_core::CronosTransactionReceiptRaw>
        return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$Erc721$transfer_from(
            *this, &from_address, &to_address, &token_id, private_key,
            &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

::org::defi_wallet_core::CronosTransactionReceiptRaw Erc721::safe_transfer_from(
    ::rust::String from_address, ::rust::String to_address,
    ::rust::String token_id,
    ::org::defi_wallet_core::PrivateKey const &private_key) const {
    ::rust::MaybeUninit<::org::defi_wallet_core::CronosTransactionReceiptRaw>
        return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$Erc721$safe_transfer_from(
            *this, &from_address, &to_address, &token_id, private_key,
            &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

::org::defi_wallet_core::CronosTransactionReceiptRaw
Erc721::safe_transfer_from_with_data(
    ::rust::String from_address, ::rust::String to_address,
    ::rust::String token_id, ::rust::Vec<::std::uint8_t> additional_data,
    ::org::defi_wallet_core::PrivateKey const &private_key) const {
    ::rust::ManuallyDrop<::rust::Vec<::std::uint8_t>> additional_data$(
        ::std::move(additional_data));
    ::rust::MaybeUninit<::org::defi_wallet_core::CronosTransactionReceiptRaw>
        return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$Erc721$safe_transfer_from_with_data(
            *this, &from_address, &to_address, &token_id,
            &additional_data$.value, private_key, &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

::org::defi_wallet_core::CronosTransactionReceiptRaw
Erc721::approve(::rust::String approved_address, ::rust::String token_id,
                ::org::defi_wallet_core::PrivateKey const &private_key) const {
    ::rust::MaybeUninit<::org::defi_wallet_core::CronosTransactionReceiptRaw>
        return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$Erc721$approve(
            *this, &approved_address, &token_id, private_key, &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

::org::defi_wallet_core::CronosTransactionReceiptRaw
Erc721::set_approval_for_all(
    ::rust::String approved_address, bool approved,
    ::org::defi_wallet_core::PrivateKey const &private_key) const {
    ::rust::MaybeUninit<::org::defi_wallet_core::CronosTransactionReceiptRaw>
        return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$Erc721$set_approval_for_all(
            *this, &approved_address, approved, private_key, &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

::rust::String Erc721::get_approved(::rust::String token_id) const {
    ::rust::MaybeUninit<::rust::String> return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$Erc721$get_approved(*this, &token_id,
                                                            &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

bool Erc721::is_approved_for_all(::rust::String owner,
                                 ::rust::String approved_address) const {
    ::rust::MaybeUninit<bool> return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$Erc721$is_approved_for_all(
            *this, &owner, &approved_address, &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

::org::defi_wallet_core::U256 Erc721::total_supply() const {
    ::rust::MaybeUninit<::org::defi_wallet_core::U256> return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$Erc721$total_supply(*this,
                                                            &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

::rust::String Erc721::token_by_index(::rust::String index) const {
    ::rust::MaybeUninit<::rust::String> return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$Erc721$token_by_index(*this, &index,
                                                              &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

::rust::String Erc721::token_of_owner_by_index(::rust::String owner,
                                               ::rust::String index) const {
    ::rust::MaybeUninit<::rust::String> return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$Erc721$token_of_owner_by_index(
            *this, &owner, &index, &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

/// Construct an Erc1155 struct
///
::org::defi_wallet_core::Erc1155
new_erc1155(::rust::String address, ::rust::String web3api_url,
            ::std::uint64_t chian_id) noexcept {
    ::rust::MaybeUninit<::org::defi_wallet_core::Erc1155> return$;
    org$defi_wallet_core$cxxbridge1$new_erc1155(&address, &web3api_url,
                                                chian_id, &return$.value);
    return ::std::move(return$.value);
}

::org::defi_wallet_core::U256
Erc1155::balance_of(::rust::String account_address,
                    ::rust::String token_id) const {
    ::rust::MaybeUninit<::org::defi_wallet_core::U256> return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$Erc1155$balance_of(
            *this, &account_address, &token_id, &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

::rust::Vec<::rust::String>
Erc1155::balance_of_batch(::rust::Vec<::rust::String> account_addresses,
                          ::rust::Vec<::rust::String> token_ids) const {
    ::rust::ManuallyDrop<::rust::Vec<::rust::String>> account_addresses$(
        ::std::move(account_addresses));
    ::rust::ManuallyDrop<::rust::Vec<::rust::String>> token_ids$(
        ::std::move(token_ids));
    ::rust::MaybeUninit<::rust::Vec<::rust::String>> return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$Erc1155$balance_of_batch(
            *this, &account_addresses$.value, &token_ids$.value,
            &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

::rust::String Erc1155::uri(::rust::String token_id) const {
    ::rust::MaybeUninit<::rust::String> return$;
    ::rust::repr::PtrLen error$ = org$defi_wallet_core$cxxbridge1$Erc1155$uri(
        *this, &token_id, &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

::org::defi_wallet_core::Erc1155 Erc1155::legacy() noexcept {
    ::rust::MaybeUninit<::org::defi_wallet_core::Erc1155> return$;
    org$defi_wallet_core$cxxbridge1$Erc1155$legacy(*this, &return$.value);
    return ::std::move(return$.value);
}

::org::defi_wallet_core::Erc1155
Erc1155::interval(::std::uint64_t polling_interval_ms) noexcept {
    ::rust::MaybeUninit<::org::defi_wallet_core::Erc1155> return$;
    org$defi_wallet_core$cxxbridge1$Erc1155$interval(*this, polling_interval_ms,
                                                     &return$.value);
    return ::std::move(return$.value);
}

::org::defi_wallet_core::CronosTransactionReceiptRaw
Erc1155::safe_transfer_from(
    ::rust::String from_address, ::rust::String to_address,
    ::rust::String token_id, ::rust::String amount,
    ::rust::Vec<::std::uint8_t> additional_data,
    ::org::defi_wallet_core::PrivateKey const &private_key) const {
    ::rust::ManuallyDrop<::rust::Vec<::std::uint8_t>> additional_data$(
        ::std::move(additional_data));
    ::rust::MaybeUninit<::org::defi_wallet_core::CronosTransactionReceiptRaw>
        return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$Erc1155$safe_transfer_from(
            *this, &from_address, &to_address, &token_id, &amount,
            &additional_data$.value, private_key, &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

::org::defi_wallet_core::CronosTransactionReceiptRaw
Erc1155::safe_batch_transfer_from(
    ::rust::String from_address, ::rust::String to_address,
    ::rust::Vec<::rust::String> token_ids, ::rust::Vec<::rust::String> amounts,
    ::rust::Vec<::std::uint8_t> additional_data,
    ::org::defi_wallet_core::PrivateKey const &private_key) const {
    ::rust::ManuallyDrop<::rust::Vec<::rust::String>> token_ids$(
        ::std::move(token_ids));
    ::rust::ManuallyDrop<::rust::Vec<::rust::String>> amounts$(
        ::std::move(amounts));
    ::rust::ManuallyDrop<::rust::Vec<::std::uint8_t>> additional_data$(
        ::std::move(additional_data));
    ::rust::MaybeUninit<::org::defi_wallet_core::CronosTransactionReceiptRaw>
        return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$Erc1155$safe_batch_transfer_from(
            *this, &from_address, &to_address, &token_ids$.value,
            &amounts$.value, &additional_data$.value, private_key,
            &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

::org::defi_wallet_core::CronosTransactionReceiptRaw
Erc1155::set_approval_for_all(
    ::rust::String approved_address, bool approved,
    ::org::defi_wallet_core::PrivateKey const &private_key) const {
    ::rust::MaybeUninit<::org::defi_wallet_core::CronosTransactionReceiptRaw>
        return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$Erc1155$set_approval_for_all(
            *this, &approved_address, approved, private_key, &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

bool Erc1155::is_approved_for_all(::rust::String owner,
                                  ::rust::String approved_address) const {
    ::rust::MaybeUninit<bool> return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$Erc1155$is_approved_for_all(
            *this, &owner, &approved_address, &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}
} // namespace defi_wallet_core
} // namespace org
