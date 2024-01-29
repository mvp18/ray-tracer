#ifndef CG1RAYTRACER_ASSERT_HEADER
#define CG1RAYTRACER_ASSERT_HEADER

#include <iostream>
#include <cstdlib>
#include <exception>
#include <sstream>

namespace rt {

class Assert : public std::exception {
public:
	Assert() = default;
	Assert(const Assert& other) : os(other.os.str()) {}

	template <class T>
	Assert& operator<<(const T& msg) {
		os << msg;
		return *this;
	}

	const char* what() const noexcept override {
        str = os.str(); // os.str() is temporary
		return str.c_str();
	}
private:
	mutable std::string str;
	std::ostringstream os;
};

class NullAssert {
public:
    NullAssert() {}
    template <class T>
    NullAssert& operator<<(const T&) { return *this; }
};

} // namespace rt

#ifndef NDEBUG
#ifndef rt_assert
#define rt_assert(cond) if(!(cond)) throw rt::Assert() << "Assertion failure at " << __FILE__ << ":" << __LINE__ << " -- " << #cond << " -- "
#endif
#ifndef rt_release_assert
#define rt_release_assert(cond) if(!(cond)) throw rt::Assert() << "Fatal error at at " << __FILE__ << ":" << __LINE__ << " -- " << #cond << " -- "
#endif
#else
#ifndef rt_assert
#define rt_assert(cond) rt::NullAssert()
#endif
#ifndef rt_release_assert
#define rt_release_assert(cond) if(!(cond)) throw rt::Assert() << "Fatal error -- "
#endif
#endif

#define UNREACHABLE do { rt_release_assert(false) << "UNREACHABLE"; } while (false)
#define NOT_IMPLEMENTED do { rt_release_assert(false) << "NOT IMPLEMENTED"; } while (false)

#endif
