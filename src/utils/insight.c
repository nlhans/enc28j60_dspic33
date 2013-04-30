#include <stdarg.h>
#include "insight.h"
#include "stddefs.h"
#include "circularBuffer.h"
#include "uart.h"

#if INSIGHT_LEVEL > 0
const insightFormats_t insightDictionary[INSIGHT_MESSAGES] = {
    INSIGHT_TABLE(INSIGHT_DICT, INSIGHT_DICT_ARGUMENT, INSIGHT_DICT_POINTER, INSIGHT_DICT_STRING)
};
circularBuffer_t insightBuffer;
UI08_t insightTextBuffer[256];
#endif

const char insightSOFBytes[4] = {'I','N','S','I'};
const char insightEOFBytes[4] = {'G','H','T','!'};

void insight_init()
{
#if INSIGHT_LEVEL > 0
    //
    circularBufferInit(&insightBuffer, insightTextBuffer, sizeof(insightTextBuffer));

    INSIGHT(INIT_INSIGHT, __DATE__, __TIME__);
#endif
}

void insight_msg(UI16_t msgCode, UI16_t fileNumber, UI16_t line, ...)
{
#if INSIGHT_LEVEL > 0
    if(msgCode == 0) return;
    UI16_t msgSize;
    UI08_t nArgs = insightDictionary[msgCode].msgCount;
    UI08_t i ;

    if (insightDictionary[msgCode].msgLvl > INSIGHT_LEVEL) return;

    // va_list
    va_list argList;
    va_start(argList, nArgs);

    circularBufferWriteBytes(&insightBuffer, (UI08_t*)insightSOFBytes, sizeof(insightSOFBytes));
    circularBufferWriteInt16(&insightBuffer, msgCode);
    circularBufferWriteInt16(&insightBuffer, line);
    circularBufferWriteInt16(&insightBuffer, fileNumber);
    
    for(i = 0; i < nArgs; i++)
    {
        msgSize = insightDictionary[msgCode].msgSizes[i];

        if ((msgSize & INSIGHT_DICT_POINTER_WORD) != 0)
        {
            if ((msgSize & INSIGHT_DICT_POINTER_SIZEMASK) == 0)
                circularBufferWriteString(&insightBuffer, va_arg(argList, char*));
            else
                circularBufferWriteBytes(&insightBuffer, va_arg(argList, UI08_t*), (msgSize & INSIGHT_DICT_POINTER_SIZEMASK));
        }
        else
        {
            switch(msgSize)
            {
                case 0:
                    //circularBufferWriteString(&insightBuffer,":)");
                    circularBufferWriteString(&insightBuffer, va_arg(argList, char*));
                    break;
                case 1: // int08
                    circularBufferWrite(&insightBuffer, va_arg(argList, UI08_t));
                    break;

                case 2: // int16
                    circularBufferWriteInt16(&insightBuffer, va_arg(argList, UI16_t));
                    break;

                case 4: // int32/float
                    circularBufferWriteInt32(&insightBuffer, va_arg(argList, UI32_t));
                    break;

                case 8: // int64/double
                    circularBufferWriteDouble(&insightBuffer, va_arg(argList, double));
                    break;
            }
        }
    }
    circularBufferWriteBytes(&insightBuffer, (UI08_t*)insightEOFBytes, sizeof(insightEOFBytes));

    //va_end(argList);

    while(circularBufferAvailable(&insightBuffer))
        uartTxByte(circularBufferRead(&insightBuffer));
#endif
}