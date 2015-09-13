#define TIMED_BLOCK__(num) timed_block TimedBlock__##num(__COUNTER__, __FILE__, __LINE__, __FUNCTION__);
#define TIMED_BLOCK_(num) TIMED_BLOCK__(num)
#define TIMED_BLOCK TIMED_BLOCK_(__LINE__)

struct timed_block {

	timed_block(OPint counter, OPchar* filename, OPint lineNumber, OPchar* filename) {

	}

	~timed_block() {

	}
};
