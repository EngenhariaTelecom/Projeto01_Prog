#include <iostream>
#include <fstream>
#include <stack>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cmath>

using namespace std;


// tipo de dado criado para armazenar os valores do arquivo de entrada que contém data e preço da ação
struct CotacaoAcoes{
    string data;
    double preco;
};

// tipo de dado criado para armazenar os valores da qtd de dias que o preço subiu
struct Resultado{
    string data;
    int dias_preco_aumentou;
};

// tipo de dado criado para armazenar o calculo feito da probabilidade de o preço crescer em determinado dias
struct Contagem{
    int dia;
    double probabilidade_dias;
};


//função que ler os dados de entrada e armazenar em um vetor do tipo "CotacaoAcoes"
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

// Função para calcular os dias com preço ascendente e armazenar em um vetor
vector<Resultado>dias_preco_ascendente(vector<CotacaoAcoes>&info_acoes){
    vector<Resultado>dia;
    //pilha para armazenar "o dia anterior mais próximo cujo preço de ação seja maior que o dia presente."
    stack<int>dias_preco_crescente;
    dias_preco_crescente.push(0);

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
            dias_preco_crescente.push(1 + i - dias_preco_crescente.top());

        }

        dias_preco_crescente.push(i);
        dia[i].data = info_acoes[i].data;
    }

    return dia;
}


// Função para calcular a probabilidade de repetição de preço ascendente e armazena o resultado em um vetor
vector<Contagem>calculo_probabilidade(vector<Resultado>&dia){
    vector<Contagem>probabilidade_aumento_preco;

    for (int i = 0; i < dia.size(); ++i) {
        double contador = 0;
        int qtd_dia = dia[i].dias_preco_aumentou;
        for (int j = 0; j < dia.size(); ++j) {
            if (dia[j].dias_preco_aumentou == qtd_dia){
                contador++;
            }
        }
        double probabilidade = contador / dia.size();
        probabilidade_aumento_preco.push_back(Contagem());
        probabilidade_aumento_preco[i].dia = qtd_dia;
        probabilidade_aumento_preco[i].probabilidade_dias = probabilidade;
    }
    return probabilidade_aumento_preco;
}

// Função para ordenar o vetor de probabilidade e remover os valores repetidos
vector<Contagem>ordena_e_remove_rep(vector<Contagem>&probabilidade_aumento_preco){
    // usando a função SORT para ordenar o vetor de probabilidade, e passado o terceiro argumento para a função, usando função lambda 
    // a função lambda faz a comparação dos valores a serem ordenados, dessa forma não é obrigatório ter uma função fora para chamá-la, sendo que usamos 
    //apenas nessa parte do código esse tipo de comparação
    sort(probabilidade_aumento_preco.begin(), probabilidade_aumento_preco.end(), [](const Contagem& a, const Contagem& b) {
        return a.dia < b.dia;
    });
    // remover elementos duplicados do vetor usando a função UNIQUE
    auto it = unique(probabilidade_aumento_preco.begin(), probabilidade_aumento_preco.end(), [](const Contagem& a, const Contagem& b) {
        return a.dia == b.dia;
    });
    probabilidade_aumento_preco.erase(it, probabilidade_aumento_preco.end());

    return probabilidade_aumento_preco;
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

    //vetor para armazenar o dia calculado e a data referente ao calculo para que possamos armazenar os dados dessa pilha em um arquivo de saida
    vector<Resultado>dia = dias_preco_ascendente(info_acoes);

    //vetor para armazenar o dia calculado da probabilidade de crescimento do preço em um determinado dia
    vector<Contagem>probabilidade_aumento_preco = calculo_probabilidade(dia);

    //chama a função para ordenar o vetor e remover os valores duplicados
    ordena_e_remove_rep(probabilidade_aumento_preco);

    // ARQUIVOS DE SAÍDA

    //criando o arquivo de saida "resultado" que contém o calculo da quantidade de dias que teve o aumento do preço
    ofstream arq_dias("resultado.txt");
    //valida se deu para abrir o arquivo criado
    if (not arq_dias.is_open()) {
        cerr << "Algum erro ao abrir o arquivo ..." << endl;
        return 0;
    }
    // Percorre o vetor "dia" e escreve cada elemento no arquivo resultado
    for (auto elemento : dia) {
        arq_dias << elemento.data << " "<< elemento.dias_preco_aumentou << endl;
    }


    //criando o arquivo de saida "contagem" que contém o calculo da probabilidade de dias que teve o aumento do preço
    ofstream arq_probabilidade("contagem.txt");
    //valida se deu para abrir o arquivo criado
    if (not arq_probabilidade.is_open()) {
        cerr << "Algum erro ao abrir o arquivo ..." << endl;
        return 0;
    }
    // Percorre o vetor "probabilidade_aumento_preco" e escreve cada elemento no arquivo resultado
    for (auto elemento : probabilidade_aumento_preco) {
        // Arredonda o valor do elemento para 4 casas decimais

        elemento.probabilidade_dias = round(elemento.probabilidade_dias * 10000) / 10000;
        arq_probabilidade << elemento.dia << " " << fixed << setprecision(4) << elemento.probabilidade_dias << endl;
    }

    return 0;

}






