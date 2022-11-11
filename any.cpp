#include <iostream>
#include <type_traits>
#include <typeinfo>

using namespace std;

class any {
    public:
        // constructor
        template <typename U>
        any(const U& object) : ptr(new Derived<U>(object)) {}
        
        // assignment operator
        template <typename U>
        auto& operator=(const U& object) {
            delete ptr;
            ptr = new Derived<U>(object);
            return ptr;
        }

        // сopy constructor
        any(const any& other) : ptr(other.ptr->getCopy()) {}

        // destructor
        ~any() {
            delete ptr;
        }

        // cast
        template<typename U>
        U cast() const
        {
            if(typeid(U) != ptr->type_info())
                throw std::runtime_error("Bad any cast");
            return static_cast<Derived<U>*>(ptr)->object;
        }
        
    private:
        struct Base {
            virtual const std::type_info& type_info() const {};
            virtual Base* getCopy() {};
            virtual ~Base() {}
        };
        template <typename T>
        struct Derived : public Base {
            T object;
            const std::type_info& type_info() const override {
                return typeid(object);
            }
            Base* getCopy() override {
                return new Derived<T>(object);
            }
            Derived(const T& object) : object(object) {}
        };
        Base* ptr = nullptr;
};


int main()
{
    any a(6);
    a = std::string("Hello");
    any b(a);

    try {
        cout << b.cast<std::string>() << endl;
    } catch(...) {
        cout << "Bad Any cast" << endl;
    }
    

    return 0;
}