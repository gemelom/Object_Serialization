## Object Serialization

An implementation of serialization/deserialization in C++.

### Usage
```
make
./main
make clean
```

### Requirements
GCC: Version 10 or later (requires for C++20 support)

### Features
- Supports **binary** serialization/deserialization
- Supports **xml** serialization/deserialization
- Supports types:
  - `std::is_arithmetic`
  - `std::string`
  - STL containers: `std::pair`, `std::vector`, `std::list`, `std::set`, `std::map`
  - user-defined types, e.g.
    ```cpp
    struct Student {
      double d;
      std::string s;
      std::vector<int> v;
    };
    ```
  - smart pointers: `std::unique_ptr`, `std::shared_ptr`
- Supports **base64** encoding/decoding to implement serialization/deserialization in XML format

### Examples
- Binary serialization/deserialization
  ```cpp
  std::vector<int> v1 = {1, 2, 3, 4, 5};
  std::vector<int> v2;
  binary::binarySerialize(v1, "vector.dat");
  binary::binaryDeserialize(v2, "vector.dat");  // v2 == {1, 2, 3, 4, 5}
  ```
- XML serialization/deserialization
  ```cpp
  std::pair<int, std::string> p1 = {2024, "OOP"};
  std::pair<int, std::string> p2;
  xml::xmlSerialize(p1, "pair.xml");
  xml::xmlDeserialize(p2, "pair.xml");  // v2 == {2024, "OOP"}
  ```
  xml file:
  ```xml
  <serialize encoding="UTF-8">
      <std_pair>
          <first>
              <i val="2024"/>
          </first>
          <second>
              <std_string>OOP</std_string>
          </second>
      </std_pair>
  </serialize>
  ```
- User-defined type serialization/deserialization
  ```cpp
  struct Student {
    double d;
    std::string s;
    std::vector<int> v;
  };
  Student s1 = {3.14, "Hello", {1, 2, 3, 4}};
  Student s2;
  xml::xmlSerialize(s1, "stu.xml");
  xml::xmlDeserialize(s2, "stu.xml");  // s2 == {3.14, "Hello", {1, 2, 3, 4}}
  ```
  xml file:
  ```xml
  <serialize encoding="UTF-8">
      <user_defined>
          <d val="3.140000"/>
          <std_string>Hello</std_string>
          <std_vector>
              <size_t size="4"/>
              <i val="1"/>
              <i val="2"/>
              <i val="3"/>
              <i val="4"/>
          </std_vector>
      </user_defined>
  </serialize>
- Smart pointer serialization/deserialization
  ```cpp
  std::unique_ptr<std::map<std::string, double>> up1 = std::make_unique<std::map<std::string, double>>({apple, 3.14});
  std::unique_ptr<int> up2;
  xml::xmlSerialize(up1, "unique_ptr.xml", up1);
  xml::xmlDeserialize(up2, "unique_ptr.xml", up2);  // *up2 == {apple, 3.14}
  ```
  xml file:
  ```xml
  <serialize encoding="UTF-8">
      <std_unique_ptr>
          <std_map>
              <size_t size="1"/>
              <entry>
                  <std_string>apple</std_string>
                  <d val="3.140000"/>
              </entry>
          </std_map>
      </std_unique_ptr>
  </serialize>
  ```
- Base64 encoding/decoding
  ```cpp
  std::list<double> l1 = {3.21, 6.54};
  std::list<double> l2;
  xml::xmlSerialize(l1, "list.xml", true);
  xml::xmlDeserialize(l2, "list.xml", true);  // l2 == {3.21, 6.54}
  ```
  xml file:
  ```xml
  <serialize encoding="base64">
      <std_list>
          <size_t size="2"/>
          <d>My4yMTAwMDA=</d>
          <d>Ni41NDAwMDA=</d>
      </std_list>
  </serialize>
  ```

### Details
For user-defined types, the serialization/deserialization is implemented by macro, 
and class name and member variables are required as parameters of the macro.
Also, copy constructor and assignment shouldn't be deleted.
```cpp
struct Student {
    double d;
    std::string s;
    std::vector<int> v;
    Student() = default;
    Student(double d, std::string s, std::vector<int> v) : d(d), s(s), v(v) {}

    GENERATE_SERIALIZATION(Student, d, s, v);
};
```
