#include "serialize_binary.h"
#include "serialize_xml.h"
#include "common.h"
#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

struct Student {
    double d;
    std::string s;
    std::vector<int> v;
    Student() = default;
    Student(double d, std::string s, std::vector<int> v) : d(d), s(s), v(v) {}

    GENERATE_SERIALIZATION(Student, d, s, v);
};

int main() {
    std::cout << "===== Binary Serialize Test =====" << std::endl;

    int n0 = 256, n1;
    std::string basedir = "./saves/";
    std::string filename = basedir + "num.dat";
    binary::binarySerialize(n0, filename);
    binary::binaryDeserialize(n1, filename);
    std::cout << "std::is_arithmetic  ";
    std::cout << "serialize: " << n0 << ", deserialize: " << n1 << std::endl;

    std::string s0 = "hello", s1;
    filename = basedir + "str.dat";
    binary::binarySerialize(s0, filename);
    binary::binaryDeserialize(s1, filename);
    std::cout << "std::string         ";
    std::cout << "serialize: " << s0 << ", deserialize: " << s1 << std::endl;

    std::pair<int, int> pp1 = {2, 3}, pp2;
    filename = basedir + "pair.dat";
    binary::binarySerialize(pp1, filename);
    binary::binaryDeserialize(pp2, filename);
    std::cout << "std::pair           ";
    std::cout << "serialize: (" << pp1.first << "," << pp1.second << "), deserialize: (" << pp2.first << "," << pp2.second << ")" << std::endl;

    std::vector<std::string> v1 = {"mamba", "out"}, v2;
    filename = basedir + "vector.dat";
    binary::binarySerialize(v1, filename);
    std::cout << "std::vector         ";
    std::cout << "serialize: {";
    for (int i = 0; i < v1.size(); i++) {
        std::cout << v1[i];
        if (i != v1.size() - 1) std::cout << ",";
        else std::cout << "}, ";
    }
    binary::binaryDeserialize(v2, filename);
    std::cout << "deserialize: {";
    for (int i = 0; i < v2.size(); i++) {
        std::cout << v2[i];
        if (i != v2.size() - 1) std::cout << ",";
        else std::cout << "}" << std::endl;
    }

    std::list<double> l1 = {1.23, 4.56}, l2;
    filename = basedir + "list.dat";
    binary::binarySerialize(l1, filename);
    std::cout << "std::list           ";
    std::cout << "serialize: {";
    int cnt = 0;
    for (auto it = l1.begin(); it != l1.end(); it++) {
        std::cout << *it;
        if (++cnt != l1.size()) std::cout << ",";
        else std::cout << "}, ";
    }
    binary::binaryDeserialize(l2, filename);
    std::cout << "deserialize: {";
    cnt = 0;
    for (auto it = l2.begin(); it != l2.end(); it++) {
        std::cout << *it;
        if (++cnt != l2.size()) std::cout << ",";
        else std::cout << "}" << std::endl;
    }

    std::set<long> st1 = {114514, 1919810}, st2;
    filename = basedir + "set.dat";
    std::cout << "std::set            ";
    binary::binarySerialize(st1, filename);
    std::cout << "serialize: {";
    cnt = 0;
    for (auto it = st1.begin(); it != st1.end(); it++) {
        std::cout << *it;
        if (++cnt != st1.size()) std::cout << ",";
        else std::cout << "}, ";
    }
    binary::binaryDeserialize(st2, filename);
    std::cout << "deserialize: {";
    cnt = 0;
    for (auto it = st2.begin(); it != st2.end(); it++) {
        std::cout << *it;
        if (++cnt != st2.size()) std::cout << ",";
        else std::cout << "}" << std::endl;
    }

    std::map<int, std::string> m1, m2;
    filename = basedir + "map.dat";
    m1[114] = "hello";
    m1[514] = "world";
    binary::binarySerialize(m1, filename);
    std::cout << "std::map            ";
    std::cout << "serialize: {";
    cnt = 0;
    for (auto it = m1.begin(); it != m1.end(); it++) {
        std::cout << "{" << it->first << "," << it->second << "}";
        if (++cnt != m1.size()) std::cout << ",";
        else std::cout<< "}, ";
    }
    binary::binaryDeserialize(m2, filename);
    std::cout << "deserialize: {";
    cnt = 0;
    for (auto it = m2.begin(); it != m2.end(); it++) {
        std::cout << "{" << it->first << "," << it->second << "}";
        if (++cnt != m1.size()) std::cout << ",";
        else std::cout<< "}" << std::endl;
    }

    std::cout << "====== XML Serialize Test ======" << std::endl;
    n0 = 100;
    filename = basedir + "num.xml";
    xml::xmlSerialize(n0, filename);
    xml::xmlDeserialize(n1, filename);
    std::cout << "std::is_arithmetic  ";
    std::cout << "serialize: " << n0 << ", deserialize: " << n1 << std::endl;

    s0 = "world";
    filename = basedir + "str.xml";
    xml::xmlSerialize(s0, filename);
    xml::xmlDeserialize(s1, filename);
    std::cout << "std::string         ";
    std::cout << "serialize: " << s0 << ", deserialize: " << s1 << std::endl;

    pp1.first = 200;
    pp1.second = 300;
    filename = basedir + "pair.xml";
    xml::xmlSerialize(pp1, filename);
    xml::xmlDeserialize(pp2, filename);
    std::cout << "std::pair           ";
    std::cout << "serialize: (" << pp1.first << "," << pp1.second << "), deserialize: (" << pp2.first << "," << pp2.second << ")" << std::endl;

    v1 = {"what", "can", "i", "say"};
    v2.clear();
    filename = basedir + "vector.xml";
    xml::xmlSerialize(v1, filename);
    std::cout << "std::vector         ";
    std::cout << "serialize: {";
    for (int i = 0; i < v1.size(); i++) {
        std::cout << v1[i];
        if (i != v1.size() - 1) std::cout << ",";
        else std::cout << "}, ";
    }
    xml::xmlDeserialize(v2, filename);
    std::cout << "deserialize: {";
    for (int i = 0; i < v2.size(); i++) {
        std::cout << v2[i];
        if (i != v2.size() - 1) std::cout << ",";
        else std::cout << "}" << std::endl;
    }

    l1 = {3.21, 6.54};
    l2.clear();
    filename = basedir + "list.xml";
    xml::xmlSerialize(l1, filename);
    std::cout << "std::list           ";
    std::cout << "serialize: {";
    cnt = 0;
    for (auto it = l1.begin(); it != l1.end(); it++) {
        std::cout << *it;
        if (++cnt != l1.size()) std::cout << ",";
        else std::cout << "}, ";
    }
    xml::xmlDeserialize(l2, filename);
    std::cout << "deserialize: {";
    cnt = 0;
    for (auto it = l2.begin(); it != l2.end(); it++) {
        std::cout << *it;
        if (++cnt != l2.size()) std::cout << ",";
        else std::cout << "}" << std::endl;
    }

    st1 = {111, 1111, 66666}, st2.clear();
    filename = basedir + "set.xml";
    xml::xmlSerialize(st1, filename);
    std::cout << "std::set            ";
    std::cout << "serialize: {";
    cnt = 0;
    for (auto it = st1.begin(); it != st1.end(); it++) {
        std::cout << *it;
        if (++cnt != st1.size()) std::cout << ",";
        else std::cout << "}, ";
    }
    xml::xmlDeserialize(st2, filename);
    std::cout << "deserialize: {";
    cnt = 0;
    for (auto it = st2.begin(); it != st2.end(); it++) {
        std::cout << *it;
        if (++cnt != st2.size()) std::cout << ",";
        else std::cout << "}" << std::endl;
    }

    m1.clear();
    m2.clear();
    filename = basedir + "map.xml";
    m1[114] = "hello";
    m1[514] = "world";
    xml::xmlSerialize(m1, filename);
    std::cout << "std::map            ";
    std::cout << "serialize: {";
    cnt = 0;
    for (auto it = m1.begin(); it != m1.end(); it++) {
        std::cout << "{" << it->first << "," << it->second << "}";
        if (++cnt != m1.size()) std::cout << ",";
        else std::cout<< "}, ";
    }
    xml::xmlDeserialize(m2, filename);
    std::cout << "deserialize: {";
    cnt = 0;
    for (auto it = m2.begin(); it != m2.end(); it++) {
        std::cout << "{" << it->first << "," << it->second << "}";
        if (++cnt != m1.size()) std::cout << ",";
        else std::cout<< "}" << std::endl;
    }

    std::cout << "===== Base64 Serialize Test =====" << std::endl;
    n0 = 100;
    filename = basedir + "num.base64.xml";
    xml::xmlSerialize(n0, filename, true);
    xml::xmlDeserialize(n1, filename, true);
    std::cout << "std::is_arithmetic  ";
    std::cout << "serialize: " << n0 << ", deserialize: " << n1 << std::endl;

    s0 = "world";
    filename = basedir + "str.base64.xml";
    xml::xmlSerialize(s0, filename, true);
    xml::xmlDeserialize(s1, filename, true);
    std::cout << "std::string         ";
    std::cout << "serialize: " << s0 << ", deserialize: " << s1 << std::endl;

    pp1.first = 200;
    pp1.second = 300;
    filename = basedir + "pair.base64.xml";
    xml::xmlSerialize(pp1, filename, true);
    xml::xmlDeserialize(pp2, filename, true);
    std::cout << "std::pair           ";
    std::cout << "serialize: (" << pp1.first << "," << pp1.second << "), deserialize: (" << pp2.first << "," << pp2.second << ")" << std::endl;

    v1 = {"what", "can", "i", "say"};
    v2.clear();
    filename = basedir + "vector.base64.xml";
    xml::xmlSerialize(v1, filename, true);
    std::cout << "std::vector         ";
    std::cout << "serialize: {";
    for (int i = 0; i < v1.size(); i++) {
        std::cout << v1[i];
        if (i != v1.size() - 1) std::cout << ",";
        else std::cout << "}, ";
    }
    xml::xmlDeserialize(v2, filename, true);
    std::cout << "deserialize: {";
    for (int i = 0; i < v2.size(); i++) {
        std::cout << v2[i];
        if (i != v2.size() - 1) std::cout << ",";
        else std::cout << "}" << std::endl;
    }

    l1 = {3.21, 6.54};
    l2.clear();
    filename = basedir + "list.base64.xml";
    xml::xmlSerialize(l1, filename, true);
    std::cout << "std::list           ";
    std::cout << "serialize: {";
    cnt = 0;
    for (auto it = l1.begin(); it != l1.end(); it++) {
        std::cout << *it;
        if (++cnt != l1.size()) std::cout << ",";
        else std::cout << "}, ";
    }
    xml::xmlDeserialize(l2, filename, true);
    std::cout << "deserialize: {";
    cnt = 0;
    for (auto it = l2.begin(); it != l2.end(); it++) {
        std::cout << *it;
        if (++cnt != l2.size()) std::cout << ",";
        else std::cout << "}" << std::endl;
    }

    st1 = {111, 1111, 66666}, st2.clear();
    filename = basedir + "set.base64.xml";
    xml::xmlSerialize(st1, filename, true);
    std::cout << "std::set            ";
    std::cout << "serialize: {";
    cnt = 0;
    for (auto it = st1.begin(); it != st1.end(); it++) {
        std::cout << *it;
        if (++cnt != st1.size()) std::cout << ",";
        else std::cout << "}, ";
    }
    xml::xmlDeserialize(st2, filename, true);
    std::cout << "deserialize: {";
    cnt = 0;
    for (auto it = st2.begin(); it != st2.end(); it++) {
        std::cout << *it;
        if (++cnt != st2.size()) std::cout << ",";
        else std::cout << "}" << std::endl;
    }

    m1.clear();
    m2.clear();
    filename = basedir + "map.base64.xml";
    m1[114] = "hello";
    m1[514] = "world";
    xml::xmlSerialize(m1, filename, true);
    std::cout << "std::map            ";
    std::cout << "serialize: {";
    cnt = 0;
    for (auto it = m1.begin(); it != m1.end(); it++) {
        std::cout << "{" << it->first << "," << it->second << "}";
        if (++cnt != m1.size()) std::cout << ",";
        else std::cout<< "}, ";
    }
    xml::xmlDeserialize(m2, filename, true);
    std::cout << "deserialize: {";
    cnt = 0;
    for (auto it = m2.begin(); it != m2.end(); it++) {
        std::cout << "{" << it->first << "," << it->second << "}";
        if (++cnt != m1.size()) std::cout << ",";
        else std::cout<< "}" << std::endl;
    }

    std::cout << "==== User Defined Class Serialize Test ====" << std::endl;
    Student stu1(3.14, "Hello", {1, 2, 3, 4}), stu2;
    filename = basedir + "stu.dat";
    binarySerialize(stu1, filename);
    std::cout << "user_defined_bin    ";
    std::cout << "serialize: {" << stu1.d << "," << stu1.s << ",";
    for (int i = 0; i < stu1.v.size(); i++) {
        if (i == 0) std::cout << "{";
        std::cout << stu1.v[i];
        if (i != stu1.v.size() - 1) std::cout << ",";
        else std::cout << "}}, ";
    }
    binaryDeserialize(stu2, filename);
    std::cout << "deserialize: {" << stu2.d << "," << stu2.s << ",";
    for (int i = 0; i < stu1.v.size(); i++) {
        if (i == 0) std::cout << "{";
        std::cout << stu1.v[i];
        if (i != stu1.v.size() - 1) std::cout << ",";
        else std::cout << "}}" << std::endl;
    }

    filename = basedir + "stu.xml";
    xmlSerialize(stu1, filename);
    std::cout << "user_defined_xml    ";
    std::cout << "serialize: {" << stu1.d << "," << stu1.s << ",";
    for (int i = 0; i < stu1.v.size(); i++) {
        if (i == 0) std::cout << "{";
        std::cout << stu1.v[i];
        if (i != stu1.v.size() - 1) std::cout << ",";
        else std::cout << "}}, ";
    }
    Student stu3;
    xmlDeserialize(stu3, filename);
    std::cout << "deserialize: {" << stu3.d << "," << stu3.s << ",";
    for (int i = 0; i < stu1.v.size(); i++) {
        if (i == 0) std::cout << "{";
        std::cout << stu1.v[i];
        if (i != stu1.v.size() - 1) std::cout << ",";
        else std::cout << "}}" << std::endl;
    }

    std::cout << "====== Smart Pointers Serialize Test ======" << std::endl;
    std::unique_ptr<std::string> up3 = std::make_unique<std::string>("OOP");
    filename = basedir + "unique_ptr.dat";
    binary::binarySerialize(up3, filename);
    std::cout << "std_unique_ptr_bin  ";
    std::cout << "serialize: {" << *up3.get() << "}, ";
    std::unique_ptr<std::string> up4 = std::make_unique<std::string>();
    binary::binaryDeserialize(up4, filename);
    std::cout << "deserialize: {" << *up4.get() << "}" << std::endl;

    std::unique_ptr<std::vector<int>> up1 = std::make_unique<std::vector<int>>(std::initializer_list<int>{2,0,2,4});
    filename = basedir + "unique_ptr.xml";
    std::cout << "std_unique_ptr_xml  ";
    xml::xmlSerialize(up1, filename);
    std::cout << "serialize: {";
    for (int i = 0; i < up1->size(); i++) {
        std::cout << up1->at(i);
        if (i != up1->size() - 1) std::cout << ",";
        else std::cout << "}, ";
    }
    std::unique_ptr<std::vector<int>> up2 = std::make_unique<std::vector<int>>();
    xml::xmlDeserialize(up2, filename);
    std::cout << "deserialize: {";
    for (int i = 0; i < up2->size(); i++) {
        std::cout << up2->at(i);
        if (i != up2->size() - 1) std::cout << ",";
        else std::cout << "}" << std::endl;
    }

}

