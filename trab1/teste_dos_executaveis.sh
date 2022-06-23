teste1="10 2500 4000000 1"
teste2="10 2500 4000000 2"
teste4="10 2500 4000000 4"
for i in {1..4}; do
	echo "./integral $teste1 <<< c" >> integral.resultado
	./integral $teste1 <<< c | grep 'Tempo' >> integral.resultado
done
for i in {1..4}; do
	echo "./integral $teste2 <<< c" >> integral.resultado
	./integral $teste2 <<< c | grep 'Tempo' >> integral.resultado
done
for i in {1..4}; do
	echo "./integral $teste4 <<< c" >> integral.resultado
	./integral $teste4 <<< c | grep 'Tempo' >> integral.resultado
done

teste1="20.24 70.45 5000000 1"
teste2="20.24 70.45 5000000 2"
teste4="20.24 70.45 5000000 4"

for i in {1..4}; do
	echo "./integral $teste1 <<< d" >> integral.resultado
	./integral $teste1 <<< d | grep 'Tempo' >> integral.resultado
done
for i in {1..4}; do
	echo "./integral $teste2 <<< d" >> integral.resultado
	./integral $teste2 <<< d | grep 'Tempo' >> integral.resultado
done
for i in {1..4}; do
	echo "./integral $teste4 <<< d" >> integral.resultado
	./integral $teste4 <<< d | grep 'Tempo' >> integral.resultado
done