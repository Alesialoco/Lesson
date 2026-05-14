#include "log.h"
#include <unistd.h>

int main(void) {
    Log *log = Log::Instance();
    log->print();
    log->message(LOG_NORMAL, "program loaded");
    sleep(1);
    log->message(LOG_ERROR, "error happens! help me!");
    log->message(LOG_WARNING, "warning 1! look here!");
    log->print();
    log->message(LOG_WARNING, "warning 2! look here!");
    log->message(LOG_ERROR, "error happens! It is a plan trick!");
    sleep(5);
    log->message(LOG_WARNING, "warning 3! look here!");
    log->message(LOG_NORMAL, "program finished");
    log->message(LOG_WARNING, "warning 4! look here!");
    log->message(LOG_ERROR, "error happens! help me!");
    sleep(3);
    log->message(LOG_NORMAL, "program ended");
    log->message(LOG_NORMAL, "program came to an end");
    sleep(2);
    log->message(LOG_ERROR, "error happens! help me!");
    log->print();
}