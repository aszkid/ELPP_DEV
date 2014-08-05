ELPP_DEV
================================

Based (symbolically) on [EasyLogging++](https://github.com/easylogging/easyloggingpp).

Not ready for production. Not ready for anything really, you may just want to take a look at the source code.

### Simple example
-------------------------


```C++
#include <el_dev.hpp>

int main()
{
	el::initialize();
	
	LOG(el::INFO) << "This is info logging: " << 32;
	
	return 0;
}
```

Will print `This is info logging: 32` in the standart output.

Simple as that. Fast and uncomplicated.

### Compilable with
-------------------------

* `clang++ 3.2.1 (Linux - 32/64)`
* `g++ 4.7.3 (Linux - 32/64)`
