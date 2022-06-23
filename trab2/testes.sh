#!/bin/bash

# gera 5 arquivos diferentes e texta cada um deles

## gerando arquivos, comente caso já tenha gerado para n perder tempo

#./gerarArquivo.sh 10 5 arqsTexto/input1.txt
#./gerarArquivo.sh 50 10 arqsTexto/input2.txt
#./gerarArquivo.sh 5000 40 arqsTexto/input3.txt
#./gerarArquivo.sh 10 4000 arqsTexto/input4.txt
#./gerarArquivo.sh 10 20000 arqsTexto/input5.txt

## testando arquivos

executavel="trab2"
i=1
for j in 5 10 40 4000 20000; do
	
	input="arqsTexto/input$i.txt"
	i=$((i + 1))

	# testando velociadede

	## 5 testes
	tempo_seq_medio=0
	tempo_conc_medio=0
	for k in {1..5}; do 
		echo "teste $k: ./$executavel 4 $j $input arqsTexto/output.txt"
		mat=$(./$executavel 4 $j $input arqsTexto/output.txt)
		tempo_seq=$(echo "$mat" | grep "Tempo sequencial" | cut -d '=' -f 2)
		tempo_conc=$(echo "$mat" | grep "Tempo concorrente" | cut -d '=' -f 2)
			
		tempo_seq_medio=`echo|awk -v y1=$tempo_seq_medio -v y2=$tempo_seq '{print y1+y2}'| tr "," .`
		tempo_conc_medio=`echo|awk -v y1=$tempo_conc_medio -v y2=$tempo_conc '{print y1+y2}'| tr "," .`

	done
	tempo_seq_medio=$(bc <<< "scale=6; $tempo_seq_medio/5")
	tempo_conc_medio=$(bc <<< "scale=6; $tempo_conc_medio/5")
	acc=$(bc <<< "scale=6; $tempo_seq_medio/$tempo_conc_medio")
	echo "Tempo sequencial médio: $tempo_seq_medio"
	echo "Tempo concorrente médio: $tempo_conc_medio"
	echo "Aceleração do $input = $acc"

	## testando confiabilidade

	while read line
	do
		if ! [[ $(cat arqsTexto/output.txt | grep -- "$line") ]]; then
			echo "$line não detectado no output do arquivo $input"
		fi
	done < solucao_sequencial.txt
	
done

rm -f solucao_sequencial.txt
