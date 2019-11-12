#!/bin/bash

checkDependencia () {
	dpkg -s "$1" >/dev/null 2>&1 && {
        echo 0
    } || {
        echo 1
    }
}

barraSeparadora='**********************************************************************************************'

echo "Bienvenido a nuestro TP de compiladores
Integrantes:
* Diaz Pablo
* Enriquez Facundo
* Selva Ricardo
$barraSeparadora
A continuacion se controlaran las dependencias"


if [[  "$(checkDependencia  'flex')" == 1 ]]; then
	paquetesAInstalar+=("flex")
fi

if [[  "$(checkDependencia  'bison')" == 1 ]]; then
	paquetesAInstalar+=("bison")
fi

if [[  "$(checkDependencia  'gcc')" == 1 ]]; then
	paquetesAInstalar+=("gcc")
fi

if [[  "$(checkDependencia  'build-essential')" == 1 ]]; then
	paquetesAInstalar+=("build-essential")
fi

if [[  "$(checkDependencia  'php-cli')" == 1 ]]; then
	paquetesAInstalar+=("php-cli")
fi

if [[ ${#paquetesAInstalar[@]} > 0 ]]; then
	echo 'Los siguientes paquetes deben ser instalados: '
	echo  ${paquetesAInstalar[@]}
	echo 'Presione Y para instalar automaticamente (se pedira acceso superusuario), de lo contrario, presione N para salir e instaalar manualmente'
	echo -n '(Y/N)'
	read ans
	if [[ ${ans,,} == 'y' ]]; then
		sudo apt-get update && sudo apt-get install ${paquetesAInstalar[@]}
	else
		echo 'Comando para instalar: sudo apt-get update && sudo apt-get install '${paquetesAInstalar[@]} 
		exit;
	fi
else
	echo "Todas las dependencias se encuentran instaladas"
	read -p 'Presione cualquier tecla para continuar...' ans
fi

if ! test -f "bin/comp"; then
	echo "$barraSeparadora"
    echo 'El conpilador debe compilarse (valga la redundancia)
Se ejecutara el comando "make"'
    make
    echo 'Compilado exitosamente!'
    read -p 'Presione cualquier tecla para continuar...' ans
fi

opcion=100

while [[ ! "$opcion" == "0" ]]; do
echo "
$barraSeparadora
Menu:
1. Compilar y ejecutar ejemplo 1 (detalles)
2. Compilar y ejecutar ejemplo 1 (detalles)
3. Compilar y ejecutar ejemplo 1 (detalles)
4. Instrucciones para compilar archivos propios
0. Salir"

read -p "Ingrese su eleccion (0-4): " opcion

case "$opcion" in
	1|2|3)
		bin/comp progs/prog"$opcion".pse compilado/prog"$opcion".php && php compilado/prog"$opcion".php
	;;
	4)
		echo 'Ejecute el binario comp localizado en la carpeta bin, el parametro es el fichero de entrada y segundo el parametro es el fichero de salida. Si no se especifica el segundo parametro, se utiliza la salida estandar.'
		read -p 'Presione cualquier tecla para continuar...' ans
	;;
	0)
		echo 'Chau'
		exit
	;;
	*)
		echo 'Opcion no valida'
	;;
esac

done












