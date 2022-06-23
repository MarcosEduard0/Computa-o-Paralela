/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Aluno: Marcos Eduardo */
/* Codigo: Leitores e escritores usando monitores em Java */
/* -------------------------------------------------------------------*/

// Monitor que implementa a logica do padrao leitores/escritores
class LE {
  private int leit, escr;
  int val;

  // Construtor
  LE() {
    this.leit = 0; // leitores lendo (0 ou mais)
    this.escr = 0; // escritor escrevendo (0 ou 1)
    this.val = 0; // variavel de uso compartilhado
  }

  // Entrada para leitores
  public synchronized void EntraLeitor(int id) {
    try {
      while (this.escr > 0) {
        System.out.println("le.leitorBloqueado(" + id + ")");
        wait(); // bloqueia pela condicao logica da aplicacao
      }
      this.leit++; // registra que ha mais um leitor lendo
      System.out.println("le.leitorLendo(" + id + ")");
    } catch (InterruptedException e) {
    }
  }

  // Saida para leitores
  public synchronized void SaiLeitor(int id) {
    this.leit--; // registra que um leitor saiu
    if (this.leit == 0)
      this.notify(); // libera escritor (caso exista escritor bloqueado)
    // this.notifyAll();
    System.out.println("le.leitorSaindo(" + id + ")");
  }

  // Entrada para escritores
  public synchronized void EntraEscritor(int id) {
    try {
      while ((this.leit > 0) || (this.escr > 0)) {
        System.out.println("le.escritorBloqueado(" + id + ")");
        wait(); // bloqueia pela condicao logica da aplicacao
      }
      this.escr++; // registra que ha um escritor escrevendo
      System.out.println("le.escritorEscrevendo(" + id + ")");
    } catch (InterruptedException e) {
    }
  }

  // Saida para escritores
  public synchronized void SaiEscritor(int id) {
    this.escr--; // registra que o escritor saiu
    notifyAll(); // libera leitores e escritores (caso existam leitores ou escritores bloqueados)
    System.out.println("le.escritorSaindo(" + id + ")");
  }

  // Entrada para LeitorEscritor
  public synchronized void EntraLeitorEscritor(int id) {
    try {
      while ((this.leit > 0) || (this.escr > 0)) {
        System.out.println("le.leitorEscritorBloqueado(" + id + ")");
        wait(); // bloqueia pela condicao logica da aplicacao
      }
      this.escr++; // registra que ha um leitor|escritor escrevendo
      System.out.println("le.leitorEscritorEscrevendo(" + id + ")");
    } catch (InterruptedException e) {
    }
  }

  // Saida para escritores
  public synchronized void SaiLeitorEscritor(int id) {
    this.escr--; // registra que o leitor|escritor saiu
    notifyAll(); // libera leitores e escritores (caso existam leitores ou escritores bloqueados)
    System.out.println("le.leitorEscritorSaindo(" + id + ")");
  }
}

// Aplicacao de exemplo--------------------------------------------------------
// Leitor
class Leitor extends Thread {
  int id; // identificador da thread
  int delay; // atraso bobo
  LE monitor;// objeto monitor para coordenar a lógica de execução das threads

  // Construtor
  Leitor(int id, int delayTime, LE m) {
    this.id = id;
    this.delay = delayTime;
    this.monitor = m;
  }

  private static boolean ehPrimo(int numero) {
    for (int j = 2; j < numero; j++) {
      if (numero % j == 0)
        return false;
    }
    return true;
  }

  // Método executado pela thread
  public void run() {
    try {
      for (;;) {
        this.monitor.EntraLeitor(this.id);

        // le a variavel e a imprime na tela indicando se e um numero primo ou nao.
        if (ehPrimo(monitor.val))
          System.out.println("#Leu " + monitor.val + " ---> é primo");
        else
          System.out.println("#Leu " + monitor.val + " ---> não é primo");

        this.monitor.SaiLeitor(this.id);
        sleep(this.delay);
      }
    } catch (InterruptedException e) {
      return;
    }
  }
}

// --------------------------------------------------------
// Escritor
class Escritor extends Thread {
  int id; // identificador da thread
  int delay; // atraso bobo...
  LE monitor; // objeto monitor para coordenar a lógica de execução das threads

  // Construtor
  Escritor(int id, int delayTime, LE m) {
    this.id = id;
    this.delay = delayTime;
    this.monitor = m;
  }

  // Método executado pela thread
  public void run() {
    try {
      for (;;) {
        this.monitor.EntraEscritor(this.id);
        monitor.val = this.id; // modifica a variavel escrevendo o valor do seu identificador de thread.
        this.monitor.SaiEscritor(this.id);
        sleep(this.delay); // atraso bobo...
      }
    } catch (InterruptedException e) {
      return;
    }
  }
}

// --------------------------------------------------------
// Leitor e Escritor
class LeitorEscritor extends Thread {
  int id; // identificador da thread
  int delay; // atraso bobo...
  LE monitor; // objeto monitor para coordenar a lógica de execução das threads

  // Construtor
  LeitorEscritor(int id, int delayTime, LE m) {
    this.id = id;
    this.delay = delayTime;
    this.monitor = m;
  }

  private static boolean ehPar(int numero) {
    if (numero % 2 == 0)
      return true;
    return false;
  }

  // Método executado pela thread
  public void run() {
    try {
      for (;;) {
        this.monitor.EntraLeitorEscritor(this.id);
        // verifica se o numero é par ou impar
        if (ehPar(monitor.val))
          System.out.println("#Leu: " + monitor.val + " ---> é par");
        else
          System.out.println("#Leu: " + monitor.val + " ---> é impar");
        monitor.val *= 2;// dobra o valor atual da variavel
        System.out.println("#Dobrou o valor para: " + monitor.val);

        this.monitor.SaiLeitorEscritor(this.id);
        sleep(this.delay); // atraso bobo...
      }
    } catch (InterruptedException e) {
      return;
    }
  }
}

// --------------------------------------------------------
// Classe principal
class lab7 {
  static final int L = 4;
  static final int E = 5;
  static final int LE = 3;

  public static void main(String[] args) {
    int i;
    LE monitor = new LE(); // Monitor (objeto compartilhado entre leitores e escritores)
    Leitor[] l = new Leitor[L]; // Threads leitores
    Escritor[] e = new Escritor[E]; // Threads escritores
    LeitorEscritor[] le = new LeitorEscritor[LE]; // Threads leitoras e escritores

    // inicia o log de saida
    System.out.println("import verificaLE");
    System.out.println("le = verificaLE.LE()");

    for (i = 0; i < L; i++) {
      l[i] = new Leitor(i + 1, (i + 1) * 500, monitor);
      l[i].start();
    }
    for (i = 0; i < E; i++) {
      e[i] = new Escritor(i + 1, (i + 1) * 500, monitor);
      e[i].start();
    }
    for (i = 0; i < LE; i++) {
      le[i] = new LeitorEscritor(i + 1, (i + 1) * 500, monitor);
      le[i].start();
    }
  }
}
