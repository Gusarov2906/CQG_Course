#include <iostream>
#include "smart_shared_ptr.h"
#include "CTrace.h"

int main()
{
    //smart_shared_ptr<CTrace> ctrace0;
    //std::cout << ctrace0.getCount() << std::endl;

    //smart_shared_ptr<CTrace> ctrace1 = makeShared(new CTrace(1));
    //std::cout << ctrace1.getCount() << std::endl;

    //{
    //    smart_shared_ptr<CTrace> ctrace2(ctrace1);
    //    std::cout << ctrace1.getCount() << std::endl;
    //    std::cout << ctrace2.getCount() << std::endl;
    //}

    //std::cout << ctrace1.getCount() << std::endl;
    //smart_shared_ptr<CTrace> ctrace3(new CTrace(2));
    //ctrace3 = ctrace0;

    //ctrace3 = std::move(ctrace1);

    //smart_shared_ptr<CTrace> ctrace4(std::move(ctrace3));
    //smart_shared_ptr<CTrace> ctrace5 = ctrace4;
    //std::cout << ctrace4.getCount() << std::endl;

    //smart_shared_ptr<CTrace> ctrace0;
    smart_shared_ptr<CTrace> ctrace1 = (new CTrace(1));
    //ctrace0 = std::move(ctrace1);
    ctrace1 = ctrace1;
    std::cout << ctrace1.getCount() << std::endl;
    //ctrace0 = std::move(ctrace1);

    //std::cout << ctrace0.getCount() << std::endl;
    ctrace1.reset();
    std::cout << ctrace1.getCount() << std::endl;
    return 0;
}