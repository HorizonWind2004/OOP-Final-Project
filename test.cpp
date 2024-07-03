#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <list>
#include <string>
#include <cassert>
#include "binSerialization.hpp"
#include "xmlSerialization.hpp"

using namespace binSerialization;
using namespace xmlSerialization;


struct person {
    std::string name;
    int age;
    std::vector<int> scores;
    bool operator == (const person& other) const {
        return name == other.name && age == other.age && scores == other.scores;
    }
    REGISTER_BINSERIALIZATION(name, age, scores);
    REGISTER_XMLSERIALIZATION(name, age, scores);
};

int main() {
    // built-in type
    int a = 123, b;
    serialize(a, "serialization_file/int.bin");
    deserialize(b, "serialization_file/int.bin");
    assert(a == b);
    std::cout << "bin::test<int> passed." << std::endl;
    serialize_xml(a, "serialization_file/int.xml");
    deserialize_xml(b, "serialization_file/int.xml");
    assert(a == b);
    std::cout << "xml::test<int> passed." << std::endl;
    serialize_xml(a, "serialization_file/int.txt", true);
    deserialize_xml(b, "serialization_file/int.txt", true);
    assert(a == b);
    std::cout << "xml::test<int> (with base64) passed." << std::endl;

    std::string s = "hello", t;
    serialize(s, "serialization_file/string.bin");
    deserialize(t, "serialization_file/string.bin");
    assert(s == t);
    std::cout << "bin::test<string> passed." << std::endl;
    serialize_xml(s, "serialization_file/string.xml");
    deserialize_xml(t, "serialization_file/string.xml");
    assert(s == t);
    std::cout << "xml::test<string> passed." << std::endl;
    serialize_xml(s, "serialization_file/string.txt", true);
    deserialize_xml(t, "serialization_file/string.txt", true);
    assert(s == t);
    std::cout << "xml::test<string> (with base64) passed." << std::endl;

    double d = 3.1415926, e;
    serialize(d, "serialization_file/double.bin");
    deserialize(e, "serialization_file/double.bin");
    assert(d == e);
    std::cout << "bin::test<double> passed." << std::endl;
    serialize_xml(d, "serialization_file/double.xml");
    deserialize_xml(e, "serialization_file/double.xml");
    assert(d == e);
    std::cout << "xml::test<double> passed." << std::endl;
    serialize_xml(d, "serialization_file/double.txt", true);
    deserialize_xml(e, "serialization_file/double.txt", true);
    assert(d == e);
    std::cout << "xml::test<double> (with base64) passed." << std::endl;

    // container
    std::vector<int> v1 = {1, 1, 2, 3, 5, 8}, v2;
    serialize(v1, "serialization_file/vector.bin");
    deserialize(v2, "serialization_file/vector.bin");
    assert(v1 == v2);
    std::cout << "bin::test<vector<int>> passed." << std::endl;
    serialize_xml(v1, "serialization_file/vector.xml");
    deserialize_xml(v2, "serialization_file/vector.xml");
    assert(v1 == v2);  
    std::cout << "xml::test<vector<int>> passed." << std::endl;
    serialize_xml(v1, "serialization_file/vector.txt", true);
    deserialize_xml(v2, "serialization_file/vector.txt", true);
    assert(v1 == v2);
    std::cout << "xml::test<vector<int>> (with base64) passed." << std::endl;

    std::set<int> s1 = {6, 7, 8, 9, 10}, s2;
    serialize(s1, "serialization_file/set.bin");
    deserialize(s2, "serialization_file/set.bin");
    assert(s1 == s2);
    std::cout << "bin::test<set<int>> passed." << std::endl;
    serialize_xml(s1, "serialization_file/set.xml");
    deserialize_xml(s2, "serialization_file/set.xml");
    assert(s1 == s2);
    std::cout << "xml::test<set<int>> passed." << std::endl;
    serialize_xml(s1, "serialization_file/set.txt", true);
    deserialize_xml(s2, "serialization_file/set.txt", true);
    assert(s1 == s2);
    std::cout << "xml::test<set<int>> (with base64) passed." << std::endl;

    std::map<int, std::string> m1 = {{1, "one"}, {2, "two"}, {3, "three"}}, m2;
    serialize(m1, "serialization_file/map.bin");
    deserialize(m2, "serialization_file/map.bin");
    assert(m1 == m2);
    std::cout << "bin::test<map<int, string>> passed." << std::endl;
    serialize_xml(m1, "serialization_file/map.xml");
    deserialize_xml(m2, "serialization_file/map.xml");
    assert(m1 == m2);
    std::cout << "xml::test<map<int, string>> passed." << std::endl;
    serialize_xml(m1, "serialization_file/map.txt", true);
    deserialize_xml(m2, "serialization_file/map.txt", true);
    assert(m1 == m2);
    std::cout << "xml::test<map<int, string>> (with base64) passed." << std::endl;

    std::list<int> l1 = {1, 2, 3, 4, 5}, l2;
    serialize(l1, "serialization_file/list.bin");
    deserialize(l2, "serialization_file/list.bin");
    assert(l1 == l2);
    std::cout << "bin::test<list<int>> passed." << std::endl;
    serialize_xml(l1, "serialization_file/list.xml");
    deserialize_xml(l2, "serialization_file/list.xml");
    assert(l1 == l2);
    std::cout << "xml::test<list<int>> passed." << std::endl;
    serialize_xml(l1, "serialization_file/list.txt", true);
    deserialize_xml(l2, "serialization_file/list.txt", true);
    assert(l1 == l2);
    std::cout << "xml::test<list<int>> (with base64) passed." << std::endl;

    // complex STL container

    std::map<int, std::vector<int>> m3 = {{1, {1, 2, 3}}, {2, {4, 5, 6}}, {3, {7, 8, 9}}}, m4;
    serialize(m3, "serialization_file/map_vector.bin");
    deserialize(m4, "serialization_file/map_vector.bin");
    assert(m3 == m4);
    std::cout << "bin::test<map<int, vector<int>>> passed." << std::endl;
    serialize_xml(m3, "serialization_file/map_vector.xml");
    deserialize_xml(m4, "serialization_file/map_vector.xml");
    assert(m3 == m4);
    std::cout << "xml::test<map<int, vector<int>>> passed." << std::endl;
    serialize_xml(m3, "serialization_file/map_vector.txt", true);
    deserialize_xml(m4, "serialization_file/map_vector.txt", true);
    assert(m3 == m4);
    std::cout << "xml::test<map<int, vector<int>>> (with base64) passed." << std::endl;

    std::set<std::list<int>> s3 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, s4;
    serialize(s3, "serialization_file/set_list.bin");
    deserialize(s4, "serialization_file/set_list.bin");
    assert(s3 == s4);
    std::cout << "bin::test<set<list<int>>> passed." << std::endl;
    serialize_xml(s3, "serialization_file/set_list.xml");
    deserialize_xml(s4, "serialization_file/set_list.xml");
    assert(s3 == s4);
    std::cout << "xml::test<set<list<int>>> passed." << std::endl;
    serialize_xml(s3, "serialization_file/set_list.txt", true);
    deserialize_xml(s4, "serialization_file/set_list.txt", true);
    assert(s3 == s4);
    std::cout << "xml::test<set<list<int>>> (with base64) passed." << std::endl;

    std::vector<std::map<int, std::string>> v3 = {{{1, "one"}, {2, "two"}}, {{3, "three"}, {4, "four"}}}, v4;
    serialize(v3, "serialization_file/vector_map.bin");
    deserialize(v4, "serialization_file/vector_map.bin");
    assert(v3 == v4);
    std::cout << "bin::test<vector<map<int, string>>> passed." << std::endl;
    serialize_xml(v3, "serialization_file/vector_map.xml");
    deserialize_xml(v4, "serialization_file/vector_map.xml");
    assert(v3 == v4);
    std::cout << "xml::test<vector<map<int, string>>> passed." << std::endl;
    serialize_xml(v3, "serialization_file/vector_map.txt", true);
    deserialize_xml(v4, "serialization_file/vector_map.txt", true);
    assert(v3 == v4);
    std::cout << "xml::test<vector<map<int, string>>> (with base64) passed." << std::endl;

    // user-defined type
    person p1 = {"Alice", 20, {90, 80, 70}}, p2;
    serialize(p1, "serialization_file/person.bin");
    deserialize(p2, "serialization_file/person.bin");
    assert(p1 == p2);
    std::cout << "bin::test<person> passed." << std::endl;
    serialize_xml(p1, "serialization_file/person.xml");
    deserialize_xml(p2, "serialization_file/person.xml");
    assert(p1 == p2);
    std::cout << "xml::test<person> passed." << std::endl;
    serialize_xml(p1, "serialization_file/person.txt", true);
    deserialize_xml(p2, "serialization_file/person.txt", true);
    assert(p1 == p2);
    std::cout << "xml::test<person> (with base64) passed." << std::endl;

    std::map<std::string, std::vector<person>> m5 = {{"Alice", {p1, p2}}, {"Bob", {p2, p1}}}, m6;
    serialize(m5, "serialization_file/map_person.bin");
    deserialize(m6, "serialization_file/map_person.bin");
    assert(m5 == m6);
    std::cout << "bin::test<map<string, vector<person>>> passed." << std::endl;
    serialize_xml(m5, "serialization_file/map_person.xml");
    deserialize_xml(m6, "serialization_file/map_person.xml");
    assert(m5 == m6);
    std::cout << "xml::test<map<string, vector<person>>> passed." << std::endl;
    serialize_xml(m5, "serialization_file/map_person.txt", true);
    deserialize_xml(m6, "serialization_file/map_person.txt", true);
    assert(m5 == m6);
    std::cout << "xml::test<map<string, vector<person>>> (with base64) passed." << std::endl;

    std::cout << "----------------------" << std::endl;
    std::cout << "All bin::tests passed." << std::endl;

    return 0;

}