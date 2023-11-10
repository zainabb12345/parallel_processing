#include <iostream>
#include <iomanip>
#include <math.h>
#include <chrono>
#include "functions.cpp"
#include <mutex>
#include <condition_variable>
#include <thread>

extern void initialize();
extern double f1();
extern double f2(double a);
extern double f3(double a);
extern double f4(double a);
extern double f5(double a);
extern double f6(double a, double b);
extern double f7(double a);

/*
 * In order to simplify the synchronisation in the parallel code, you can use
 * a holder class Double for the values, which implements the synchronisation,
 * like this:
 */
  class Double {
  private:
  	double val;
  	std::mutex mutex;
  	bool isValid;
  	std::condition_variable valid;
  public:
  	Double() {
  		isValid = false;
  	}
  	void set(double v) {
  		// implement mutal exclusion!
		std::unique_lock<std::mutex> lock(mutex);
  		val = v;
    	isValid = true;
       // notify all threads waiting for the value!
	   valid.notify_all();
  	}
  	double get() {
  		// implement mutal exclusion!
		std::unique_lock<std::mutex> lock(mutex);
  		// wait until the value is valid!
		valid.wait(lock, [this]() { return isValid; });
  		return val;
  	}
  };
 /*
 * You can then pass pointers to Double objects to the thread functions,
 * both for the input argument(s) and the result.
 */

int main(int argc, char **argv)
{
	initialize();
	
	auto start = std::chrono::high_resolution_clock::now();
	double a, b, c, res;
	// a = f1();
    // b = f2(a);
	// c = f3(a);
	// a = f4(a);
	// res = f5(b) + f7(a) + f6(b,c);
	
	Double result_f1, result_f2, result_f3, result_f4, result_f5, result_f6, result_f7;

    std::thread thread_f1([&result_f1]() {
        result_f1.set(f1());
    });

    std::thread thread_f2([&result_f2, &result_f1]() {
        result_f2.set(f2(result_f1.get()));
    });

    std::thread thread_f3([&result_f3, &result_f1]() {
        result_f3.set(f3(result_f1.get()));
    });

    std::thread thread_f4([&result_f4, &result_f1]() {
        result_f4.set(f4(result_f1.get()));
    });

    std::thread thread_f5([&result_f5, &result_f2]() {
        result_f5.set(f5(result_f2.get()));
    });

    std::thread thread_f6([&result_f6, &result_f2, &result_f3]() {
        result_f6.set(f6(result_f2.get(), result_f3.get()));
    });

    std::thread thread_f7([&result_f7, &result_f1]() {
        result_f7.set(f7(result_f1.get()));
    });

    thread_f1.join();
    thread_f2.join();
    thread_f3.join();
    thread_f4.join();
    thread_f5.join();
    thread_f6.join();
    thread_f7.join();

	a = result_f1.get();
	b = result_f2.get();
	c = result_f3.get();
	a = result_f4.get();

    res = result_f5.get() + result_f7.get() + result_f6.get();
	auto end = std::chrono::high_resolution_clock::now();
	
	std::cout << "Result = " << std::setprecision(18) << res << std::endl;
	
	std::cout << "Time: " << (end - start)/std::chrono::milliseconds(1) << "ms" << std::endl;
	return 0;
}
