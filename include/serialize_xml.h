#ifndef SERIALIZE_XML_H
#define SERIALIZE_XML_H

#include "tinyxml2.h"
#include "base64.h"
#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <system_error>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

// template <typename T>
// struct is_string_type : std::false_type {};
//
// template <>
// struct is_string_type<std::string> : std::true_type {};

namespace xml {
    template <class T>
    concept Arithmetic = std::is_arithmetic_v<T>;

    template <class T>
    concept UserDefined = std::is_class_v<T>;
    // concept UserDefined = std::is_class_v<T> && requires(T t) {
    //     { t.to_tuple() } -> std::convertible_to<std::tuple<>>;
    // };

    template <class T>
    void xmlSerialize(T &value, std::string filename, bool base64 = false);

    template <class T>
    void xmlDeserialize(T &value, std::string filename, bool base64 = false);

    // std::is_arithmetic
    template <Arithmetic T>
    tinyxml2::XMLElement *serialize(const T &value, tinyxml2::XMLDocument &doc, bool base64) {
        tinyxml2::XMLElement *element = doc.NewElement(typeid(value).name());
        if (base64) {
            element->SetText(base64::encode(std::to_string(value)).c_str());
        } else {
            element->SetAttribute("val", std::to_string(value).c_str());
        }
        return element;
    }

    template <Arithmetic T>
    void deserialize(T &value, tinyxml2::XMLElement *element, bool base64) {
        if (base64) {
            std::string str(element->GetText());
            str = base64::decode(str);
            value = static_cast<T>(std::stod(str));
        } else {
            std::string str(element->FirstAttribute()->Value());
            value = static_cast<T>(std::stod(str));
        }
    }

    // std::string
    inline tinyxml2::XMLElement *serialize(const std::string &value, tinyxml2::XMLDocument &doc, bool base64) {
        tinyxml2::XMLElement *element = doc.NewElement("std_string");
        if (base64) {
            element->SetText(base64::encode(value).c_str());
        } else {
            element->SetText(value.c_str());
        }
        return element;
    }

    inline void deserialize(std::string &value, tinyxml2::XMLElement *element, bool base64) {
        if (base64) value = base64::decode(element->GetText());
        else value = element->GetText();
    }

    // std::pair
    template<class T1, class T2>
    tinyxml2::XMLElement *serialize(const std::pair<T1, T2> &value, tinyxml2::XMLDocument &doc, bool base64) {
        tinyxml2::XMLElement *element = doc.NewElement("std_pair");
        tinyxml2::XMLElement *first = serialize(value.first, doc, base64);
        tinyxml2::XMLElement *second = serialize(value.second, doc, base64);
        tinyxml2::XMLElement *f = doc.NewElement("first");
        tinyxml2::XMLElement *s = doc.NewElement("second");

        f->InsertEndChild(first);
        s->InsertEndChild(second);
        element->InsertEndChild(f);
        element->InsertEndChild(s);
        return element;
    }

    template<class T1, class T2>
    void deserialize(std::pair<T1, T2> &value, tinyxml2::XMLElement *element, bool base64) {
        deserialize(value.first, element->FirstChildElement("first")->FirstChildElement(), base64);
        deserialize(value.second, element->FirstChildElement("second")->FirstChildElement(), base64);
    }

    // std::vector
    template<class T>
    tinyxml2::XMLElement *serialize(const std::vector<T> &value, tinyxml2::XMLDocument &doc, bool base64) {
        tinyxml2::XMLElement *element = doc.NewElement("std_vector");
        size_t _size = value.size();
        tinyxml2::XMLElement *size = element->InsertNewChildElement("size_t");
        size->SetAttribute("size", _size);
        for (int i = 0; i < _size; i++) {
            element->InsertEndChild(serialize(value[i], doc, base64));
        }
        return element;
    }

    template<class T>
    void deserialize(std::vector<T> &value, tinyxml2::XMLElement *element, bool base64) {
        int size;
        tinyxml2::XMLElement *it = element->FirstChildElement();
        deserialize(size, it, false);
        for (int i = 0; i < size; i++) {
            T item;
            it = it->NextSiblingElement();
            deserialize(item, it, base64);
            value.push_back(item);
        }
    }

    // std::list
    template<class T>
    tinyxml2::XMLElement *serialize(const std::list<T> &value, tinyxml2::XMLDocument &doc, bool base64) {
        tinyxml2::XMLElement *element = doc.NewElement("std_list");
        size_t _size = value.size();
        tinyxml2::XMLElement *size = element->InsertNewChildElement("size_t");
        size->SetAttribute("size", _size);
        for (auto it = value.begin(); it != value.end(); it++) {
            element->InsertEndChild(serialize(*it, doc, base64));
        }
        return element;
    }

    template<class T>
    void deserialize(std::list<T> &value, tinyxml2::XMLElement *element, bool base64) {
        int size;
        tinyxml2::XMLElement *it = element->FirstChildElement();
        deserialize(size, it, false);
        for (int i = 0; i < size; i++) {
            T item;
            it = it->NextSiblingElement();
            deserialize(item, it, base64);
            value.push_back(item);
        }
    }

    // std::set
    template<class T>
    tinyxml2::XMLElement *serialize(const std::set<T> &value, tinyxml2::XMLDocument &doc, bool base64) {
        tinyxml2::XMLElement *element = doc.NewElement("std_set");
        size_t _size = value.size();
        tinyxml2::XMLElement *size = element->InsertNewChildElement("size_t");
        size->SetAttribute("size", _size);
        for (auto it = value.begin(); it != value.end(); it++) {
            element->InsertEndChild(serialize(*it, doc, base64));
        }
        return element;
    }

    template<class T>
    void deserialize(std::set<T> &value, tinyxml2::XMLElement *element, bool base64) {
        int size;
        tinyxml2::XMLElement *it = element->FirstChildElement();
        deserialize(size, it, false);
        for (int i = 0; i < size; i++) {
            T item;
            it = it->NextSiblingElement();
            deserialize(item, it, base64);
            value.insert(item);
        }
    }

    // std::map
    template<class T1, class T2>
    tinyxml2::XMLElement *serialize(const std::map<T1, T2> &value, tinyxml2::XMLDocument &doc, bool base64) {
        tinyxml2::XMLElement *element = doc.NewElement("std_map");
        size_t _size = value.size();
        tinyxml2::XMLElement *size = element->InsertNewChildElement("size_t");
        size->SetAttribute("size", _size);
        for (auto it = value.begin(); it != value.end(); it++) {
            tinyxml2::XMLElement *entry = doc.NewElement("entry");
            entry->InsertEndChild(serialize(it->first, doc, base64));
            entry->InsertEndChild(serialize(it->second, doc, base64));
            element->InsertEndChild(entry);
        }
        return element;
    }

    template<class T1, class T2>
    void deserialize(std::map<T1, T2> &value, tinyxml2::XMLElement *element, bool base64) {
        int size;
        tinyxml2::XMLElement *it = element->FirstChildElement();
        deserialize(size, it, false);
        for (int i = 0; i < size; i++) {
            T1 k;
            T2 v;
            it = it->NextSiblingElement();
            deserialize(k, it->FirstChildElement(), base64);
            deserialize(v, it->LastChildElement(), base64);
            value[k] = v;
        }
    }

    // std::unique_ptr
    template<class T>
    tinyxml2::XMLElement *serialize(const std::unique_ptr<T> &value, tinyxml2::XMLDocument &doc, bool base64) {
        tinyxml2::XMLElement *element = doc.NewElement("std_unique_ptr");
        element->InsertEndChild(serialize(*value.get(), doc, base64));
        return element;
    }

    template<class T>
    void deserialize(std::unique_ptr<T> &value, tinyxml2::XMLElement *element, bool base64) {
        deserialize(*value.get(), element->FirstChildElement(), base64);
    }

    // xmlSerialize && xmlDeserialize
    template <class T>
    void xmlSerialize(T &value, std::string filename, bool base64) {
        tinyxml2::XMLDocument doc;
        tinyxml2::XMLElement *element = doc.NewElement("serialize");
        if (base64) element->SetAttribute("encoding", "base64");
        else element->SetAttribute("encoding", "UTF-8");
        doc.InsertEndChild(element);
        element->InsertEndChild(serialize(value, doc, base64));
        doc.SaveFile(filename.c_str());
    }

    template<class T>
    void xmlDeserialize(T &value, std::string filename, bool base64) {
        tinyxml2::XMLDocument doc;
        doc.LoadFile(filename.c_str());
        // doc.Print();
        tinyxml2::XMLElement *element = doc.FirstChildElement()->FirstChildElement();

        deserialize(value, element, base64);
    }
}

#endif // !SERIALIZE_XML_H
