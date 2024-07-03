#ifndef _XML_SERIALIZE_HPP_
#define _XML_SERIALIZE_HPP_

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <string>
#include <type_traits>
#include <sstream>
#include "thirdparty/tinyxml2.h"
#include "mybase64.hpp"

using namespace tinyxml2;

namespace xmlSerialization {

    // Register functions
    template <typename Func, typename... Members>
    void registerConstFunction(const Func& func, const Members&... members) {
        (func(members), ...);
    }
    template <typename Func, typename... Members>
    void registerFunction(const Func& func, Members&... members) {
        (func(members), ...);
    }

    // Handling arithmetic types
    template<typename T>
    typename std::enable_if_t<std::is_arithmetic_v<T>, void>
    serialize_xml_(const T& value, XMLDocument& doc, XMLElement* parent) {
        XMLElement* element = doc.NewElement("value");
        element->SetText(value);
        parent->InsertEndChild(element);
    }

    template<typename T>
    typename std::enable_if_t<std::is_arithmetic_v<T>, void>
    deserialize_xml_(T& value, XMLDocument& doc, XMLElement* parent) {
        XMLElement* element = parent->FirstChildElement("value");
        std::string str = element->GetText();
        std::stringstream ss(str);
        ss >> value;
    }

    // Handling std::string
    void serialize_xml_(const std::string& str, XMLDocument& doc, XMLElement* parent) {
        XMLElement* element = doc.NewElement("std_string");
        element->SetText(str.c_str());
        parent->InsertEndChild(element);
    }

    void deserialize_xml_(std::string& str, XMLDocument& doc, XMLElement* parent) {
        XMLElement* element = parent->FirstChildElement("std_string");
        str = element->GetText();
    }

    // Some declaration
    template<typename Container>
    void serializeContainer_xml(const Container& container, XMLDocument& doc, XMLElement* parent, const char* containerName);
    template<typename Container>
    void deserializeContainer_xml(Container& container, XMLDocument& doc, XMLElement* parent, const char* containerName);
    template<typename Value, typename Key>
    void deserializeContainer_xml(std::map<Key, Value>& container, XMLDocument& doc, XMLElement* parent);
    template<typename T1, typename T2>
    void serialize_xml_(const std::pair<T1, T2>& pair, XMLDocument& doc, XMLElement* parent);
    template<typename T1, typename T2>
    void deserialize_xml_(std::pair<T1, T2>& pair, XMLDocument& doc, XMLElement* parent);
    template<typename T>
    typename std::enable_if_t<std::is_class_v<T>, void>
    serialize_xml_(const T& value, XMLDocument& doc, XMLElement* parent);
    template<typename T>
    typename std::enable_if_t<std::is_class_v<T>, void>
    deserialize_xml_(T& value, XMLDocument& doc, XMLElement* parent);

    template<typename T>
    void serialize_xml_(const std::vector<T>& vec, XMLDocument& doc, XMLElement* parent) {
        serializeContainer_xml(vec, doc, parent, "std_vector");
    }

    template<typename T>
    void deserialize_xml_(std::vector<T>& vec, XMLDocument& doc, XMLElement* parent) {       
        deserializeContainer_xml(vec, doc, parent, "std_vector");
    }

    template<typename T>
    void serialize_xml_(const std::list<T>& list, XMLDocument& doc, XMLElement* parent) {
        serializeContainer_xml(list, doc, parent, "std_list");
    }

    template<typename T>
    void deserialize_xml_(std::list<T>& list, XMLDocument& doc, XMLElement* parent) {
        deserializeContainer_xml(list, doc, parent, "std_list");
    }

    template<typename Key, typename Value>
    void serialize_xml_(const std::map<Key, Value>& map, XMLDocument& doc, XMLElement* parent) {
        serializeContainer_xml(map, doc, parent, "std_map");
    }

    template<typename Key, typename Value>
    void deserialize_xml_(std::map<Key, Value>& map, XMLDocument& doc, XMLElement* parent) {
        deserializeContainer_xml(map, doc, parent);
    }

    template<typename T>
    void serialize_xml_(const std::set<T>& set, XMLDocument& doc, XMLElement* parent) {
        serializeContainer_xml(set, doc, parent, "std_set");
    }

    template<typename T>
    void deserialize_xml_(std::set<T>& set, XMLDocument& doc, XMLElement* parent) {
        deserializeContainer_xml(set, doc, parent, "std_set");
    }

    // Handling std::pair
    template<typename T1, typename T2>
    void serialize_xml_(const std::pair<T1, T2>& pair, XMLDocument& doc, XMLElement* parent) {
        XMLElement* element = doc.NewElement("std_pair");
        parent->InsertEndChild(element);
        XMLElement* elementFirst = doc.NewElement("first");
        element->InsertEndChild(elementFirst);
        serialize_xml_(pair.first, doc, elementFirst);
        XMLElement* elementSecond = doc.NewElement("second");
        element->InsertEndChild(elementSecond);
        serialize_xml_(pair.second, doc, elementSecond);

    }

    template<typename T1, typename T2>
    void deserialize_xml_(std::pair<T1, T2>& pair, XMLDocument& doc, XMLElement* parent) {
        XMLElement* element = parent->FirstChildElement("std_pair");
        XMLElement* elementFirst = element->FirstChildElement("first");
        deserialize_xml_(pair.first, doc, elementFirst);
        XMLElement* elementSecond = element->FirstChildElement("second");
        deserialize_xml_(pair.second, doc, elementSecond);
    }

    // Handling User-defined class
    template<typename T>
    typename std::enable_if_t<std::is_class_v<T>, void>
    serialize_xml_(const T& value, XMLDocument& doc, XMLElement* parent) {
        value.serialize_xml_(doc, parent);
    }

    template<typename T>
    typename std::enable_if_t<std::is_class_v<T>, void>
    deserialize_xml_(T& value, XMLDocument& doc, XMLElement* parent) {
        value.deserialize_xml_(doc, parent);
    }

    // Serialization and deserialization methods for containers
    template<typename Container>
    void serializeContainer_xml(const Container& container, XMLDocument& doc, XMLElement* parent, const char* containerName) {
        XMLElement* containerElement = doc.NewElement(containerName);
        parent->InsertEndChild(containerElement);
        XMLElement* sizeElement = doc.NewElement("size");
        sizeElement->SetText(std::to_string(container.size()).c_str());
        containerElement->InsertEndChild(sizeElement);
        int index = 0;
        for (const auto& element : container) {
            XMLElement* element_ = doc.NewElement(("index_" + std::to_string(index)).c_str());
            containerElement->InsertEndChild(element_);
            serialize_xml_(element, doc, element_);
            ++index;
        }
    }

    template<typename Container>
    void deserializeContainer_xml(Container& container, XMLDocument& doc, XMLElement* parent, const char* containerName) {
        XMLElement* containerElement = parent->FirstChildElement(containerName); 
        XMLElement* sizeElement = containerElement->FirstChildElement("size");
        unsigned int size;
        sizeElement->QueryUnsignedText(&size);
        container.clear();
        for (size_t i = 0; i < size; ++i) {
            typename Container::value_type element;
            deserialize_xml_(element, doc, containerElement->FirstChildElement(("index_" + std::to_string(i)).c_str()));
            container.insert(container.end(), element);
        }
    }

    // special case for std::map, because key is CONST.
    template<typename Value, typename Key>
    void deserializeContainer_xml(std::map<Key, Value>& map, XMLDocument& doc, XMLElement* parent) {
        XMLElement* containerElement = parent->FirstChildElement("std_map");
        XMLElement* sizeElement = containerElement->FirstChildElement("size");
        unsigned int size;
        sizeElement->QueryUnsignedText(&size);
        map.clear();
        for (size_t i = 0; i < size; ++i) {
            std::pair<Key, Value> element; 
            deserialize_xml_(element, doc, containerElement->FirstChildElement(("index_" + std::to_string(i)).c_str()));
            map.insert(map.end(), element);
        }
    }

    /*
    * Serialize the value to the file with the given filename.
    * @param 
    *     value: the value to be serialized.
    *     filename: the name of the file to store the serialized data.
    */
    template<typename T>
    void serialize_xml(const T& value, const std::string& filename, bool use_base64 = false) {
        XMLDocument doc;
        XMLElement* root = doc.NewElement("serialization");
        doc.InsertFirstChild(root);
        serialize_xml_(value, doc, root);
        if (!use_base64) {
            doc.SaveFile(filename.c_str());
            return;
        }
        std::string data;
        XMLPrinter printer;
        doc.Print(&printer);
        data = printer.CStr();
        std::ofstream file(filename);
        file << base64_encode(reinterpret_cast<const unsigned char*>(data.c_str()), data.size());
        file.close();
    }

    /*
    * Deserialize the value from the file with the given filename.
    * @param
    *    value: the value to be deserialized.
    *   filename: the name of the file to read the serialized data.
    */
    template<typename T>
    void deserialize_xml(T& value, const std::string& filename, bool use_base64 = false) {
        if (!use_base64) {
            XMLDocument doc;
            doc.LoadFile(filename.c_str());
            XMLElement* root = doc.FirstChildElement("serialization");
            deserialize_xml_(value, doc, root);
        } else {
            std::ifstream file(filename);
            std::string data, line;
            while (std::getline(file, line)) {
                data += line;
            }
            file.close();
            std::string decoded = base64_decode(data);
            XMLDocument doc;
            doc.Parse(decoded.c_str());
            XMLElement* root = doc.FirstChildElement("serialization");
            deserialize_xml_(value, doc, root);
        }
    }

}

// Macro for registering serialization and deserialization functions
#define REGISTER_XMLSERIALIZATION(...) \
    void serialize_xml_(XMLDocument& doc, XMLElement* parent) const{ \
        xmlSerialization::registerConstFunction([&](const auto& member){xmlSerialization::serialize_xml_(member, doc, parent);}, __VA_ARGS__); \
    } \
    void deserialize_xml_(XMLDocument& doc, XMLElement* parent) { \
        xmlSerialization::registerFunction([&](auto& member){xmlSerialization::deserialize_xml_(member, doc, parent);}, __VA_ARGS__); \
    }


#endif // _SERIALIZE_XML_HPP_
