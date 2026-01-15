# <img src="https://github.com/jeduardo-00/Rat_Advencture/blob/main/assets/imagens/icon.png" alt="alt text" style="max-width: 100%;" width="40"> Rat_Adventure 

O jogo Rat_Adventure é um jogo de plataforma que tem como objetivo somar maior quantidade de pontos e destacar o jogador no top 3 melhores. 

## 🚀 Começando

Antes de adquirir o projeto, siga o tutorial para instalar os softwares necessários para rodar o jogo no VSCODE

Consulte **[Parte 1: Configuração Única do Ambiente](https://github.com/DanielNeres/allegro?tab=readme-ov-file#parte-1-configura%C3%A7%C3%A3o-%C3%BAnica-do-ambiente)** para saber como implantar o projeto.

### 📋 Softwares instalados

Com os devidos programas baixados e o projeto. Você vai adquirir o repositorio e abrir o MSYS2 MINGW64, colocando os seguintes comandos:

#### Caminho do projeto
```
cd C:/caminho/do/projeto
```

#### Compilar
```
gcc main.c paginas/paginas.c paginas/gamer.c funcoes_gerais/lista.c funcoes_gerais/gerais.c \-I paginas -I funcoes_gerais -o main $(pkg-config --libs --cflags allegro-5 allegro_font-5 allegro_ttf-5 allegro_primitives-5 allegro_image-5 allegro_audio-5 allegro_acodec-5)
```

#### Executar
```
cd ./main
```
