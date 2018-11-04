#! /usr/bin/env python3

import random
import os
import sys
import atexit
import time


def getTermSize():
    sz = os.get_terminal_size()
    return sz.lines, sz.columns


def enterAlternativeScreen():
    print('\033[?1049h', end='')


def leaveAlternativeScreen():
    print('\033[?1049l', end='')


def setCursorPos(x, y):
    print('\033[%d;%dH' % (x, y), end='')


def clearScreen():
    print('\033[3J', end='')


def drawFrame():
    global height, width
    setCursorPos(1, 1)
    print('╭'+''.join('─'*(width-2))+'╮', end='')
    setCursorPos(2, 1)
    print('│'+''.join(' '*(width-2))+'│', end='')
    setCursorPos(3, 1)
    print('├'+''.join('─'*(width-2))+'┤', end='')
    for i in range(4, height):
        setCursorPos(i, 1)
        print('│'+''.join(' '*(width-2))+'│', end='')
    """
    setCursorPos(height-2, 1)
    print('├'+''.join('─'*(width-2))+'┤', end='')
    setCursorPos(height-1, 1)
    print('│'+''.join(' '*(width-2))+'│', end='')
    """
    setCursorPos(height, 1)
    print('╰'+''.join('─'*(width-2))+'╯', end='')


def clearBuffer():
    global buffer
    buffer = []
    redrawScreen()


def bufferAddNewLine(l):
    global buffer
    buffer.append(l)
    redrawScreen()


def setTitle(x):
    global title
    title = x
    redrawScreen()


def setStatus(x):
    global status
    status = x
    redrawScreen()


def redrawScreen():
    global title, buffer, status, height, width
    if len(buffer) < height - 4:
        to_print = buffer + [''] * (height - 4 - len(buffer))
    else:
        to_print = buffer[-(height-4):]
    clearScreen()
    drawFrame()
    setCursorPos(2, 2)
    print(title, end='')
    for i in range(0, height-4):
        setCursorPos(4+i, 2)
        print(to_print[i], end='')
    """
    setCursorPos(height-1, 2)
    print(status, end='')
    """
    setCursorPos(height-1, width-1)
    sys.stdout.flush()


def readFirstLine(name):
    f = open(name)
    ret = f.readline().strip()
    f.close()
    return ret


def halt():
    while True:
        time.sleep(1)


def main():
    global title, buffer, status, height, width
    enterAlternativeScreen()
    atexit.register(leaveAlternativeScreen)
    setCursorPos(0, 0)

    height, width = getTermSize()

    title, buffer, status = '', [], ''

    if (height < 16):
        print("This program needs at least 10 rows to exec...", file=sys.stderr)
        input()
    else:
        redrawScreen()
        testId = 0
        while True:
            progSeed = random.randint(0, 19260817)
            setTitle("PROGRAM #%d : SEED #%d" % (testId, progSeed))
            clearBuffer()
            bufferAddNewLine("Generating program...")
            os.system("./bin/generator %d 2>/dev/null > ./tmp/QAQ.c" %
                      progSeed)
            bufferAddNewLine("Running Assembler...")
            os.system("./bin/compiler < ./tmp/QAQ.c > ./tmp/my_QAQ.s")
            bufferAddNewLine("Successfully assembled...")
            bufferAddNewLine("Compiling Program...")
            if os.system("gcc ./tmp/QAQ.c ./src/main.c -o ./tmp/QAQ 2> ./tmp/compiler_output_ans") + os.system("gcc ./tmp/my_QAQ.s ./src/main.c -o ./tmp/my_QAQ 2> ./tmp/compiler_output_my") != 0:
                bufferAddNewLine("Compile ERROR!")
                halt()
            for i in range(1, 11):
                dataSeed = random.randint(0, 19260817)
                bufferAddNewLine("")
                bufferAddNewLine("Running test #%d" % i)
                bufferAddNewLine("Random Seed # %d" % dataSeed)
                os.system("./tmp/QAQ %d 2>/dev/null > ./tmp/ans" % dataSeed)
                os.system("./tmp/my_QAQ %d 2>/dev/null > ./tmp/out" % dataSeed)
                ans = readFirstLine('./tmp/ans')
                out = readFirstLine('./tmp/out')
                if ans == out:
                    bufferAddNewLine('Correct Answer:' + out + ' == ' + ans)
                else:
                    bufferAddNewLine('Wrong Answer:' + out + ' != ' + ans)
                    halt()

            testId += 1
            time.sleep(0.1)


if __name__ == '__main__':
    main()
