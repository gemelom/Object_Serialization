#ifndef SERIALIZE_BINARY_H
#define SERIALIZE_BINARY_H

#include <cstddef>
#include <ios>
#include <iostream>
#include <fstream>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>
#include <type_traits>
#include <utility>

namespace binary {
    template <class T>
    concept Arithmetic = std::is_arithmetic_v<T>;

    // std::is_arithmetic
    template <Arithmetic T>
    void serialize(const T &value, std::ofstream &ofs) {
        ofs.write(reinterpret_cast<const char *>(&value), sizeof(T));
    }

    template <Arithmetic T>
    void deserialize(T &value, std::ifstream &ifs) {
        ifs.read(reinterpret_cast<char *>(&value), sizeof(T));
    }

    // std::string
    inline void serialize(const std::string &value, std::ofstream &ofs) {
        size_t size = value.size();
        ofs.write(reinterpret_cast<const char *>(&size), sizeof(size));
        ofs.write(value.c_str(), size);
    }

    inline void deserialize(std::string &value, std::ifstream &ifs) {
        size_t size;
        ifs.read(reinterpret_cast<char *>(&size), sizeof(size));
        value.resize(size);
        ifs.read(&value[0], size);
    }

    // std::pair<T1, T2>
    template<class T1, class T2>
    void serialize(const std::pair<T1, T2> &value, std::ofstream &ofs) {
        serialize(value.first, ofs);
        serialize(value.second, ofs);
    }

    template<class T1, class T2>
    void deserialize(std::pair<T1, T2> &value, std::ifstream &ifs) {
        deserialize(value.first, ifs);
        deserialize(value.second, ifs);
    }

    // std::vector<T>
    template<class T>
    void serialize(const std::vector<T> &value, std::ofstream &ofs) {
        size_t size = value.size();
        ofs.write(reinterpret_cast<const char *>(&size), sizeof(size));
        for (int i = 0; i < size; i++) {
            serialize(value[i], ofs);
        }
    }

    template<class T>
    void deserialize(std::vector<T> &value, std::ifstream &ifs) {
        size_t size;
        ifs.read(reinterpret_cast<char *>(&size), sizeof(size));
        for (int i = 0; i < size; i++) {
            T item;
            deserialize(item, ifs);
            value.push_back(item);
        }
    }

    // std::list<T>
    template<class T>
    void serialize(const std::list<T> &value, std::ofstream &ofs) {
        size_t size = value.size();
        ofs.write(reinterpret_cast<const char *>(&size), sizeof(size));
        for (auto item = value.begin(); item != value.end(); item++) {
            serialize(*item, ofs);
        }
    }

    template<class T>
    void deserialize(std::list<T> &value, std::ifstream &ifs) {
        size_t size;
        ifs.read(reinterpret_cast<char *>(&size), sizeof(size));
        for (int i = 0; i < size; i++) {
            T item;
            deserialize(item, ifs);
            value.push_front(item);
        }
    }

    // std::set<T>
    template<class T>
    void serialize(const std::set<T> &value, std::ofstream &ofs) {
        size_t size = value.size();
        ofs.write(reinterpret_cast<const char *>(&size), sizeof(size));
        for (auto item = value.begin(); item != value.end(); item++) {
            serialize(*item, ofs);
        }
    }

    template<class T>
    void deserialize(std::set<T> &value, std::ifstream &ifs) {
        size_t size;
        ifs.read(reinterpret_cast<char *>(&size), sizeof(size));
        for (int i = 0; i < size; i++) {
            T item;
            deserialize(item, ifs);
            value.insert(item);
        }
    }

    // std::map<T1, T2>
    template<class T1, class T2>
    void serialize(const std::map<T1, T2> &value, std::ofstream &ofs) {
        size_t size = value.size();
        ofs.write(reinterpret_cast<const char *>(&size), sizeof(size));
        for (auto it = value.begin(); it != value.end(); it++) {
            serialize(it->first, ofs);
            serialize(it->second, ofs);
        }
    }

    template<class T1, class T2>
    void deserialize(std::map<T1, T2> &value, std::ifstream &ifs) {
        size_t size;
        ifs.read(reinterpret_cast<char *>(&size), sizeof(size));
        for (int i = 0; i < size; i++) {
            T1 t1;
            T2 t2;
            deserialize(t1, ifs);
            deserialize(t2, ifs);
            value[t1] = t2;
        }
    }

    // std::unique_ptr<T>
    template<class T>
    void serialize(const std::unique_ptr<T> &value, std::ofstream &ofs) {
        serialize(*value.get(), ofs);
    }

    template<class T>
    void deserialize(std::unique_ptr<T> &value, std::ifstream &ifs) {
        deserialize(*value.get(), ifs);
    }
    // binarySerialize && binaryDeserialize
    template<class T>
    void binarySerialize(const T &value, std::string &filename) {
        std::ofstream ofs(filename, std::ios::binary);
        if (!ofs) {
            std::cerr << "Error opening file when serilization." << std::endl;
            return;
        }
        serialize(value, ofs);
        ofs.close();
    }

    template<class T>
    void binaryDeserialize(T &value, std::string &filename) {
        std::ifstream ifs(filename, std::ios::binary);
        if (!ifs) {
            std::cerr << "Error opening file when deserilization." << std::endl;
            return;
        }
        deserialize(value, ifs);
        ifs.close();
    }

}

#endif // !SERIALIZE_BINARY_H

