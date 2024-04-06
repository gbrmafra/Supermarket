#include <iostream>
#include <iomanip> 

using namespace std;


struct No
{
    string nome;
    double valor;
    No *elo=nullptr;
};

struct Lista
{
    No *start=nullptr;
    No *end=nullptr;
};

void adicionar(Lista &lista, string nome, double valor)
{
    No *newNo= new No;
    newNo->nome=nome;
    newNo->valor=valor;
    newNo->elo=nullptr;

    if(lista.start==nullptr){
        lista.start=newNo;
        lista.end=newNo;
    }else{
        lista.end->elo=newNo;
        lista.end=newNo;
    }
}

  void exibir(Lista lista) {
    No *temp = lista.start;

    // Exibindo cabeçalho da tabela
    system("cls");
    cout << setw(15) << left << "Nome:";
    cout << setw(10) << left << "Preco:" << endl;

    // Exibindo elementos da lista
    while (temp != nullptr) {
        cout << setw(15) << left << temp->nome;
        cout << setw(10) << left << temp->valor << endl;
        temp = temp->elo;
    }
    cout << endl;
}


void novoProduto(Lista &lista){
    string nome;
    double valor;
    cout<<"\n digite o nome: "<<endl;
    cin>>nome;
    cout<<"\n digite o valor: "<<endl;
    cin>>valor;
    adicionar(lista,nome,valor);
}

void somaCarrinho(Lista lista){
    No *newLista =lista.start;
    double soma=0;
    while (newLista!=nullptr){
        soma += newLista->valor;
        newLista= newLista->elo;
    }

    cout<<setw(15)<<left<<"Total:";
    cout<<setw(20)<<left<<soma<<endl;
}




