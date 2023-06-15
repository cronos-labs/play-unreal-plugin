#pragma once
#include "uint.rs.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
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

#ifndef CXXBRIDGE1_RUST_STR
#define CXXBRIDGE1_RUST_STR
class Str final {
  public:
    Str() noexcept;
    Str(const String &) noexcept;
    Str(const std::string &);
    Str(const char *);
    Str(const char *, std::size_t);

    Str &operator=(const Str &) &noexcept = default;

    explicit operator std::string() const;

    const char *data() const noexcept;
    std::size_t size() const noexcept;
    std::size_t length() const noexcept;
    bool empty() const noexcept;

    Str(const Str &) noexcept = default;
    ~Str() noexcept = default;

    using iterator = const char *;
    using const_iterator = const char *;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    bool operator==(const Str &) const noexcept;
    bool operator!=(const Str &) const noexcept;
    bool operator<(const Str &) const noexcept;
    bool operator<=(const Str &) const noexcept;
    bool operator>(const Str &) const noexcept;
    bool operator>=(const Str &) const noexcept;

    void swap(Str &) noexcept;

  private:
    class uninit;
    Str(uninit) noexcept;
    friend impl<Str>;

    std::array<std::uintptr_t, 2> repr;
};
#endif // CXXBRIDGE1_RUST_STR

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
} // namespace cxxbridge1
} // namespace rust

namespace org {
namespace defi_wallet_core {
enum class CoinType : ::std::uint8_t;
enum class MnemonicWordCount : ::std::uint8_t;
enum class EthAmount : ::std::uint8_t;
struct EthTxInfoRaw;
struct CosmosSDKTxInfoRaw;
struct CosmosAccountInfoRaw;
struct CosmosTransactionReceiptRaw;
struct CronosTransactionReceiptRaw;
struct PrivateKey;
struct CosmosSDKMsgRaw;
struct Wallet;
struct CppLoginInfo;
} // namespace defi_wallet_core
} // namespace org

namespace org {
namespace defi_wallet_core {
#ifndef CXXBRIDGE1_ENUM_org$defi_wallet_core$CoinType
#define CXXBRIDGE1_ENUM_org$defi_wallet_core$CoinType
enum class CoinType : ::std::uint8_t {
    /// Crypto.org Chain mainnet
    ///
    CryptoOrgMainnet = 0,
    /// Crypto.org Chain testnet
    ///
    CryptoOrgTestnet = 1,
    /// Cronos mainnet beta
    ///
    CronosMainnet = 2,
    /// Cosmos Hub mainnet
    ///
    CosmosHub = 3,
    /// Ethereum
    ///
    Ethereum = 4,
};
#endif // CXXBRIDGE1_ENUM_org$defi_wallet_core$CoinType

#ifndef CXXBRIDGE1_ENUM_org$defi_wallet_core$MnemonicWordCount
#define CXXBRIDGE1_ENUM_org$defi_wallet_core$MnemonicWordCount
enum class MnemonicWordCount : ::std::uint8_t {
    /// Word 12
    ///
    Twelve = 0,
    /// Word 18
    ///
    Eighteen = 1,
    /// Word 24
    ///
    TwentyFour = 2,
};
#endif // CXXBRIDGE1_ENUM_org$defi_wallet_core$MnemonicWordCount

#ifndef CXXBRIDGE1_ENUM_org$defi_wallet_core$EthAmount
#define CXXBRIDGE1_ENUM_org$defi_wallet_core$EthAmount
enum class EthAmount : ::std::uint8_t {
    /// 10^-18 ETH
    ///
    WeiDecimal = 0,
    /// 10^-9 ETH
    ///
    GweiDecimal = 1,
    EthDecimal = 2,
};
#endif // CXXBRIDGE1_ENUM_org$defi_wallet_core$EthAmount

#ifndef CXXBRIDGE1_STRUCT_org$defi_wallet_core$EthTxInfoRaw
#define CXXBRIDGE1_STRUCT_org$defi_wallet_core$EthTxInfoRaw
struct EthTxInfoRaw final {
    ::rust::String to_address;
    ::rust::String amount;
    ::org::defi_wallet_core::EthAmount amount_unit;
    ::rust::String nonce;
    ::rust::String gas_limit;
    ::rust::String gas_price;
    ::org::defi_wallet_core::EthAmount gas_price_unit;
    ::rust::Vec<::std::uint8_t> data;

    using IsRelocatable = ::std::true_type;
};
#endif // CXXBRIDGE1_STRUCT_org$defi_wallet_core$EthTxInfoRaw

#ifndef CXXBRIDGE1_STRUCT_org$defi_wallet_core$CosmosSDKTxInfoRaw
#define CXXBRIDGE1_STRUCT_org$defi_wallet_core$CosmosSDKTxInfoRaw
struct CosmosSDKTxInfoRaw final {
    /// global account number of the sender
    ///
    ::std::uint64_t account_number;
    /// equivalent of "account nonce"
    ///
    ::std::uint64_t sequence_number;
    /// the maximum gas limit
    ///
    ::std::uint64_t gas_limit;
    /// the amount fee to be paid (gas_limit * gas_price)
    ///
    ::std::uint64_t fee_amount;
    /// the fee's denomination
    ///
    ::rust::String fee_denom;
    /// transaction timeout
    ///
    ::std::uint32_t timeout_height;
    /// optional memo
    ///
    ::rust::String memo_note;
    /// the network chain id
    ///
    ::rust::String chain_id;
    /// bech32 human readable prefix
    ///
    ::rust::String bech32hrp;
    /// the coin type to use
    ///
    ::std::uint32_t coin_type;

    using IsRelocatable = ::std::true_type;
};
#endif // CXXBRIDGE1_STRUCT_org$defi_wallet_core$CosmosSDKTxInfoRaw

#ifndef CXXBRIDGE1_STRUCT_org$defi_wallet_core$CosmosAccountInfoRaw
#define CXXBRIDGE1_STRUCT_org$defi_wallet_core$CosmosAccountInfoRaw
struct CosmosAccountInfoRaw final {
    ::std::uint64_t account_number;
    ::std::uint64_t sequence_number;

    using IsRelocatable = ::std::true_type;
};
#endif // CXXBRIDGE1_STRUCT_org$defi_wallet_core$CosmosAccountInfoRaw

#ifndef CXXBRIDGE1_STRUCT_org$defi_wallet_core$CosmosTransactionReceiptRaw
#define CXXBRIDGE1_STRUCT_org$defi_wallet_core$CosmosTransactionReceiptRaw
struct CosmosTransactionReceiptRaw final {
    /// tendermint transaction hash in hexadecimal
    ///
    ::rust::String tx_hash_hex;
    /// error code (0 if success)
    ///
    ::std::uint32_t code;
    /// possible error log
    ///
    ::rust::String log;

    using IsRelocatable = ::std::true_type;
};
#endif // CXXBRIDGE1_STRUCT_org$defi_wallet_core$CosmosTransactionReceiptRaw

#ifndef CXXBRIDGE1_STRUCT_org$defi_wallet_core$CronosTransactionReceiptRaw
#define CXXBRIDGE1_STRUCT_org$defi_wallet_core$CronosTransactionReceiptRaw
struct CronosTransactionReceiptRaw final {
    ::rust::Vec<::std::uint8_t> transaction_hash;
    ::rust::String transaction_index;
    ::rust::Vec<::std::uint8_t> block_hash;
    ::rust::String block_number;
    ::rust::String cumulative_gas_used;
    ::rust::String gas_used;
    ::rust::String contract_address;
    ::rust::Vec<::rust::String> logs;
    /// Status: either 1 (success) or 0 (failure)
    ///
    ::rust::String status;
    ::rust::Vec<::std::uint8_t> root;
    ::rust::Vec<::std::uint8_t> logs_bloom;
    ::rust::String transaction_type;
    ::rust::String effective_gas_price;

    using IsRelocatable = ::std::true_type;
};
#endif // CXXBRIDGE1_STRUCT_org$defi_wallet_core$CronosTransactionReceiptRaw

#ifndef CXXBRIDGE1_STRUCT_org$defi_wallet_core$PrivateKey
#define CXXBRIDGE1_STRUCT_org$defi_wallet_core$PrivateKey
struct PrivateKey final : public ::rust::Opaque {
    ~PrivateKey() = delete;

  private:
    friend ::rust::layout;
    struct layout {
        static ::std::size_t size() noexcept;
        static ::std::size_t align() noexcept;
    };
};
#endif // CXXBRIDGE1_STRUCT_org$defi_wallet_core$PrivateKey

#ifndef CXXBRIDGE1_STRUCT_org$defi_wallet_core$CosmosSDKMsgRaw
#define CXXBRIDGE1_STRUCT_org$defi_wallet_core$CosmosSDKMsgRaw
struct CosmosSDKMsgRaw final : public ::rust::Opaque {
    ~CosmosSDKMsgRaw() = delete;

  private:
    friend ::rust::layout;
    struct layout {
        static ::std::size_t size() noexcept;
        static ::std::size_t align() noexcept;
    };
};
#endif // CXXBRIDGE1_STRUCT_org$defi_wallet_core$CosmosSDKMsgRaw

#ifndef CXXBRIDGE1_STRUCT_org$defi_wallet_core$Wallet
#define CXXBRIDGE1_STRUCT_org$defi_wallet_core$Wallet
struct Wallet final : public ::rust::Opaque {
    /// get backup mnemonic phrase
    ///
    ::rust::String get_backup_mnemonic_phrase() const;

    /// returns the default address of the wallet
    ///
    ::rust::String
    get_default_address(::org::defi_wallet_core::CoinType coin) const;

    /// returns the address from index in wallet
    ///
    ::rust::String get_address(::org::defi_wallet_core::CoinType coin,
                               ::std::uint32_t index) const;

    /// returns the ethereum address from index in wallet
    ///
    ::rust::String get_eth_address(::std::uint32_t index) const;

    /// return the secret key for a given derivation path
    ///
    ::rust::Box<::org::defi_wallet_core::PrivateKey>
    get_key(::rust::String derivation_path) const;

    ~Wallet() = delete;

  private:
    friend ::rust::layout;
    struct layout {
        static ::std::size_t size() noexcept;
        static ::std::size_t align() noexcept;
    };
};
#endif // CXXBRIDGE1_STRUCT_org$defi_wallet_core$Wallet

#ifndef CXXBRIDGE1_STRUCT_org$defi_wallet_core$CppLoginInfo
#define CXXBRIDGE1_STRUCT_org$defi_wallet_core$CppLoginInfo
struct CppLoginInfo final : public ::rust::Opaque {
    /// Sign Login Info
    /// constructs the plaintext message and signs it according to EIP-191
    /// (as per EIP-4361). The returned vector is a serialized recoverable
    /// signature (as used in Ethereum).
    ::rust::Vec<::std::uint8_t> sign_logininfo(
        ::org::defi_wallet_core::PrivateKey const &private_key) const;

    /// Verify Login Info
    /// It verified the signature matches + also verifies the content of the
    /// message:
    /// - address in the message matches the address recovered from the
    /// signature
    /// - the time is valid
    /// ...
    /// NOTE: the server may still need to do extra verifications according to
    /// its needs (e.g. verify chain-id, nonce, uri + possibly fetch additional
    /// data associated with the given Ethereum address, such as
    /// ERC-20/ERC-721/ERC-1155 asset ownership)
    void verify_logininfo(::rust::Slice<::std::uint8_t const> signature) const;

    ~CppLoginInfo() = delete;

  private:
    friend ::rust::layout;
    struct layout {
        static ::std::size_t size() noexcept;
        static ::std::size_t align() noexcept;
    };
};
#endif // CXXBRIDGE1_STRUCT_org$defi_wallet_core$CppLoginInfo

/// query account details from cosmos address
///
::rust::String query_account_details(::rust::String api_url,
                                     ::rust::String address);

/// query account details info from cosmos address
///
::org::defi_wallet_core::CosmosAccountInfoRaw
query_account_details_info(::rust::String api_url, ::rust::String address);

/// broadcast the cosmos transaction
///
::org::defi_wallet_core::CosmosTransactionReceiptRaw
broadcast_tx(::rust::String tendermint_rpc_url,
             ::rust::Vec<::std::uint8_t> raw_signed_tx);

/// query account balance from cosmos address and denom name
///
::rust::String query_account_balance(::rust::String grpc_url,
                                     ::rust::String address,
                                     ::rust::String denom);

/// creates the signed transaction for cosmos
///
::rust::Vec<::std::uint8_t>
get_msg_signed_tx(::org::defi_wallet_core::CosmosSDKTxInfoRaw tx_info,
                  ::org::defi_wallet_core::PrivateKey const &private_key,
                  ::org::defi_wallet_core::CosmosSDKMsgRaw const &msg);

/// creates the transaction signing payload (`SignDoc`)
/// for `MsgSend` from the Cosmos SDK bank module
::rust::Vec<::std::uint8_t> get_single_bank_send_signdoc(
    ::org::defi_wallet_core::CosmosSDKTxInfoRaw tx_info,
    ::rust::Vec<::std::uint8_t> sender_pubkey, ::rust::String recipient_address,
    ::std::uint64_t amount, ::rust::String denom);

/// creates the signed transaction
/// for `MsgSend` from the Cosmos SDK bank module
::rust::Vec<::std::uint8_t> get_single_bank_send_signed_tx(
    ::org::defi_wallet_core::CosmosSDKTxInfoRaw tx_info,
    ::org::defi_wallet_core::PrivateKey const &private_key,
    ::rust::String recipient_address, ::std::uint64_t amount,
    ::rust::String denom);

/// generates the HD wallet with a BIP39 backup phrase (English words) and
/// password
///
::rust::Box<::org::defi_wallet_core::Wallet>
new_wallet(::rust::String password,
           ::org::defi_wallet_core::MnemonicWordCount word_count);

/// generate mnemonics
///
::rust::String
generate_mnemonics(::rust::String password,
                   ::org::defi_wallet_core::MnemonicWordCount word_count);

/// recovers/imports HD wallet from a BIP39 backup phrase (English words) and
/// password and save to secure storage
::rust::Box<::org::defi_wallet_core::Wallet>
restore_wallet_save_to_securestorage(::rust::String mnemonic,
                                     ::rust::String password,
                                     ::rust::String servicename,
                                     ::rust::String username);

/// recovers/imports HD wallet from a BIP39 backup phrase (English words) and
/// password from secure storage
::rust::Box<::org::defi_wallet_core::Wallet>
restore_wallet_load_from_securestorage(::rust::String servicename,
                                       ::rust::String username);

/// recovers/imports HD wallet from a BIP39 backup phrase (English words) and
/// password
///
::rust::Box<::org::defi_wallet_core::Wallet>
restore_wallet(::rust::String mnemonic, ::rust::String password);

/// generates a random private key
///
::rust::Box<::org::defi_wallet_core::PrivateKey> new_privatekey() noexcept;

/// constructs private key from bytes
///
::rust::Box<::org::defi_wallet_core::PrivateKey>
new_privatekey_from_bytes(::rust::Vec<::std::uint8_t> bytes);

/// constructs private key from hex string
///
::rust::Box<::org::defi_wallet_core::PrivateKey>
new_privatekey_from_hex(::rust::String hex);

/// creates the signed transaction
/// for `MsgDelegate` from the Cosmos SDK staking module
::rust::Vec<::std::uint8_t> get_staking_delegate_signed_tx(
    ::org::defi_wallet_core::CosmosSDKTxInfoRaw tx_info,
    ::org::defi_wallet_core::PrivateKey const &private_key,
    ::rust::String validator_address, ::std::uint64_t amount,
    ::rust::String denom, bool with_reward_withdrawal);

/// creates the signed transaction
/// for `MsgBeginRedelegate` from the Cosmos SDK staking module
::rust::Vec<::std::uint8_t> get_staking_redelegate_signed_tx(
    ::org::defi_wallet_core::CosmosSDKTxInfoRaw tx_info,
    ::org::defi_wallet_core::PrivateKey const &private_key,
    ::rust::String validator_src_address, ::rust::String validator_dst_address,
    ::std::uint64_t amount, ::rust::String denom, bool with_reward_withdrawal);

/// creates the signed transaction
/// for `MsgUndelegate` from the Cosmos SDK staking module
::rust::Vec<::std::uint8_t> get_staking_unbond_signed_tx(
    ::org::defi_wallet_core::CosmosSDKTxInfoRaw tx_info,
    ::org::defi_wallet_core::PrivateKey const &private_key,
    ::rust::String validator_address, ::std::uint64_t amount,
    ::rust::String denom, bool with_reward_withdrawal);

/// creates the signed transaction
/// for `MsgSetWithdrawAddress` from the Cosmos SDK distributon module
::rust::Vec<::std::uint8_t> get_distribution_set_withdraw_address_signed_tx(
    ::org::defi_wallet_core::CosmosSDKTxInfoRaw tx_info,
    ::org::defi_wallet_core::PrivateKey const &private_key,
    ::rust::String withdraw_address);

/// creates the signed transaction
/// for `MsgWithdrawDelegatorReward` from the Cosmos SDK distributon module
::rust::Vec<::std::uint8_t> get_distribution_withdraw_reward_signed_tx(
    ::org::defi_wallet_core::CosmosSDKTxInfoRaw tx_info,
    ::org::defi_wallet_core::PrivateKey const &private_key,
    ::rust::String validator_address);

/// creates the signed transaction
/// for `MsgTransfer` from the Cosmos SDK ibc module
::rust::Vec<::std::uint8_t> get_ibc_transfer_signed_tx(
    ::org::defi_wallet_core::CosmosSDKTxInfoRaw tx_info,
    ::org::defi_wallet_core::PrivateKey const &private_key,
    ::rust::String receiver, ::rust::String source_port,
    ::rust::String source_channel, ::rust::String denom, ::std::uint64_t token,
    ::std::uint64_t revision_height, ::std::uint64_t revision_number,
    ::std::uint64_t timeout_timestamp);

/// Create Login Info by `msg`
/// all information from the EIP-4361 plaintext message:
/// https://eips.ethereum.org/EIPS/eip-4361
::rust::Box<::org::defi_wallet_core::CppLoginInfo>
new_logininfo(::rust::String msg);

/// create cronos tx info to sign
///
::org::defi_wallet_core::EthTxInfoRaw new_eth_tx_info() noexcept;

/// sign cronos tx with private key
///
::rust::Vec<::std::uint8_t>
build_eth_signed_tx(::org::defi_wallet_core::EthTxInfoRaw tx_info,
                    ::rust::Str network,
                    ::org::defi_wallet_core::PrivateKey const &secret_key);

/// sign cronos tx with private key in custom network
///
::rust::Vec<::std::uint8_t>
build_eth_signed_tx(::org::defi_wallet_core::EthTxInfoRaw tx_info,
                    ::std::uint64_t chain_id, bool legacy,
                    ::org::defi_wallet_core::PrivateKey const &secret_key);

/// given the account address, it returns the amount of native token it owns
///
::org::defi_wallet_core::U256 get_eth_balance(::rust::Str address,
                                              ::rust::Str api_url);

/// Returns the corresponding account's nonce / number of transactions
/// sent from it.
::rust::String get_eth_nonce(::rust::Str address, ::rust::Str api_url);

::rust::String get_block_number_blocking(::rust::String api_url);

::rust::String
get_eth_transaction_receipt_blocking(::rust::Vec<::std::uint8_t> tx_hash,
                                     ::rust::String api_url);

::rust::String get_eth_transaction_receipt_blocking(::rust::String tx_hash,
                                                    ::rust::String api_url);

::rust::String
wait_for_transaction_receipt_blocking(::rust::Vec<::std::uint8_t> tx_hash,
                                      ::rust::String api_url);

::rust::String wait_for_transaction_receipt_blocking(::rust::String tx_hash,
                                                     ::rust::String api_url);

/// broadcast signed cronos tx
///
::org::defi_wallet_core::CronosTransactionReceiptRaw
broadcast_eth_signed_raw_tx(::rust::Vec<::std::uint8_t> raw_tx,
                            ::rust::Str web3api_url,
                            ::std::uint64_t polling_interval_ms);

/// set cronos http-agent name
///
void set_cronos_httpagent(::rust::Str agent);
} // namespace defi_wallet_core
} // namespace org
