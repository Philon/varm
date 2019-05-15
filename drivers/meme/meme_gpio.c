#include <linux/sched.h>
#include <asm/irq.h>
#include <linux/irq.h>
#include <linux/interrupt.h>

static irqreturn_t meme_interrupt(int irq, void* dev_id)
{
  printk(KERN_ALERT"meme irq\n");
  return IRQ_HANDLED;
}

int meme_gpio_irq_init(void)
{

  int rc = request_any_context_irq(0x111111, meme_interrupt, IRQF_TRIGGER_FALLING, "meme_irq", NULL);
  return rc;
}

int meme_gpio_irq_free(void)
{
  free_irq(0x111111, NULL);
  return 0;
}