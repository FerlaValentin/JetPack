@REM Compilación y Enlace con biblioteca gráfica.
@cls
@echo ---------------------------------------------------
@echo  ESAT Curso 2025-2026 Asignatura PRG Primero
@echo ---------------------------------------------------
@echo  Proceso por lotes iniciado.
@echo ---------------------------------------------------
@echo off

@REM ESAT Librarie
set esat_include=.\Lib_Graph\ESAT_rev248\include
set esat_lib=.\Lib_Graph\ESAT_rev248\bin\ESAT.lib
set opengl_lib=opengl32.lib user32.lib gdi32.lib shell32.lib Ws2_32.lib

@REM START custom libraries
set local_include=.

set main_file=.\merge.cc
set sources=.\audio.cc .\data.cc .\interface.cc .\jugador.cc .\nave.cc .\enemigos.cc .\colisiones.cc .\level_config.cc

cl /nologo /Zi /GR- /EHs /MD %main_file% %sources% -I%esat_include% -I%local_include% %esat_lib% %opengl_lib%
 
@echo ---------------------------------------------------
@echo  Proceso por lotes finalizado.
@echo ---------------------------------------------------