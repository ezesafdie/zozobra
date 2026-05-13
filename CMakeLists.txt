cmake_minimum_required(VERSION 3.29)
project(zozobra C)

set(CMAKE_C_STANDARD 11)

include_directories(tetris_zozobra)
include_directories(tetris_zozobra/release)
include_directories(tetris_zozobra/release/GBT_v2026.1C.01)
include_directories(tetris_zozobra/release/GBT_v2026.1C.01/include)
include_directories(tetris_zozobra/release/GBT_v2026.1C.01/include/GBT)

add_executable(zozobra
        tetris_zozobra/release/GBT_v2026.1C.01/include/GBT/gbt.h
        tetris_zozobra/release/GBT_v2026.1C.01/include/GBT/gbt_entrada.h
        tetris_zozobra/release/GBT_v2026.1C.01/include/GBT/gbt_graficos.h
        tetris_zozobra/release/GBT_v2026.1C.01/include/GBT/gbt_logger.h
        tetris_zozobra/release/GBT_v2026.1C.01/include/GBT/gbt_temporizador.h
        tetris_zozobra/release/GBT_v2026.1C.01/include/GBT/gbt_vector.h
        tetris_zozobra/estados.h
         tetris_zozobra/codigos_retorno.h
        tetris_zozobra/fin_juego.c
        tetris_zozobra/fin_juego.h
        tetris_zozobra/ingreso_nombre.c
        tetris_zozobra/ingreso_nombre.h
        tetris_zozobra/juego.c
        tetris_zozobra/juego.h
        tetris_zozobra/main.c
        tetris_zozobra/menu_principal.c
        tetris_zozobra/menu_principal.h
        tetris_zozobra/pausa.c
        tetris_zozobra/pausa.h
        tetris_zozobra/tablero.c
        tetris_zozobra/tablero.h
        tetris_zozobra/utils.c
        tetris_zozobra/utils.h
        tetris_zozobra/render.c
        tetris_zozobra/tetromino.c)
