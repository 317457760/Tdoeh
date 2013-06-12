all:
	g++ -g test_config.cpp ../src/tdoeh_config.cpp ../src/tdoeh_base.cpp \
		-o test_config \
		-I ../thirdparty/gtest-1.6.0/include/ \
		-I ../src/ \
		-L ../thirdparty/gtest-1.6.0/lib/.libs \
		-lgtest -lgtest_main -lpthread
