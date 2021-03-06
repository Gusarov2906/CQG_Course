# C++ Coding Standard

## Файлы

### Имена файлов

#### Имена

* Используются следующие расширения файлов:
  - `.h` - для заголовочных файлов
  - `.cpp` – для файлов С++
* Имя заголовочного файла, содержащего объявление класса, соответствует имени класса.
* Имена заголовочных файлов и имена файлов С++ совпадают.
* Пример: `MyClass.h`/`MyClass.cpp` для класса `MyClass`.
* Каждому классу соответствует своя пара файлов `.h`/`.cpp`.
* Исключение: группа небольших классов, имеющих общее назначение, может находиться в одной паре файлов `.h`/`.cpp`.

#### Структура файла

* Длина строки не превышает 120 символов.
* Заголовочные файлы C++ защищены от многократных включений с помощью директивы `#pragma once`, которая находится в самом начале файла.
* Все директивы #include располагаются в начале файла.
* Порядок секций в объявлении класса должен быть следующим:

  - `public`
  - `protected`
  - `private`

### Форматирование

#### Общее

* Длина строки не превышает 120 символов.
* Отступ в коде составляет 4 пробела. Табуляция не используется.
* Одна строка – одно действие (за исключением арифметических операторов и вложенных вызовов функций).
* В одной строке объявляется (и инициализируется) одна переменная.
* Фигурные скобки располагаются каждая на своей строке.

#### Имена переменных

* Классы называются в стиле `CamelCase` (с большой буквы).
* Переменные называются в стиле `camelCase` (с маленькой буквы).
* Свободные функции и public методы классов называются в стиле `CamelCase` (с большой буквы).
* private/protected методы классов называются в стиле `camelCase` (с маленькой буквы).
* Для private и protected полей класса используется префикс `m_`.
* Для интерфейсов (класс без данных с чистыми виртуальными функциями) используется префикс `I`.
* Другие префиксы не использются.

#### Комментарии

* Используется однострочные комментарии (`//`).
* Символы для многострочных комментариев (`/**/`) не используются.
* Все комментарии должны быть на английском языке.

### C++

#### Общее

* `using namespace std` не используется. Префикс `std::` обязателен.
* В качестве константы нулевого указателя используется `nullptr`. `NULL` и `0` не используются для обозначения нулевого указателя.
* Базовый класс в иерархии имеет виртуальный деструктор.
* Виртуальные методы не вызываются в конструкторе или деструкторе.
* Модификатор `const` используется везде, где это возможно:
  * Для локальных переменных, если они не изменяются;
  * Для параметров функций, если они не изменяются;
  * Для методов класса, если эти методы не меняют состояние объекта.
