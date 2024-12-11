# Stepper595

Simple stepper motor driver library utilizing SPI to send control signals through a shift register. Features include:
- Adjustable delay to enable different motor speeds
- Non-blocking code
- Returns true after each step to allow for accurate step counting (can be used without caring about this)

# Compatible Motors
- **ULN2003** driver board for **28BYJ-48**.
- Probably any 4-wire stepper motors (no guarantee)

# Wiring
Uses the standard **CLK** and **MOSI** pins connected to the **CLK** and **SER**(data) pins on the shift register.\
You can choose any pin for **CS**(latch).
```
            74HC595N
             __   __
IN2 <-  QB =|1 *-*16|= vcc  <- +5v
IN3 <-  QC =|2    15|= QA   -> IN1
IN4 <-  QD =|3    14|= SER  <- MOSI
IN1 <-  QE =|4    13|= OE   -> GND
IN2 <-  QF =|5    12|= RCK  <- LATCH
IN3 <-  QG =|6    11|= SCK  <- SCK
IN4 <-  QH =|7    10|= SCLR <- +5v
GND <- GND =|8_____9|= SQH
```

# Basic Code

```
#include <Stepper595.h>

#define LATCH 4

Stepper595 stepper(LATCH);
int steps = 0;

void setup()
{}

void loop()
{
    if (stepper.step(MOTOR_1, CW) steps++);
}
```

# Hardware
- Microcontroller or dev board with SPI support
- Stepper motor driver w/ board (optional)
- Stepper motor

