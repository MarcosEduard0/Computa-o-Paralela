import java.util.Random;

/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Aluno.: Marcos Eduardo */
/* Laboratório: 6 */
/* Codigo: Acessando variável compartilhada em um programa multithreading Java */
/* -------------------------------------------------------------------*/

//classe da estrutura de dados (recurso) compartilhado entre as threads
class Dados {
   // recurDadoso compartilhado
   int vetor[];
   int NTHREADS;
   private int quantPares;

   // construtor
   public Dados() {
      this.quantPares = 0;
   }

   public void inc(int id) {
      for (int i = id; i < vetor.length; i += NTHREADS)
         if (vetor[i] % 2 == 0)
            this.set();
   }

   public synchronized int set() {
      return this.quantPares++;
   }

   public synchronized int get() {
      return this.quantPares;
   }

}

// classe que estende Thread e implementa a tarefa de cada thread do programa
class Terefa extends Thread {
   // identificador da thread
   private int id;
   // objeto compartilhado com outras threads
   Dados d;

   // construtor
   public Terefa(int tid, Dados d) {
      this.id = tid;
      this.d = d;
   }

   // metodo main da thread
   public void run() {
      System.out.println("Thread " + this.id + " iniciou!");
      this.d.inc(id);
      System.out.println("Thread " + this.id + " terminou!");
   }
}

// classe da aplicacao
class lab6 {
   static int N;
   static int TAM;
   static int quantParesSeq = 0;

   public static void main(String[] args) {

      // recebimento de argumentos por linha de comando
      if (args.length < 2) {
         System.out.printf("java <nome> <nº de threads> <tamanho do vetor> \n");
         System.exit(1);
      }
      N = Integer.parseInt(args[0]);
      TAM = Integer.parseInt(args[1]);

      // vetor de inteiros para verificação de quant. de pares
      int v[] = new int[TAM];

      // instância um objeto da classe Random usando o construtor padrão
      Random gerador = new Random();

      // Preenchendo vetor
      for (int i = 0; i < v.length; i++)
         v[i] = gerador.nextInt();

      // reserva espaço para um vetor de threads
      Thread[] threads = new Thread[N];

      // cria uma instancia do recurso compartilhado entre as threads
      Dados d = new Dados();
      d.vetor = v;
      d.NTHREADS = N;

      // cria as threads da aplicacao
      for (int i = 0; i < threads.length; i++)
         threads[i] = new Terefa(i, d);

      // inicia as threads
      for (int i = 0; i < threads.length; i++)
         threads[i].start();

      // espera pelo termino de todas as threads
      for (int i = 0; i < threads.length; i++) {
         try {
            threads[i].join();
         } catch (InterruptedException e) {
            return;
         }
      }

      // verificacao de forma sequencial para garantia da corretudo
      for (int i = 0; i < v.length; i++)
         if (v[i] % 2 == 0)
            quantParesSeq++;

      if (quantParesSeq != d.get()) {
         System.out.println("ERRO: Houve condicao de corrida na versao concorrente.");
         System.exit(2);
      }

      System.out.println("Quantidade de pares Sequencial = " + quantParesSeq);
      System.out.println("Quantidade de pares Concorrente = " + d.get());
   }
}
