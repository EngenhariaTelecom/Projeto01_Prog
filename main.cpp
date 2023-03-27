#include <iostream>
#include <fstream>
#include <error.h>
#include <sstream>
#include <list>
#include <stack>

using namespace std;

//função para obter a ultima string da frase do arquivo, no qual é o preco
string copia_ultima(const string & linha) {
    // encontra a posição do primeiro espaco

    int pos = linha.find(' ');
    //caso encontrar o espaço na frase:
    if (pos != string::npos) {
        // retorna a string contendo o trecho da linha posterior ao espaço
        return linha.substr(pos);
    }
    // caso não encontre o espaço, retorna uma string vazia

    return "";
}

int main(int argc, char * argv[]) {

    //criando um arquivo para leitura
    ifstream arq(argv[1]);

    //verifica se conseguiu abrir o arquivo
    if (! arq.is_open()){
        perror("Ao abrir ");
        return errno;
    }

    string linha, palavra, preco_dia;
    list<string>precos;

    //ler as linhas do arquivo, armazenando em uma lista os precos
    while (getline(arq, linha)){
            preco_dia = copia_ultima(linha);
            precos.push_back(preco_dia);
        }

    return 0;
}
