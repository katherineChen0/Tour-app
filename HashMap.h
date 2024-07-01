#ifndef HashMap_h
#define HashMap_h

#include <vector>
#include <list>
#include <string>

template <typename T>
class HashMap {
public:
    HashMap(double max_load = 0.75); // constructor
    ~HashMap(); // destructor; deletes all of the items in the hashmap
    int size() const { return m_size; }
    void insert(const std::string& key, const T& value);
    const T* find(const std::string& key) const;
    
    T* find(const std::string& key) {
     const auto& hm = *this;
     return const_cast<T*>(hm.find(key));
     }
    
    T& operator[](const std::string& key);
    HashMap(const HashMap&) = delete;
    HashMap& operator=(const HashMap&) = delete;

private:
    struct Item {
        Item(const std::string& key, const T& value) : m_key(key), m_value(value) {}
        std::string m_key;
        T m_value;
    };

    std::vector<std::list<Item*>> m_hashTable;
    int m_size;
    double m_maxLoad;
    int m_nbuckets;

    int hash(const std::string& key) const { return std::hash<std::string>()(key) % m_nbuckets; }
    void rehash();
};

template <typename T>
HashMap<T>::HashMap(double max_load) : m_hashTable(10), m_size(0), m_maxLoad(max_load), m_nbuckets(10) {}

template <typename T>
HashMap<T>::~HashMap() {
    for (auto& bucket : m_hashTable) {
        for (auto& item : bucket) {
            delete item;
        }
    }
}

template <typename T>
void HashMap<T>::insert(const std::string& key, const T& value) {
    for (auto& item : m_hashTable[hash(key)]) {
        if (item->m_key == key) {
            item->m_value = value;
            return;
        }
    }
    m_hashTable[hash(key)].push_back(new Item(key, value));
    m_size++;
    if (m_size * 1.0 / m_nbuckets >= m_maxLoad) {
        rehash();
    }
}

template <typename T>
const T* HashMap<T>::find(const std::string& key) const {
    for (const auto& obj : m_hashTable[hash(key)]) {
        if (obj->m_key == key) {
            return &(obj->m_value);
        }
    }
    return nullptr;
}

template <typename T>
T& HashMap<T>::operator[](const std::string& key) {
    int index = hash(key);
    for (auto& obj : m_hashTable[index]) {
        if (obj->m_key == key) {
            return obj->m_value;
        }
    }
    // If the key doesn't exist, insert it with a default value
    T defaultValue = T(); // Initialize default value
    m_hashTable[index].push_back(new Item(key, defaultValue));
    m_size++;
    if (m_size * 1.0 / m_nbuckets >= m_maxLoad) {
        rehash();
    }
    return m_hashTable[index].back()->m_value;
}


template <typename T>
void HashMap<T>::rehash(){
    m_nbuckets*=2;
    std::vector<std::list<Item*>> newTable(m_nbuckets);
    for(auto bucket : m_hashTable){
        for(auto obj : bucket){
            newTable[hash(obj->m_key)].push_back(obj);
        }
    }
    m_hashTable = newTable;
}

#endif /* HashMap_h */
