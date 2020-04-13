=============================
THE I2S DMA CONTROLLER DRIVER
=============================
The I2S peripheral can be utilised in audio applications where MCU is connected to an CODEC via 
I2S for digital audio signal and I2C/SPI for CODEC configuration interface(CODEC configuration interface varies from codec to codec)

I2S peripheral can be used along with DMAC module. DMAC is system driver which is included by default in all Atmel START projects.
To make this driver work, 3 parts to be taken care correctly(apart from clock etc)

    1. DMAC configurations in GUI
    2. I2S configuration in GUI
    3. DMAC related configuration from Application file(source buffer pointer, array size etc)

FEATURES
--------
APIs available to perform following functions.

    1. DMA Init - Initialize the DMA module with the configurations selected from GUI
    2. DMA set destination address
    3. DMA set source address
    4. DMA set Next descriptor address
    5. DMA set Data amount
    6. DMA enable transaction with or without software trigger
    7. Handling DMAC Interrupts using DMAC Interrupt handler

APPLICATIONS
------------
Audio applications uses I2S driver. Most audio applicaions use I2S in DMA mode.

DEPENDENCIES
------------
* DEPENDENCIES for SAMD21&DA1:
    DMAC system driver needs to be enabled

* DEPENDENCIES for SAMD5x/E5x Device Example routine in driver_example.c: 
    DMAC system driver needs to be enabled with following configurations

    1. In Dashboard, Enable "Show system drivers" (On Top right Corner)
    2. Click the DMAC Module
    3. Tick the Checkbox "ENABLE"(In lower left corner) to enable the DMAC Module
    4. Enable "Channel 0"(Tick the checkbox for CHANNEL 0 SETTINGS) with SETTINGS as mentioned below.
    
        a. Leave all configurations to default except below
        b. Set trigger action to    => One trigger required for each beat transfer
        c. Set trigger source to    => I2S Tx 0 Trigger
        d. Select Step Selection to => Step size settings apply to the source address
        e. Tick the checkbox        => Source Address Increment
        f. Select the Beat size     => 32-bit bus transfer


CONCURRENCY
-----------
N/A

LIMITATIONS
-----------
I2S driver works only with DMAC.

KNOWN ISSUES AND WORKAROUNDS
----------------------------
N/A
