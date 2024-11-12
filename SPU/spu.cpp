#include "spu.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "../program.h"
#include "../Stack/stack.h"
#include "../My_lib/Assert/my_assert.h"
#include "../My_lib/Logger/logging.h"
#include "../My_lib/My_stdio/my_stdio.h"
#include "../My_lib/helpful.h"

static bool get_two_args (stack_elem* const first, stack_elem* const second, stack_t* const stk);
static enum SPU_ERROR push_cmd (spu_t* const processor, const command_t argument);
static enum SPU_ERROR pop_cmd  (spu_t* const processor, const command_t argument);
static enum SPU_ERROR draw_cmd (spu_t* const processor);
static enum SPU_ERROR meow_cmd (spu_t* const processor);

#define LOG_CMD_DEFINED(cmd)                                                                    \
    LOG (DEBUG, "Command " #cmd " was defined\n");

#define CASE_JUMP_EQUAL(jump_cmd, compare)                                                      \
    case jump_cmd:                                                                              \
    {                                                                                           \
        LOG_CMD_DEFINED (jump_cmd);                                                             \
        stack_elem first  = 0;                                                                  \
        stack_elem second = 0;                                                                  \
        if (!(get_two_args (&first, &second, &(processor->stk))))                               \
        {                                                                                       \
            fprintf (stderr, "There is no element in stack to do the command " #jump_cmd "\n"); \
            return CANT_ ## jump_cmd ## _SPU;                                                   \
        }                                                                                       \
                                                                                                \
        size_t pointer = 0;                                                                     \
        memcpy (&pointer, processor->code + processor->ip, sizeof (pointer));                   \
                                                                                                \
        if (memcmp (&first, &second, sizeof (second)) compare 0)                                \
        {                                                                                       \
            processor->ip = pointer;                                                            \
        }                                                                                       \
        else                                                                                    \
        {                                                                                       \
            processor->ip += sizeof (pointer);                                                  \
        }                                                                                       \
                                                                                                \
        break;                                                                                  \
    }
//-----------------------------------------------------------------------------------------------

#define CASE_JUMP(jump_cmd, compare)                                                            \
    case jump_cmd:                                                                              \
    {                                                                                           \
        LOG_CMD_DEFINED (jump_cmd);                                                             \
        stack_elem first  = 0;                                                                  \
        stack_elem second = 0;                                                                  \
        if (!(get_two_args (&first, &second, &(processor->stk))))                               \
        {                                                                                       \
            fprintf (stderr, "There is no element in stack to do the command " #jump_cmd "\n"); \
            return CANT_ ## jump_cmd ## _SPU;                                                   \
        }                                                                                       \
                                                                                                \
        size_t pointer = 0;                                                                     \
        memcpy (&pointer, processor->code + processor->ip, sizeof (pointer));                   \
                                                                                                \
        if (first  compare  second)                                                             \
        {                                                                                       \
            processor->ip = pointer;                                                            \
        }                                                                                       \
        else                                                                                    \
        {                                                                                       \
            processor->ip += sizeof (pointer);                                                  \
        }                                                                                       \
                                                                                                \
        break;                                                                                  \
    }
// ----------------------------------------------------------------------------------------------

enum SPU_ERROR processing (spu_t* const processor)
{
    ASSERT (processor != NULL, "Invalid argument for processing = %p\n", processor);

    while (processor->ip < processor->count_cmd)
    {
        command_t cmd_and_arg = processor->code [processor->ip];
        command_t cmd = (cmd_and_arg >> ARGUMENT_TYPE);
        command_t arg = (cmd_and_arg & ALL_ARGS);

        (processor->ip)++;
        switch (cmd)
        {
            case PUSH:
            {
                LOG_CMD_DEFINED (PUSH);
                enum SPU_ERROR result = push_cmd (processor, arg);
                if (result != DONE_SPU)
                {
                    return result;
                }
                break;
            }
            case POP:
            {
                LOG_CMD_DEFINED (POP);
                enum SPU_ERROR result = pop_cmd (processor, arg);
                if (result != DONE_SPU)
                {
                    return result;
                }
                break;
            }
            case ADD:
            {
                LOG_CMD_DEFINED (ADD);
                stack_elem first  = 0;
                stack_elem second = 0;

                if (!(get_two_args (&first, &second, &(processor->stk))))
                {
                    return CANT_ADD_SPU;
                }
                stack_push (processor->stk, first + second);
                break;
            }
            case SUB:
            {
                LOG_CMD_DEFINED (SUB);
                stack_elem first  = 0;
                stack_elem second = 0;
                if (!(get_two_args (&first, &second, &(processor->stk))))
                {
                    return CANT_SUB_SPU;
                }
                stack_push (processor->stk, first - second);
                break;
            }
            case MUL:
            {
                LOG_CMD_DEFINED (MUL);
                stack_elem first  = 0;
                stack_elem second = 0;
                if (!(get_two_args (&first, &second, &(processor->stk))))
                {
                    return CANT_MUL_SPU;
                }
                stack_push (processor->stk, first * second);
                break;
            }
            case DIV:
            {
                LOG_CMD_DEFINED (DIV);
                stack_elem first = 0;
                stack_elem second = 0;
                if (!(get_two_args (&first, &second, &(processor->stk))))
                {
                    return CANT_DIV_SPU;
                }

                stack_elem zero = 0;

                if (memcmp (&second, &zero, sizeof (stack_elem)) == 0)
                {
                    return CANT_DIV_SPU;
                }
                stack_push (processor->stk, first / second);
                break;
            }
            case SQRT:
            {
                LOG_CMD_DEFINED (SQRT);
                stack_elem number = 0;
                if (stack_pop (processor->stk, &number) == CANT_POP)
                {
                    return CANT_POP_IN_SQRT_SPU;
                }
                if (number < 0)
                {
                    return CANT_FIND_SQRT_SPU;
                }
                if (stack_push (processor->stk, sqrt (number)) == CANT_PUSH)
                {
                    return CANT_PUSH_IN_SQRT_SPU;
                }
                break;
            }
            case SIN:
            {
                LOG_CMD_DEFINED (SIN);
                stack_elem number = 0;
                if (stack_pop (processor->stk, &number) == CANT_POP)
                {
                    return CANT_POP_IN_SIN_SPU;
                }
                if (stack_push (processor->stk, sin (number)) == CANT_PUSH)
                {
                    return CANT_PUSH_IN_SIN_SPU;
                }
                break;
            }
            case COS:
            {
                LOG_CMD_DEFINED (COS);
                stack_elem number = 0;
                if (stack_pop (processor->stk, &number) == CANT_POP)
                {
                    return CANT_POP_IN_COS_SPU;
                }
                if (stack_push (processor->stk, cos (number)) == CANT_PUSH)
                {
                    return CANT_PUSH_IN_COS_SPU;
                }
                break;
            }
            case IN:
            {
                LOG_CMD_DEFINED (IN);
                stack_elem number = 0;
                int return_value = fscanf (stdin, "%lf", &number);
                if ((return_value == 0) ||(stack_push (processor->stk, number) == CANT_PUSH))
                {
                    return CANT_PUSH_IN_IN_SPU;
                }
                break;
            }
            case OUT:
            {
                LOG_CMD_DEFINED (OUT);
                stack_elem number = 0;
                if (stack_pop (processor->stk, &number) == CANT_POP)
                {
                    return CANT_POP_IN_OUT_SPU;
                }
                fprintf (stdout, "\nOUT = %.5lf\n", number);
                break;
            }
            case DUMP:
            {
                LOG_CMD_DEFINED (DUMP);
                if (DUMP_STACK (processor->stk) == CANT_DUMP)
                {
                    return CANT_DUMP_SPU;
                }
                break;
            }
            case CALL:
            {
                LOG_CMD_DEFINED (CALL);
                size_t pointer_size_t = processor->ip + sizeof (processor->ip);
                stack_elem pointer_stack_elem = 0;
                memcpy (&pointer_stack_elem, &pointer_size_t, sizeof (pointer_size_t));
                if (stack_push (processor->stack_func_call_ip, pointer_stack_elem) == CANT_PUSH)
                {
                    return CANT_PUSH_CALL_SPU;
                }
                memcpy (&(processor->ip), processor->code + processor->ip, sizeof (processor->ip));
                break;
            }
            case RET:
            {
                LOG_CMD_DEFINED (RET);
                stack_elem pointer_stack_elem = 0;
                if (stack_pop (processor->stack_func_call_ip, &pointer_stack_elem) == CANT_POP)
                {
                    return CANT_POP_CALL_SPU;
                }
                memcpy (&(processor->ip), &pointer_stack_elem, sizeof (processor->ip));
                break;
            }
            case DRAW:
            {
                LOG_CMD_DEFINED (DRAW);
                enum SPU_ERROR result = draw_cmd (processor);
                if (result != DONE_SPU)
                {
                    return result;
                }
                break;
            }
            case MEOW:
            {
                LOG_CMD_DEFINED (MEOW);
                enum SPU_ERROR result = meow_cmd (processor);
                if (result != DONE_SPU)
                {
                    return result;
                }
                break;
            }

            case JMP:
            {
                LOG_CMD_DEFINED (JMP);
                memcpy (&(processor->ip), processor->code + processor->ip, sizeof (processor->ip));
                break;
            }

            CASE_JUMP (JA,  > );
            CASE_JUMP (JAE, >=);
            CASE_JUMP (JB,  < );
            CASE_JUMP (JBE, <=);

            CASE_JUMP_EQUAL (JE,  ==);
            CASE_JUMP_EQUAL (JNE, !=);

            case HLT:
            {
                LOG_CMD_DEFINED (HLT);
                return DONE_SPU;
            }

            default:
            {
                LOG_CMD_DEFINED (Invalid command);
                return INVAL_COMMAND;
            }
        }
    }

    return WITHOUT_HLT;
}

#undef CASE_JUMP

#undef LOG_CMD_DEFINED

enum SPU_ERROR spu_ctor (spu_t* const processor, const char* const name_file_input)
{
    ASSERT (processor       != NULL, "Invalid argument for spu_ctor processor = %p\n", processor);
    ASSERT (name_file_input != NULL, "Invalid argument for spu_ctor name_file_input = %p\n", name_file_input);

    FILE* input = NULL;
    FOPEN (input, name_file_input, "r+b");
    if (input == NULL)
    {
        return CANT_CTOR_SPU;
    }

    if (fread (&(processor->count_cmd), 1, sizeof (processor->count_cmd), input) == 0)
    {
        FCLOSE (input);
        return CANT_CTOR_SPU;
    }

    processor->ip = 0;

    processor->code = (command_t*) calloc (processor->count_cmd, sizeof (command_t));
    if (processor->code == NULL)
    {
        FCLOSE (input);
        return CANT_CTOR_SPU;
    }

    if (fread (processor->code, sizeof (command_t), processor->count_cmd, input) == 0)
    {
        FCLOSE (input);
        FREE_AND_NULL (processor->code);
        return CANT_CTOR_SPU;
    }

    FCLOSE (input);

    memset (processor->regs, 0, COUNT_REGS * sizeof (processor->regs [0]));

    enum STACK_ERROR result = STACK_INIT_SHORT (processor->stk, processor->count_cmd);
    if (result != DONE)
    {
        return CANT_CTOR_SPU;
    }

    result = STACK_INIT_SHORT (processor->stack_func_call_ip, processor->count_cmd);
    if (result != DONE)
    {
        return CANT_CTOR_SPU;
    }

    processor->ram = (double*) calloc (RAM_SIZE, sizeof (double));
    if (processor->ram == NULL)
    {
        return CANT_CTOR_SPU;
    }
    memset (processor->ram, 0, RAM_SIZE * sizeof (double));

    return DONE_SPU;
}

enum SPU_ERROR spu_dtor (spu_t* const processor)
{
    ASSERT (processor != NULL, "Invalid argument for spu_dtor processor = %p\n", processor);

    processor->count_cmd = 0;

    processor->ip = 0;

    enum STACK_ERROR result = STACK_DTOR_SHORT (processor->stk);
    if (result != DONE)
    {
        return CANT_CTOR_SPU;
    }

    result = STACK_DTOR_SHORT (processor->stack_func_call_ip);
    if (result != DONE)
    {
        return CANT_CTOR_SPU;
    }

    free (processor->code);
    free (processor->ram);

    return DONE_SPU;
}

static enum SPU_ERROR push_cmd (spu_t* const processor, const command_t argument)
{
    ASSERT (processor != NULL, "Invalid argument for push_cmd processor = %p\n", processor);

    double number = 0;

    if (argument & REGISTER)
    {
        size_t number_reg = 0;

        memcpy (&number_reg, processor->code + processor->ip, sizeof (number_reg));
        processor->ip += sizeof (number_reg);

        if (number_reg >= COUNT_REGS)
        {
            return PUSH_INVAL_REG;
        }

        number += processor->regs [number_reg];
    }

    if (argument & IMMED_NUM)
    {
        double temp = 0;

        memcpy (&temp, processor->code + processor->ip, sizeof (temp));
        number += temp;

        processor->ip += sizeof (temp);
    }

    if (argument & RAM)
    {
        sleep (SLEEP_IN_RAM); //roflanEbalo - ахахаха, я поржал (вова)

        if (number >= RAM_SIZE)
        {
            return CANT_PUSH_IN_PUSH_SPU;
        }

        size_t temp = (size_t) round (number);
        number = processor->ram [temp];
    }

    if (stack_push (processor->stk, number) == CANT_PUSH)
    {
        return CANT_PUSH_IN_IN_SPU;
    }

    return DONE_SPU;
}

static enum SPU_ERROR pop_cmd (spu_t* const processor, const command_t argument)
{
    ASSERT (processor != NULL, "Invalid argument for pop_cmd processor = %p\n", processor);

    double* ptr_number = 0;

    if (argument & REGISTER)
    {
        size_t number_reg = 0;

        memcpy (&number_reg, processor->code + processor->ip, sizeof (number_reg));
        processor->ip += sizeof (number_reg);

        if (number_reg >= COUNT_REGS)
        {
            return POP_INVAL_REG;
        }

        ptr_number = processor->regs + number_reg;
    }

    if (argument & RAM)
    {
        sleep (SLEEP_IN_RAM);

        if ((ptr_number != 0) && (*(ptr_number) >= RAM_SIZE))
        {
            return CANT_POP_IN_POP_SPU;
        }

        ptr_number = (ptr_number == 0) ? processor->ram
                                       : processor->ram + (size_t) round (*ptr_number);
    }

    if (argument & IMMED_NUM)
    {
        size_t temp = 0;

        memcpy (&temp, processor->code + processor->ip, sizeof (temp));
        ptr_number = ptr_number + temp;
        processor->ip += sizeof (temp);
    }

    if (stack_pop (processor->stk, ptr_number) == CANT_POP)
    {
        return CANT_POP_IN_POP_SPU;
    }

    return DONE_SPU;
}

static enum SPU_ERROR draw_cmd (spu_t* const processor)
{
    ASSERT (processor != NULL, "Invalid argument for draw_cmd processor = %p\n", processor);

    sleep (SLEEP_IN_RAM);

//-------------------------SFML-------------------------------------------------------------------------------
    sf::RenderWindow window (sf::VideoMode ((SIZE_PIXEL + LEN_BORDER) * SIZE_COLUMNS + LEN_BORDER,
                                            (SIZE_PIXEL + LEN_BORDER) * SIZE_STRINGS + LEN_BORDER),
                                            "Draw");

    window.setFramerateLimit (60);

    bool done = false;

    while (window.isOpen ())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        if (!done)
        {
            for (size_t string_counter = 0; string_counter < SIZE_STRINGS; string_counter++)
            {
                for (size_t column_counter = 0; column_counter < SIZE_COLUMNS; column_counter++)
                {
                    sf::RectangleShape rectangle (sf::Vector2f ((float) SIZE_PIXEL, (float) SIZE_PIXEL));

                    rectangle.setPosition ((float) (column_counter * (SIZE_PIXEL + LEN_BORDER) + LEN_BORDER),
                                           (float) (string_counter * (SIZE_PIXEL + LEN_BORDER) + LEN_BORDER));

                    if ((size_t) processor->ram [string_counter * SIZE_COLUMNS + column_counter] != 0)
                    {
                        size_t color_elem = (size_t) *(processor->ram
                                                       + (string_counter * SIZE_COLUMNS + column_counter));

                        rectangle.setFillColor (sf::Color (*((unsigned char*) &color_elem),

                                                           *((unsigned char*) &color_elem),

                                                           *((unsigned char*) &color_elem)));
                    }
                    else
                    {
                        rectangle.setFillColor (sf::Color::Red);
                    }

                    window.draw (rectangle);
                }
            }
            window.display ();
            done = true;
        }
    }

    char control_symbol = '\0';

    fprintf (stderr, "Put \'T\' to get interpretation in symbols of draw of RAM in terminal.\n"
                     "Put any another symbol to continue without it\n");
    if (fscanf (stdin, "%c", &control_symbol) != 1)
    {
        return DONE_SPU;
    }
//------------------------------------------------------------------------------------------------------------

    if (control_symbol == 'T')
    {
        fputc ('\n', stdout);

        for (size_t string_counter = 0; string_counter < SIZE_STRINGS; string_counter++)
        {
            for (size_t column_counter = 0; column_counter < SIZE_COLUMNS; column_counter++)
            {
                if (((unsigned char) processor->ram [string_counter * SIZE_COLUMNS + column_counter] == '\0')
                    || (isspace ((char) processor->ram [string_counter * SIZE_COLUMNS + column_counter])))
                {
                    fputc (' ', stdout);
                }
                else
                {
                    fputc ((unsigned char) processor->ram [string_counter * SIZE_COLUMNS + column_counter], stdout);
                }
            }
            fputc ('\n', stdout);
        }
    }

    return DONE_SPU;
}
static enum SPU_ERROR meow_cmd (spu_t* const processor)
{
    ASSERT (processor != NULL, "Invalid argument processor = %p\n", processor);

    stack_elem number = 0;

    if (stack_pop (processor->stk, &number) != DONE)
    {
        return CANT_POP_IN_MEOW_SPU;
    }

    stack_elem counter = 0;

    while (counter < number)
    {
        fprintf (stdout, "meow ");
        counter++;
    }

    return DONE_SPU;
}

static bool get_two_args (stack_elem* const first, stack_elem* const second, stack_t* const stk)
{
    ASSERT (first  != NULL, "Invalid argument for get_two_args first = %p\n", first);
    ASSERT (second != NULL, "Invalid argument for get_two_args second = %p\n", second);
    ASSERT (stk    != NULL, "Invalid argument for get_two_args stk = %p\n", stk);

    if (stack_pop (*stk, first) == CANT_POP)
    {
        return false;
    }
    if (stack_pop (*stk, second) == CANT_POP)
    {
        return false;
    }

    return true;
}
