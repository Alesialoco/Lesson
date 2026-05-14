#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <memory>

// Абстрактная реализация
template<typename T>
class Interface {
public:
    virtual ~Interface() = default;
    
    virtual void insertElement(const T& elem) = 0;
    virtual void deleteElement(const T& elem) = 0;
    virtual bool findElement(const T& elem) const = 0;
    virtual size_t getCount() const = 0;
    virtual void printElements() const = 0;
    virtual std::unique_ptr<Interface<T>> makeCopy() const = 0;
};

// Конкретная реализация: Массив
template<typename T>
class Array : public Interface<T> {
private:
    std::vector<T> data;
    
public:
    void insertElement(const T& elem) override {
        if (!findElement(elem)) {
            data.push_back(elem);
        }
    }
    
    void deleteElement(const T& elem) override {
        auto it = std::find(data.begin(), data.end(), elem);
        if (it != data.end()) {
            data.erase(it);
        }
    }
    
    bool findElement(const T& elem) const override {
        return std::find(data.begin(), data.end(), elem) != data.end();
    }
    
    size_t getCount() const override {
        return data.size();
    }
    
    void printElements() const override {
        std::cout << "{ ";
        for (size_t i = 0; i < data.size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << data[i];
        }
        std::cout << " }";
    }
    
    std::unique_ptr<Interface<T>> makeCopy() const override {
        auto copy = std::make_unique<Array<T>>();
        copy->data = this->data;
        return copy;
    }
    
    const std::vector<T>& getData() const {
        return data;
    }
};

// Конкретная реализация: Хэш-таблица
template<typename T>
class HashTable : public Interface<T> {
private:
    std::unordered_set<T> data;
    
public:
    void insertElement(const T& elem) override {
        data.insert(elem);
    }
    
    void deleteElement(const T& elem) override {
        data.erase(elem);
    }
    
    bool findElement(const T& elem) const override {
        return data.find(elem) != data.end();
    }
    
    size_t getCount() const override {
        return data.size();
    }
    
    void printElements() const override {
        std::cout << "{ ";
        size_t count = 0;
        for (const auto& element : data) {
            if (count > 0) std::cout << ", ";
            std::cout << element;
            ++count;
        }
        std::cout << " }";
    }
    
    std::unique_ptr<Interface<T>> makeCopy() const override {
        auto copy = std::make_unique<HashTable<T>>();
        copy->data = this->data;
        return copy;
    }
    
    const std::unordered_set<T>& getData() const {
        return data;
    }
};

// Абстракция - Множество
template<typename T>
class Set {
private:
    std::unique_ptr<Interface<T>> pointer;
    static const size_t THRESHOLD = 30;
    
    void selectOptimal() {
        if (!pointer) {
            pointer = std::make_unique<Array<T>>();
            return;
        }
        
        size_t currentSize = pointer->getCount();
        bool isArray = (dynamic_cast<Array<T>*>(pointer.get()) != nullptr);
        bool isHash = (dynamic_cast<HashTable<T>*>(pointer.get()) != nullptr);
        
        if (currentSize > THRESHOLD && isArray) {
            auto arrayImpl = dynamic_cast<Array<T>*>(pointer.get());
            auto elements = arrayImpl->getData();
            pointer = std::make_unique<HashTable<T>>();
            for (const auto& elem : elements) {
                pointer->insertElement(elem);
            }
        }
        else if (currentSize <= THRESHOLD && isHash) {
            auto hashImpl = dynamic_cast<HashTable<T>*>(pointer.get());
            auto elements = hashImpl->getData();
            pointer = std::make_unique<Array<T>>();
            for (const auto& elem : elements) {
                pointer->insertElement(elem);
            }
        }
    }
    
public:
    Set() {
        pointer = std::make_unique<Array<T>>();
    }
    
    Set(const Set& rhs) {
        pointer = rhs.pointer->makeCopy();
    }
    
    Set& operator=(const Set& rhs) {
        if (this != &rhs) {
            pointer = rhs.pointer->makeCopy();
        }
        return *this;
    }
    
    void add(const T& element) {
        pointer->insertElement(element);
        selectOptimal();
    }
    
    void remove(const T& element) {
        pointer->deleteElement(element);
        selectOptimal();
    }
    
    bool contains(const T& element) const {
        return pointer->findElement(element);
    }
    
    void print() const {
        pointer->printElements();
        
        if (dynamic_cast<Array<T>*>(pointer.get())) {
            std::cout << " [Array, size=" << pointer->getCount() << "]";
        } else if (dynamic_cast<HashTable<T>*>(pointer.get())) {
            std::cout << " [HashTable, size=" << pointer->getCount() << "]";
        }
    }
    
    Set unionWith(const Set& rhs) const {
        Set result = *this;
        
        if (auto otherArray = dynamic_cast<Array<T>*>(rhs.pointer.get())) {
            for (const auto& elem : otherArray->getData()) {
                result.add(elem);
            }
        } else if (auto otherHash = dynamic_cast<HashTable<T>*>(rhs.pointer.get())) {
            for (const auto& elem : otherHash->getData()) {
                result.add(elem);
            }
        }
        
        return result;
    }
    
    Set intersectWith(const Set& rhs) const {
        Set result;
        
        if (auto thisArray = dynamic_cast<Array<T>*>(pointer.get())) {
            for (const auto& elem : thisArray->getData()) {
                if (rhs.contains(elem)) {
                    result.add(elem);
                }
            }
        } else if (auto thisHash = dynamic_cast<HashTable<T>*>(pointer.get())) {
            for (const auto& elem : thisHash->getData()) {
                if (rhs.contains(elem)) {
                    result.add(elem);
                }
            }
        }
        
        return result;
    }
};

int main() {   
    Set<int> set1;
    Set<int> set2;
    
    std::cout << "Set1: add elements 0-14\n";
    for (int i = 0; i < 15; ++i) {
        set1.add(i);
    }
    std::cout << "Set1: ";
    set1.print();
    std::cout << "\n\n";
    
    std::cout << "Set2: add elements 10-24\n";
    for (int i = 10; i < 25; ++i) {
        set2.add(i);
    }
    std::cout << "Set2: ";
    set2.print();
    std::cout << "\n\n";
    
    std::cout << "Set1 contains 7: " << (set1.contains(7) ? "Yes" : "No") << std::endl;
    std::cout << "Set1 contains 20: " << (set1.contains(20) ? "Yes" : "No") << std::endl;
    std::cout << "\n";
    
    set1.remove(7);
    std::cout << "Set1 after remove: ";
    set1.print();
    std::cout << "\n";
    std::cout << "Set1 contains 7: " << (set1.contains(7) ? "Yes" : "No") << std::endl;
    std::cout << "\n";
    
    Set<int> unionSet = set1.unionWith(set2);
    std::cout << "Union: ";
    unionSet.print();
    std::cout << "\n\n";
    
    Set<int> intersectSet = set1.intersectWith(set2);
    std::cout << "Intersection: ";
    intersectSet.print();
    std::cout << "\n\n";
    
    Set<int> set3;
    std::cout << "Set3: add 35 elements\n";
    for (int i = 0; i < 35; ++i) {
        set3.add(i);
    }
    std::cout << "Set3: ";
    set3.print();
    std::cout << "\n\n";
    
    std::cout << "Set3: delete 20 elements\n";
    for (int i = 0; i < 20; ++i) {
        set3.remove(i);
    }
    std::cout << "Set3: ";
    set3.print();
    
    return 0;
}