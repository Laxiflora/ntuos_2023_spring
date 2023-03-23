#include "kernel/types.h"
#include "user/setjmp.h"
#include "user/threads.h"
#include "user/user.h"
#define NULL 0


static struct thread* current_thread = NULL;
static int id = 1;
static jmp_buf env_st;
static jmp_buf env_tmp;

struct thread *thread_create(void (*f)(void *), void *arg){
    struct thread *t = (struct thread*) malloc(sizeof(struct thread));
    unsigned long new_stack_p;
    unsigned long new_stack;
    new_stack = (unsigned long) malloc(sizeof(unsigned long)*0x100);
    new_stack_p = new_stack +0x100*8-0x2*8;
    t->fp = f;
    t->arg = arg;
    t->ID  = id;
    t->buf_set = 0;
    t->stack = (void*) new_stack;
    t->stack_p = (void*) new_stack_p;
    id++;

    // part 2
    t->sig_handler[0] = NULL_FUNC;
    t->sig_handler[1] = NULL_FUNC;
    t->signo = -1;
    t->handler_buf_set = 0;
    return t;
}
void thread_add_runqueue(struct thread *t){
    if(current_thread == NULL){
        // TODO
    }
    else{
        // TODO
    }
}
void thread_yield(void){
    // TODO
}
void dispatch(void){
    // TODO
}
void schedule(void){
    // TODO
}
void thread_exit(void){
    if(current_thread->next != current_thread){
        // TODO
    }
    else{
        // TODO
        // Hint: No more thread to execute
    }
}
void thread_start_threading(void){
    // TODO
}
// part 2
void thread_register_handler(int signo, void (*handler)(int)){
    // TODO
}
void thread_kill(struct thread *t, int signo){
    // TODO
}