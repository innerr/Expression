#include <cstring>
#include <cassert>
#define DEFAULT_INIT_SIZE 256


template<typename T>
struct stack {
    stack() : tail(0) {
        N_LEN = DEFAULT_INIT_SIZE;
        content = new T[N_LEN];
    }

    explicit stack(const size_t& len) : N_LEN(len), tail(0) {
        content = new T[N_LEN];
    }

    ~stack() {
        delete[] content;
    }

    inline size_t size() {
        return N_LEN;
    }

    inline bool empty() {
        return !tail;
    }

    inline const T top() {
        if(tail == 0) {
            assert(false);
            return T();
        }
        return content[tail - 1];
    }

    inline const T pop() {
        if (tail == 0) {
            assert(false);
            return T();
        }
        return content[--tail];
    }

    inline void push(const T& x) {
        if (tail == N_LEN) {
            auto *tmp = new T[N_LEN << 1];
            memcpy(content, tmp, sizeof(T) * N_LEN);
            delete[] content;
            content = tmp;
            N_LEN <<= 1;
        }
        content[tail++] = x;
    }

    inline void print() {
        for(int i = (int)tail - 1; i >= 0; i --) {
            content[i].print();
        }
        std::cout << std::endl;
    }

    size_t N_LEN;
    size_t tail;
    T *content;
};