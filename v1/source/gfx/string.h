#ifndef __UTIL_STRING_H__
#define __UTIL_STRING_H__

namespace util {

    // Main class definition
    class string : public std::string /*super string*/ {
    public:
        string(void) {
            *this =  std::string();
        }

        // automatically appends whatever is passed to it as a string
        template <typename T>
        string & operator<<(const T &v) {
            std::stringstream ss;
            ss << v;
            (*this) += ss.str ();

            return *this;
        }

        // convert the string into whatever type it needs to be
        template <typename T>
        const string & operator>>(T &v) const {
            std::stringstream ss (*this);
            ss >> v;
            return *this;
        }

        // convert whatever is passed in and store it as a string
        template <typename T>
        string(const T &v) {
            *this << v;
        }

        // cast the string into whatever
        template <typename T>
        operator T(void) {
            T v;
            *this >> v;
            return v;
        }

        // determine if a string ends with a given substring
        bool endsWith(string end) {
            size_t index = (*this).find(end);
            size_t size_of_end = end.size();
            size_t size = (*this).size();

            return (index + (size_of_end) == size);
        }
    };

} // namespace util

#endif // __UTIL_STRING_H__