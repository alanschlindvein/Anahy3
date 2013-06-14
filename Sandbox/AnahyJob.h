#include <cassert>
#include "AnahyJobAttributes.h"

typedef void*(*ParFunc)(void*);

typedef enum {
	AnahyJobStateReady,
	AnahyJobStateRunning,
	AnahyJobStateFinished
} AnahyJobState;

class AnahyJob {
	AnahyJob* _parent;
	AnahyJobState _state;
	ParFunc _function;
	void* _args;
	void* _result;
	AnahyJobAttributes* _attr;
	unsigned short _join_counter;
	unsigned short _fork_counter;
	bool _smart;

	// hidden constructors
	AnahyJob() {}
	AnahyJob(AnahyJob&) {}
	
	// private methods that can be called by a VP
	void run();
	void set_parent(AnahyJob* parent) { _parent = parent; }

public:

	AnahyJob(ParFunc function, void* args, AnahyJobAttributes* attr, bool smart=false);
	static AnahyJob* new_smart_job(ParFunc function, void* args, AnahyJobAttributes* attr);
	~AnahyJob();

	unsigned short join_counter() { return _join_counter; }
	void set_join_counter(unsigned short join_counter);
	
	unsigned short fork_counter() { return _fork_counter; }
	void set_fork_counter(unsigned short fork_counter);
	
	// atomic operations
	bool compare_and_swap_state(AnahyJobState target_value, AnahyJobState new_value);
	void atomic_decrement_join_counter() { __sync_fetch_and_sub(&_join_counter, 1); }
	void atomic_decrement_fork_counter() { __sync_fetch_and_sub(&_fork_counter, 1); }
	
	bool smart() { return _smart; }
	
	AnahyJobAttributes* attributes() { return _attr; }
	void set_attributes(AnahyJobAttributes* attr) { _attr = attr; }
};