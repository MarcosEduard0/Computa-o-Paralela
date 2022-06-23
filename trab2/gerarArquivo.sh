#!/bin/bash

is_default=0
# checa as varaveis
if [ -z "$1" ]; then
	echo "Você pode modificar o número de linhas. Default = 50"
	linhas=50
	is_default=1
else
	linhas=$1
fi

if [ -z "$2" ]; then
	echo "Você pode modificar o número de colunas. Default = 5"
	colunas=5
	is_default=1
else
	colunas=$2
fi

# decide nome do arquivo
if [ -z "$3" ]; then
	echo "Você pode modificar o nome do arquivo de saída. Default = input.txt"
	filename="input.txt"
	is_default=1
else
	filename=$3
fi
rm -f -- $filename
touch $filename

#bota msg pro usuario
if [ $is_default -eq 1 ]; then
	echo ''
	echo "Caso queira:"
	echo "$0 <numero de linhas> <numero de colunas> <nome do arquivo>"
	echo ''
fi

# cria os arquivos
echo "$(($linhas * $colunas))" >> $filename
for ((i=1;i<=$linhas;i++)); do
	str=""
	for ((j=1;j<=$colunas;j++)); do
		str+="$((-50 + $RANDOM % 100))"
		str+=" "
	done
	echo $str >> $filename
done
