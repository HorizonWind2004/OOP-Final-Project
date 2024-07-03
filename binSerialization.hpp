#ifndef _BIN_SERIALIZE_HPP_
#define _BIN_SERIALIZE_HPP_

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <string>
#include <type_traits>

namespace binSerialization {

    // Register functions
    template <typename Func, typename... Members>
    void registerConstFunction(const Func& func, const Members&... members) {
        (func(members), ...);
    }

    template <typename Func, typename... Members>
    void registerFunction(const Func& func, Members&... members) {
        (func(members), ...);
    }

    template<typename T>
    void writeBinary(std::ostream& stream, const T& data) {
        stream.write(reinterpret_cast<const char*>(&data), sizeof(data));
    }

    template<typename T>
    void readBinary(std::istream& stream, T& data) {
        stream.read(reinterpret_cast<char*>(const_cast<std::remove_const_t<T>*>(&data)), sizeof(T));
    }

    // Handling arithmetic types
    template<typename T>
    typename std::enable_if_t<std::is_arithmetic_v<T>, void>
    serialize_(const T& value, std::ostream& stream) {
        writeBinary(stream, value);
    }

    template<typename T>
    typename std::enable_if_t<std::is_arithmetic_v<T>, void>
    deserialize_(T& value, std::istream& stream) {
        readBinary(stream, value);
    }

    // Handling std::string
    void serialize_(const std::string& str, std::ostream& stream) {
        size_t length = str.size();
        writeBinary(stream, length);
        stream.write(str.data(), length);
    }

    void deserialize_(std::string& str, std::istream& stream) {
        size_t length;
        readBinary(stream, length);
        str.resize(length);
        stream.read(&str[0], length);
    }

    // Some declaration
    template<typename Container>
    void serializeContainer(const Container& container, std::ostream& stream);
    template<typename Container>
    void deserializeContainer(Container& container, std::istream& stream);
    template<typename Key, typename Value>
    void deserializeContainer(std::map<Key, Value>& map, std::istream& stream);
    template<typename T1, typename T2>
    void serialize_(const std::pair<T1, T2>& pair, std::ostream& stream);
    template<typename T1, typename T2>
    void deserialize_(std::pair<T1, T2>& pair, std::istream& stream);
    template<typename T>
    typename std::enable_if_t<std::is_class_v<T>, void>
    serialize_(const T& obj, std::ostream& stream);
    template<typename T>
    typename std::enable_if_t<std::is_class_v<T>, void>
    deserialize_(T& obj, std::istream& stream);


    template<typename T>
    void serialize_(const std::vector<T>& vec, std::ostream& stream) {
        serializeContainer(vec, stream);
    }

    template<typename T>
    void deserialize_(std::vector<T>& vec, std::istream& stream) {
        deserializeContainer(vec, stream);
    }

    template<typename T>
    void serialize_(const std::list<T>& list, std::ostream& stream) {
        serializeContainer(list, stream);
    }

    template<typename T>
    void deserialize_(std::list<T>& list, std::istream& stream) {
        deserializeContainer(list, stream);
    }

    template<typename Key, typename Value>
    void serialize_(const std::map<Key, Value>& map, std::ostream& stream) {
        serializeContainer(map, stream);
    }

    template<typename Key, typename Value>
    void deserialize_(std::map<Key, Value>& map, std::istream& stream) {
        deserializeContainer(map, stream);
    }

    template<typename T>
    void serialize_(const std::set<T>& set, std::ostream& stream) {
        serializeContainer(set, stream);
    }

    template<typename T>
    void deserialize_(std::set<T>& set, std::istream& stream) {
        deserializeContainer(set, stream);
    }

    // Handling std::pair
    template<typename T1, typename T2>
    void serialize_(const std::pair<T1, T2>& pair, std::ostream& stream) {
        serialize_(pair.first, stream);
        serialize_(pair.second, stream);
    }

    template<typename T1, typename T2>
    void deserialize_(std::pair<T1, T2>& pair, std::istream& stream) {
        deserialize_(pair.first, stream);
        deserialize_(pair.second, stream);
    }

    // Handling User-defined class
    template<typename T>
    typename std::enable_if_t<std::is_class_v<T>, void>
    serialize_(const T& obj, std::ostream& stream) {
        obj.serialize_(stream);
    }

    template<typename T>
    typename std::enable_if_t<std::is_class_v<T>, void>
    deserialize_(T& obj, std::istream& stream) {
        obj.deserialize_(stream);
    }

    // Serialization and deserialization methods for containers
    template<typename Container>
    void serializeContainer(const Container& container, std::ostream& stream) {
        size_t size = container.size();
        writeBinary(stream, size);
        for (const auto& element : container) {
            serialize_(element, stream);
        }
    }

    template<typename Container>
    void deserializeContainer(Container& container, std::istream& stream) {
        size_t size;
        readBinary(stream, size);
        container.clear();
        for (size_t i = 0; i < size; ++i) {
            typename Container::value_type element;
            deserialize_(element, stream);
            container.insert(container.end(), std::move(element));
        }
    }

    // special case for std::map, because key is CONST.
    template<typename Key, typename Value>
    void deserializeContainer(std::map<Key, Value>& map, std::istream& stream) { 
        size_t size;
        readBinary(stream, size);
        map.clear();
        for (size_t i = 0; i < size; ++i) {
            std::pair<Key, Value> element;
            deserialize_(element, stream);
            map.insert(std::move(element));
        }
    }

    /*
    * Serialize the value to the file with the given filename.
    * @param 
    *     value: the value to be serialized.
    *     filename: the name of the file to store the serialized data.
    */
    template<typename T>
    void serialize(const T& obj, const std::string& filename) {
        std::ofstream outStream(filename, std::ios::binary);
        if (!outStream) {
            throw std::runtime_error("File cannot be created!");
        }
        serialize_(obj, outStream);
    }

    /*
    * Deserialize the value from the file with the given filename.
    * @param
    *    value: the value to be deserialized.
    *   filename: the name of the file to read the serialized data.
    */
    template<typename T>
    void deserialize(T& obj, const std::string& filename) {
        std::ifstream inStream(filename, std::ios::binary);
        if (!inStream) {
            throw std::runtime_error("File not found!");
        }
        deserialize_(obj, inStream);
    }

}

// Macro for registering serialization and deserialization functions
#define REGISTER_BINSERIALIZATION(...) \
    void serialize_(std::ostream& stream) const{ \
        binSerialization::registerConstFunction([&](const auto& member){binSerialization::serialize_(member, stream);}, __VA_ARGS__); \
    } \
    void deserialize_(std::istream& stream) { \
        binSerialization::registerFunction([&](auto& member){binSerialization::deserialize_(member, stream);}, __VA_ARGS__); \
    }


#endif // _BIN_SERIALIZE_HPP_
