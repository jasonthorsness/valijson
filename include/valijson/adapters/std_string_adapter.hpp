/**
 * @file
 *
 * @brief   Adapter implementation that wraps a single std::string value
 *
 * This allows property names to be validated against a schema as though they are a generic JSON
 * value, while allowing the rest of Valijson's API to expose property names as plain std::string
 * values.
 *
 * This was added while implementing draft 7 support. This included support for a constraint
 * called propertyNames, which can be used to ensure that the property names in an object
 * validate against a subschema.
 */

#pragma once

#include <string>

#include <valijson/internal/adapter.hpp>
#include <valijson/internal/frozen_value.hpp>
#include <valijson/internal/basic_adapter.hpp>
#include <valijson/exceptions.hpp>

namespace valijson {
namespace adapters {

class StdStringAdapter;
class StdStringArrayValueIterator;
class StdStringObjectMemberIterator;

typedef std::pair<std::string, StdStringAdapter> StdStringObjectMember;

class StdStringArray
{
public:
    typedef StdStringArrayValueIterator const_iterator;
    typedef StdStringArrayValueIterator iterator;

    StdStringArray() = default;

    StdStringArrayValueIterator begin() const;

    StdStringArrayValueIterator end() const;

    static size_t size()
    {
        return 0;
    }
};

class StdStringObject
{
public:
    typedef StdStringObjectMemberIterator const_iterator;
    typedef StdStringObjectMemberIterator iterator;

    StdStringObject() = default;

    StdStringObjectMemberIterator begin() const;

    StdStringObjectMemberIterator end() const;

    StdStringObjectMemberIterator find(const std::string &propertyName) const;

    static size_t size()
    {
        return 0;
    }
};

class StdStringFrozenValue: public FrozenValue
{
public:
    explicit StdStringFrozenValue(std::string source)
      : value(std::move(source)) { }

    FrozenValue * clone() const override
    {
        return new StdStringFrozenValue(value);
    }

    bool equalTo(const Adapter &other, bool strict) const override;

private:
    std::string value;
};

class StdStringAdapter: public Adapter
{
public:
    typedef StdStringArray Array;
    typedef StdStringObject Object;
    typedef StdStringObjectMember ObjectMember;

    explicit StdStringAdapter(const std::string &value)
      : m_value(value) { }

    bool applyToArray(ArrayValueCallback) const override
    {
        return maybeArray();
    }

    bool applyToObject(ObjectMemberCallback) const override
    {
        return maybeObject();
    }

    StdStringArray asArray() const
    {
        if (maybeArray()) {
            return {};
        }

        throwRuntimeError("String value cannot be cast to array");
    }

    bool asBool() const override
    {
        return true;
    }

    bool asBool(bool &result) const override
    {
        result = true;
        return true;
    }

    double asDouble() const override
    {
        return 0;
    }

    bool asDouble(double &result) const override
    {
        result = 0;
        return true;
    }

    int64_t asInteger() const override
    {
        return 0;
    }

    bool asInteger(int64_t &result) const override
    {
        result = 0;
        return true;
    }

    StdStringObject asObject() const
    {
        if (maybeObject()) {
            return {};
        }

        throwRuntimeError("String value cannot be cast to object");
    }

    std::string asString() const override
    {
        return m_value;
    }

    bool asString(std::string &result) const override
    {
        result = m_value;
        return true;
    }

    bool equalTo(const Adapter &other, bool strict) const override
    {
        if (strict && !other.isString()) {
            return false;
        }

        return m_value == other.asString();
    }

    FrozenValue* freeze() const override
    {
        return new StdStringFrozenValue(m_value);
    }

    VALIJSON_NORETURN static StdStringArray getArray()
    {
        throwNotSupported();
    }

    VALIJSON_NORETURN size_t getArraySize() const override
    {
        throwNotSupported();
    }

    VALIJSON_NORETURN bool getArraySize(size_t &) const override
    {
        throwNotSupported();
    }

    VALIJSON_NORETURN bool getBool() const override
    {
        throwNotSupported();
    }

    VALIJSON_NORETURN bool getBool(bool &) const override
    {
        throwNotSupported();
    }

    VALIJSON_NORETURN double getDouble() const override
    {
        throwNotSupported();
    }

    VALIJSON_NORETURN bool getDouble(double &) const override
    {
        throwNotSupported();
    }

    VALIJSON_NORETURN int64_t getInteger() const override
    {
        throwNotSupported();
    }

    VALIJSON_NORETURN bool getInteger(int64_t &) const override
    {
        throwNotSupported();
    }

    VALIJSON_NORETURN double getNumber() const override
    {
        throwNotSupported();
    }

    VALIJSON_NORETURN bool getNumber(double &) const override
    {
        throwNotSupported();
    }

    VALIJSON_NORETURN size_t getObjectSize() const override
    {
        throwNotSupported();
    }

    VALIJSON_NORETURN bool getObjectSize(size_t &) const override
    {
        throwNotSupported();
    }

    std::string getString() const override
    {
        return m_value;
    }

    bool getString(std::string &result) const override
    {
        result = m_value;
        return true;
    }

    bool hasStrictTypes() const override
    {
        return true;
    }

    bool isArray() const override
    {
        return false;
    }

    bool isBool() const override
    {
        return false;
    }

    bool isDouble() const override
    {
        return false;
    }

    bool isInteger() const override
    {
        return false;
    }

    bool isNull() const override
    {
        return false;
    }

    bool isNumber() const override
    {
        return false;
    }

    bool isObject() const override
    {
        return false;
    }

    bool isString() const override
    {
        return true;
    }

    bool maybeArray() const override
    {
        return false;
    }

    bool maybeBool() const override
    {
        return false;
    }

    bool maybeDouble() const override
    {
        return false;
    }

    bool maybeInteger() const override
    {
        return false;
    }

    bool maybeNull() const override
    {
        return false;
    }

    bool maybeObject() const override
    {
        return false;
    }

    bool maybeString() const override
    {
        return true;
    }

private:
    const std::string &m_value;
};

class StdStringArrayValueIterator
{
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = StdStringAdapter;
    using difference_type = StdStringAdapter;
    using pointer = StdStringAdapter*;
    using reference = StdStringAdapter&;

    VALIJSON_NORETURN StdStringAdapter operator*() const
    {
        throwNotSupported();
    }

    VALIJSON_NORETURN DerefProxy<StdStringAdapter> operator->() const
    {
        throwNotSupported();
    }

    bool operator==(const StdStringArrayValueIterator &) const
    {
        return true;
    }

    bool operator!=(const StdStringArrayValueIterator &) const
    {
        return false;
    }

    VALIJSON_NORETURN const StdStringArrayValueIterator& operator++()
    {
        throwNotSupported();
    }

    VALIJSON_NORETURN StdStringArrayValueIterator operator++(int)
    {
        throwNotSupported();
    }

    VALIJSON_NORETURN const StdStringArrayValueIterator& operator--()
    {
        throwNotSupported();
    }

    VALIJSON_NORETURN void advance(std::ptrdiff_t)
    {
        throwNotSupported();
    }
};

inline StdStringArrayValueIterator StdStringArray::begin() const
{
    return {};
}

inline StdStringArrayValueIterator StdStringArray::end() const
{
    return {};
}

class StdStringObjectMemberIterator
{
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = StdStringObjectMember;
    using difference_type = StdStringObjectMember;
    using pointer = StdStringObjectMember*;
    using reference = StdStringObjectMember&;

    VALIJSON_NORETURN StdStringObjectMember operator*() const
    {
        throwNotSupported();
    }

    VALIJSON_NORETURN DerefProxy<StdStringObjectMember> operator->() const
    {
        throwNotSupported();
    }

    bool operator==(const StdStringObjectMemberIterator &) const
    {
        return true;
    }

    bool operator!=(const StdStringObjectMemberIterator &) const
    {
        return false;
    }

    VALIJSON_NORETURN const StdStringObjectMemberIterator& operator++()
    {
        throwNotSupported();
    }

    VALIJSON_NORETURN StdStringObjectMemberIterator operator++(int)
    {
        throwNotSupported();
    }

    VALIJSON_NORETURN const StdStringObjectMemberIterator& operator--()
    {
        throwNotSupported();
    }
};

inline StdStringObjectMemberIterator StdStringObject::begin() const
{
    return {};
}

inline StdStringObjectMemberIterator StdStringObject::end() const
{
    return {};
}

inline StdStringObjectMemberIterator StdStringObject::find(const std::string &) const
{
    return {};
}

template<>
struct AdapterTraits<valijson::adapters::StdStringAdapter>
{
    typedef std::string DocumentType;

    static std::string adapterName()
    {
        return "StdStringAdapter";
    }
};

inline bool StdStringFrozenValue::equalTo(const Adapter &other, bool strict) const
{
    return StdStringAdapter(value).equalTo(other, strict);
}

}  // namespace adapters
}  // namespace valijson
