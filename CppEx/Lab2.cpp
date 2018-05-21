/**
* Cpp Lab2
* @since C++ 11
*/
#include <iostream>
#include <utility> // std::pair
#include <complex>
#include <sstream>
#include <string>
#include <cstdio>

/**
 * 编程题1
 * 计数器
 */
class CounterType {
private:
    // C++ 11
    size_t count = 0;
public:
    CounterType(): count(0) {

    }

    CounterType(int cnt) {
        this->count = cnt < 0 ? 0 : cnt;
    }

    /**
     * 获得计数器当前的值
     */
    size_t getCount() const {
        return count;
    }

    /**
     * 计数器 +1
     * ! be careful overflow
     */
    size_t addOne() {
        return ++count;
    }

    /**
     * 计数器 -1
     */
    size_t minusOne() {
        return count < 1 ? 0 : --count;
    }
};

/**
 * 编程题2
 */
class Store {
private:
    // 'static' is only use to *declaration*(not definition) static memberm, i.e., incomplete type
    // static members are not associated with any object
    // if static is const/constexpr, it can be initialized with constant expression, (const static
    // can be initialized outside the class while constexpr must be initialized when declared.)
    static constexpr double discount = 0.8, DiscountMoreThan10 = 0.98;
    static std::pair<int, double> sales[];
    static double sum;
    static size_t n;

public:
    /**
     * 计算平均价格
     * @return 平均价格
     */
    static double average();

    /**
     * 输出信息
     */
    static void display();
};

// initialize them outside the class (without static keyword)
std::pair<int, double> Store::sales[] = {{5, 23.5}, {12, 24.56}, {100, 21.5}};
size_t Store::n = 0;
double Store::sum = 0.0;

double Store::average() {
    // C++ 11
    for (auto i : sales) {
        Store::n += i.first;
        sum += (i.first > 10 ? i.first * i.second * discount * DiscountMoreThan10
            : i.first * i.second * discount);
    }
    return sum / n;
}

void Store::display() {
    std::cout << "Average price is: " << average() << std::endl;
}


/**
 * 3
 */
class Counter {
private:
    unsigned max;
    // c++ 11
    unsigned cnt = 0;
public:
    Counter(unsigned max = 9999) {
        this->max = max > 9999 ? 9999 : max;
    }

    void getCount() {
        std::cout << "cnt: " << cnt << std::endl;
    }

    /**
     * 判断是否溢出
     */
    bool overflow() {
        return cnt > max;
    }
 
    /**
     * 个位加1
     */
    void incr1() {
        cnt++;
        if (overflow()) {
            reset();
            std::cout << "溢出\n";
        }
    }

    /**
     * 十位加1
     */
    void incr10() {
        cnt += 10;
        if (overflow()) {
            reset();
            std::cout << "溢出\n";
        }
    }

    /**
     * 百位加1
     */
    void incr100() {
        cnt += 100;
        if (overflow()) {
            reset();
            std::cout << "溢出\n";
        }
    }

    /**
     * 千位加1
     */
    void incr1000() {
        cnt += 1000;
        if (overflow()) {
            reset();
            std::cout << "溢出\n";
        }
    }

    /**
     * 重置为 0
     */
    void reset() {
        cnt = 0;
    }
};

/**
 * 4
 */
class Pet {
protected:
    std::string name;
    size_t age = 0;
    double weight = 0.0;

public:
    explicit Pet(): Pet("unknown", 0) {

    }

    explicit Pet(std::string name, double weight): name(name), weight(weight) {

    }

    std::string getName() const {
        return name;
    }




    void setName(std::string name) {
        this->name = name;
    }

    double getWeight() const {
        return weight;
    }

    void setWeight(double weight) {
        this->weight = weight;
    }

    void getLifespan() const {
        std::cout << "unknown lifespan\n";
    }
};

class Dog: public Pet{
private:
    std::string bread;
public:
    // 继承构造器
    using Pet::Pet;
    std::string getBread() const {
        return bread;
    }

    void setBread(std::string bread) {
        this->bread = bread;
    }

    void getLifespan() const {
        std::cout << (weight > 100 ? "Approximately 7 years\n" : "Approximately 10 years\n");
    }
};

// main entry
int main(void) {
    CounterType cnt;
    cnt.addOne();
    cnt.minusOne();
    cnt.minusOne();
    std::cout << "cnt: " << cnt.getCount() << std::endl;

    Store::display();

    Counter counter = Counter();
    std::string line;
    while (std::getline(std::cin, line) && !line.empty()) {
        std::stringstream ss(line);
        char c = 0;
        ss >> c;
        unsigned num = 0;
        ss >> num;
        if (c == 'o') {
            c = 'f';
            num = 10;
        }
        while (num-- > 0) {
            switch(c) {
                case 'a': counter.incr1();break;
                case 's': counter.incr10();break;
                case 'd': counter.incr100();break;
                case 'f': counter.incr1000();break;
            }
        }
        counter.getCount();
    }

    Dog d = Dog("jerry", 101);
    d.getLifespan();
    std::cout << "weight: " << d.getWeight() << ", name: " << d.getName() << std::endl;

    return 0;
}