cmake_minimum_required(VERSION 3.22)
project(ficheirosTP)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_executable(ficheirosTP
        cmd.cpp
        cmd.h
        curses.h
        main.cpp
        Terminal.cpp
        Terminal.h
        Zona.cpp
        Zona.h
        Processador.cpp
        Processador.h
        Regra.cpp
        Regra.h
        Propriedade.cpp
        Propriedade.h
        Sensor.cpp
        Sensor.h
        Aparelho.cpp
        Aparelho.h
        Habitacao.cpp
        Habitacao.h Simulacao.cpp Simulacao.h)

target_link_libraries(${PROJECT_NAME}
        ${PROJECT_SOURCE_DIR}/../PDCurses-3.9/wincon/pdcurses.a)

