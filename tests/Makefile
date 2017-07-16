GTEST_DIR = ../thirdparty/googletest/googletest
GMOCK_DIR = ../thirdparty/googletest/googlemock
CPP_TEST_FLAGS = -pthread

GTEST_LIB = $(GTEST_DIR)/../googlemock/gtest/libgtest.a
GMOCK_LIB = $(GTEST_DIR)/../googlemock/libgmock.a

TESTS = test_basic

all: clean $(GTEST_LIB) $(TESTS) check

clean:
	rm -f $(TESTS) *.o

check:
	$(foreach var, $(TESTS), ./$(var);)

test_basic:
	$(CXX) -I $(GMOCK_DIR)/include/ -I $(GTEST_DIR)/include/  \
	test_basic.cpp  $(GTEST_LIB) $(GMOCK_LIB) $(CPP_TEST_FLAGS) -o test_basic
