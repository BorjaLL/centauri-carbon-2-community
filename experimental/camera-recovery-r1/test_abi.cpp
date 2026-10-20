#include <string>
#include <memory>
#include <cstring>
#include <cassert>
#include <cstdint>
#include <cstdio>
struct NativeString { const char *data; uint32_t size; unsigned char storage[16]; };
int main(){
    assert(sizeof(void*)==4&&sizeof(std::string)==24&&sizeof(std::shared_ptr<int>)==8);
    std::string s="Camera device insertion";NativeString native{};std::memcpy(&native,&s,24);
    assert(native.size==s.size()&&!std::memcmp(native.data,s.data(),s.size()));
    auto ptr=std::make_shared<int>(42);void *parts[2];std::memcpy(parts,&ptr,8);
    assert(parts[0]==ptr.get()&&parts[1]);
    puts("PASS: ARM libstdc++ string/shared_ptr layout");
}
