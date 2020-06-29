//
// Created by parallels on 6/29/20.
//

#ifndef MY_LIBCO_NONCOPYABLE_H
#define MY_LIBCO_NONCOPYABLE_H


class noncopyable {

public:
    noncopyable(const noncopyable &) = delete;
    noncopyable &operator=(const noncopyable &) = delete;

protected:
    noncopyable() = default;
    ~noncopyable() = default;

};


#endif //MY_LIBCO_NONCOPYABLE_H
