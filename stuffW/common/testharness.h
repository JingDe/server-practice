#ifndef TESTHARNESS_H_
#define TESTHARNESS_H_

#include<sstream>

namespace test {

	extern int RunAllTests();

	class Tester {
	public:
		bool ok_;
		const char* fname_;
		int line_;
		std::stringstream ss_;

	public:
		Tester(const char* f, int l)
			:ok_(true), fname_(f), line_(l) {}

		~Tester() {
			if (!ok_) {
				fprintf(stderr, "%s:%d:%s\n", fname_, line_, ss_.str().c_str());
				exit(1);
			}
		}

		Tester& Is(bool b, const char* msg)
		{
			if (!b)
			{
				ss_ << " Assertion failure " << msg;
				ok_ = false;
			}
			return *this;
		}

		//Tester& IsOk(const Status& s)

#define BINARY_OP(name, op) \
	template<class X, class Y> \
	Tester& name(const X& x, const Y& y){ \
		if(!(x op y)) { \
			ss_<< " failed: "<<x<<( " " #op " " )<<y; \
			ok_=false; \
		} \
		return *this; \
	} \

		BINARY_OP(IsEq, == )
		BINARY_OP(IsNe, != )
		BINARY_OP(IsGe, >= )
		BINARY_OP(IsGt, >)
		BINARY_OP(IsLe, <= )
		BINARY_OP(IsLt, <)
#undef BINARY_OP

		template<class V>
		Tester& operator<<(const V& value) 
		{
			if (!ok_)
				ss_ << " " << value;
			return *this;
		}
	};

#define ASSERT_TRUE(c) ::test::Tester(__FILE__, __LINE__).Is((c), #c)
#define ASSERT_EQ(a,b) ::test::Tester(__FILE__, __LINE__).IsEq((a),(b))
#define ASSERT_NE(a,b) ::test::Tester(__FILE__, __LINE__).IsNe((a),(b))
#define ASSERT_GE(a,b) ::test::Tester(__FILE__, __LINE__).IsGe((a),(b))
#define ASSERT_GT(a,b) ::test::Tester(__FILE__, __LINE__).IsGt((a),(b))
#define ASSERT_LE(a,b) ::test::Tester(__FILE__, __LINE__).IsLe((a),(b))
#define ASSERT_LT(a,b) ::test::Tester(__FILE__, __LINE__).IsLt((a),(b))


#define TCONCAT(a, b) TCONCAT1(a, b)
#define TCONCAT1(a, b) a##b // ??

	// TEST宏，定义一个派生类，注册static的_RunIt函数，后接_Run函数体
#define TEST(base, name) \
class TCONCAT(_Test_, name) : public base { \
public: \
	void _Run(); \
	static void _RunIt() {	\
		TCONCAT(_Test_, name) t; \
		t._Run(); \
	} \
}; \
bool TCONCAT(_Test_ignored_, name) = ::test::RegisterTest(#base, #name, &TCONCAT(_Test_, name)::_RunIt); \
void TCONCAT(_Test_, name)::_Run()

	extern bool RegisterTest(const char* base, const char* name, void(*func)());

	extern int RandomSeed();
}

#endif
