#### *Este projeto foi criado como parte do currículo da 42 por jnovais, rida-cos*.

# Description

Shells existem desde o início da TI.

Naquela época, todos os desenvolvedores concordavam que se comunicar com um computador via interruptores de entrada/saída alinhados era extremamente frustrante.

Era natural que eles tivessem a ideia de criar um software para se comunicar com um computador usando linhas de comando interativas em uma linguagem um tanto próxima da linguagem humana.

O **Minishell** é uma versão simplificado do bash original que nos permite viajar no tempo e experimentar os desafios que os desenvolvedores enfrentaram antes da existência do Windows. Um projeto no qual fomos desafiados a reproduzir comportamentos básicos do shell, ler comandos da entrada padrão, parseá-los e executá-los corretamente, lidando com processos filhos, redirecionamentos e pipes.

### Overview

Dividimos o projeto em duas grandes partes: o ***Parsing*** e o ***Executor***. O primeiro fica encarregado de tratar a entrada do dado desde o input do usuário até entregar no formato adequado para ser consumido pelo Executor. O segundo é responsável por...[jnovais].

### Parsing
O Parsing, por sua vez, foi dividido em 5 diferentes partes: ***Lexer***, ***Expander***, ***Splitter***, ***Quote Removal***, e o prórpio ***Parser***.

* ***Lexer***: transforma a linha de comando em uma lista de "Tokens" (palavras, pipes, redirecionamentos).
* ***Expander***: substitui variáveis de ambiente (como `$USER` ou `$?`) pelos seus valores reais, respeitando as aspas.
* ***Splitter***: divide os tokens em unidades lógicas de execução.
* ***Quote Removal***: remove as aspas desnecessárias (' ou ") após a interpretação do conteúdo.
* ***Parser***: organiza tudo em uma estrutura de comandos pronta para ser executada.


### Executor
[jnovais]

# Instructions

Esta seção descreve como preparar o ambiente, compilar e executar o Minishell.

### Pre-requisites
O projeto depende da biblioteca GNU Readline. Caso não a tenha instalada (comum em ambientes Linux baseados em Debian/Ubuntu), execute:
```bash
sudo apt-get install libreadline-dev
```

### Compilação

Utilize o Makefile incluído na raiz do repositório para compilar o executável:
```bash
make
```
* `make clean`: Remove os arquivos objetos (`*.o`).
* `make fclean`: Remove os objetos e o executável final.
* `make re`: Limpa e recompila todo o projeto.

### Execução
Após a compilação bem-sucedida, inicie o shell interativo:
```bash
./minishell
```
### Restrições de Uso

Conforme os requisitos do projeto, o Minishell não aceita argumentos na sua inicialização.
* Correto: `./minishell`
* Incorreto: `./minishell script.sh` (O programa exibirá uma mensagem de erro e encerrará).

# Resourses

1. **[The Open Gourp Base Specifications Issue](https://pubs.opengroup.org/onlinepubs/9699919799/functions/contents.html)**: Aqui é possível encontrar informações sobre diversos comandos basicos do C.
2. **[CodeVault](https://www.youtube.com/@CodeVault)**: Um canal no YouTube que explica muito bem funções básicas do C, como aplicar e cuidados a tomar.
3. **Man pages**: Consulta constante às definições de `execve(2)`, `sigaction(2)` e `dup2(2)`.
4. **Bash**: Utilizado como a principal referência de comportamento esperado para expansões e redirecionamentos.

### Como IA foi usada
Neste projeto, a Inteligência Artificial (Gemini 3 Flash e a [jnovais]) foi integrada como uma ferramenta central de aceleração de aprendizado e suporte à decisão técnica. O uso da IA focou em quatro pilares fundamentais:

* ***Agilidade no Aprendizado e Depuração***: utilizei a IA para antecipar e diagnosticar erros complexos de lógica (como falhas de expansão de variáveis, falhas na interpreteção de sinais) que, em um cenário de desenvolvimento isolado, exigiriam um tempo maior de depuração, permitindo um foco maior na compreensão da solução.
* ***Análise de Trade-offs e Pontos de Vista***: a IA foi consultada para oferecer múltiplas perspectivas sobre a mesma funcionalidade, permitindo comparar diferentes abordagens arquiteturais (como o uso de listas encadeadas vs. matrizes para o ambiente) sem a necessidade de implementação prévia de cada uma.
* ***Otimização do Desenvolvimento***: ao simular o comportamento de diferentes abordagens, foi possível economizar tempo de desenvolvimento e focar na implementação da solução que melhor se adequava aos requisitos do projeto e às normas da 42.
* ***Validação de Casos de Borda***: em tarefas críticas como o Parsing e a Gestão de Sinais, a IA serviu como um "peer reviewer" para validar se a lógica implementada (ex: precedência de dígitos no $1USER e status de saída 131) estava em conformidade com o padrão POSIX.
* ***Geração de Casos de Teste (QA)***: a IA foi utilizada para gerar combinações complexas de comandos e casos de borda (edge cases) para testes comparativos com o Bash original. Isso permitiu estressar o parser e o executor com sequências de redirecionamentos, aspas aninhadas e expansões de variáveis que garantiram a robustez do programa.

# Challenges

## Lista Encadeada VS Árvore Binária

### 1. Lista Encadeada (Linked List)

Como o Minishell básico só exige pipes lineares (cmd1 | cmd2 | cmd3), uma lista é perfeitamente capaz de representar essa sequência.

* **Vantagens**:
    * **Simplicidade**: muito mais fácil de iterar e dar free.
    * **Linearidade**: casa perfeitamente com o loop de execução onde você faz um fork e passa o pipe para o próximo comando.
    * **Menos Leaks**: o gerenciamento de memória em listas encadeadas é menos propenso a erros que em estruturas recursivas.

* **Desvantagens**:
    * **Limitada**: se você decidir fazer o bônus de **Operadores Lógicos** (`&&` e `||`) ou **Parênteses**, a lista se torna um pesadelo de lógica, pois ela não representa bem a precedência de execução.

### 2. Árvore Binária (Abstract Syntax Tree - AST)

Geralmente usada por quem quer implementar o bônus completo com parênteses e operadores lógicos.

* **Vantagens**:

    * **Precedência**: a árvore resolve naturalmente a ordem de execução. O que está mais "fundo" na árvore (folhas) é executado primeiro.

    * **Escalabilidade**: é como o Bash real funciona. Se você quiser adicionar comandos complexos no futuro, a estrutura já está pronta.

* **Desvantagens**:

    * **Complexidade de Parsing**: criar a árvore exige um parser muito mais sofisticado (recursivo).

    * **Execução Complexa**: precisa de funções recursivas para percorrer a árvore e gerenciar os pipes entre os nós.


* Crtl+C
* HERE_DOC
* Status Quote - Falar sobre o comportamento das aspas
* Redirections
