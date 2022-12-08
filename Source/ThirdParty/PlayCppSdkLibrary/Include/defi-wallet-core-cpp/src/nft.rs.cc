#include "lib.rs.h"
#include "../../nft.h"
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

#ifndef CXXBRIDGE1_RUST_BOX
#define CXXBRIDGE1_RUST_BOX
template <typename T> class Box final {
  public:
    using element_type = T;
    using const_pointer =
        typename std::add_pointer<typename std::add_const<T>::type>::type;
    using pointer = typename std::add_pointer<T>::type;

    Box() = delete;
    Box(Box &&) noexcept;
    ~Box() noexcept;

    explicit Box(const T &);
    explicit Box(T &&);

    Box &operator=(Box &&) &noexcept;

    const T *operator->() const noexcept;
    const T &operator*() const noexcept;
    T *operator->() noexcept;
    T &operator*() noexcept;

    template <typename... Fields> static Box in_place(Fields &&...);

    void swap(Box &) noexcept;

    static Box from_raw(T *) noexcept;

    T *into_raw() noexcept;

    /* Deprecated */ using value_type = element_type;

  private:
    class uninit;
    class allocation;
    Box(uninit) noexcept;
    void drop() noexcept;

    friend void swap(Box &lhs, Box &rhs) noexcept { lhs.swap(rhs); }

    T *ptr;
};

template <typename T> class Box<T>::uninit {};

template <typename T> class Box<T>::allocation {
    static T *alloc() noexcept;
    static void dealloc(T *) noexcept;

  public:
    allocation() noexcept : ptr(alloc()) {}
    ~allocation() noexcept {
        if (this->ptr) {
            dealloc(this->ptr);
        }
    }
    T *ptr;
};

template <typename T> Box<T>::Box(Box &&other) noexcept : ptr(other.ptr) {
    other.ptr = nullptr;
}

template <typename T> Box<T>::Box(const T &val) {
    allocation alloc;
    ::new (alloc.ptr) T(val);
    this->ptr = alloc.ptr;
    alloc.ptr = nullptr;
}

template <typename T> Box<T>::Box(T &&val) {
    allocation alloc;
    ::new (alloc.ptr) T(std::move(val));
    this->ptr = alloc.ptr;
    alloc.ptr = nullptr;
}

template <typename T> Box<T>::~Box() noexcept {
    if (this->ptr) {
        this->drop();
    }
}

template <typename T> Box<T> &Box<T>::operator=(Box &&other) &noexcept {
    if (this->ptr) {
        this->drop();
    }
    this->ptr = other.ptr;
    other.ptr = nullptr;
    return *this;
}

template <typename T> const T *Box<T>::operator->() const noexcept {
    return this->ptr;
}

template <typename T> const T &Box<T>::operator*() const noexcept {
    return *this->ptr;
}

template <typename T> T *Box<T>::operator->() noexcept { return this->ptr; }

template <typename T> T &Box<T>::operator*() noexcept { return *this->ptr; }

template <typename T>
template <typename... Fields>
Box<T> Box<T>::in_place(Fields &&...fields) {
    allocation alloc;
    auto ptr = alloc.ptr;
    ::new (ptr) T{std::forward<Fields>(fields)...};
    alloc.ptr = nullptr;
    return from_raw(ptr);
}

template <typename T> void Box<T>::swap(Box &rhs) noexcept {
    using std::swap;
    swap(this->ptr, rhs.ptr);
}

template <typename T> Box<T> Box<T>::from_raw(T *raw) noexcept {
    Box box = uninit{};
    box.ptr = raw;
    return box;
}

template <typename T> T *Box<T>::into_raw() noexcept {
    T *raw = this->ptr;
    this->ptr = nullptr;
    return raw;
}

template <typename T> Box<T>::Box(uninit) noexcept {}
#endif // CXXBRIDGE1_RUST_BOX

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

#ifndef CXXBRIDGE1_RUST_OPAQUE
#define CXXBRIDGE1_RUST_OPAQUE
class Opaque {
  public:
    Opaque() = delete;
    Opaque(const Opaque &) = delete;
    ~Opaque() = delete;
};
#endif // CXXBRIDGE1_RUST_OPAQUE

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
struct Denom;
struct BaseNft;
struct IdCollection;
struct Owner;
struct Collection;
using Pagination = ::org::defi_wallet_core::Pagination;
struct GrpcClient;
} // namespace defi_wallet_core
} // namespace org

namespace org {
namespace defi_wallet_core {
#ifndef CXXBRIDGE1_STRUCT_org$defi_wallet_core$Denom
#define CXXBRIDGE1_STRUCT_org$defi_wallet_core$Denom
struct Denom final {
    ::rust::String id;
    ::rust::String name;
    ::rust::String schema;
    ::rust::String creator;

    ::rust::String to_string() const noexcept;
    using IsRelocatable = ::std::true_type;
};
#endif // CXXBRIDGE1_STRUCT_org$defi_wallet_core$Denom

#ifndef CXXBRIDGE1_STRUCT_org$defi_wallet_core$BaseNft
#define CXXBRIDGE1_STRUCT_org$defi_wallet_core$BaseNft
struct BaseNft final {
    ::rust::String id;
    ::rust::String name;
    ::rust::String uri;
    ::rust::String data;
    ::rust::String owner;

    ::rust::String to_string() const noexcept;
    using IsRelocatable = ::std::true_type;
};
#endif // CXXBRIDGE1_STRUCT_org$defi_wallet_core$BaseNft

#ifndef CXXBRIDGE1_STRUCT_org$defi_wallet_core$IdCollection
#define CXXBRIDGE1_STRUCT_org$defi_wallet_core$IdCollection
struct IdCollection final {
    ::rust::String denom_id;
    ::rust::Vec<::rust::String> token_ids;

    using IsRelocatable = ::std::true_type;
};
#endif // CXXBRIDGE1_STRUCT_org$defi_wallet_core$IdCollection

#ifndef CXXBRIDGE1_STRUCT_org$defi_wallet_core$Owner
#define CXXBRIDGE1_STRUCT_org$defi_wallet_core$Owner
struct Owner final {
    ::rust::String address;
    ::rust::Vec<::org::defi_wallet_core::IdCollection> id_collections;

    ::rust::String to_string() const noexcept;
    using IsRelocatable = ::std::true_type;
};
#endif // CXXBRIDGE1_STRUCT_org$defi_wallet_core$Owner

#ifndef CXXBRIDGE1_STRUCT_org$defi_wallet_core$Collection
#define CXXBRIDGE1_STRUCT_org$defi_wallet_core$Collection
struct Collection final {
    bool denom_option;
    ::org::defi_wallet_core::Denom denom_value;
    ::rust::Vec<::org::defi_wallet_core::BaseNft> nfts;

    ::rust::String to_string() const noexcept;
    using IsRelocatable = ::std::true_type;
};
#endif // CXXBRIDGE1_STRUCT_org$defi_wallet_core$Collection

#ifndef CXXBRIDGE1_STRUCT_org$defi_wallet_core$GrpcClient
#define CXXBRIDGE1_STRUCT_org$defi_wallet_core$GrpcClient
struct GrpcClient final : public ::rust::Opaque {
    /// Supply queries the total supply of a given denom or owner
    ///
    ::std::uint64_t supply(::rust::String denom_id, ::rust::String owner) const;

    /// Owner queries the NFTs of the specified owner
    ///
    ::org::defi_wallet_core::Owner
    owner(::rust::String denom_id, ::rust::String owner,
          ::org::defi_wallet_core::Pagination const &pagination) const;

    /// Collection queries the NFTs of the specified denom
    ///
    ::org::defi_wallet_core::Collection
    collection(::rust::String denom_id,
               ::org::defi_wallet_core::Pagination const &pagination) const;

    /// Denom queries the definition of a given denom
    ///
    ::org::defi_wallet_core::Denom denom(::rust::String denom_id) const;

    /// DenomByName queries the definition of a given denom by name
    ///
    ::org::defi_wallet_core::Denom
    denom_by_name(::rust::String denom_name) const;

    /// Denoms queries all the denoms
    ///
    ::rust::Vec<::org::defi_wallet_core::Denom>
    denoms(::org::defi_wallet_core::Pagination const &pagination) const;

    /// NFT queries the NFT for the given denom and token ID
    ///
    ::org::defi_wallet_core::BaseNft nft(::rust::String denom_id,
                                         ::rust::String token_id) const;

    ~GrpcClient() = delete;

  private:
    friend ::rust::layout;
    struct layout {
        static ::std::size_t size() noexcept;
        static ::std::size_t align() noexcept;
    };
};
#endif // CXXBRIDGE1_STRUCT_org$defi_wallet_core$GrpcClient
} // namespace defi_wallet_core
} // namespace org

static_assert(
    ::rust::IsRelocatable<::org::defi_wallet_core::CosmosSDKTxInfoRaw>::value,
    "type org::defi_wallet_core::CosmosSDKTxInfoRaw should be trivially move "
    "constructible and trivially destructible in C++ to be used as an argument "
    "of `get_nft_issue_denom_signed_tx`, `get_nft_mint_signed_tx`, "
    "`get_nft_edit_signed_tx` in Rust");

namespace org {
namespace defi_wallet_core {
extern "C" {
bool org$defi_wallet_core$cxxbridge1$Pagination$get_enable(
    ::org::defi_wallet_core::Pagination const &self) noexcept {
    bool (::org::defi_wallet_core::Pagination::*get_enable$)() const =
        &::org::defi_wallet_core::Pagination::get_enable;
    return (self.*get_enable$)();
}

void org$defi_wallet_core$cxxbridge1$Pagination$get_key(
    ::org::defi_wallet_core::Pagination const &self,
    ::rust::Vec<::std::uint8_t> *return$) noexcept {
    ::rust::Vec<::std::uint8_t> (
        ::org::defi_wallet_core::Pagination::*get_key$)() const =
        &::org::defi_wallet_core::Pagination::get_key;
    new (return$)::rust::Vec<::std::uint8_t>((self.*get_key$)());
}

::std::uint64_t org$defi_wallet_core$cxxbridge1$Pagination$get_offset(
    ::org::defi_wallet_core::Pagination const &self) noexcept {
    ::std::uint64_t (::org::defi_wallet_core::Pagination::*get_offset$)()
        const = &::org::defi_wallet_core::Pagination::get_offset;
    return (self.*get_offset$)();
}

::std::uint64_t org$defi_wallet_core$cxxbridge1$Pagination$get_limit(
    ::org::defi_wallet_core::Pagination const &self) noexcept {
    ::std::uint64_t (::org::defi_wallet_core::Pagination::*get_limit$)() const =
        &::org::defi_wallet_core::Pagination::get_limit;
    return (self.*get_limit$)();
}

bool org$defi_wallet_core$cxxbridge1$Pagination$get_count_total(
    ::org::defi_wallet_core::Pagination const &self) noexcept {
    bool (::org::defi_wallet_core::Pagination::*get_count_total$)() const =
        &::org::defi_wallet_core::Pagination::get_count_total;
    return (self.*get_count_total$)();
}

bool org$defi_wallet_core$cxxbridge1$Pagination$get_reverse(
    ::org::defi_wallet_core::Pagination const &self) noexcept {
    bool (::org::defi_wallet_core::Pagination::*get_reverse$)() const =
        &::org::defi_wallet_core::Pagination::get_reverse;
    return (self.*get_reverse$)();
}

::rust::repr::PtrLen
org$defi_wallet_core$cxxbridge1$get_nft_issue_denom_signed_tx(
    ::org::defi_wallet_core::CosmosSDKTxInfoRaw *tx_info,
    ::org::defi_wallet_core::PrivateKey const &private_key, ::rust::String *id,
    ::rust::String *name, ::rust::String *schema,
    ::rust::Vec<::std::uint8_t> *return$) noexcept;

::rust::repr::PtrLen org$defi_wallet_core$cxxbridge1$get_nft_mint_signed_tx(
    ::org::defi_wallet_core::CosmosSDKTxInfoRaw *tx_info,
    ::org::defi_wallet_core::PrivateKey const &private_key, ::rust::String *id,
    ::rust::String *denom_id, ::rust::String *name, ::rust::String *uri,
    ::rust::String *data, ::rust::String *recipient,
    ::rust::Vec<::std::uint8_t> *return$) noexcept;

::rust::repr::PtrLen org$defi_wallet_core$cxxbridge1$get_nft_edit_signed_tx(
    ::org::defi_wallet_core::CosmosSDKTxInfoRaw *tx_info,
    ::org::defi_wallet_core::PrivateKey const &private_key, ::rust::String *id,
    ::rust::String *denom_id, ::rust::String *name, ::rust::String *uri,
    ::rust::String *data, ::rust::Vec<::std::uint8_t> *return$) noexcept;

::rust::repr::PtrLen org$defi_wallet_core$cxxbridge1$get_nft_transfer_signed_tx(
    ::org::defi_wallet_core::CosmosSDKTxInfoRaw *tx_info,
    ::org::defi_wallet_core::PrivateKey const &private_key, ::rust::String *id,
    ::rust::String *denom_id, ::rust::String *recipient,
    ::rust::Vec<::std::uint8_t> *return$) noexcept;

::rust::repr::PtrLen org$defi_wallet_core$cxxbridge1$get_nft_burn_signed_tx(
    ::org::defi_wallet_core::CosmosSDKTxInfoRaw *tx_info,
    ::org::defi_wallet_core::PrivateKey const &private_key, ::rust::String *id,
    ::rust::String *denom_id, ::rust::Vec<::std::uint8_t> *return$) noexcept;
::std::size_t
org$defi_wallet_core$cxxbridge1$GrpcClient$operator$sizeof() noexcept;
::std::size_t
org$defi_wallet_core$cxxbridge1$GrpcClient$operator$alignof() noexcept;

::rust::repr::PtrLen org$defi_wallet_core$cxxbridge1$new_grpc_client(
    ::rust::String *grpc_url,
    ::rust::Box<::org::defi_wallet_core::GrpcClient> *return$) noexcept;

::rust::repr::PtrLen org$defi_wallet_core$cxxbridge1$GrpcClient$supply(
    ::org::defi_wallet_core::GrpcClient const &self, ::rust::String *denom_id,
    ::rust::String *owner, ::std::uint64_t *return$) noexcept;

::rust::repr::PtrLen org$defi_wallet_core$cxxbridge1$GrpcClient$owner(
    ::org::defi_wallet_core::GrpcClient const &self, ::rust::String *denom_id,
    ::rust::String *owner,
    ::org::defi_wallet_core::Pagination const &pagination,
    ::org::defi_wallet_core::Owner *return$) noexcept;

::rust::repr::PtrLen org$defi_wallet_core$cxxbridge1$GrpcClient$collection(
    ::org::defi_wallet_core::GrpcClient const &self, ::rust::String *denom_id,
    ::org::defi_wallet_core::Pagination const &pagination,
    ::org::defi_wallet_core::Collection *return$) noexcept;

::rust::repr::PtrLen org$defi_wallet_core$cxxbridge1$GrpcClient$denom(
    ::org::defi_wallet_core::GrpcClient const &self, ::rust::String *denom_id,
    ::org::defi_wallet_core::Denom *return$) noexcept;

::rust::repr::PtrLen org$defi_wallet_core$cxxbridge1$GrpcClient$denom_by_name(
    ::org::defi_wallet_core::GrpcClient const &self, ::rust::String *denom_name,
    ::org::defi_wallet_core::Denom *return$) noexcept;

::rust::repr::PtrLen org$defi_wallet_core$cxxbridge1$GrpcClient$denoms(
    ::org::defi_wallet_core::GrpcClient const &self,
    ::org::defi_wallet_core::Pagination const &pagination,
    ::rust::Vec<::org::defi_wallet_core::Denom> *return$) noexcept;

::rust::repr::PtrLen org$defi_wallet_core$cxxbridge1$GrpcClient$nft(
    ::org::defi_wallet_core::GrpcClient const &self, ::rust::String *denom_id,
    ::rust::String *token_id,
    ::org::defi_wallet_core::BaseNft *return$) noexcept;

void org$defi_wallet_core$cxxbridge1$Owner$to_string(
    ::org::defi_wallet_core::Owner const &self,
    ::rust::String *return$) noexcept;

void org$defi_wallet_core$cxxbridge1$Collection$to_string(
    ::org::defi_wallet_core::Collection const &self,
    ::rust::String *return$) noexcept;

void org$defi_wallet_core$cxxbridge1$Denom$to_string(
    ::org::defi_wallet_core::Denom const &self,
    ::rust::String *return$) noexcept;

void org$defi_wallet_core$cxxbridge1$BaseNft$to_string(
    ::org::defi_wallet_core::BaseNft const &self,
    ::rust::String *return$) noexcept;
} // extern "C"

/// creates the signed transaction
/// for `MsgIssueDenom` from the Chainmain nft module
::rust::Vec<::std::uint8_t> get_nft_issue_denom_signed_tx(
    ::org::defi_wallet_core::CosmosSDKTxInfoRaw tx_info,
    ::org::defi_wallet_core::PrivateKey const &private_key, ::rust::String id,
    ::rust::String name, ::rust::String schema) {
    ::rust::ManuallyDrop<::org::defi_wallet_core::CosmosSDKTxInfoRaw> tx_info$(
        ::std::move(tx_info));
    ::rust::MaybeUninit<::rust::Vec<::std::uint8_t>> return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$get_nft_issue_denom_signed_tx(
            &tx_info$.value, private_key, &id, &name, &schema, &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

/// creates the signed transaction
/// for `MsgMintNft` from the Chainmain nft module
::rust::Vec<::std::uint8_t>
get_nft_mint_signed_tx(::org::defi_wallet_core::CosmosSDKTxInfoRaw tx_info,
                       ::org::defi_wallet_core::PrivateKey const &private_key,
                       ::rust::String id, ::rust::String denom_id,
                       ::rust::String name, ::rust::String uri,
                       ::rust::String data, ::rust::String recipient) {
    ::rust::ManuallyDrop<::org::defi_wallet_core::CosmosSDKTxInfoRaw> tx_info$(
        ::std::move(tx_info));
    ::rust::MaybeUninit<::rust::Vec<::std::uint8_t>> return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$get_nft_mint_signed_tx(
            &tx_info$.value, private_key, &id, &denom_id, &name, &uri, &data,
            &recipient, &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

/// creates the signed transaction
/// for `MsgEditNft` from the Chainmain nft module
::rust::Vec<::std::uint8_t>
get_nft_edit_signed_tx(::org::defi_wallet_core::CosmosSDKTxInfoRaw tx_info,
                       ::org::defi_wallet_core::PrivateKey const &private_key,
                       ::rust::String id, ::rust::String denom_id,
                       ::rust::String name, ::rust::String uri,
                       ::rust::String data) {
    ::rust::ManuallyDrop<::org::defi_wallet_core::CosmosSDKTxInfoRaw> tx_info$(
        ::std::move(tx_info));
    ::rust::MaybeUninit<::rust::Vec<::std::uint8_t>> return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$get_nft_edit_signed_tx(
            &tx_info$.value, private_key, &id, &denom_id, &name, &uri, &data,
            &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

/// creates the signed transaction
/// for `MsgTransferNft` from the Chainmain nft module
::rust::Vec<::std::uint8_t> get_nft_transfer_signed_tx(
    ::org::defi_wallet_core::CosmosSDKTxInfoRaw tx_info,
    ::org::defi_wallet_core::PrivateKey const &private_key, ::rust::String id,
    ::rust::String denom_id, ::rust::String recipient) {
    ::rust::ManuallyDrop<::org::defi_wallet_core::CosmosSDKTxInfoRaw> tx_info$(
        ::std::move(tx_info));
    ::rust::MaybeUninit<::rust::Vec<::std::uint8_t>> return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$get_nft_transfer_signed_tx(
            &tx_info$.value, private_key, &id, &denom_id, &recipient,
            &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

/// creates the signed transaction
/// for `MsgBurnNft` from the Chainmain nft module
::rust::Vec<::std::uint8_t>
get_nft_burn_signed_tx(::org::defi_wallet_core::CosmosSDKTxInfoRaw tx_info,
                       ::org::defi_wallet_core::PrivateKey const &private_key,
                       ::rust::String id, ::rust::String denom_id) {
    ::rust::ManuallyDrop<::org::defi_wallet_core::CosmosSDKTxInfoRaw> tx_info$(
        ::std::move(tx_info));
    ::rust::MaybeUninit<::rust::Vec<::std::uint8_t>> return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$get_nft_burn_signed_tx(
            &tx_info$.value, private_key, &id, &denom_id, &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

::std::size_t GrpcClient::layout::size() noexcept {
    return org$defi_wallet_core$cxxbridge1$GrpcClient$operator$sizeof();
}

::std::size_t GrpcClient::layout::align() noexcept {
    return org$defi_wallet_core$cxxbridge1$GrpcClient$operator$alignof();
}

/// Create a new grpc client
///
::rust::Box<::org::defi_wallet_core::GrpcClient>
new_grpc_client(::rust::String grpc_url) {
    ::rust::MaybeUninit<::rust::Box<::org::defi_wallet_core::GrpcClient>>
        return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$new_grpc_client(&grpc_url,
                                                        &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

::std::uint64_t GrpcClient::supply(::rust::String denom_id,
                                   ::rust::String owner) const {
    ::rust::MaybeUninit<::std::uint64_t> return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$GrpcClient$supply(
            *this, &denom_id, &owner, &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

::org::defi_wallet_core::Owner
GrpcClient::owner(::rust::String denom_id, ::rust::String owner,
                  ::org::defi_wallet_core::Pagination const &pagination) const {
    ::rust::MaybeUninit<::org::defi_wallet_core::Owner> return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$GrpcClient$owner(
            *this, &denom_id, &owner, pagination, &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

::org::defi_wallet_core::Collection GrpcClient::collection(
    ::rust::String denom_id,
    ::org::defi_wallet_core::Pagination const &pagination) const {
    ::rust::MaybeUninit<::org::defi_wallet_core::Collection> return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$GrpcClient$collection(
            *this, &denom_id, pagination, &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

::org::defi_wallet_core::Denom
GrpcClient::denom(::rust::String denom_id) const {
    ::rust::MaybeUninit<::org::defi_wallet_core::Denom> return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$GrpcClient$denom(*this, &denom_id,
                                                         &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

::org::defi_wallet_core::Denom
GrpcClient::denom_by_name(::rust::String denom_name) const {
    ::rust::MaybeUninit<::org::defi_wallet_core::Denom> return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$GrpcClient$denom_by_name(
            *this, &denom_name, &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

::rust::Vec<::org::defi_wallet_core::Denom> GrpcClient::denoms(
    ::org::defi_wallet_core::Pagination const &pagination) const {
    ::rust::MaybeUninit<::rust::Vec<::org::defi_wallet_core::Denom>> return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$GrpcClient$denoms(*this, pagination,
                                                          &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

::org::defi_wallet_core::BaseNft
GrpcClient::nft(::rust::String denom_id, ::rust::String token_id) const {
    ::rust::MaybeUninit<::org::defi_wallet_core::BaseNft> return$;
    ::rust::repr::PtrLen error$ =
        org$defi_wallet_core$cxxbridge1$GrpcClient$nft(
            *this, &denom_id, &token_id, &return$.value);
    if (error$.ptr) {
        throw ::rust::impl<::rust::Error>::error(error$);
    }
    return ::std::move(return$.value);
}

::rust::String Owner::to_string() const noexcept {
    ::rust::MaybeUninit<::rust::String> return$;
    org$defi_wallet_core$cxxbridge1$Owner$to_string(*this, &return$.value);
    return ::std::move(return$.value);
}

::rust::String Collection::to_string() const noexcept {
    ::rust::MaybeUninit<::rust::String> return$;
    org$defi_wallet_core$cxxbridge1$Collection$to_string(*this, &return$.value);
    return ::std::move(return$.value);
}

::rust::String Denom::to_string() const noexcept {
    ::rust::MaybeUninit<::rust::String> return$;
    org$defi_wallet_core$cxxbridge1$Denom$to_string(*this, &return$.value);
    return ::std::move(return$.value);
}

::rust::String BaseNft::to_string() const noexcept {
    ::rust::MaybeUninit<::rust::String> return$;
    org$defi_wallet_core$cxxbridge1$BaseNft$to_string(*this, &return$.value);
    return ::std::move(return$.value);
}
} // namespace defi_wallet_core
} // namespace org

extern "C" {
void cxxbridge1$rust_vec$org$defi_wallet_core$IdCollection$new(
    ::rust::Vec<::org::defi_wallet_core::IdCollection> const *ptr) noexcept;
void cxxbridge1$rust_vec$org$defi_wallet_core$IdCollection$drop(
    ::rust::Vec<::org::defi_wallet_core::IdCollection> *ptr) noexcept;
::std::size_t cxxbridge1$rust_vec$org$defi_wallet_core$IdCollection$len(
    ::rust::Vec<::org::defi_wallet_core::IdCollection> const *ptr) noexcept;
::std::size_t cxxbridge1$rust_vec$org$defi_wallet_core$IdCollection$capacity(
    ::rust::Vec<::org::defi_wallet_core::IdCollection> const *ptr) noexcept;
::org::defi_wallet_core::IdCollection const *
cxxbridge1$rust_vec$org$defi_wallet_core$IdCollection$data(
    ::rust::Vec<::org::defi_wallet_core::IdCollection> const *ptr) noexcept;
void cxxbridge1$rust_vec$org$defi_wallet_core$IdCollection$reserve_total(
    ::rust::Vec<::org::defi_wallet_core::IdCollection> *ptr,
    ::std::size_t new_cap) noexcept;
void cxxbridge1$rust_vec$org$defi_wallet_core$IdCollection$set_len(
    ::rust::Vec<::org::defi_wallet_core::IdCollection> *ptr,
    ::std::size_t len) noexcept;
void cxxbridge1$rust_vec$org$defi_wallet_core$IdCollection$truncate(
    ::rust::Vec<::org::defi_wallet_core::IdCollection> *ptr,
    ::std::size_t len) noexcept;

void cxxbridge1$rust_vec$org$defi_wallet_core$BaseNft$new(
    ::rust::Vec<::org::defi_wallet_core::BaseNft> const *ptr) noexcept;
void cxxbridge1$rust_vec$org$defi_wallet_core$BaseNft$drop(
    ::rust::Vec<::org::defi_wallet_core::BaseNft> *ptr) noexcept;
::std::size_t cxxbridge1$rust_vec$org$defi_wallet_core$BaseNft$len(
    ::rust::Vec<::org::defi_wallet_core::BaseNft> const *ptr) noexcept;
::std::size_t cxxbridge1$rust_vec$org$defi_wallet_core$BaseNft$capacity(
    ::rust::Vec<::org::defi_wallet_core::BaseNft> const *ptr) noexcept;
::org::defi_wallet_core::BaseNft const *
cxxbridge1$rust_vec$org$defi_wallet_core$BaseNft$data(
    ::rust::Vec<::org::defi_wallet_core::BaseNft> const *ptr) noexcept;
void cxxbridge1$rust_vec$org$defi_wallet_core$BaseNft$reserve_total(
    ::rust::Vec<::org::defi_wallet_core::BaseNft> *ptr,
    ::std::size_t new_cap) noexcept;
void cxxbridge1$rust_vec$org$defi_wallet_core$BaseNft$set_len(
    ::rust::Vec<::org::defi_wallet_core::BaseNft> *ptr,
    ::std::size_t len) noexcept;
void cxxbridge1$rust_vec$org$defi_wallet_core$BaseNft$truncate(
    ::rust::Vec<::org::defi_wallet_core::BaseNft> *ptr,
    ::std::size_t len) noexcept;

::org::defi_wallet_core::GrpcClient *
cxxbridge1$box$org$defi_wallet_core$GrpcClient$alloc() noexcept;
void cxxbridge1$box$org$defi_wallet_core$GrpcClient$dealloc(
    ::org::defi_wallet_core::GrpcClient *) noexcept;
void cxxbridge1$box$org$defi_wallet_core$GrpcClient$drop(
    ::rust::Box<::org::defi_wallet_core::GrpcClient> *ptr) noexcept;

void cxxbridge1$rust_vec$org$defi_wallet_core$Denom$new(
    ::rust::Vec<::org::defi_wallet_core::Denom> const *ptr) noexcept;
void cxxbridge1$rust_vec$org$defi_wallet_core$Denom$drop(
    ::rust::Vec<::org::defi_wallet_core::Denom> *ptr) noexcept;
::std::size_t cxxbridge1$rust_vec$org$defi_wallet_core$Denom$len(
    ::rust::Vec<::org::defi_wallet_core::Denom> const *ptr) noexcept;
::std::size_t cxxbridge1$rust_vec$org$defi_wallet_core$Denom$capacity(
    ::rust::Vec<::org::defi_wallet_core::Denom> const *ptr) noexcept;
::org::defi_wallet_core::Denom const *
cxxbridge1$rust_vec$org$defi_wallet_core$Denom$data(
    ::rust::Vec<::org::defi_wallet_core::Denom> const *ptr) noexcept;
void cxxbridge1$rust_vec$org$defi_wallet_core$Denom$reserve_total(
    ::rust::Vec<::org::defi_wallet_core::Denom> *ptr,
    ::std::size_t new_cap) noexcept;
void cxxbridge1$rust_vec$org$defi_wallet_core$Denom$set_len(
    ::rust::Vec<::org::defi_wallet_core::Denom> *ptr,
    ::std::size_t len) noexcept;
void cxxbridge1$rust_vec$org$defi_wallet_core$Denom$truncate(
    ::rust::Vec<::org::defi_wallet_core::Denom> *ptr,
    ::std::size_t len) noexcept;
} // extern "C"

namespace rust {
inline namespace cxxbridge1 {
template <> Vec<::org::defi_wallet_core::IdCollection>::Vec() noexcept {
    cxxbridge1$rust_vec$org$defi_wallet_core$IdCollection$new(this);
}
template <> void Vec<::org::defi_wallet_core::IdCollection>::drop() noexcept {
    return cxxbridge1$rust_vec$org$defi_wallet_core$IdCollection$drop(this);
}
template <>
::std::size_t
Vec<::org::defi_wallet_core::IdCollection>::size() const noexcept {
    return cxxbridge1$rust_vec$org$defi_wallet_core$IdCollection$len(this);
}
template <>
::std::size_t
Vec<::org::defi_wallet_core::IdCollection>::capacity() const noexcept {
    return cxxbridge1$rust_vec$org$defi_wallet_core$IdCollection$capacity(this);
}
template <>
::org::defi_wallet_core::IdCollection const *
Vec<::org::defi_wallet_core::IdCollection>::data() const noexcept {
    return cxxbridge1$rust_vec$org$defi_wallet_core$IdCollection$data(this);
}
template <>
void Vec<::org::defi_wallet_core::IdCollection>::reserve_total(
    ::std::size_t new_cap) noexcept {
    return cxxbridge1$rust_vec$org$defi_wallet_core$IdCollection$reserve_total(
        this, new_cap);
}
template <>
void Vec<::org::defi_wallet_core::IdCollection>::set_len(
    ::std::size_t len) noexcept {
    return cxxbridge1$rust_vec$org$defi_wallet_core$IdCollection$set_len(this,
                                                                         len);
}
template <>
void Vec<::org::defi_wallet_core::IdCollection>::truncate(::std::size_t len) {
    return cxxbridge1$rust_vec$org$defi_wallet_core$IdCollection$truncate(this,
                                                                          len);
}
template <> Vec<::org::defi_wallet_core::BaseNft>::Vec() noexcept {
    cxxbridge1$rust_vec$org$defi_wallet_core$BaseNft$new(this);
}
template <> void Vec<::org::defi_wallet_core::BaseNft>::drop() noexcept {
    return cxxbridge1$rust_vec$org$defi_wallet_core$BaseNft$drop(this);
}
template <>
::std::size_t Vec<::org::defi_wallet_core::BaseNft>::size() const noexcept {
    return cxxbridge1$rust_vec$org$defi_wallet_core$BaseNft$len(this);
}
template <>
::std::size_t Vec<::org::defi_wallet_core::BaseNft>::capacity() const noexcept {
    return cxxbridge1$rust_vec$org$defi_wallet_core$BaseNft$capacity(this);
}
template <>
::org::defi_wallet_core::BaseNft const *
Vec<::org::defi_wallet_core::BaseNft>::data() const noexcept {
    return cxxbridge1$rust_vec$org$defi_wallet_core$BaseNft$data(this);
}
template <>
void Vec<::org::defi_wallet_core::BaseNft>::reserve_total(
    ::std::size_t new_cap) noexcept {
    return cxxbridge1$rust_vec$org$defi_wallet_core$BaseNft$reserve_total(
        this, new_cap);
}
template <>
void Vec<::org::defi_wallet_core::BaseNft>::set_len(
    ::std::size_t len) noexcept {
    return cxxbridge1$rust_vec$org$defi_wallet_core$BaseNft$set_len(this, len);
}
template <>
void Vec<::org::defi_wallet_core::BaseNft>::truncate(::std::size_t len) {
    return cxxbridge1$rust_vec$org$defi_wallet_core$BaseNft$truncate(this, len);
}
template <>
::org::defi_wallet_core::GrpcClient *
Box<::org::defi_wallet_core::GrpcClient>::allocation::alloc() noexcept {
    return cxxbridge1$box$org$defi_wallet_core$GrpcClient$alloc();
}
template <>
void Box<::org::defi_wallet_core::GrpcClient>::allocation::dealloc(
    ::org::defi_wallet_core::GrpcClient *ptr) noexcept {
    cxxbridge1$box$org$defi_wallet_core$GrpcClient$dealloc(ptr);
}
template <> void Box<::org::defi_wallet_core::GrpcClient>::drop() noexcept {
    cxxbridge1$box$org$defi_wallet_core$GrpcClient$drop(this);
}
template <> Vec<::org::defi_wallet_core::Denom>::Vec() noexcept {
    cxxbridge1$rust_vec$org$defi_wallet_core$Denom$new(this);
}
template <> void Vec<::org::defi_wallet_core::Denom>::drop() noexcept {
    return cxxbridge1$rust_vec$org$defi_wallet_core$Denom$drop(this);
}
template <>
::std::size_t Vec<::org::defi_wallet_core::Denom>::size() const noexcept {
    return cxxbridge1$rust_vec$org$defi_wallet_core$Denom$len(this);
}
template <>
::std::size_t Vec<::org::defi_wallet_core::Denom>::capacity() const noexcept {
    return cxxbridge1$rust_vec$org$defi_wallet_core$Denom$capacity(this);
}
template <>
::org::defi_wallet_core::Denom const *
Vec<::org::defi_wallet_core::Denom>::data() const noexcept {
    return cxxbridge1$rust_vec$org$defi_wallet_core$Denom$data(this);
}
template <>
void Vec<::org::defi_wallet_core::Denom>::reserve_total(
    ::std::size_t new_cap) noexcept {
    return cxxbridge1$rust_vec$org$defi_wallet_core$Denom$reserve_total(
        this, new_cap);
}
template <>
void Vec<::org::defi_wallet_core::Denom>::set_len(::std::size_t len) noexcept {
    return cxxbridge1$rust_vec$org$defi_wallet_core$Denom$set_len(this, len);
}
template <>
void Vec<::org::defi_wallet_core::Denom>::truncate(::std::size_t len) {
    return cxxbridge1$rust_vec$org$defi_wallet_core$Denom$truncate(this, len);
}
} // namespace cxxbridge1
} // namespace rust
