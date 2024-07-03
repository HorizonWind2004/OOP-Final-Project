## Object Serialization

    Done by 谢集-3220103501.

### Introduction

此项目是本人完成的 OOP 大作业，实现了简单的对象序列化（Serialization）功能。项目中实现了两种序列化方式：二进制序列化和 XML 序列化（**以及其 base64 编码**）。

该项目支持的对象类型为：

- `std::is_arithmetic` 下的所有类型；

- `std::string` 类型；

- `std::vector` 类型。

- `std::map` 类型。

- `std::list` 类型。

- `std::set` 类型。

- `std::pair` 类型。

- 由上述类型组合而成的用户自定义类型。

### File structure

```
Serialization/
        ├── binSerialization.hpp
        ├── xmlSerialization.hpp
        ├── mybase64.hpp
        ├── test.cpp
        ├── main
        ├── serialization_file
        ├── Makefile
        ├── build/
        │      ├── (build files)
        │
        └── thirdparty/
               ├── tinyxml2.cpp
               ├─- tinyxml2.h
```

### Environment

OS: `macOS Sonoma 14.1`

g++ version: `Apple clang version 15.0.0 (clang-1500.3.9.4)`

### Using Method

#### 1. 二进制序列化

使用命名空间 `binSerialization` 中的函数 `serialize` 和 `deserialize` 即可，用法示例如下：

```cpp
#include "binSerialization.hpp"
using namespace binSerialization;
...
int main() {
    // Serialize
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::string filename = "vector.bin";
    serialize(vec, filename);

    // Deserialize
    std::vector<int> vec2;
    deserialize(vec2, filename);
    for (auto &i : vec2) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}
```

对于用户自定义类型，项目中实现了一个宏 `REGISTER_BINSERIALIZATION`，用于方便地定义序列化和反序列化函数。

```cpp
#include "binSerialization.hpp"
using namespace binSerialization;
struct Student {
    std::string name;
    int age;
    std::vector<int> scores;
    std::map<std::string, int> course;
    REGISTER_BINSERIALIZATION(Student, name, age, scores, course);
};
...
int main() {
    // Serialize
    Student stu = {"Tom", 20, {90, 80, 70}, {{"Math", 90}, {"English", 80}}};
    std::string filename = "student.bin";
    serialize(stu, filename);

    // Deserialize
    Student stu2;
    deserialize(stu2, filename);
    std::cout << stu2.name << " " << stu2.age << std::endl;
    for (auto &i : stu2.scores) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    for (auto &i : stu2.course) {
        std::cout << i.first << " " << i.second << std::endl;
    }
}
```

#### 2. XML 序列化

使用命名空间 `xmlSerialization` 中的函数 `serialize_xml` 和 `deserialize_xml` 即可，用法示例如下：

```cpp
#include "xmlSerialization.hpp"
using namespace xmlSerialization;
...
int main() {
    // Serialize
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::string filename = "vector.xml";
    serialize_xml(vec, filename);

    // Deserialize
    std::vector<int> vec2;
    deserialize_xml(vec2, filename);
    for (auto &i : vec2) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}
```

如果想使用 `base64` 编码，只需要在函数 `serialize_xml` 和 `deserialize_xml` 的第三个参数传入 `true` 即可。也就是：

```cpp
...
int main() {
    ...
    serialize_xml(vec, filename, true);
    ...
    deserialize_xml(vec2, filename, true);
    ...
}
```

对于用户自定义类型，项目中实现了一个宏 `REGISTER_XMLSERIALIZATION`，用于方便地定义序列化和反序列化函数。

```cpp
#include "xmlSerialization.hpp"
using namespace xmlSerialization;
struct Student {
    std::string name;
    int age;
    std::vector<int> scores;
    std::map<std::string, int> course;
    REGISTER_XMLSERIALIZATION(Student, name, age, scores, course);
};
...
int main() {
    // Serialize
    Student stu = {"Tom", 20, {90, 80, 70}, {{"Math", 90}, {"English", 80}}};
    std::string filename = "student.xml";
    serialize_xml(stu, filename);

    // Deserialize
    Student stu2;
    deserialize_xml(stu2, filename);
    std::cout << stu2.name << " " << stu2.age << std::endl;
    for (auto &i : stu2.scores) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    for (auto &i : stu2.course) {
        std::cout << i.first << " " << i.second << std::endl;
    }
}
```

### Test

项目中提供了一个测试文件 `test.cpp`，测试文件中包含了对所有支持的类型的三种序列化（二进制、`xml`、`xml` + `base64`）测试，以及对用户自定义类型的测试。包含的类型如下：

- `int` 类型；

- `std::string` 类型；

- `double` 类型；

- `std::vector<int>` 类型；

- `std::set<int>` 类型；

- `std::map<int, std::string>` 类型；

- `std::list<int>` 类型；

- `std::map<int, std::vector<int>>` 类型；

- `std::set<std::list<int>>` 类型；

- `std::vector<std::map<int, std::string>>` 类型；

- 用户自定义类型 `person`；

- `std::map<std::string, std::vector<person>>` 类型。

### How to build

使用 `make` 命令编译，生成的可执行文件为 `main`。

```bash
$ make
$ ./main
```

### Result

运行后输出结果为：

```
bin::test<int> passed.
xml::test<int> passed.
xml::test<int> (with base64) passed.
bin::test<string> passed.
xml::test<string> passed.
xml::test<string> (with base64) passed.
bin::test<double> passed.
xml::test<double> passed.
xml::test<double> (with base64) passed.
bin::test<vector<int>> passed.
xml::test<vector<int>> passed.
xml::test<vector<int>> (with base64) passed.
bin::test<set<int>> passed.
xml::test<set<int>> passed.
xml::test<set<int>> (with base64) passed.
bin::test<map<int, string>> passed.
xml::test<map<int, string>> passed.
xml::test<map<int, string>> (with base64) passed.
bin::test<list<int>> passed.
xml::test<list<int>> passed.
xml::test<list<int>> (with base64) passed.
bin::test<map<int, vector<int>>> passed.
xml::test<map<int, vector<int>>> passed.
xml::test<map<int, vector<int>>> (with base64) passed.
bin::test<set<list<int>>> passed.
xml::test<set<list<int>>> passed.
xml::test<set<list<int>>> (with base64) passed.
bin::test<vector<map<int, string>>> passed.
xml::test<vector<map<int, string>>> passed.
xml::test<vector<map<int, string>>> (with base64) passed.
bin::test<person> passed.
xml::test<person> passed.
xml::test<person> (with base64) passed.
bin::test<map<string, vector<person>>> passed.
xml::test<map<string, vector<person>>> passed.
xml::test<map<string, vector<person>>> (with base64) passed.
----------------------
All bin::tests passed.
```

打开 `serialization_file` 文件夹，可以看到测试文件生成的序列化文件。其中 `*.bin` 文件为二进制序列化文件，`*.xml` 文件为 XML 序列化文件，`*.txt` 文件为 base64 编码后的 XML 序列化文件。

`vector_map.xml` 的内容如下：

```xml
<serialization>
    <std_vector>
        <size>2</size>
        <index_0>
            <std_map>
                <size>2</size>
                <index_0>
                    <std_pair>
                        <first>
                            <value>1</value>
                        </first>
                        <second>
                            <std_string>one</std_string>
                        </second>
                    </std_pair>
                </index_0>
                <index_1>
                    <std_pair>
                        <first>
                            <value>2</value>
                        </first>
                        <second>
                            <std_string>two</std_string>
                        </second>
                    </std_pair>
                </index_1>
            </std_map>
        </index_0>
        <index_1>
            <std_map>
                <size>2</size>
                <index_0>
                    <std_pair>
                        <first>
                            <value>3</value>
                        </first>
                        <second>
                            <std_string>three</std_string>
                        </second>
                    </std_pair>
                </index_0>
                <index_1>
                    <std_pair>
                        <first>
                            <value>4</value>
                        </first>
                        <second>
                            <std_string>four</std_string>
                        </second>
                    </std_pair>
                </index_1>
            </std_map>
        </index_1>
    </std_vector>
</serialization>
```

`string.txt` 的内容如下：

```
PHNlcmlhbGl6YXRpb24+CiAgICA8c3RkX3N0cmluZz5oZWxsbzwvc3RkX3N0cmluZz4KPC9zZXJpYWxpemF0aW9uPgo=
```

利用[在线网站](https://www.base64decode.org/)解码后可以得到：

```xml
<serialization>
    <std_string>hello</std_string>
</serialization>
```

这也验证了 `base64` 编码的正确性。

### Reference

1. cppreference.com. (n.d.). `std::is_arithmetic`. Retrieved from [https://en.cppreference.com/w/cpp/types/is_arithmetic](https://en.cppreference.com/w/cpp/types/is_arithmetic)
2. Wikipedia. (n.d.). Serialization. Retrieved from [https://en.wikipedia.org/wiki/Serialization](https://en.wikipedia.org/wiki/Serialization)
3. Wikipedia. (n.d.). Base64. Retrieved from [https://en.wikipedia.org/wiki/Base64](https://en.wikipedia.org/wiki/Base64)