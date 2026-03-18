#### *Este projeto foi criado como parte do currículo da 42 por jnovais, rida-cos*.

# Description

Shells existem desde o início da TI.

Naquela época, todos os desenvolvedores concordavam que se comunicar com um computador via interruptores de entrada/saída alinhados era extremamente frustrante.

Era natural que eles tivessem a ideia de criar um software para se comunicar com um computador usando linhas de comando interativas em uma linguagem um tanto próxima da linguagem humana.

O **Minishell** é uma versão simplificado do bash original que nos permite viajar no tempo e experimentar os desafios que os desenvolvedores enfrentaram antes da existência do Windows. Um projeto no qual fomos desafiados a reproduzir comportamentos básicos do shell, ler comandos da entrada padrão, parseá-los e executá-los corretamente, lidando com processos filhos, redirecionamentos e pipes.

### Overview

Dividimos o projeto em duas grandes partes: o ***Parsing*** e o ***Executor***. O primeiro fica encarregado de tratar a entrada do dado desde o input do usuário até entregar no formato adequado para ser consumido pelo Executor. O segundo é responsável por executar os comandos já estruturados pelo parser, aplicando redirecionamentos e pipes, criando processos filhos quando necessário, executando builtins, e por atualizar o `g_exit_status` seguindo o comportamento do bash.

### Parsing
O Parsing, por sua vez, foi dividido em 5 diferentes partes: ***Lexer***, ***Expander***, ***Splitter***, ***Quote Removal***, e o prórpio ***Parser***.

* ***Lexer***: transforma a linha de comando em uma lista de "Tokens" (palavras, pipes, redirecionamentos).
* ***Expander***: substitui variáveis de ambiente (como `$USER` ou `$?`) pelos seus valores reais, respeitando as aspas.
* ***Splitter***: divide os tokens em unidades lógicas de execução.
* ***Quote Removal***: remove as aspas desnecessárias (' ou ") após a interpretação do conteúdo.
* ***Parser***: organiza tudo em uma estrutura de comandos pronta para ser executada.


### Executor
O Executor recebe uma lista encadeada de comandos (`t_cmd`) já pronta (com `args`, `fd_in`, `fd_out`, e flags como `invalid`) e decide como executar cada comando.

De forma geral, ele cobre:

1. **Comando único** (`execute_cmd`)
   - Valida o nó (`invalid`, `args`, etc.).
   - Resolve o path do executável via `find_cmd_path`.
   - Faz `fork()` e no filho executa:
     - `apply_redirections` para conectar `fd_in`/`fd_out` em `STDIN_FILENO`/`STDOUT_FILENO` via `dup2`.
     - `execve`.
   - No pai faz `waitpid` e atualiza `g_exit_status`.
   - Tratamento de erros segue o padrão:
     - **127**: comando inexistente (`command not found` / `ENOENT`).
     - **126**: falha ao executar (ex: permissão / `execve` falhou por outro motivo).

2. **Pipeline** (`execute_pipeline`)
   - Percorre a lista `t_cmd` em loop (usando um contexto `t_exec_ctx` para manter estado: comando atual, `last_pid`, flags, `envp`).
   - Para cada comando:
     - Se o nó estiver inválido, fecha e reseta FDs (`close_and_reset_fds`) e continua.
     - Se for um builtin e **não houver pipeline** (apenas 1 comando), executa no processo pai (`execute_builtin`) para permitir efeitos colaterais (ex: `cd`, `export`, `unset`) e depois fecha FDs.
     - Caso contrário, cria um processo filho (`create_child_process`). No filho:
       - Fecha FDs que não pertencem ao comando (`close_other_fds`).
       - Aplica redirecionamentos (`apply_redirections`).
       - Executa builtin (se for o caso) ou chama `execve`.
   - Após iniciar os processos, o pai:
     - Aguarda o último comando do pipeline (`waitpid(last_pid, ...)`) e usa `handle_pipeline_status` para definir o `g_exit_status` final.
     - Aguarda os demais filhos (`restore_signals_and_wait`).

3. **Sinais (Ctrl+C / Ctrl+\\)**
   - Durante a execução de pipelines, o processo pai ignora `SIGINT` e `SIGQUIT` para não “matar” o prompt.
   - Os filhos usam comportamento padrão para sinais (ex.: `SIGQUIT` causa status **131**).
   - O status final é convertido para o padrão do shell: `128 + sinal`.
     - Exemplo: `SIGINT` => 130, `SIGQUIT` => 131.
   - Para manter o comportamento visível do bash, quando o último processo termina por `SIGQUIT`, imprimimos `Quit (core dumped)`.

4. **Arquitetura de arquivos (referência rápida)**
   - `src/executor.c`: entrada principal (`execute_cmd`, `execute_pipeline`) e helpers locais de wait/status para comando único.
   - `src/executor_helpers.c`: loop de execução do pipeline e utilitários de controle do fluxo.
   - `src/executor_utils.c`: utilitários de execução no filho (redirecionamentos, `execve`, tratamento de erro).
   - `src/executor_pipeline.c`: helpers de pipeline (fechamento de FDs de outros comandos, reset de FDs, status final do pipeline).

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
Neste projeto, a Inteligência Artificial (Gemini 3 Flash e a Claude Code) foi integrada como uma ferramenta central de aceleração de aprendizado e suporte à decisão técnica. O uso da IA focou em quatro pilares fundamentais:

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


## Status Quote - Tratamento de Aspas

Tratar as aspas foi um verdadeiro desafio. A lógica adotada foi criar uma espécie de status que ajudava a nos guiar e saber se o parser estava dentro ou fora de uma aspa e se ela era simples ou dupla. Algo que facilitou muito foi criar uma estrutura enum com os tipos `OUT_QUOTE`, `IN_SQUOTE` e `IN_DQUOTE`.

### 1. Aspas Simples (`'`) - "O Literal Absoluto"

As aspas simples são as mais rudes. Elas ignoram **tudo** o que está dentro delas. Nada é expandido, nada é interpretado.
- **O que acontece:** `'$VAR'` será sempre a string literal `$`, `V`, `A`, `R`.

### 2. Aspas Duplas (`"`) - "O Filtro Seletivo"

As aspas duplas protegem a string de ser dividida em múltiplos tokens (mantendo espaços), mas permitem que o cifrão (`$`) e a interrogação (`$?`) façam seu trabalho.
- **O que acontece:** `"$VAR"` vira o valor da variável, mas se o valor for `ls -l`, ele continua sendo **um único token**.

Em outras palavras..

As aspas simples (') preservam o valor literal de todos os caracteres, enquanto as aspas duplas (") preservam o valor literal de quase todos, exceto pelo cifrão ($), que ainda permite a expansão de variáveis, e pela manutenção da string como um único argumento.

