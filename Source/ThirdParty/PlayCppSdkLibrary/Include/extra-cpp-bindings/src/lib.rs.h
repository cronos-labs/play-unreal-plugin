#pragma once
#include "../../walletconnectcallback.h"
#include "../../walletconnectcallback.h"
#include "../../pay.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <iterator>
#include <memory>
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

    String &operator=(const String &) & noexcept;
    String &operator=(String &&) & noexcept;

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

    Str &operator=(const Str &) & noexcept = default;

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
    operator=(const copy_assignable_if &) & noexcept = delete;
    copy_assignable_if &operator=(copy_assignable_if &&) & noexcept = default;
};
} // namespace detail

template <typename T>
class Slice final
    : private detail::copy_assignable_if<std::is_const<T>::value> {
  public:
    using value_type = T;

    Slice() noexcept;
    Slice(T *, std::size_t count) noexcept;

    Slice &operator=(const Slice<T> &) & noexcept = default;
    Slice &operator=(Slice<T> &&) & noexcept = default;

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

    Box &operator=(Box &&) & noexcept;

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

template <typename T> Box<T> &Box<T>::operator=(Box &&other) & noexcept {
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

    Vec &operator=(Vec &&) & noexcept;
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

template <typename T> Vec<T> &Vec<T>::operator=(Vec &&other) & noexcept {
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

namespace com {
namespace crypto {
namespace game_sdk {
struct WalletConnectTransactionReceiptRaw;
using WalletConnectCallback = ::com::crypto::game_sdk::WalletConnectCallback;
using WalletConnectSessionInfo =
    ::com::crypto::game_sdk::WalletConnectSessionInfo;
struct WalletEntry;
enum class Platform : ::std::uint8_t;
struct ImageUrl;
struct WalletQrcode;
struct WalletConnectTxCommon;
struct WalletConnectTxEip155;
struct WalletConnectAddress;
struct WalletConnectEnsureSessionResult;
struct CryptoComPaymentResponse;
struct RawTxDetail;
struct RawTokenResult;
struct TokenHolderDetail;
enum class QueryOption : ::std::uint8_t;
struct WalletconnectClient;
using OptionalArguments = ::com::crypto::game_sdk::OptionalArguments;
} // namespace game_sdk
} // namespace crypto
} // namespace com

namespace com {
namespace crypto {
namespace game_sdk {
#ifndef CXXBRIDGE1_STRUCT_com$crypto$game_sdk$WalletConnectTransactionReceiptRaw
#define CXXBRIDGE1_STRUCT_com$crypto$game_sdk$WalletConnectTransactionReceiptRaw
struct WalletConnectTransactionReceiptRaw final {
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
#endif // CXXBRIDGE1_STRUCT_com$crypto$game_sdk$WalletConnectTransactionReceiptRaw

#ifndef CXXBRIDGE1_STRUCT_com$crypto$game_sdk$ImageUrl
#define CXXBRIDGE1_STRUCT_com$crypto$game_sdk$ImageUrl
/// The icon URLs
///
struct ImageUrl final {
    /// small
    ///
    ::rust::String sm;
    /// medium
    ///
    ::rust::String md;
    /// large
    ///
    ::rust::String lg;

    using IsRelocatable = ::std::true_type;
};
#endif // CXXBRIDGE1_STRUCT_com$crypto$game_sdk$ImageUrl

#ifndef CXXBRIDGE1_STRUCT_com$crypto$game_sdk$WalletEntry
#define CXXBRIDGE1_STRUCT_com$crypto$game_sdk$WalletEntry
/// The wallet registry entry
///
struct WalletEntry final {
    /// wallet id
    ///
    ::rust::String id;
    /// its name
    ///
    ::rust::String name;
    /// icon URLs
    ///
    ::com::crypto::game_sdk::ImageUrl image_url;
    /// mobile native link, empty if none
    ///
    ::rust::String mobile_native_link;
    /// mobile universal link, empty if none
    ///
    ::rust::String mobile_universal_link;
    /// desktop native link, empty if none
    ///
    ::rust::String desktop_native_link;
    /// desktop universal link, empty if none
    ///
    ::rust::String desktop_universal_link;

    using IsRelocatable = ::std::true_type;
};
#endif // CXXBRIDGE1_STRUCT_com$crypto$game_sdk$WalletEntry

#ifndef CXXBRIDGE1_ENUM_com$crypto$game_sdk$Platform
#define CXXBRIDGE1_ENUM_com$crypto$game_sdk$Platform
/// The target platform
///
enum class Platform : ::std::uint8_t {
    Mobile = 0,
    Desktop = 1,
};
#endif // CXXBRIDGE1_ENUM_com$crypto$game_sdk$Platform

#ifndef CXXBRIDGE1_STRUCT_com$crypto$game_sdk$WalletQrcode
#define CXXBRIDGE1_STRUCT_com$crypto$game_sdk$WalletQrcode
struct WalletQrcode final {
    ::rust::String qrcode;
    ::rust::Vec<::std::uint8_t> image;
    ::std::uint32_t size;

    using IsRelocatable = ::std::true_type;
};
#endif // CXXBRIDGE1_STRUCT_com$crypto$game_sdk$WalletQrcode

#ifndef CXXBRIDGE1_STRUCT_com$crypto$game_sdk$WalletConnectTxCommon
#define CXXBRIDGE1_STRUCT_com$crypto$game_sdk$WalletConnectTxCommon
struct WalletConnectTxCommon final {
    ::rust::String gas_limit;
    ::rust::String gas_price;
    ::rust::String nonce;
    ::std::uint64_t chainid;
    ::rust::String web3api_url;

    using IsRelocatable = ::std::true_type;
};
#endif // CXXBRIDGE1_STRUCT_com$crypto$game_sdk$WalletConnectTxCommon

#ifndef CXXBRIDGE1_STRUCT_com$crypto$game_sdk$WalletConnectTxEip155
#define CXXBRIDGE1_STRUCT_com$crypto$game_sdk$WalletConnectTxEip155
/// wallet connect cronos(eth) eip155-tx signing info
///
struct WalletConnectTxEip155 final {
    ::rust::String to;
    ::rust::String value;
    ::rust::Vec<::std::uint8_t> data;
    ::com::crypto::game_sdk::WalletConnectTxCommon common;

    using IsRelocatable = ::std::true_type;
};
#endif // CXXBRIDGE1_STRUCT_com$crypto$game_sdk$WalletConnectTxEip155

#ifndef CXXBRIDGE1_STRUCT_com$crypto$game_sdk$WalletConnectAddress
#define CXXBRIDGE1_STRUCT_com$crypto$game_sdk$WalletConnectAddress
/// cronos address info
///
struct WalletConnectAddress final {
    ::std::array<::std::uint8_t, 20> address;

    using IsRelocatable = ::std::true_type;
};
#endif // CXXBRIDGE1_STRUCT_com$crypto$game_sdk$WalletConnectAddress

#ifndef CXXBRIDGE1_STRUCT_com$crypto$game_sdk$WalletConnectEnsureSessionResult
#define CXXBRIDGE1_STRUCT_com$crypto$game_sdk$WalletConnectEnsureSessionResult
/// walletconnect ensure-session result
///
struct WalletConnectEnsureSessionResult final {
    ::rust::Vec<::com::crypto::game_sdk::WalletConnectAddress> addresses;
    ::std::uint64_t chain_id;

    using IsRelocatable = ::std::true_type;
};
#endif // CXXBRIDGE1_STRUCT_com$crypto$game_sdk$WalletConnectEnsureSessionResult

#ifndef CXXBRIDGE1_STRUCT_com$crypto$game_sdk$CryptoComPaymentResponse
#define CXXBRIDGE1_STRUCT_com$crypto$game_sdk$CryptoComPaymentResponse
/// the subset of payment object from https://pay-docs.crypto.com
///
struct CryptoComPaymentResponse final {
    /// uuid of the payment object
    ///
    ::rust::String id;
    /// the base64 payload to be displayed as QR code that
    /// can be scanned by the main app
    ::rust::String main_app_qr_code;
    /// if the on-chain payment is desired, this will
    /// have the cryptocurrency address that can be displayed
    /// as a QR code or put in a tx to be signed via WalletConnect
    ::rust::String onchain_deposit_address;
    /// the amount in base denomination
    /// e.g. for USD, it's cents (1 USD == 100 cents)
    ::rust::String base_amount;
    /// the 3-letter currency code
    ///
    ::rust::String currency;
    /// expiration time in unix timestamp (10 minutes)
    ///
    ::std::uint64_t expiration;
    /// the status of the payment
    ///
    ::rust::String status;

    using IsRelocatable = ::std::true_type;
};
#endif // CXXBRIDGE1_STRUCT_com$crypto$game_sdk$CryptoComPaymentResponse

#ifndef CXXBRIDGE1_STRUCT_com$crypto$game_sdk$RawTxDetail
#define CXXBRIDGE1_STRUCT_com$crypto$game_sdk$RawTxDetail
/// Raw transaction details (extracted from Cronoscan/Etherscan or BlockScout
/// API)
///
struct RawTxDetail final {
    /// Transaction hash
    ///
    ::rust::String hash;
    /// the hexadecimal address of the receiver
    ///
    ::rust::String to_address;
    /// the hexadecimal address of the sender
    ///
    ::rust::String from_address;
    /// the value sent in decimal (in base tokens)
    ///
    ::rust::String value;
    /// block number when it happened
    ///
    ::std::uint64_t block_no;
    /// the time it happened
    ///
    ::rust::String timestamp;
    /// the address of the contract (if no contract, it's an empty string)
    ///
    ::rust::String contract_address;

    bool operator==(RawTxDetail const &) const noexcept;
    bool operator!=(RawTxDetail const &) const noexcept;
    using IsRelocatable = ::std::true_type;
};
#endif // CXXBRIDGE1_STRUCT_com$crypto$game_sdk$RawTxDetail

#ifndef CXXBRIDGE1_STRUCT_com$crypto$game_sdk$RawTokenResult
#define CXXBRIDGE1_STRUCT_com$crypto$game_sdk$RawTokenResult
/// Token ownership result detail from BlockScout API
///
struct RawTokenResult final {
    /// how many tokens are owned by the address
    ///
    ::rust::String balance;
    /// the deployed contract address
    ///
    ::rust::String contract_address;
    /// the number of decimal places
    ///
    ::rust::String decimals;
    /// the token id
    ///
    ::rust::String id;
    /// the human-readable name of the token
    ///
    ::rust::String name;
    /// the ticker for the token
    ///
    ::rust::String symbol;
    /// the token type (ERC-20, ERC-721, ERC-1155)
    ///
    ::rust::String token_type;

    bool operator==(RawTokenResult const &) const noexcept;
    bool operator!=(RawTokenResult const &) const noexcept;
    using IsRelocatable = ::std::true_type;
};
#endif // CXXBRIDGE1_STRUCT_com$crypto$game_sdk$RawTokenResult

#ifndef CXXBRIDGE1_STRUCT_com$crypto$game_sdk$TokenHolderDetail
#define CXXBRIDGE1_STRUCT_com$crypto$game_sdk$TokenHolderDetail
/// Token holder detail from BlockScout API
///
/// tokenid is not supported yet.
struct TokenHolderDetail final {
    /// the holder address
    ///
    ::rust::String address;
    /// balance of the target token
    ///
    ::rust::String value;

    bool operator==(TokenHolderDetail const &) const noexcept;
    bool operator!=(TokenHolderDetail const &) const noexcept;
    using IsRelocatable = ::std::true_type;
};
#endif // CXXBRIDGE1_STRUCT_com$crypto$game_sdk$TokenHolderDetail

#ifndef CXXBRIDGE1_ENUM_com$crypto$game_sdk$QueryOption
#define CXXBRIDGE1_ENUM_com$crypto$game_sdk$QueryOption
enum class QueryOption : ::std::uint8_t {
    ByContract = 0,
    ByAddressAndContract = 1,
    ByAddress = 2,
};
#endif // CXXBRIDGE1_ENUM_com$crypto$game_sdk$QueryOption

#ifndef CXXBRIDGE1_STRUCT_com$crypto$game_sdk$WalletconnectClient
#define CXXBRIDGE1_STRUCT_com$crypto$game_sdk$WalletconnectClient
/// WallnetConnect API
///
struct WalletconnectClient final : public ::rust::Opaque {
    /// setup callback
    ///
    void setup_callback_blocking(
        ::std::unique_ptr<::com::crypto::game_sdk::WalletConnectCallback>
            usercallback);

    /// create or restore a session
    /// once session is created, it will be reused
    ::com::crypto::game_sdk::WalletConnectEnsureSessionResult
    ensure_session_blocking();

    /// get connection string for qrcode
    ///
    ::rust::String get_connection_string();

    /// write session-info to string, which can be written to file
    ///
    ::rust::String save_client();

    /// print qrcode in termal, for debugging
    ///
    ::rust::String print_uri();

    /// sign message
    ///
    ::rust::Vec<::std::uint8_t>
    sign_personal_blocking(::rust::String message,
                           ::std::array<::std::uint8_t, 20> address);

    /// build cronos(eth) eip155 transaction
    /// Supported Wallets: Trust Wallet, Crypto.com Desktop Defi Wallet
    ::rust::Vec<::std::uint8_t> sign_eip155_transaction_blocking(
        ::com::crypto::game_sdk::WalletConnectTxEip155 const &info,
        ::std::array<::std::uint8_t, 20> address);

    /// send cronos(eth) eip155 transaction
    /// Supported Wallets: Trust Wallet, MetaMask and Crypto.com Mobile Defi
    /// Wallet
    ::rust::Vec<::std::uint8_t> send_eip155_transaction_blocking(
        ::com::crypto::game_sdk::WalletConnectTxEip155 const &info,
        ::std::array<::std::uint8_t, 20> address);

    /// eip1559_transaction_request: json string of Eip1559TransactionRequest
    /// return signed transaction bytes
    ::rust::Vec<::std::uint8_t>
    sign_transaction(::rust::String eip1559_transaction_request,
                     ::std::array<::std::uint8_t, 20> address);

    /// eip1559_transaction_request: json string of Eip1559TransactionRequest
    /// return transaction hash bytes
    ::rust::Vec<::std::uint8_t>
    send_transaction(::rust::String eip1559_transaction_request,
                     ::std::array<::std::uint8_t, 20> address);

    /// sign a contract transaction
    /// contract_action is a json string of `ContractAction` type, for example:
    /// for example, transfer Erc20 token
    /// {
    ///     "ContractTransfer": {
    ///         "Erc20Transfer": {
    ///             "contract_address": "0xxxxx",
    ///             "to_address": "0xxxxx",
    ///             "amount": "1000000000000000000"
    ///         }
    ///     }
    /// }
    /// return signed transaction bytes
    ::rust::Vec<::std::uint8_t> sign_contract_transaction(
        ::rust::String contract_action,
        ::com::crypto::game_sdk::WalletConnectTxCommon const &common,
        ::std::array<::std::uint8_t, 20> address);

    /// contract_action is a json string of `ContractAction` type
    /// for example, transfer Erc20 token
    /// {
    ///     "ContractTransfer": {
    ///         "Erc20Transfer": {
    ///             "contract_address": "0xxxxx",
    ///             "to_address": "0xxxxx",
    ///             "amount": "1000000000000000000"
    ///         }
    ///     }
    /// }
    ::rust::Vec<::std::uint8_t> send_contract_transaction(
        ::rust::String contract_action,
        ::com::crypto::game_sdk::WalletConnectTxCommon const &common,
        ::std::array<::std::uint8_t, 20> address);

    ~WalletconnectClient() = delete;

  private:
    friend ::rust::layout;
    struct layout {
        static ::std::size_t size() noexcept;
        static ::std::size_t align() noexcept;
    };
};
#endif // CXXBRIDGE1_STRUCT_com$crypto$game_sdk$WalletconnectClient

/// filter wallets by platform
/// (`registry_local_path` can be empty string if it is not needed to store the
/// `cached` registry result)
::rust::Vec<::com::crypto::game_sdk::WalletEntry>
filter_wallets(bool cached, ::rust::String registry_local_path,
               ::com::crypto::game_sdk::Platform platform);

/// get all possible wallets
/// (`registry_local_path` can be empty string if it is not needed to store the
/// `cached` registry result)
::rust::Vec<::com::crypto::game_sdk::WalletEntry>
get_all_wallets(bool cached, ::rust::String registry_local_path);

/// check wallet by `id` for supported `platform` listing or not
/// Check wallet id at https://explorer.walletconnect.com/
/// (`registry_local_path` can be empty string if it is not needed to store the
/// `cached` registry result)
bool check_wallet(bool cached, ::rust::String registry_local_path,
                  ::rust::String id,
                  ::com::crypto::game_sdk::Platform platform);

/// get a wallet by `id`
/// Check wallet id at https://explorer.walletconnect.com/
/// (`registry_local_path` can be empty string if it is not needed to store the
/// `cached` registry result)
::com::crypto::game_sdk::WalletEntry
get_wallet(bool cached, ::rust::String registry_local_path, ::rust::String id);

::com::crypto::game_sdk::WalletQrcode
generate_qrcode(::rust::String qrcodestring);

/// restore walletconnect-session from string
///
::rust::Box<::com::crypto::game_sdk::WalletconnectClient>
walletconnect_restore_client(::rust::String session_info);

/// create walletconnect-session
/// the chain id (if 0, retrived and decided by wallet, if > 0, decided by the
/// client)
::rust::Box<::com::crypto::game_sdk::WalletconnectClient>
walletconnect_new_client(::rust::String description, ::rust::String url,
                         ::rust::Vec<::rust::String> icon_urls,
                         ::rust::String name, ::std::uint64_t chain_id);

/// returns the transactions of a given address.
/// The API key can be obtained from https://cronoscan.com
::rust::Vec<::com::crypto::game_sdk::RawTxDetail>
get_transaction_history_blocking(::rust::String address,
                                 ::rust::String api_key);

/// returns the ERC20 transfers of a given address of a given contract.
/// (address can be empty if option is ByContract)
/// default option is by address
/// The API key can be obtained from https://cronoscan.com
::rust::Vec<::com::crypto::game_sdk::RawTxDetail>
get_erc20_transfer_history_blocking(::rust::String address,
                                    ::rust::String contract_address,
                                    ::com::crypto::game_sdk::QueryOption option,
                                    ::rust::String api_key);

/// returns the ERC721 transfers of a given address of a given contract.
/// (address can be empty if option is ByContract)
/// default option is by address
/// The API key can be obtained from https://cronoscan.com
::rust::Vec<::com::crypto::game_sdk::RawTxDetail>
get_erc721_transfer_history_blocking(
    ::rust::String address, ::rust::String contract_address,
    ::com::crypto::game_sdk::QueryOption option, ::rust::String api_key);

/// given the BlockScout REST API base url and the account address
/// (hexadecimal), it will return the list of all owned tokens (ref:
/// https://cronos.org/explorer/testnet3/api-docs)
::rust::Vec<::com::crypto::game_sdk::RawTokenResult>
get_tokens_blocking(::rust::String blockscout_base_url,
                    ::rust::String account_address);

/// given the BlockScout REST API base url and the account address (hexadecimal;
/// required) and optional contract address (hexadecimal; optional -- it can be
/// empty if the option is ByAddress), it will return all the token transfers
/// (ERC20, ERC721... in the newer BlockScout releases, also ERC1155) (ref:
/// https://cronos.org/explorer/testnet3/api-docs) NOTE: QueryOption::ByContract
/// is not supported by BlockScout
::rust::Vec<::com::crypto::game_sdk::RawTxDetail>
get_token_transfers_blocking(::rust::String blockscout_base_url,
                             ::rust::String address,
                             ::rust::String contract_address,
                             ::com::crypto::game_sdk::QueryOption option);

/// given the BlockScout REST API base url and the contract address
/// (hexadecimal),
///
/// page: A nonnegative integer that represents the page number to be used for
/// pagination. 'offset' must be provided in conjunction.
///
/// offset: A nonnegative integer that represents the maximum number of records
/// to return when paginating. 'page' must be provided in conjunction.
///
/// it will return the list of owners and balances (sorting from largest to
/// smallest), but no token ids.
///
/// (ref: https://cronos.org/explorer/api-docs#token)
///
/// ::TIPS:: Use another functions to get more token/owner details, e.g.
/// `get_tokens_blocking` to get owned tokens by account_address
::rust::Vec<::com::crypto::game_sdk::TokenHolderDetail>
get_token_holders(::rust::String blockscout_base_url,
                  ::rust::String contract_address, ::std::uint64_t page,
                  ::std::uint64_t offset);

/// it creates the payment object
/// https://pay-docs.crypto.com/#api-reference-resources-payments-create-a-payment
/// This API can be called using either your Secret Key or Publishable Key.
/// The amount should be given in base units (e.g. for USD, the base unit is
/// cents 1 USD == 100 cents).
::com::crypto::game_sdk::CryptoComPaymentResponse
create_payment(::rust::String secret_or_publishable_api_key,
               ::rust::String base_unit_amount, ::rust::String currency,
               ::com::crypto::game_sdk::OptionalArguments const &optional_args);

/// it returns the payment object by id
/// https://pay-docs.crypto.com/#api-reference-resources-payments-get-payment-by-id
/// This API can be called using either your Secret Key or Publishable Key.
::com::crypto::game_sdk::CryptoComPaymentResponse
get_payment(::rust::String secret_or_publishable_api_key,
            ::rust::String payment_id);
} // namespace game_sdk
} // namespace crypto
} // namespace com
