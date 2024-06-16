#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <fstream>

#define MAKE_TUPLE(...) \
    auto to_tuple() const { \
        return std::tie(__VA_ARGS__); \
    }

#define GENERATE_SERIALIZATION(Class, ...)                                        \
    friend void binarySerialize(const Class& obj, const std::string& filename) {        \
        obj.binarySerialize(filename);                                                  \
    }                                                                             \
    friend void binaryDeserialize(Class& obj, const std::string& filename) {            \
        obj.binaryDeserialize(filename);                                                \
    }                                                                             \
    void binarySerialize(const std::string& filename) const {                           \
        std::ofstream ofs(filename, std::ios::binary);                            \
        if (!ofs) {                                                               \
            std::cerr << "Error opening file when serilization." << std::endl;    \
            return;                                                               \
        }                                                                         \
        apply([&](const auto&... args) { serialize_impl(ofs, args...); });        \
        ofs.close();                                                              \
    }                                                                             \
    void binaryDeserialize(const std::string& filename) {                               \
        std::ifstream ifs(filename, std::ios::binary);                            \
        if (!ifs) {                                                               \
            std::cerr << "Error opening file when deserilization." << std::endl;  \
            return;                                                               \
        }                                                                         \
        apply([&](auto&... args) { deserialize_impl(ifs, args...); });            \
        ifs.close();                                                              \
    }                                                                             \
    template <typename Func>                                                      \
    void apply(Func&& func) const {                                               \
        func(__VA_ARGS__);                                                        \
    }                                                                             \
    template <typename Func>                                                      \
    void apply(Func&& func) {                                                     \
        func(__VA_ARGS__);                                                        \
    }                                                                             \
    static void serialize_impl(std::ofstream& os) {}                              \
    static void deserialize_impl(std::ifstream& is) {}                            \
    template <typename T, typename... Args>                                       \
    static void serialize_impl(std::ofstream& os, const T& first, const Args&... rest) { \
        binary::serialize(first, os);                                             \
        serialize_impl(os, rest...);                                              \
    }                                                                             \
    template <typename T, typename... Args>                                       \
    static void deserialize_impl(std::ifstream& is, T& first, Args&... rest) {    \
        binary::deserialize(first, is);                                           \
        deserialize_impl(is, rest...);                                            \
    }                                                                             \
                                                                                  \
    friend void xmlSerialize(const Class& obj, const std::string& filename) {        \
        obj.xmlSerialize(filename);                                                  \
    }                                                                             \
    friend void xmlDeserialize(Class& obj, std::string filename) {            \
        obj.xmlDeserialize(filename);                                                \
    }                                                                             \
    void xmlSerialize(const std::string& filename) const {                           \
        tinyxml2::XMLDocument doc;                                                \
        tinyxml2::XMLElement *element = doc.NewElement("serialize");    \
        element->SetAttribute("encoding", "UTF-8");    \
        doc.InsertEndChild(element); \
        Class obj; \
        tinyxml2::XMLElement *element1 = doc.NewElement("user_defined");    \
        apply([&](const auto&... args) { element->InsertEndChild(xml_serialize_impl(doc, element1, args...)); });        \
        doc.SaveFile(filename.c_str());                                           \
    }                                                                             \
    void xmlDeserialize(std::string filename) {                            \
        tinyxml2::XMLDocument doc;                                                \
        doc.LoadFile(filename.c_str());                                           \
        tinyxml2::XMLElement *element = doc.FirstChildElement()->FirstChildElement(); \
        tinyxml2::XMLElement *it = element->FirstChildElement(); \
        apply([&](auto&... args) { xml_deserialize_impl(it, args...); });        \
    }                                                                             \
    static tinyxml2::XMLElement *xml_serialize_impl(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement *element) {return nullptr;}                              \
    static void xml_deserialize_impl(tinyxml2::XMLElement *element) {}                            \
    template <typename T, typename... Args>                                       \
    static tinyxml2::XMLElement *xml_serialize_impl(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement *element, const T& first, const Args&... rest) { \
        tinyxml2::XMLElement *ele1 = xml::serialize(first, doc, false);                                             \
        if (ele1 != nullptr) element->InsertEndChild(ele1);             \
        xml_serialize_impl(doc, element, rest...); \
        return element;                                                    \
    }                                                                             \
    template <typename T, typename... Args>                                       \
    static void xml_deserialize_impl(tinyxml2::XMLElement *element, T& first, Args&... rest) {    \
        if (element != nullptr) { \
            xml::deserialize(first, element, false);                                           \
        } \
        element = element->NextSiblingElement(); \
        xml_deserialize_impl(element, rest...);                                            \
    }                                                                             \


#endif // !COMMON_H
