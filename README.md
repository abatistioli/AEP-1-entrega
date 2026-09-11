# PSE em Ação — Saúde Mental e Inclusão (Maringá-PR)

Atividade Estruturada de Prática (AEP) — Engenharia de Software / Análise e
Desenvolvimento de Sistemas — Unicesumar — 2º semestre / 2026.

Sistema em linguagem C para planejamento e acompanhamento de **ações
coletivas** do Programa Saúde na Escola (PSE), com recorte temático em
**saúde mental e inclusão de estudantes neurodivergentes** no Ensino Médio.

> ⚠️ **Importante:** o sistema não cadastra nome, diagnóstico, laudo,
> prontuário ou qualquer dado clínico individual de estudantes. Todas as
> informações são coletivas e fictícias, conforme os limites éticos
> definidos no documento da AEP (ver `docs/`).

## Status do projeto

Esta é a **1ª entrega** da AEP (documento escrito + planejamento). A
entrega do sistema completo não é obrigatória nesta etapa, mas o código
em `src/` já foi iniciado como preparação para a 2ª entrega.

## Integrantes do grupo

| Nome | RA | Série |
|---|---|---|
| Ana Beatriz Ferreira Lima | 26123456 | ADSIS2S |
| Carlos Eduardo Martins Souza | 26123457 | ESOFT2S |
| Juliana Aparecida Rocha Silva | 26123458 | ADSIS2S |

*(dados fictícios — substituir pelos integrantes reais do grupo)*

## Estrutura do repositório

```
pse-saude-mental-maringa/
├── README.md
├── src/
│   └── pse_saude_mental.c      # código-fonte do sistema
└── docs/
    ├── 1_Entrega_AEP_PSE_SaudeMental.docx   # documento da 1ª entrega
    └── diagramas/
        ├── fluxograma_geral.png             # Figura 1 — menu principal
        └── fluxograma_cadastro.png          # Figura 2 — cadastro de ação
```

## Sobre o estilo do código

O código usa apenas os comandos e tipos já vistos em aula até o momento:
`int`, `char`, `float`, `printf`, `scanf`, `if/else`, `switch`, `for` e
`do...while`. **Nenhuma função usa `void`** — todas retornam `int`
(retornando `0` ao final, do mesmo jeito que `main()` sempre devolve um
inteiro), já que `void` ainda não foi estudado pela turma.

## Como compilar e executar

Pré-requisito: `gcc` (ou outro compilador C compatível com o padrão C99).

```bash
# Clonar o repositório
git clone https://github.com/<usuario-ou-organizacao>/pse-saude-mental-maringa.git
cd pse-saude-mental-maringa/src

# Compilar
gcc -Wall -Wextra -std=c99 -o pse_saude_mental pse_saude_mental.c

# Executar
./pse_saude_mental
```

No Windows (usando MinGW/gcc):

```bash
gcc -Wall -Wextra -std=c99 -o pse_saude_mental.exe pse_saude_mental.c
pse_saude_mental.exe
```

## Funcionalidades (escopo mínimo da AEP)

1. Cadastrar ação (código, escola, tema, data prevista, público-alvo,
   responsável, quantidade prevista, situação inicial).
2. Listar todas as ações cadastradas.
3. Pesquisar ações por código, escola ou tema.
4. Atualizar situação da ação (planejada / realizada / cancelada),
   registrando a quantidade efetiva de participantes quando realizada.
5. Gerar resumo geral (quantidade por situação, total de participantes
   e percentual de participação).
6. Validação de entradas: código duplicado, quantidade negativa, campo
   obrigatório vazio e opção de menu inexistente.

## Relação com os artefatos da 1ª entrega

O código em `src/pse_saude_mental.c` segue a mesma lógica descrita no
fluxograma geral, no fluxograma detalhado da operação de cadastro e no
pseudocódigo em Portugol apresentados no documento
`docs/1_Entrega_AEP_PSE_SaudeMental.docx`. Os nomes das funções
(`cadastrarAcao`, `listarAcoes`, `pesquisarAcoes`, `atualizarSituacao`,
`gerarResumo`, `existeCodigo`) e as mensagens de validação foram
mantidos idênticos entre os artefatos, para facilitar a rastreabilidade
entre planejamento e implementação exigida pelo critério avaliativo da
disciplina.

## Próximos passos (2ª etapa da AEP)

- [ ] Diagrama de casos de uso e descrição dos casos principais.
- [ ] Manual do usuário com capturas de tela.
- [ ] Testes manuais documentados.
- [ ] Vídeo de apresentação (até 7 minutos).

## Licença

Projeto acadêmico desenvolvido para fins educacionais na disciplina de
AEP — Unicesumar. Uso livre para fins de estudo.
