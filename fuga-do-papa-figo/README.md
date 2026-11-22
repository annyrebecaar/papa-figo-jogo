# A Fuga do Papa-Figo

Jogo de terminal em C com tema do folclore nordestino. Você está voltando para casa à noite e o Papa-Figo está na rua. Chegue em casa sem ser pego!

## Como jogar

- Ande com `w` (cima), `a` (esquerda), `s` (baixo) e `d` (direita) e aperte Enter
- Cada passo gasta 1 de coragem; se a coragem acabar, você perde
- 🏃 é você
- 🍯 é rapadura: dá +5 de coragem
- 👹 é o Papa-Figo: se ele te alcançar, você perde
- 🏠 é a sua casa: chegue nela para vencer e entrar no ranking
- ⛔️ é parede
- Digite `q` para desistir da partida

## Como rodar

É preciso ter o compilador `gcc` instalado.

```bash
gcc -o jogo jogo.c
./jogo
```

No Windows:

```bash
gcc -o jogo.exe jogo.c
jogo.exe
```

O `mapa.txt` precisa estar na mesma pasta do jogo.

## Arquivos

| Arquivo | O que é |
|---|---|
| `jogo.c` | código do jogo |
| `mapa.txt` | mapa da cidade, lido pelo jogo (`#` parede, `.` rua, `R` rapadura, `C` casa, `J` início do jogador, `P` início do Papa-Figo; o jogo troca as letras por emojis na tela) |
| `ranking.txt` | criado sozinho quando alguém vence; guarda nome e passos |

## Requisitos do projeto

- **Modularização:** o `main()` só mostra o menu; cada parte do jogo é uma função
- **Matriz:** o mapa é `char mapa[LINHAS][COLUNAS + 2]`
- **Structs:** `Coordenada`, `Status` e `Personagem`
- **Struct aninhada:** `Personagem` tem `Coordenada` e `Status` dentro (ex.: `jogador.posicao.linha`)
- **Arquivos:** lê o mapa do `mapa.txt` (`fgets`) e salva/lê o ranking no `ranking.txt` (`fprintf` e `fscanf`)
