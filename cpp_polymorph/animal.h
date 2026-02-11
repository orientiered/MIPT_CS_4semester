#pragma once
#include <string>

struct animal {
    std::string name_;


    virtual void voice() = 0;

    void set_name(const char *name);
    virtual ~animal() = default;
    protected:
        animal() = default;
};
