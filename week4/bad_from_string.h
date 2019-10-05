#pragma once

class bad_from_string : public std::exception {
private:
    std::string msg;

public:
    bad_from_string(const std::string & s) : msg(s) {}
    bad_from_string(const char *        s) : msg(s) {}

    const char * what() const noexcept {
        return msg.c_str();
    }
};