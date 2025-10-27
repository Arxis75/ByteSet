#include <memory>
#include <iostream>

class MyClass {
protected:
    MyClass(int x) : value(x) {
        std::cout << "MyClass constructed with " << value << "\n";
    }

public:
    int value;

    // Static factory
    static std::shared_ptr<MyClass> Factory(int x) {
        return std::make_shared<MyClass>(x); // ✅ can access protected constructor
    }
};

int main() {
    auto obj = MyClass::Factory(42);
    std::cout << "Value: " << obj->value << "\n";
}