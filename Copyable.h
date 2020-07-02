//
// Created by parallels on 7/2/20.
//

#ifndef MY_LIBCO_COPYABLE_H
#define MY_LIBCO_COPYABLE_H


class Copyable {
public:
    Copyable(const Copyable & copyable) = default;
    Copyable &operator=(const Copyable & copyable) = default;
protected:
    Copyable()= default;
    ~Copyable() = default;

};


#endif //MY_LIBCO_COPYABLE_H
