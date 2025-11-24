# Documentação do Projeto - Analisador Léxico e Sintático

Este projeto implementa um analisador léxico (Lexer) e analisador sintático (Parser) visto em aula. Ele processa o código-fonte, realiza a análise léxica, a análise sintática e gerar uma Árvore de Sintaxe Abstrata (AST) representando a estrutura do programa.

## Analisador Léxico (Lexer)

O analisador léxico é implementado usando uma máquina de estados finita (autômato) com 11 estados (q0 a q10).

- q0: Estado inicial - identifica o tipo de caractere e direciona para o estado apropriado
- q1: Reconhece números inteiros e reais
- q2: Reconhece operadores aritméticos (`+`, `-`, `*`, `/`)
- q3: Reconhece identificadores e palavras reservadas
- q4: Reconhece operadores relacionais (`>`, `<`, `=`, `>=`, `<=`, `==`, `!=`)
- q5: Reconhece pontuação (`;`, `(`, `)`, `{`, `}`, `[`, `]`, `,`, `.`, `:`, `?`)
- q6: Reconhece strings (delimitadas por `"`)
- q7: Reconhece operador de incremento (`++`)
- q8: Reconhece operador de decremento (`--`)
- q9: Reconhece operadores lógicos (`&&`, `||`, `!`)
- q10: Reconhece a parte decimal de números reais

O lexer reconhece os seguintes tipos de tokens:

- Números: `NUMERO_INTEIRO`, `NUMERO_REAL`
- Identificadores: `IDENTIFICADOR`
- Palavras Reservadas: `int`, `double`, `string`, `main`, `if`, `else`, `while`, `for`, `do`, `return`
- Operadores Aritméticos: `+`, `-`, `*`, `/`
- Operadores Relacionais: `>`, `<`, `>=`, `<=`, `==`, `!=`
- Operadores Lógicos: `&&`, `||`, `!`
- Operadores de Atribuição: `=`
- Operadores de Incremento/Decremento: `++`, `--`
- Pontuação: `;`, `(`, `)`, `{`, `}`, `[`, `]`, `,`, `.`, `:`, `?`
- Strings: Delimitadas por aspas duplas

## Analisador Sintático (Parser)

O analisador sintático implementa um parser descendente recursivo que constrói a AST seguindo a gramática da linguagem.

A gramática segue uma estrutura similar a C:

```
Program -> Function*

Function -> Tipo IDENTIFICADOR "(" ParamList? ")" Block

ParamList -> Tipo IDENTIFICADOR ("," Tipo IDENTIFICADOR)*

Block -> "{" Statement* "}"

Statement -> VariableDeclaration
          | Assignment
          | IfStatement
          | WhileStatement
          | ForStatement
          | ReturnStatement
          | ExpressionStatement

VariableDeclaration -> Tipo IDENTIFICADOR ("=" Expression)? ("," IDENTIFICADOR ("=" Expression)?)* ";"

Assignment -> IDENTIFICADOR ("[" Expression "]")? "=" Expression ";"

IfStatement -> "if" "(" Expression ")" (Block | Statement) ("else" (Block | Statement))?

WhileStatement -> "while" "(" Expression ")" (Block | Statement)

ForStatement -> "for" "(" (VariableDeclaration | Assignment | Expression)? ";" Expression? ";" Expression? ")" Block

ReturnStatement -> "return" Expression? ";"

Expression -> E (OPERADOR_RELACIONAL E)* (OPERADOR_LOGICO E)*

E -> T E'
E' -> ("+" | "-") T E' | ε

T -> F T'
T' -> ("*" | "/") F T' | ε

F -> IDENTIFICADOR
   | NUMERO_INTEIRO
   | NUMERO_REAL
   | STRING
   | "(" Expression ")"
   | IDENTIFICADOR "(" Expression* ")"
   | IDENTIFICADOR "[" Expression "]"
   | "!" F
```

#### Recursos do Parser

- Suporte a precedência de operadores: A gramática garante a precedência correta (multiplicação/divisão antes de adição/subtração)
- Suporte a associatividade: Operadores são associativos à esquerda
- Parsing de expressões complexas: Suporta expressões aninhadas com parênteses
- Parsing de funções: Reconhece definições de funções com parâmetros
- Parsing de blocos: Suporta blocos de código com múltiplos statements
- Parsing de estruturas de controle: Suporta `if`, `while`, `for` com ou sem chaves
- Parsing de arrays: Suporta acesso a arrays com `[índice]`
- Parsing de operadores unários: Suporta negação lógica (`!`)

### 3. Árvore de Sintaxe Abstrata (AST)

A AST é uma representação hierárquica da estrutura do programa. Cada nó da árvore representa uma construção da linguagem.

#### Nós da AST Implementados

##### Nós de Expressão (ExpressionNode)

- LiteralNode: Representa literais (números inteiros, reais, strings)
- IdentifierNode: Representa identificadores (variáveis)
- BinaryOpNode: Representa operações binárias (aritméticas, relacionais, lógicas)
- UnaryOpNode: Representa operações unárias (negação lógica)
- FunctionCallNode: Representa chamadas de função
- ArrayAccessNode: Representa acesso a arrays (`array[índice]`)

##### Nós de Statement (StatementNode)

- VariableDeclarationNode: Representa declarações de variáveis
- AssignmentNode: Representa atribuições
- IfStatementNode: Representa estruturas condicionais `if-else`
- WhileStatementNode: Representa loops `while`
- ForStatementNode: Representa loops `for`
- ReturnStatementNode: Representa statements `return`
- ExpressionStatementNode: Representa expressões usadas como statements

##### Nós de Estrutura

- BlockNode: Representa blocos de código
- FunctionNode: Representa definições de funções
- ProgramNode: Nó raiz que contém todas as funções do programa
- ParameterNode: Representa parâmetros de função

## Funcionalidades Implementadas

### Tipos de Dados Suportados

- int: Números inteiros
- double: Números reais (ponto flutuante)
- string: Strings de caracteres

### Operadores Suportados

#### Operadores Aritméticos

- Adição: `+`
- Subtração: `-`
- Multiplicação: `*`
- Divisão: `/`

#### Operadores Relacionais

- Maior que: `>`
- Menor que: `<`
- Maior ou igual: `>=`
- Menor ou igual: `<=`
- Igual: `==`
- Diferente: `!=`

#### Operadores Lógicos

- E lógico: `&&`
- OU lógico: `||`
- Negação: `!`

#### Operadores de Atribuição

- Atribuição: `=`
- Incremento: `++`
- Decremento: `--`

### Estruturas de Controle

#### Condicionais

- if: Estrutura condicional simples
- if-else: Estrutura condicional com alternativa
- Suporta blocos com chaves `{}` ou statements únicos sem chaves

#### Loops

- while: Loop com condição no início
- for: Loop com inicialização, condição e atualização
  - Suporta declaração de variável na inicialização
  - Suporta atribuições na inicialização e atualização
  - Suporta expressões vazias em qualquer parte

### Outras Funcionalidades

- Funções: Definição de funções com tipo de retorno, nome, parâmetros e corpo
- Arrays: Acesso a arrays usando `[índice]`
- Strings: Literais de string delimitados por aspas duplas
- Números Reais: Números com parte decimal (ex: `9.75`, `123.456`)
- Múltiplas Declarações: Declaração de múltiplas variáveis na mesma linha separadas por vírgula
- Expressões Aninhadas: Suporte completo a expressões complexas com parênteses
- Chamadas de Função: Suporte a chamadas de função com argumentos

## Testes Implementados

O projeto inclui 7 testes que verificam diferentes aspectos do analisador:

1. Teste: Expressão Aritmética

   - Testa operações aritméticas básicas
   - Código: `10 + 20 * 30`

2. Teste: Atribuição

   - Testa declaração e atribuição de variável
   - Código: `int x = 10;`

3. Teste: Atribuição 2

   - Testa atribuição com expressão
   - Código: `int menino = neymar + 23;`

4. Teste: Programa Completo

   - Testa um programa completo com função main, variáveis e return
   - Código: `int main() { int x = 10; int y = 20; return x + y; }`

5. Teste: Programa Completo 2

   - Testa recursos avançados: tipos double/string, arrays, operadores relacionais/lógicos, estruturas de controle aninhadas
   - Código: `int main() { double _neymar_10 = 9.75, messi = 0; string texto = "santos"; if (xxx >= r9) while (teste == 10 || teste2 != 20 && !sair) a[75] = (123.456 - 5) / 2; i++; return 0; }`

6. Teste: Estruturas de Controle

   - Testa estruturas if e while
   - Código: `int main() { int x = 10; if (x > 0) { x = x + 1; } while (x < 20) { x = x + 1; } return x; }`

7. Teste: Loop For
   - Testa estrutura for com inicialização, condição e atualização
   - Código: `int main() { int i; for (i = 0; i < 10; i = i + 1) { int x = i; } return 0; }`

## Como executar?

```bash
make
```

Isso irá compilar todos os arquivos fonte e gerar o executável `lexer_program`.

### Execução

```bash
./lexer_program
```

O programa executará todos os testes e exibirá:

1. Os tokens gerados pelo lexer para cada teste
2. A AST construída pelo parser em formato textual

### Limpeza

Para remover os arquivos compilados:

```bash
make clean
```
