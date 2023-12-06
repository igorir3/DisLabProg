import serial
import curses
import time
from curses import wrapper
import serial.tools.list_ports
import random

def strtointifreal(str): # Переводит str в int, если это возможно
    try:
        str = int(str)
    except:
        return str
    return str

def checkedinput(inputtext, filter, errormessage): # Это функция нужна для запроса у пользователя ввода, но возвращает
                                                   # ввод, только если функция-фильтр вернёт true
    while True:
        userinput = input(inputtext)
        if filter(userinput):
            return userinput
        else:
            print(errormessage)

def main(stdscr):

    #Глобализация
    global PORT
    global BAUDRATE
    global SER

    #Настройка основного окна curses
    stdscr.refresh()
    stdscr.nodelay(True)
    stdscr.keypad(True)
    textlist = []

    lines = 30
    cols = 70

    #Создание второстепенных окон со своей координтной системой
    outputtext = curses.newwin(lines, cols, 5, 5)
    inputtext = curses.newwin(3, cols, lines + 5, 5)
    debugwin = curses.newwin(3, 5, lines + 5, cols + 5)
    datawin = curses.newwin(4, 22, 5, cols + 5)

    #Рисуем рамочку
    outputtext.box()
    inputtext.box()
    debugwin.box()
    datawin.box()

    inputtextstr = ''

    while True:

        #Чтобы не обновлять основное окно, обновляем все второстепенные
        outputtext.refresh()
        inputtext.refresh()
        debugwin.refresh()
        datawin.refresh()

        #Проверяем есть ли ввод, если нет выводим None
        try:
            key = stdscr.getkey()
        except:
            key = None

        if key != None:
            if key == '\b': #Если пользователь нажимает backspace убираем последний элемент str строки
                if len(inputtextstr) > 1:
                    inputtextstr = inputtextstr[:-1]
                else:
                    inputtextstr = ''
            elif key == '\n': #Если пользователь нажимает Enter отправляет данные на ардуино, а также очищает str
                if inputtextstr != '':
                    SER.write(bytes(inputtextstr + "\n", "ASCII"))
                    textlist.append(inputtextstr)
                inputtextstr = ''
            elif len(inputtextstr) < cols - 3 and len(key) == 1: # Фильтр на спец. символы, если пользователь нажимает,
                                                                 # только один символ, следовательно это знак или буква
                try:
                    inputtextstr = inputtextstr + key
                except:
                    inputtextstr = inputtextstr
            else:
                inputtextstr = inputtextstr

        SER.write(b'OUTPUT\n') #Отправляем на ардуино OUTPUT
        response = SER.readline() # Читаем ввод
        if response != b'': # Если ввод пустой, игнорируем
            textlist.append(response.decode().rstrip())
            with open('output.log', 'a+') as outputfile:
                outputfile.write(response.decode().rstrip() + '\n')


        if len(textlist) > lines - 2: # Если массив textlist больше чем можно отобразить в окне, удаляем первое значение
            del textlist[0]

        for aa in range(len(textlist)): # Выводим массив по строчно
            outputtext.addstr(aa + 1, 1, f"{textlist[aa] + (' ' * (cols - len(textlist[aa]) - 2))}") # При выводе
            # заполняем пустое пространство пробелами, дабы не очищать окно, а то слишком долгий и не красивый процесс

        inputtext.addstr(1, 1, f"{inputtextstr + (' ' * (cols - len(inputtextstr) - 3))}")
        # выводим, то что вводит пользователь

        newlinechar = str(len(inputtextstr))
        debugwin.addstr(1, 1, f"{(' ' if len(newlinechar) == 1 else '') + newlinechar}")
        # вывод длины строки, что ввёл пользователь

        datawin.addstr(1, 1, f"Port    : {PORT}    ") # вывод порта
        datawin.addstr(2, 1, f"Baudrate: {BAUDRATE}") # вывод скорости передачи (А МОЖЕТ ЕЁ УВЕЛИЧИТЬ?)
        time.sleep(0.2)

ports = [[port.name, port.description] for port in serial.tools.list_ports.comports()] # записываем в массив порт и
                                                                                # устройство подписанное под этим портом

for aa in range(len(ports)): print(f"{aa + 1}. {ports[aa][0]}: {ports[aa][1]}") # вывод портов

portn = checkedinput("Введите номер порта из списка: ", lambda x: True if isinstance(strtointifreal(x), int) and strtointifreal(x) > 0 and strtointifreal(x) <= len(ports) else False, "Ввод не верный повторите попытку")

#Тут нужно описывать по кусочкам:
#Тут мы запрашиваем функцию checkedinput, 1 - это то, что выводится при запросе ввода
#3 - это то, что выводится при ошибке
#2 - это функция фильтр
#т.к. мне нужна мелкая функция в виде значения объявляю её через команду lambda
#(x) - это значение, которая должна вводится при запросе функции
#Далее после ":" мы вводим код, который в обычном виде выглядит так:
#
#if isinstance(strtointifreal(x), int) and strtointifreal(x) > 0 and strtointifreal(x) <= len(ports):
#   return True
#else:
#   return False
#
#Для этого я вначале пишу, чему равно значение, которое выдаст if-овая конструкция, в данном случае вывод, а после пишу
#if, и если значение будет истинным, то выдаст значение вначале, а если нет, то значение написанное после конструкции else

PORT = ports[int(portn) - 1][0]

BAUDRATE = checkedinput("Введите baudrate: ", lambda x: True if isinstance(strtointifreal(x), int) and strtointifreal(x) > 0 else False, "Ввод не верный повторите попытку")
#Тоже самое, что и portn, но с более простой функцией

SER = serial.Serial(PORT, BAUDRATE, timeout=1, bytesize=8, parity='N', stopbits=1, xonxoff=False, rtscts=False)
#Открываем Serial, Тут представлены все возможные (если верить StackOverflow), аргументы запуска, но по факту нам нужны, только первые 3.

wrapper(main) #Инициализация curses с функцией main, в теории можно и без такой комманды, но я сделал так.