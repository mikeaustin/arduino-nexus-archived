//
// Commands.h
//

#include <Arduino.h>

#ifndef __Nexus_Commands__
#define __Nexus_Commands__

namespace Nexus {

    inline Stream& operator <<(Stream& stream, symbol symbol)
    {
        return stream.print(symbol._string), stream;
    }

    inline Stream& operator <<(Stream& stream, const __FlashStringHelper* string)
    {
        return stream.print(string), stream;
    }

    inline Stream& operator <<(Stream& stream, char value)
    {
        return stream.print(value), stream;
    }

    inline Stream& operator <<(Stream& stream, int value)
    {
        return stream.print(value), stream;
    }

    inline Stream& operator <<(Stream& stream, unsigned long value)
    {
        return stream.print(value), stream;
    }

    // template<typename DataType>
    // inline Stream& operator <<(Stream& stream, DataType value)
    // {
    //     return stream.print(value), stream;
    // }

    inline Stream& operator <<(Stream& stream, const void* pointer)
    {
        return stream.print(reinterpret_cast<intptr_t>(pointer)), stream;
    }

    inline Stream& operator <<(Stream& stream, Stream& (*func)(Stream&))
    {
        return func(stream);
    }
    
    inline Stream& endl(Stream& stream)
    {
        stream.println(F(""));

        return stream;
    }

    struct Column {

        symbol  sym;
        uint8_t width;

    };

    inline Stream& operator <<(Stream& stream, const Column& column)
    {
        stream.print(column.sym);

        for (int i = 0; i < column.width - column.sym.size(); ++i)
        {
            stream.print(' ');
        }

        return stream;
    }

    inline Column column(symbol symbol, uint8_t width)
    {
        Column column = { symbol, width };

        return column;
    }

    struct Commands {

        static const char help_name[] PROGMEM;
        static const char info_name[] PROGMEM;
        static const char tasks_name[] PROGMEM;

        static Task* help(Task* parent)
        {
            Stream& stream = parent->getStream();

            Shell* shell = static_cast<Shell*>(parent);

            for (const Command* command = shell->getCommands(); pgm_ptr<void*>(command->name) != NULL; ++command)
            {
                const symbol name = pgm_ptr<const __FlashStringHelper*>(command->name);

                stream << name << endl;
            }

            return NULL;
        }

        static Task* info(Task* parent)
        {
            Stream& stream = parent->getStream();

            stream << F("Up for ") << millis() / 1000 << F(" seconds.") << endl;
            stream << availableMemory << F(" bytes free.") << endl;

            return NULL;
        }

        static Task* tasks(Task* parent)
        {
            Stream& stream = parent->getStream();

            for (Task* task = Scheduler.getTasks(); task != NULL; task = task->getNext())
            {
                stream << column(task->getName(), 16) << task << endl;
            }

            return NULL;
        }

    };

}

#endif
