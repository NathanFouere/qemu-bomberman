#ifndef RESOURCE_H_
#define RESOURCE_H_

template <typename T>
class Resource {
private:
    T* ptr;
    
public:
    Resource() : ptr(nullptr) {}
    Resource(T* p) : ptr(p) {}
    ~Resource() { delete ptr; }
    
    
    Resource(const Resource&) = delete;
    Resource& operator=(const Resource&) = delete;
    
    
    Resource(Resource&& other) : ptr(other.ptr) { other.ptr = nullptr; }
    Resource& operator=(Resource&& other) {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }
    
    T* operator->() { return ptr; }
    T& operator*() { return *ptr; }
    
    T* get() const { return ptr; }
    T* release() { T* tmp = ptr; ptr = nullptr; return tmp; }
    void reset(T* p = nullptr) { delete ptr; ptr = p; }
};

#endif /* RESOURCE_H_ */