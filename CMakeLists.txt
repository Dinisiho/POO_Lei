cmake_minimum_required(VERSION 3.20)
project(TrabalhoPOO)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(TrabalhoPOO main.cpp
        ecra/Ecra.h
        ecra/Ecra.cpp
        fabrica/Fabrica.h
        fabrica/Fabrica.cpp
        terreno/terreno.cpp
        terreno/terreno.h
        posicao/Posicao.cpp
        posicao/Posicao.h
        vista/VistaFabrica.h
        vista/VistaFabrica.cpp
        settings/Settings.h)
