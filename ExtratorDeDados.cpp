#include <iostream>
#include <cmath>
#include <sstream>
#include <vector>

#include "ExtratorDeDados.hpp"

/**
 * \class Num2Sqrt
 *
 * \brief Esta class mapeia numeros em suas raizes quadradas.
 *
 * \tparam NUM_TYPE o tipo do numero. Note que qualquer tipo que aceite a
 * operacao sqrt eh valido: char, int, float, double, etc.
 */
template <class NUM_TYPE>
class Num2Sqrt : public Mapeador<NUM_TYPE, NUM_TYPE>
{
  NUM_TYPE transformaDado(NUM_TYPE &d) const override
  {
    return sqrt(d);
  }
};

/**
 * \class String2Int
 *
 * \brief Esta classe implementa um Mapeador que transforma uma string no
 * numero que aquela string representa. Eh um erro invocar o metodo
 * "transformaDado" em uma string que nao represente um numero.
 */
class String2Int : public Mapeador<std::string, int>
{
  int transformaDado(std::string &d) const override
  {
    std::stringstream intValue(d);
    int number = 0;
    intValue >> number;
    return number;
  }
};

/**
 * \class Num2MaiorFator
 *
 * \brief Esta classe implementa um Mapeador que mapeia um numero inteiro em
 * seu maior fator. Exemplos:
 *
 * 10 => 5
 * 12 => 6
 * 9 => 3
 * 3 => 1
 */
class Num2MaiorFator : public Mapeador<int, int>
{
  int transformaDado(int &d) const override
  {
    int retorno;
    for (int i = 1; i < d; i++)
    {
      if (d % i == 0)
        retorno = i;
    }
    return retorno;
  }
};

/**
 * \class FiltroNumPositivo.
 *
 * \brief Esta class filtra numeros positivos.
 *
 * \tparam NUM_TYPE o tipo do numero. Note que qualquer tipo que aceite a
 * operacao de comparacao contra zero eh aceitavel: double, int, short, etc.
 */
template <class NUM_TYPE>
class FiltroNumPositivo : public Filtro<NUM_TYPE>
{
  bool dadoValido(NUM_TYPE &d) const override
  {
    return (d > 0);
  }
};

/**
 * \class FiltroNumAbundante.
 *
 * \brief Esta class filtra numeros "abundantes", isto eh, cuja soma dos
 * divisores seja maior que o proprio numero.
 *
 * \details
 * Por exemplo, 12 é um número abundante, pois a soma dos seus divisores é 16
 * (exemplo da wikipedia)
 * 1 + 2 + 3 + 4 + 6 = 16
 *
 * Os primeiros dez numeros abundantes sao:
 * 12, 18, 20, 24, 30, 36, 40, 42, 48, 54
 */
class FiltroNumAbundante : public Filtro<int>
{
  bool dadoValido(int &num) const override
  {
    std::vector<int> divisores;
    for (int i = 1; i < num; i++)
    {
      if (num % i == 0)
      {
        divisores.push_back(i);
      }
    }

    int total = 0;
    for (int i = 1; i < divisores.size(); i++)
    {
      total += divisores[i];
    }
    return total > num;
  }
};

/**
 * \class FiltroNumTriangular.
 *
 * \brief Esta class filtra numeros "triangulares".
 * 
 * \details
 * Numeros triangulares sao numeros inteiros que sao a soma de uma progressao
 * aritmetica de incremento 1:
 * 1: 1
 * 3: 1+2
 * 6: 1+2+3
 * 10: 1+2+3+4
 * etc
 */
class FiltroNumTriangular : public Filtro<int>
{
  bool dadoValido(int &num) const override
  {
    int contador = 1, total = 0;
    while (total < num)
    {
      total += contador;

      if (total == num)
      {
        return true;
      }

      contador++;
    }
    return false;
  }
};

/**
 * \class FiltroStringNatural
 *
 * \brief Filtra strings que representam numeros naturais.
 *
 * \details
 * As strings validas devem conter somente digitos. Nao consideramos sinais
 * de mais ou menos, nem ponto ou virgula decimal. Somente numeros.
 */
class FiltroStringNatural : public Filtro<std::string>
{
  bool dadoValido(std::string &d) const override
  {
    for (int i = 0; i < d.size(); i++)
    {
      if (d[i] < '0' || d[i] > '9')
        return false;
    }

    return true;
  }
};

/**
 * \brief Preenche um vetor com dados lidos a partir da entrada padrao.
 *
 * \param vec O vetor que serah preenchido
 *
 * \tparam T o tipo dos dados que serao lidos.
 */
template <class T>
void read_input(std::vector<T> &vec)
{
  T val;
  while (std::cin >> val)
  {
    vec.push_back(val);
  }
}

/**
 * \brief Imprime na saida padrao os dados armazenados em um vetor.
 *
 * \param vec O vetor que contem os dados a serem impressos.
 *
 * \tparam T o tipo dos dados que serao impressos.
 */
template <class T>
void print_output(std::vector<T> &vec)
{
  for (T &d : vec)
  {
    std::cout << d << std::endl;
  }
}

/**
 * \brief Converte strings que forem numeros em valores do tipo inteiro.
 *
 * \details
 * Essa funcao atravesa o vetor de entrada "in", e filtra as strings que
 * denotam numeros "naturais". Um numero natural eh um numero inteiro, sem
 * sinal (e por consequinte, necessariamente positivo). Exemplos:
 *
 * "1.0" -> Nao eh dado valido
 * "-1" -> Nao eh dado valido
 * "1" -> Eh dado valido
 * "1.0e2" -> Nao eh dado valido
 * "Abacaxi" -> Nao eh dado valido
 *
 * \param in Vetor contendo strings de entrada.
 * \param out Vetor que irah armazenar os numeros que puderem ser convertidos.
 */
void convert2int(std::vector<std::string> &in, std::vector<int> &out)
{
  Filtro<std::string> *f = new FiltroStringNatural();
  Mapeador<std::string, int> *m = new String2Int();

  for (int i = 0; i < in.size(); i++)
  {
    if (f->dadoValido(in[i]))
    {
      out.push_back(m->transformaDado(in[i]));
    }
  }
}

/**
 * \brief Testa se a funcao que extrai raizes quadradas funciona corretamente.
 *
 * \details
 * Este eh um codigo de teste. Normalmente, o codigo de teste eh pre-construido
 * no VPL. Porem, a fim de explicar ao aluno como instanciar um algoritmo
 * generico, ele/a deverah preencher este teste. Fique a vontade para olhar
 * como os demais testes foram feitos. Esse codigo serah muito similar.
 *
 * Os passos para montar o teste sao dados como comentarios no corpo do
 * metodo test_filter_square_roots, logo abaixo.
 */
template <class NUM_TYPE>
void test_filter_square_roots()
{
  std::vector<NUM_TYPE> ler;
  std::vector<NUM_TYPE> escrever;

  read_input(ler);

  Filtro<NUM_TYPE> *f = new FiltroNumPositivo<NUM_TYPE>();
  Mapeador<NUM_TYPE, NUM_TYPE> *m = new Num2Sqrt<NUM_TYPE>();

  ExtratorDeDados<NUM_TYPE, NUM_TYPE> extractor(ler, *f, *m);
  extractor.getData(escrever);
  print_output(escrever);

  delete f;
  delete m;
}

/*
 * Codigo de teste.
 * ----------------
 * Voce pode estudar esse codigo, se quiser, mas trata-se de codigo gerado
 * mecanicamente, somente para testar as funcoes desenvolvidas nesse exercicio.
 * 
 * Importante: o codigo abaixo nao usa boas normas de programacao: ele eh
 * redundante e desnecessariamente extenso.
 */

void print_only_naturals()
{
  std::vector<std::string> input_data;
  std::vector<int> output_data;
  read_input(input_data);
  convert2int(input_data, output_data);
  print_output(output_data);
}

void print_maior_fator_de_numeros_triangulares()
{
  std::vector<int> input_data;
  std::vector<int> output_data;
  read_input(input_data);
  Filtro<int> *filtro = new FiltroNumTriangular();
  Mapeador<int, int> *mapeador = new Num2MaiorFator();
  ExtratorDeDados<int, int> extractor(input_data, *filtro, *mapeador);
  extractor.getData(output_data);
  print_output(output_data);
  delete filtro;
  delete mapeador;
}
void print_maior_fator_de_numeros_abundantes()
{
  std::vector<int> input_data;
  std::vector<int> output_data;
  read_input(input_data);
  Filtro<int> *filtro = new FiltroNumAbundante();
  Mapeador<int, int> *mapeador = new Num2MaiorFator();
  ExtratorDeDados<int, int> extractor(input_data, *filtro, *mapeador);
  extractor.getData(output_data);
  print_output(output_data);
  delete filtro;
  delete mapeador;
}

void print_maior_fator_de_strings_que_sao_numeros_triangulares()
{
  std::vector<std::string> input_data;
  std::vector<int> in_numbers_only;
  read_input(input_data);
  convert2int(input_data, in_numbers_only);
  Filtro<int> *filtro = new FiltroNumTriangular();
  Mapeador<int, int> *mapeador = new Num2MaiorFator();
  ExtratorDeDados<int, int> extractor(in_numbers_only, *filtro, *mapeador);
  std::vector<int> output_data;
  extractor.getData(output_data);
  print_output(output_data);
  delete filtro;
  delete mapeador;
}

void test_filter_maior_fator_positivo()
{
  std::vector<int> input_data;
  std::vector<int> output_data;
  read_input(input_data);
  Filtro<int> *filtro = new FiltroNumPositivo<int>();
  Mapeador<int, int> *mapeador = new Num2MaiorFator();
  std::cout << "oi";
  ExtratorDeDados<int, int> extractor(input_data, *filtro, *mapeador);
  extractor.getData(output_data);
  print_output(output_data);
  delete filtro;
  delete mapeador;
}

int main()
{
  int option = 0;
  std::cin >> option;
  switch (option)
  {
  case 0:
    test_filter_square_roots<int>();
    break;
  case 1:
    test_filter_square_roots<double>();
    break;
  case 2:
    print_maior_fator_de_numeros_triangulares();
    break;
  case 3:
    print_only_naturals();
    break;
  case 4:
    print_maior_fator_de_strings_que_sao_numeros_triangulares();
    break;
  case 5:
    test_filter_maior_fator_positivo();
    break;
  case 6:
    print_maior_fator_de_numeros_abundantes();
    break;
  default:
    std::cout << "Caso de teste desconhecido " << option << std::endl;
  }
}