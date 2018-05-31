#ifndef NUM_PHILOSOPHERS
#define NUM_PHILOSOPHERS 5
#endif
void eating(int id);
void thinking(int id);
void* philosphers(void *ids);
void put_forks(int id);
void take_forks(int id);
void right_fork(int id);
void left_fork(int id);
void printStatus();
void dawdle();

