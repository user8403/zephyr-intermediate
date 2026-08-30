#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(demo, LOG_LEVEL_DBG);

#define STACK_SIZE 1024

#define PRIO 1

volatile uint32_t counter = 0;
K_MUTEX_DEFINE(mymutex);

void t_work_fn(void *p1, void *p2, void *p3)
{
    LOG_INF("running %u", (uint32_t)p1);
    for (int i= 0; i < 1000000; i++)
    {
        k_mutex_lock(&mymutex, K_FOREVER);
        counter++;
        k_mutex_unlock(&mymutex);
    }
}

K_THREAD_DEFINE(thread_a, STACK_SIZE, t_work_fn,
                1, NULL, NULL, PRIO, 0, 0);
K_THREAD_DEFINE(thread_b, STACK_SIZE, t_work_fn,
                2, NULL, NULL, PRIO, 0, 0);

int main(void)
{
    k_thread_join(thread_a, K_FOREVER);
    k_thread_join(thread_b, K_FOREVER);
    LOG_INF("counter: %u", counter);
    return 0;
}

