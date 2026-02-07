#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/timekeeping.h>
#include <linux/time64.h>
#include <linux/moduleparam.h>

#define DEFAULT_FILE_PATH "/var/tmp/test_module"
#define DEFAULT_PERIOD    5

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dranevich");
MODULE_DESCRIPTION("Does nothing but writes some messages");

/* Main parameters - set to default, can be changed later */
/* 0644 means what root user may read & write the value, others - just read */
static char *log_path = DEFAULT_FILE_PATH;
module_param(log_path, charp, 0644);
MODULE_PARM_DESC(log_path, "Path to the current log file");

static unsigned int period_sec = DEFAULT_PERIOD;
module_param(period_sec, uint, 0644);
MODULE_PARM_DESC(period_sec, "Timer period, [s]");

/* Extra data structures */
static struct file *log_file;
static struct timer_list log_timer;
static unsigned long counter;
static loff_t file_pos;

/* Is called when previous interval is up */
static void timer_callback(struct timer_list *t)
{
    char buffer[128];
    int len;

    struct timespec64 ts;
    struct tm tm;

    /* System time - is used for timestamps */
    ktime_get_real_ts64(&ts);
    time64_to_tm(ts.tv_sec, 0, &tm);
    
    /* Increment message counter */
    counter++;
    
    /* Create human-readable timestamps & form output string*/
    len = snprintf(buffer, sizeof(buffer),
        "[%04ld-%02d-%02d %02d:%02d:%02d] Hello from kernel module (%lu)\n",
        tm.tm_year + 1900,
        tm.tm_mon + 1,
        tm.tm_mday,
        tm.tm_hour,
        tm.tm_min,
        tm.tm_sec,
        counter
    );

    kernel_write(log_file, buffer, len, &file_pos);
    
    /* Reset the timer */
    mod_timer(&log_timer, jiffies + period_sec * HZ);
}

/* Initialization function - should be called once when module is activated */
static int __init test_module_init(void)
{
    /* Open the output file, i.e. receives it's descriptor */
    log_file = filp_open(log_path,
                         O_WRONLY | O_CREAT | O_APPEND,
                         0644);
    
    /* Provides correct shutdown if something went wrong */
    if (IS_ERR(log_file)) {
        pr_err("test_module: can not open output file\n");
        return PTR_ERR(log_file);
    }
    
    /* Set all counters to zero*/
    counter  = 0;
    file_pos = 0;

    timer_setup(&log_timer, timer_callback, 0);
    mod_timer(&log_timer, jiffies + period_sec * HZ);

    pr_info("test_module: loaded successfully\n");
    return 0;
}

/* Exit function - frees used resources when the module is removed */
static void __exit test_module_exit(void)
{
    del_timer_sync(&log_timer);

    if (log_file && !IS_ERR(log_file))
        filp_close(log_file, NULL);

    pr_info("test_module: removed sucsessfully\n");
}

/* Entry & Exit points */
module_init(test_module_init);
module_exit(test_module_exit);

