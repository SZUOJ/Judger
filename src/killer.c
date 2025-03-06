#define _POSIX_SOURCE
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

#include "killer.h"


int kill_pid(pid_t pid) {
    return kill(pid, SIGKILL);
}


// time limit should be at least 1000ms
int get_timeout(int timeout){
    if (timeout < 1000) return 1000;
    return timeout;
}


void *timeout_killer(void *timeout_killer_args) {
    // this is a new thread, kill the process if timeout
    pid_t pid = ((struct timeout_killer_args *)timeout_killer_args)->pid;
    int timeout = ((struct timeout_killer_args *)timeout_killer_args)->timeout;
    // On success, pthread_detach() returns 0; on error, it returns an error number.
    if (pthread_detach(pthread_self()) != 0) {
        kill_pid(pid);
        return NULL;
    }
    // usleep can't be used, for time args must < 1000ms
    // this may sleep longer that expected, but we will have a check at the end
    if (sleep((unsigned int)(get_timeout(timeout) / 1000)) != 0) {
        kill_pid(pid);
        return NULL;
    }
    if (kill_pid(pid) != 0) {
        return NULL;
    }
    return NULL;
}
