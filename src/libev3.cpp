#include <stdcfg.hpp>
#include <stdio.h>
#include <ev.h>


static void
stdin_cb (struct ev_loop * loop, ev_io *w, int revents)
{
	puts ("stdin ready");
	// for one-shot events, one must manually stop the watcher
	// with its corresponding stop function.
	ev_io_stop (EV_A_ w);

	// this causes all nested ev_run's to stop iterating
	//ev_break (EV_A_ EVBREAK_ALL);
}


int main(int argc, char** argv){

    struct ev_loop *loop = ev_loop_new( EVFLAG_AUTO );
//    struct ev_loop *loop = EV_DEFAULT;

    ev_io stdin_watcher;
    
    ev_io_init (&stdin_watcher, stdin_cb, /*STDIN_FILENO*/ 0, EV_READ);
    ev_io_start (loop, &stdin_watcher);

	ev_run (loop, 0);
//    ev_tstamp  ts=ev_time();
//    printf("current time %D", ts);    
    return 0;
}
