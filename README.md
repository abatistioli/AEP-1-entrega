# PSE em Ação — Saúde Mental e Inclusão (Maringá-PR)

Atividade Estruturada de Prática (AEP) — Análise e Desenvolvimento de Sistemas — Unicesumar — 2º semestre / 2026.

Sistema em linguagem C para planejamento e acompanhamento de **ações
coletivas** do Programa Saúde na Escola (PSE), com recorte temático em
**saúde mental e inclusão de estudantes neurodivergentes** no Ensino Médio.

> ⚠️ **Importante:** o sistema não cadastra nome, diagnóstico, laudo,
> prontuário ou qualquer dado clínico individual de estudantes. Todas as
> informações são coletivas e fictícias, conforme os limites éticos
> definidos no documento da AEP (ver `docs/`).

## Status do projeto

Esta é a **1ª entrega** da AEP (documento escrito + planejamento), mas o código
em `src/` já foi iniciado como preparação para a 2ª entrega.

## Integrantes do grupo

| Nome | RA | Série |
| Wagner Felipe Lima da Silva | 26012632-2 | ADSIS2B |
| Alessandra Alves Batistioli | 26010319-2| ADSIS2B |
| Adriel Nícolas Souza Santos | 26012871-2 | ADSIS2B |


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

## Funcionalidades do código

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

## Relação entre os códigos da 1ª entrega

O código em `src/pse_saude_mental.c` segue a mesma lógica descrita no
fluxograma geral, no fluxograma detalhado da operação de cadastro e no
pseudocódigo em Portugol apresentados no documento
`docs/1_Entrega_AEP_PSE_SaudeMental.docx`.

## Próximos passos (2ª etapa da AEP)

- [ ] Diagrama de casos de uso e descrição dos casos principais.
- [ ] Manual do usuário com capturas de tela.
- [ ] Testes manuais documentados.
- [ ] Vídeo de apresentação (até 7 minutos).

## Licença

Projeto acadêmico desenvolvido para fins educacionais no curso de ADS — Unicesumar. Uso livre para fins de estudo.
