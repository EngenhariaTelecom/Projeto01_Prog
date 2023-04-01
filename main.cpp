#include <iostream>
#include <fstream>
#include <error.h>
#include <sstream>
#include <stack>
#include <vector>

using namespace std;

// tipo de dado criado para armazenar os valores do arquivo de entrada
struct CotacaoAcoes{
    string data;
    double preco;
};

struct Resultado{
    string data;
    int dias_preco_aumentou;
};

struct Contagem{
    int dia;
    double probabilidade_dias = 0;
};


//função que ler os dados de entrada e armazena em um vetor do tipo "CotacaoAcoes"
vector<CotacaoAcoes>dados_entrada(ifstream * arq){
    string linha;
    vector<CotacaoAcoes>info_acoes;

    while (getline(*arq, linha)) {

        istringstream frase(linha);
        CotacaoAcoes dados_acoes;

        frase >> dados_acoes.data >> dados_acoes.preco; // ler a linha do arquivo e armazena a data e o preço na struct criada através do operador de extração ">>"
        info_acoes.push_back(dados_acoes); //adiciona ao vetor a struct "dados_acoes"
    }
    return info_acoes;
}


int main(int argc, char * argv[]) {

    //criando um arquivo para leitura
    ifstream arq(argv[1]);

    //verifica se conseguiu abrir o arquivo
    if (! arq.is_open()){
        perror("Ao abrir ");
        return errno;
    }

    // vetor que armazena os dados lidos do arquivo usando a função "dados_entrada" para tal operação
    vector<CotacaoAcoes>info_acoes = dados_entrada(&arq);


    //pilha para armazenar "o dia anterior mais próximo cujo preço de ação seja maior que o dia presente."
    stack<int>dias_preco_crescente;
    dias_preco_crescente.push(0);

    //vetor para armazenar o dia calculado e a data referente ao calculo para que possamos armazenar os dados dessa pilha em um arquivo de saida
    vector<Resultado>dia;

    for (int i = 0; i < info_acoes.size(); ++i) {

        dia.push_back(Resultado());

        while (!dias_preco_crescente.empty() && info_acoes[dias_preco_crescente.top()].preco <= info_acoes[i].preco) {
            dias_preco_crescente.pop();
        }

        if (dias_preco_crescente.empty()) {
            dias_preco_crescente.push(i + 1);
            dia[i].dias_preco_aumentou = i + 1;
        } else {
            dia[i].dias_preco_aumentou = i - dias_preco_crescente.top();
            dias_preco_crescente.push(1 - dias_preco_crescente.top());
        }

        dias_preco_crescente.push(i);
        dia[i].data = info_acoes[i].data;

    }

    //vector<Contagem>probabilidade_aumento_preco;
    vector<Contagem>probabilidade_aumento_preco;

    // loop para calcular as probabilidades de duração de preço ascendente

        int contador = 0;
        for (int i = 0; i < info_acoes.size(); i++) {
            int numero = dia[i].dias_preco_aumentou;
            while (i < dia.size()) {
                numero = dia[i].dias_preco_aumentou;
                int digitoAtual = numero % 10;
                if (digitoAtual == dia[i].dias_preco_aumentou) {
                    contador++;
                }
                int num = numero /= 10;
                i++;
            }
        }

    return 0;

}