import random
import os
import time

def inicializar():
    for i in range(linhas):
        for j in range(colunas):
            tabuleiro[i][j] = 0
            visualizacao[i][j] = 0

    minasColocadas = 0
    while minasColocadas < minas:
        linha = random.randint(0, linhas-1)
        coluna = random.randint(0, colunas-1)

        if tabuleiro[linha][coluna] != -1:
            tabuleiro[linha][coluna] = -1
            minasColocadas += 1

            for i in range(linha - 1, linha + 2):
                for j in range(coluna - 1, coluna + 2):
                    if 0<=i<linhas and 0<=j< colunas and tabuleiro[i][j]!=-1:
                        tabuleiro[i][j] += 1

def imprimir(revelarTudo):
    print("\n      ", end="")
    for j in range(colunas):
        print(f"{chr(65 + j):2}  ", end="")
    print()

    print("   ", end="")
    for j in range(colunas):
        print("\033[1;30m----\033[0m", end="")
    print("\033[1;30m---\033[0m")

    for i in range(linhas):
        print(f"{i + 1:2} \033[1;30m|\033[0m", end="")
        for j in range(colunas):
            if revelarTudo:
                if tabuleiro[i][j] == -1 and visualizacao[i][j] == -3:
                    print("  \033[1;31mX\033[0m ", end="")
                elif visualizacao[i][j] == -3 and tabuleiro[i][j] != -1:
                    print("  \033[1;34m!\033[0m ", end="")
                elif tabuleiro[i][j] == -1:
                    print("  \033[1;31mX\033[0m ", end="")
                else:
                    print(f" {tabuleiro[i][j]:2} ", end="")
            else:
                if visualizacao[i][j] == -3:
                    print("  ⚑ ", end="")
                elif visualizacao[i][j] == -2:
                    if tabuleiro[i][j] == -1:
                        print("  \033[1;31mX\033[0m ", end="")
                    else:
                        print(f" {tabuleiro[i][j]:2} ", end="")
                else:
                    print("  \033[1;30m?\033[0m ", end="")
        print("\033[1;30m |\033[0m")
    
    print("   ", end="")
    for j in range(colunas):
        print("\033[1;30m----\033[0m", end="")
    print("\033[1;30m---\033[0m")

def revelar(linha, coluna):
    if linha<0 or linha>=linhas or coluna<0 or coluna>=colunas:
        print("Coordenadas inválidas!")
        return 0
    if visualizacao[linha][coluna] == -2:
        print("Essa posição já foi revelada!")
        return 0
    visualizacao[linha][coluna] = -2

    if tabuleiro[linha][coluna] == -1:
        return -1
    if tabuleiro[linha][coluna] == 0:
        for i in range(linha - 1, linha + 2):
            for j in range(coluna - 1, coluna + 2):
                if 0 <= i < linhas and 0 <= j < colunas and visualizacao[i][j] != -2:
                    revelar(i, j)
    return 1

def checar():
    for i in range(linhas):
        for j in range(colunas):
            if tabuleiro[i][j] != -1 and visualizacao[i][j] != -2:
                return 0
    return 1

################################################################################

print("\033[1;34m!!! BEM-VINDO(A) AO CAMPO MINADO !!!\033[0m")
input("Pressione Enter para continuar...")
os.system("clear")

linhas = int(input(f"Digite quantas \033[1;34mlinhas\033[0m deseja no tabuleiro (máximo 20): "))
colunas = int(input(f"Digite quantas \033[1;35mcolunas\033[0m deseja no tabuleiro (máximo 20): "))
minas = int(input("Digite quantas \033[1;31mminas\033[0m deseja esconder: "))

tabuleiro = [[0]*20 for _ in range(20)]
visualizacao = [[0]*20 for _ in range(20)]
modo = 0

inicializar()
os.system("clear")

while True:
    print("\033[1;34m!!! CAMPO MINADO !!!\033[0m\n")
    print("Modo: {}".format("\033[1;34mMarcar\033[0m" if modo else "\033[1;35mRevelar\033[0m"))
    imprimir(False)
    print("\nPara marcar uma possível mina, digite 'B'.")
    comando = input("Digite as coordenadas (ex: A1, B3, C5): ").strip()

    os.system("clear")
    
    if comando.lower() == 'b':
        modo = 1
        os.system("clear")
        print("Modo de marcação ativado. Digite as coordenadas para marcar uma possível mina com uma bandeira.")
    else:
        try:
            coluna = ord(comando[0].upper()) - 65
            linha = int(comando[1:]) - 1
            if coluna < 0 or coluna >= colunas or linha < 0 or linha >= linhas:
                raise ValueError("Coordenadas fora do tabuleiro!")
        except (ValueError, IndexError):
            os.system("clear")
            print("Coordenadas inválidas!")
            continue

        if modo:
            if visualizacao[linha][coluna] == -2:
                print("Essa posição já foi revelada, não pode ser marcada!")
            else:
                visualizacao[linha][coluna] = -3
                modo = 0
        else:
            resultado = revelar(linha, coluna)

            if resultado == -1:
                os.system("clear")
                print("\033[1;31m!!! GAME OVER !!!\033[0m")
                imprimir(True)
                print("\n\033[1;31mVocê revelou uma mina!\033[0m\n")
                break
            elif checar():
                os.system("clear")
                print("\033[1;33m!!! PARABÉNS !!!\033[0m")
                imprimir(True)
                print("\n\033[1;33mVocê revelou todas as células sem minas!\033[0m\n")
                break
