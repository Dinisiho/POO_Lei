"""Testes do executável CMake, usando apenas a biblioteca standard de Python.

Uso: python testes/checkpoint2.py CAMINHO_DO_EXECUTAVEL [normal|sem_perdas|perdas_totais]
Os dois últimos modos exigem builds isoladas com silo_prob_perda a 0 e 100.
"""

import argparse
import re
import subprocess
from pathlib import Path
from tempfile import TemporaryDirectory


class Testes:
    def __init__(self, executavel, pasta):
        self.executavel = executavel.resolve()
        self.pasta = pasta
        self.cenarios = 0

    def ficheiro(self, nome, texto):
        (self.pasta / nome).write_bytes(texto.encode("ascii"))

    def corre(self, comandos):
        resultado = subprocess.run(
            [str(self.executavel)], cwd=self.pasta,
            input="\n".join(comandos) + "\n", text=True, encoding="utf-8",
            capture_output=True, timeout=25, check=True,
        )
        assert not resultado.stderr, resultado.stderr
        return resultado.stdout

    def verifica(self, nome, saida, presentes=(), ausentes=()):
        for esperado in presentes:
            assert esperado in saida, f"{nome}: faltou {esperado!r}\n{saida[-2800:]}"
        for inesperado in ausentes:
            assert inesperado not in saida, f"{nome}: encontrou {inesperado!r}"
        self.cenarios += 1
        print(f"OK {nome}")

    def normal(self):
        self.ficheiro("mapa.txt", "3 5\n.%#..\n~....\n..$..\n")
        self.ficheiro("mapa-crlf.txt", "1 1\r\n.\r\n")
        self.ficheiro("mapa-maximo.txt", "26 26\n" + ("." * 26 + "\n") * 26)
        self.ficheiro("interior.txt", "compra s ad d\nroda ad b\navanca 2\nestado\nfim\nignorar_interior\n")
        self.ficheiro("exterior.txt", "carrega mapa.txt\nexecuta interior.txt\nignorar_exterior\n")

        ajuda = self.corre(["ajuda", "ajuda compra", "ajuda grava", "ajuda desconhecido", "fim"])
        sintaxes = [
            "fabrica <nl> <nc>", "carrega <nome-do-ficheiro>", "avanca [n]",
            "compra <tipo> <lc> <dir>", "vende <lc>", "roda <lc> <dir>", "melhora <lc>",
            "contrata <nome>", "atribui <id> <lc>", "despede <id>", "estado",
            "lposicao <lc> [n]", "grava <nome>", "recupera <nome>",
            "executa <nome-do-ficheiro>", "ajuda [comando]", "fim",
        ]
        self.verifica("ajuda breve e detalhada", ajuda,
                      [f"  {s}\n" for s in sintaxes] + ["p perfuradora", "Substitui a copia", "nao existe o comando"])
        self.verifica("fabricas obrigatorias e fim sem fabrica",
                      self.corre(["estado", "compra s aa d", "lposicao aa", "fim", "fabrica 5 5"]),
                      ["cria primeiro", "A terminar"], ["Fabrica criada"])
        aleatoria = self.corre(["fabrica 4 6", "fabrica 26 26", "carrega mapa.txt", "lposicao aa 26", "fim"])
        self.verifica("criacao aleatoria, dimensoes maximas e segunda criacao",
                      aleatoria, ["nao tem posicoes suficientes", "ABCDEFGHIJKLMNOPQRSTUVWXYZ", "ja existe"])
        assert len(re.findall(r"^[a-z]{2}: Cratera$", aleatoria, re.M)) == 5
        for tipo, total in [("minerio de ferro", 8), ("silicio bruto", 6), ("gelo", 6)]:
            quantidades = re.findall(rf"^[a-z]{{2}}: Deposito de {tipo} \((\d+) unidades\)", aleatoria, re.M)
            assert len(quantidades) == total and all(40 <= int(q) <= 60 for q in quantidades)
        self.verifica("CRLF e carregamento apos falha de abertura",
                      self.corre(["carrega ausente.txt", "carrega mapa-crlf.txt", "lposicao aa", "fim"]),
                      ["Nao foi possivel abrir", "aa: Solo"])
        for texto, erro in [
            ("1+1\n.\n", "duas dimensoes inteiras"),
            ("1 1 lixo\n.\n", "duas dimensoes inteiras"),
            ("27 1\n", "Dimensoes da fabrica invalidas"),
            ("1 2\n.\n", "numero esperado de colunas"),
            ("1 1\n!\n", "caracter de terreno invalido"),
            ("1 1\n.\n.\n", "linhas a mais"),
            ("2 1\n.\n", "Faltam linhas"),
        ]:
            self.ficheiro("invalido.txt", texto)
            self.verifica(f"mapa invalido {texto!r}", self.corre(["carrega invalido.txt", "fim"]),
                          [erro], ["Fabrica carregada"])

        compras = self.corre([
            "carrega mapa.txt", "compra p ab d", "compra t aa e", "compra r ad c",
            "compra o ae b", "compra s ba d", "compra l bb d", "estado",
            "compra s ab d", "compra s ac d", "compra s zz d", "compra o bc c", "compra o bd d",
            "roda aa b", "roda be d", "vende az", "vende be", "vende aa", "vende ba", "estado", "fim",
        ])
        self.verifica("seis tipos, debitos, devolucoes, rotacao e recusas", compras, [
            "Creditos: 345", "Creditos: 207", "Maquinas: 6", "Maquinas: 5",
            "Perfuradora", "Transportador", "Refinaria", "Oficina", "Silo", "Lancador",
            "ja tem uma maquina", "numa cratera", "fora da fabrica", "Creditos insuficientes",
            "Direcao atualizada", "A posicao nao tem maquina", "Maquina vendida", "AvP#RO",
        ])
        assert compras.count("Maquina comprada.") == 7 and compras.count("Maquina vendida.") == 2

        raio = self.corre(["carrega mapa.txt", "compra s ab d", "lposicao aa 1", "lposicao zz", "fim"])
        self.verifica("lposicao recortada e detalhes de terreno e Silo", raio,
                      ["aa: Solo", "ab: Deposito", "ba: Deposito", "bb: Solo", "perda por instante: 20%", "Posicao ou raio invalido"],
                      ["ac: Cratera", "bc: Solo"])
        self.verifica("painel na fabrica de 26 colunas",
                      self.corre(["carrega mapa-maximo.txt", "compra s zz d", "lposicao zz", "fim"]),
                      ["ABCDEFGHIJKLMNOPQRSTUVWXYZ", "zz: Solo | Silo"])
        avanco = self.corre(["carrega mapa.txt", "avanca", "avanca 3", "fim"])
        self.verifica("avanca por omissao e desenha cada passo", avanco, ["Instante: 4"])
        assert re.findall(r"Instante: (\d+) ", avanco) == ["0", "1", "2", "3", "4"]

        self.verifica("sequencia aninhada, acoes e fim global",
                      self.corre(["executa exterior.txt", "ignorar_teclado"]),
                      ["Maquina comprada.", "Direcao atualizada.", "Instante: 2", "ad: Silo", "A terminar"],
                      ["ignorar_interior", "ignorar_exterior", "ignorar_teclado"])
        self.ficheiro("retoma.txt", "ajuda fim\n")
        self.ficheiro("sequencia.txt", "executa retoma.txt\ncomando_inexistente\n")
        self.verifica("retoma do ficheiro anterior e do teclado",
                      self.corre(["executa sequencia.txt", "executa falta.txt", "fim"]),
                      ["Exemplo: fim", "Comando desconhecido", "nao foi possivel abrir", "A terminar"])

        invalidos = [
            ("fabrica 3x 9", "dimensoes invalidas"), ("carrega a b", "numero incorreto"),
            ("avanca 0", "inteiro positivo"), ("compra p aa x", "tipo, posicao ou direcao invalido"),
            ("vende a", "posicao invalida"), ("roda aa x", "posicao ou direcao invalida"),
            ("melhora 11", "posicao invalida"), ("contrata", "numero incorreto"),
            ("atribui 0 aa", "ID ou posicao invalido"), ("despede -1", "ID deve ser inteiro positivo"),
            ("estado a", "numero incorreto"), ("lposicao aa -1", "posicao ou raio invalido"),
            ("grava a b", "numero incorreto"), ("recupera", "numero incorreto"),
            ("executa", "numero incorreto"), ("ajuda x y", "numero incorreto"),
            ("fim x", "numero incorreto"),
        ]
        for comando, erro in invalidos:
            self.ficheiro("sintaxe.txt", comando + "\n")
            for origem, entrada in [("teclado", comando), ("ficheiro", "executa sintaxe.txt")]:
                self.verifica(f"sintaxe por {origem}: {comando}", self.corre([entrada, "fim"]),
                              [erro, "A terminar"])
        finais = ["melhora aa", "contrata ana", "atribui 1 aa", "despede 1", "grava antes", "recupera antes"]
        saida = self.corre(["carrega mapa.txt", *finais, "fim"])
        self.verifica("seis acoes adiadas aceitam argumentos validos",
                      saida, ["Operacao reservada"], ["Erro:"])
        assert saida.count("Operacao reservada") == len(finais)

    def sem_perdas(self):
        for inversa in (False, True):
            self.ficheiro("linha.txt", "1 4\n" + ("...%" if inversa else "%...") + "\n")
            origem, primeiro, segundo, ultimo = ("ad", "ac", "ab", "aa") if inversa else ("aa", "ab", "ac", "ad")
            direcao = "e" if inversa else "d"
            saida = self.corre([
                "carrega linha.txt", f"compra p {origem} {direcao}",
                f"compra s {primeiro} {direcao}", f"compra s {segundo} {direcao}",
                f"compra s {ultimo} {direcao}", "avanca 3", f"lposicao {origem}", f"lposicao {primeiro}",
                "avanca", f"lposicao {primeiro}", f"lposicao {segundo}",
                "avanca", f"lposicao {segundo}", f"lposicao {ultimo}",
                "avanca", f"lposicao {ultimo}", "fim",
            ])
            consultas = re.findall(r"^[a-z]{2}: .* \| (?:Perfuradora|Silo) \|.*$", saida, re.M)
            assert len(consultas) == 7, consultas
            assert "Minerio de ferro" in consultas[0]
            assert ["1 item(ns)" in linha for linha in consultas[1:]] == [False, True, False, True, False, True]
            self.verifica(f"um movimento por instante ({direcao})", saida,
                          ["Instante: 6", "perda por instante: 0%"], ["Perdeu"])

        self.ficheiro("fila.txt", "3 3\n.%.\n~..\n...\n")
        fila = ["carrega fila.txt", "compra p ab b", "compra p ba d", "compra s bb c", "avanca 4"]
        saida = self.corre([
            *fila, "lposicao bb", "avanca", "lposicao bb", "compra s bc d", "roda bb d",
            "avanca", "lposicao bb", "lposicao bc", "avanca", "lposicao bc", "vende bc", "fim",
        ])
        self.verifica("FIFO de dois tipos, recusa conserva item, venda de Silo cheio", saida,
                      ["2 item(ns) | perda por instante: 0% | Minerio de ferro (f, 2 creditos) | Gelo (g, 1 creditos)",
                       "bb: Bloqueada com Minerio de ferro",
                       "1 item(ns) | perda por instante: 0% | Gelo", "Maquina vendida."], ["Perdeu"])
        assert saida.count("2 item(ns) | perda por instante: 0% | Minerio de ferro (f, 2 creditos) | Gelo") == 3
        self.ficheiro("muitos.txt", "1 2\n%.\n")
        quantidade = self.corre([
            "carrega muitos.txt", "lposicao aa", "compra p aa d", "compra s ab d",
            "avanca 182", "lposicao aa", "lposicao ab", "vende ab", "fim",
        ])
        inicial = int(re.search(r"aa: Deposito de minerio de ferro \((\d+) unidades\)", quantidade).group(1))
        self.verifica("Silo acumula todo o deposito e este passa a solo", quantidade,
                      ["Deposito esgotado", "aa: Solo | Perfuradora", f"{inicial} item(ns) | perda por instante: 0%"])

    def perdas_totais(self):
        self.ficheiro("fila.txt", "3 3\n.%.\n~..\n...\n")
        saida = self.corre([
            "carrega fila.txt", "compra p ab b", "compra p ba d", "compra s bb c",
            "avanca 4", "lposicao bb", "avanca", "lposicao bb", "avanca", "lposicao bb", "fim",
        ])
        self.verifica("perde apenas o mais antigo por instante", saida,
                      ["2 item(ns) | perda por instante: 100% | Minerio de ferro",
                       "bb: Perdeu Minerio de ferro", "1 item(ns) | perda por instante: 100% | Gelo",
                       "bb: Perdeu Gelo", "0 item(ns) | perda por instante: 100%"])
        assert saida.index("bb: Perdeu Minerio de ferro") < saida.index("bb: Perdeu Gelo")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("executavel", type=Path)
    parser.add_argument("modo", nargs="?", default="normal",
                        choices=["normal", "sem_perdas", "perdas_totais"])
    opcoes = parser.parse_args()
    with TemporaryDirectory() as pasta:
        testes = Testes(opcoes.executavel, Path(pasta))
        getattr(testes, opcoes.modo)()
        print(f"{testes.cenarios} cenarios OK ({opcoes.modo}).")
