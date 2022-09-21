#include <init.h>
#include <drivers/gpio.h>

#define SKY_UFLn_GPIO_NAME      DT_GPIO_LABEL(DT_INST(0, skyworks_sky13351), vctl1_gpios)
#define SKY_UFLn_GPIO_FLAGS     DT_GPIO_FLAGS(DT_INST(0, skyworks_sky13351), vctl1_gpios)
#define SKY_UFLn_GPIO_PIN       DT_GPIO_PIN(DT_INST(0, skyworks_sky13351), vctl1_gpios)
#define SKY_PCBn_GPIO_NAME      DT_GPIO_LABEL(DT_INST(0, skyworks_sky13351), vctl2_gpios)
#define SKY_PCBn_GPIO_FLAGS     DT_GPIO_FLAGS(DT_INST(0, skyworks_sky13351), vctl2_gpios)
#define SKY_PCBn_GPIO_PIN       DT_GPIO_PIN(DT_INST(0, skyworks_sky13351), vctl2_gpios)

static inline void external_antenna(bool on)
{
        const struct device *ufl_gpio_dev, *pcb_gpio_dev;

        ufl_gpio_dev = device_get_binding(SKY_UFLn_GPIO_NAME);
        if (!ufl_gpio_dev) {
                return;
        }

        pcb_gpio_dev = device_get_binding(SKY_PCBn_GPIO_NAME);
        if (!pcb_gpio_dev) {
                return;
        }

        gpio_pin_configure(ufl_gpio_dev, SKY_UFLn_GPIO_PIN,
                           SKY_UFLn_GPIO_FLAGS
                           | (on
                              ? GPIO_OUTPUT_ACTIVE
                              : GPIO_OUTPUT_INACTIVE));
        gpio_pin_configure(pcb_gpio_dev, SKY_PCBn_GPIO_PIN,
                           SKY_PCBn_GPIO_FLAGS
                           | (on
                              ? GPIO_OUTPUT_INACTIVE
                              : GPIO_OUTPUT_ACTIVE));
}

static int antenna_init(const struct device *dev)
{
        ARG_UNUSED(dev);

        /*
         * On power-up the SKY13351 is left uncontrolled, so neither
         * PCB nor external antenna is selected.  Select the PCB
         * antenna.
         */
        external_antenna(true);

        return 0;
}

SYS_INIT(antenna_init, APPLICATION, 99);
