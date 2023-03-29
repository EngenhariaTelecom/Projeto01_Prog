#include <iostream>
#include <fstream>
#include <error.h>
#include <sstream>
#include <stack>
#include <vector>

using namespace std;

struct Acoes{
    string data;
    double preco;
};


struct ResultadoUm{
    string data;
    int dia_com_aumento_preco;
};


struct ResultadoDois{
    string data;
    double probabilidade_dias = 0;
};



//função para armazenar os dados de entrada em uma vetor
vector<Acoes>dados_entrada(string * arq){
    string linha;
    vector<Acoes>info_acoes;

    while (getline(arq, linha)) {

        istringstream frase(linha);
        Acoes dados_acoes;

        frase >> dados_acoes.data >> dados_acoes.preco; // ler a linha do arquivo e armazena a data e o preço na struct criada através do operador de extração ">>"
        info_acoes.push_back(dados_acoes); //adiciona ao vetor a struct
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

    //criando um vetor do tipo Acoes que foi criado
    vector<Acoes>info_acoes;

    //cria uma variavel linha para conseguir armazenar a linha lida do arquivo no loop
    string linha;

    //ler as linhas do arquivo, armazenando em um vetor os precos
    while (getline(arq, linha)) {

        istringstream frase(linha);
        Acoes dados_acoes;

        frase >> dados_acoes.data >> dados_acoes.preco; // ler a linha do arquivo e armazena a data e o preço na struct criada através do operador de extração ">>"
        info_acoes.push_back(dados_acoes); //adiciona ao vetor a struct
    }

    //pilha para armazenar "o dia anterior mais próximo cujo preço de ação seja maior que o dia presente."
     stack<int>dias_preco_cresce;
     dias_preco_cresce.push(0);


     //CORRIGIR O LOOP
     while (!dias_preco_cresce.empty() && info_acoes[dias_preco_cresce.top()].data) {
         dias_preco_crescente.pop();

         if (dias_preco_crescente.empty()) {
             dias_preco_crescente.push(dia_com_aumento_preco = i + 1);
         } else {
             dias_preco_crescente.push(1 - dia_com_aumento_preco);
         }

         dias_preco_crescente.push(dia_com_aumento_preco == i);
     }

    return 0;

}