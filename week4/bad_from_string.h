#pragma once

class bad_from_string : public std::exception {
    const char * what() const noexcept {
        return "";
    }
};