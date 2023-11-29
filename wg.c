// GPIO IP Example, gpio.c
// GPIO Shell Command
// Jason Losh

//-----------------------------------------------------------------------------
// Hardware Target
//-----------------------------------------------------------------------------

// Target Platform: Xilinx XUP Blackboard

// Hardware configuration:
//
// Add specifications here

//-----------------------------------------------------------------------------

#include <stdlib.h>          // EXIT_ codes
#include <stdio.h>           // printf
#include <string.h>          // strcmp
#include "wg_ip.h"           // Wavegen IP library

// field flags
#define FIELD_A 0
#define FIELD_B 1
#define FIELD_ERR -113

// mode defines for legibility
#define MODE_DC 0
#define MODE_SINE 1
#define MODE_SAW 2 
#define MODE_TRI 3
#define MODE_SQ 4

int8_t selField(char out)
{
    if (out == 'A' || out == 'a')
        return FIELD_A;
    else if (out == 'B' || out == 'b')
        return FIELD_B;
    else
        return FIELD_ERR;
}

void printHelp(void)
{
    printf("Commands:\n");
    printf("  dc \t\tOUT OFS\n");
    printf("  cycles \tOUT N\n");
    printf("  cycles \tcontinuous\n");
    printf("  sine \t\tOUT FREQ AMP {OFS}\n");
    printf("  sawtooth \tOUT FERQ AMP {OFS}\n");
    printf("  triangle \tOUT FREQ AMP {OFS}\n");
    printf("  square \tOUT FREQ AMP {OFS} {DC}\n");
    printf("  run\n");
    printf("  stop\n");
    printf("Note: brackets {} indicate optional field\n");
}

int main(int argc, char* argv[])
{ 

    if (!strcmp(argv[1], "debug"))  // REMOVE ON FINAL
    {
        wgOpen();

        // add custom debug code here
    }

    else if (argc == 4)
    {
        wgOpen();

        char out = argv[2][0];
        int8_t field = selField(out);

        if (!strcmp(argv[1], "dc"))
        {
            // printf("Output: %c\n", out);    // REMOVE FOR FINAL

            if (field != FIELD_ERR)
            {
                setMode(MODE_DC, field);

                int16_t offset = atof(argv[3]) * 100000;
                if (offset >= -25000 && offset <= 25000)
                {
                    // printf("Offset: %d\n", offset);     // REMOVE FOR FINAL
                    setOffset(offset, field);
                }
                else
                {
                    setOffset(0, field);
                    printf("Error: offset out of range [-2.500 V to 2.500 V]. Defaulting to 0.000 V\n");
                }
            }
            else
                printf("Error: could not select A or B\n");
        }

        else if (!strcmp(argv[1], "cycles"))
        {
            char out = argv[2][0];
            int8_t field = selField(out);

            if (field != FIELD_ERR)
            {
                int16_t cycles = atoi(argv[3]);
                setCycles(cycles, field);
            }
            else
                printf("Error: could not select A or B\n");
        }

        else
            printf("Argument %s is unknown\n", argv[1]);
    }

    else if (argc == 3)
    {
        wgOpen();

        if (!strcmp(argv[1], "cycles"))
        {
            char out = argv[2][0];
            int8_t field = selField(out);

            if (field != FIELD_ERR)
            {
                if (!strcmp(argv[2], "continuous"))
                    setCycles((int16_t)-1, field);
            }        
        }

        else
            printf("Argument %s is unknown\n", argv[1]);
    }

    else if (argc >= 5 && argc <= 7)
    {
        wgOpen();

        char out = argv[2][0];
        int8_t field = selField(out);

        if (!strcmp(argv[1], "sine"))
        {
            // printf("Output: %c\n", out);    // REMOVE FOR FINAL

            if (field != FIELD_ERR)
            {
                setMode(MODE_SINE, field);

                uint32_t frequency = atoi(argv[3]);
                if (frequency >= 1)
                {
                    if (field == FIELD_A)
                        setFreqA(frequency);
                    else
                        setFreqB(frequency);
                }
                else
                {
                    if (field == FIELD_A)
                        setFreqA(1000);
                    else
                        setFreqB(1000);
                    printf("Error: frequency out of range [1 Hz to 2500 Hz]. Defaulting to 1000 Hz\n");
                }

                uint16_t amplitude = atof(argv[4]) * 10;
                if (amplitude >= 0)
                {
                    setAmplitude(amplitude, field);
                }
                else
                {
                    setAmplitude(10, field);
                    printf("Error: amplitude out of range [0.0 V to 2.5 V]. Defaulting to 1.0 V\n");
                }

                if (argc >= 6)
                {
                    int16_t offset = atof(argv[5]) * 25000;
                    if (offset >= -62500 && offset <= 62500)
                    {
                        // printf("Offset: %d\n", offset);     // REMOVE FOR FINAL
                        setOffset(offset, field);
                    }
                    else
                    {
                        setOffset(0, field);
                        printf("Error: offset out of range [-2.500 V to 2.500 V]. Defaulting to 0.000 V\n");
                    }
                }
                else
                    setOffset(0, field);
            }
            else
                printf("Error: could not select A or B\n");
        }

        else if (!strcmp(argv[1], "sawtooth"))
        {
            // printf("Output: %c\n", out);    // REMOVE FOR FINAL

            if (field != FIELD_ERR)
            {
                setMode(MODE_SAW, field);

                uint32_t frequency = atoi(argv[3]);
                if (frequency >= 1) // && frequency <= 2500)
                {
                    // printf("Frequency: %d\n", frequency);   // REMOVE FOR FINAL
                    if (field == FIELD_A)
                        setFreqA(frequency);
                    else
                        setFreqB(frequency);
                }
                else
                {
                    if (field == FIELD_A)
                        setFreqA(1000);
                    else
                        setFreqB(1000);
                    printf("Error: frequency out of range [1 Hz to 2500 Hz]. Defaulting to 1000 Hz\n");
                }

                uint16_t amplitude = atof(argv[4]) * 10;
                if (amplitude >= 0) // && amplitude <= 2500)
                {
                    // printf("Amplitude: %d\n", amplitude);   // REMOVE FOR FINAL
                    setAmplitude(amplitude, field);
                }
                else
                {
                    setAmplitude(10, field);
                    printf("Error: amplitude out of range [0.0 V to 2.5 V]. Defaulting to 1.0 V\n");
                }

                if (argc >= 6)
                {
                    int16_t offset = atof(argv[5]) * 10000;
                    if (offset >= -25000 && offset <= 25000)
                    {
                        // printf("Offset: %d\n", offset);     // REMOVE FOR FINAL
                        setOffset(offset, field);
                    }
                    else
                    {
                        setOffset(0, field);
                        printf("Error: offset out of range [-2.500 V to 2.500 V]. Defaulting to 0.000 V\n");
                    }
                }
                else
                    setOffset(0, field);
            }
            else
                printf("Error: could not select A or B\n");
        }

        else if (!strcmp(argv[1], "triangle"))
        {
            // printf("Output: %c\n", out);    // REMOVE FOR FINAL

            if (field != FIELD_ERR)
            {
                setMode(MODE_TRI, field);

                uint32_t frequency = atoi(argv[3]);
                if (frequency >= 1) // && frequency <= 2500)
                {
                    // printf("Frequency: %d\n", frequency);   // REMOVE FOR FINAL
                    if (field == FIELD_A)
                        setFreqA(frequency);
                    else
                        setFreqB(frequency);
                }
                else
                {
                    if (field == FIELD_A)
                        setFreqA(1000);
                    else
                        setFreqB(1000);
                    printf("Error: frequency out of range [1 Hz to 2500 Hz]. Defaulting to 1000 Hz\n");
                }

                uint16_t amplitude = atof(argv[4]) * 10;
                if (amplitude >= 0) // && amplitude <= 2500)
                {
                    // printf("Amplitude: %d\n", amplitude);   // REMOVE FOR FINAL
                    setAmplitude(amplitude, field);
                }
                else
                {
                    setAmplitude(10, field);
                    printf("Error: amplitude out of range [0.0 V to 2.5 V]. Defaulting to 1.0 V\n");
                }

                if (argc >= 6)
                {
                    int16_t offset = atof(argv[5]) * 10000;
                    if (offset >= -25000 && offset <= 25000)
                    {
                        // printf("Offset: %d\n", offset);     // REMOVE FOR FINAL
                        setOffset(offset, field);
                    }
                    else
                    {
                        setOffset(0, field);
                        printf("Error: offset out of range [-2.500 V to 2.500 V]. Defaulting to 0.000 V\n");
                    }
                }
                else
                    setOffset(0, field);
            }
            else
                printf("Error: could not select A or B\n");
        }

        else if (!strcmp(argv[1], "square"))
        {
            // printf("Output: %c\n", out);    // REMOVE FOR FINAL

            if (field != FIELD_ERR)
            {
                setMode(MODE_SQ, field);

                uint32_t frequency = atoi(argv[3]);
                if (frequency >= 1) // && frequency <= 2500)
                {
                    // printf("Frequency: %d\n", frequency);   // REMOVE FOR FINAL
                    if (field == FIELD_A)
                        setFreqA(frequency);
                    else
                        setFreqB(frequency);
                }
                else
                {
                    if (field == FIELD_A)
                        setFreqA(1000);
                    else
                        setFreqB(1000);
                    printf("Error: frequency out of range [1 Hz to 2500 Hz]. Defaulting to 1000 Hz\n");
                }

                uint16_t amplitude = atof(argv[4]) * 10;
                if (amplitude >= 0) // && amplitude <= 2500)
                {
                    // printf("Amplitude: %d\n", amplitude);   // REMOVE FOR FINAL
                    setAmplitude(amplitude, field);
                }
                else
                {
                    setAmplitude(10, field);
                    printf("Error: amplitude out of range [0.0 V to 2.5 V]. Defaulting to 1.0 V\n");
                }

                if (argc >= 6)
                {
                    int16_t offset = atof(argv[5]) * 10000;
                    if (offset >= -25000 && offset <= 25000)
                    {
                        // printf("Offset: %d\n", offset);     // REMOVE FOR FINAL
                        setOffset(offset, field);
                    }
                    else
                    {
                        setOffset(0, field);
                        printf("Error: offset out of range [-2.500 V to 2.500 V]. Defaulting to 0.000 V\n");
                    }

                    if (argc == 7)
                    {
                        uint16_t dtycyc = 2045 * (atoi(argv[6]) / 100);
                        if (dtycyc >= 0 && dtycyc <= 100)
                        {
                            // printf("Duty cycle: %d\n", dtycyc);     // REMOVE FOR FINAL
                            setDutyCycle(dtycyc, field);
                        }
                        else
                        {
                            setDutyCycle(100, field);
                            printf("Error: duty cycle out of range [0 to 50]. Defaulting to 50\n");
                        }
                    }
                    else
                        setDutyCycle(100, field);
                }
                else
                    setOffset(0, field);
            }
            else
                printf("Error: could not select A or B\n");
        }

        else
            printf("Argument %s is unknown\n", argv[1]);
    }

    else if (argc == 2)
    {
        wgOpen();

        if (!strcmp(argv[1], "run"))
        {
            printf("Starting waveform\n");
            setRun(true, FIELD_A);
            setRun(true, FIELD_B);
        }

        else if (!strcmp(argv[1], "stop"))
        {
            printf("Stopping waveform\n");
            setRun(false, FIELD_A);
            setRun(false, FIELD_B);
        }

        else if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
        {
            printHelp();
        }

        else
            printf("Argument %s is unknown\n", argv[1]);
    }

    else
    {
        printf("No arguments found. Here's how to use this program:\n");
        printHelp();
    }

    return EXIT_SUCCESS;
}
