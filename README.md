# Compilação
Pode-se usar o comando `make` ou executar o script `./compile.sh`, ambos possuem a mesma funcionalidade.

# Uso
Para resolver o problema do caixeiro viajante:
```sh
make
./bin/solve
> Uso: ./bin/solve <arquivo> <algoritmo>
./bin/solve random/10.tsp cf
> name is rand10
> comment is 10 random cities TSP
> Total cost was 759310.393790
./bin/solve random/10.tsp tatt
> name is rand10
> comment is 10 random cities TSP
> Total cost was 677562.403818
./bin/solve random/10.tsp bab
> name is rand10
> comment is 10 random cities TSP
> Total cost was 563389.592504
```

# Dados
Os dados de execução estão na pasta `results`. Os arquivos `.log` são dados de execução, enquanto os `.csv` foram formatados. O arquivo `with_optima.csv` foi gerado á mão, e contém os dados correlacionados com as soluções ótimas fornecidas pela TSPLIB [neste link](http://comopt.ifi.uni-heidelberg.de/software/TSPLIB95/STSP.html). Os gráficos gerados a partir dos dados estão no diretório da documentação, e são utilizados no pdf da submissão.

# Descrição de Arquivos
- `main.cpp`, que resolve o problema do Caixeiro Viajante no arquivo fornecido usando o algorítmo requisitado.
- `make_random.c`, que cria casos teste aleatórios no formato TSPLIB.
- `make_csvs.cpp`, que transforma os logs de execução gerados pelos experimentos em arquivos csvs para serem consumidos pelo gnuplot.
- `experiment.sh`, que experimenta com o branch and bound até que ele supere 30m em algum caso e gera logs de execução.
- `experiment_poly.sh`, que experimenta com os algorítmos polinomiais até um limite de 30 cidades e gera logs de execução.
- `experiment_tsplib.sh`, que experimenta com os algorítmos polinomiais nos casos de teste da TSPLIB e gera logs de execução.
- `plots.gnuplot`, um script do GNUPlot que gera visualizações.

# Requerimentos
O programa usa da biblioteca de C++ [Lemon](https://lemon.cs.elte.hu/trac/lemon), e foi desenvolvido com a versão disponível pelo gerenciador de pacotes [Nix](https://nixos.org/). É possível, mas não foi testado, obter a biblioteca
separadamente, ou por outros gerenciadores de pacotes, e então compilar. Para
o desenvolvimento desse trabalho, os comandos foram todos executados dentro
da `nix-shell` presente na raiz do repositório.
```sh
nix-shell ./shell.nix
```