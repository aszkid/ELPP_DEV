ELPP_DEV
================================

Based (symbolically) on [EasyLoggin++](https://github.com/easylogging/easyloggingpp), ELPP_DEV provides an unbloated framework for versatile logging.

Very early developement stage, but steadily growing in stability and usability.

### Simple example


```C++
#include <el_dev.hpp>

int main()
{
	el::initialize();
	
	LOG(el::INFO) << "This is info logging: " << 32;
	
	return 0;
}
```

Easy as that.
