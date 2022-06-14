/*
	c++ -o main calcola_dati.cpp `root-config --glibs --cflags`
*/

#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include "TGraph.h"
#include "TApplication.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TFitResultPtr.h"
#include "TFitResult.h"
#include "TGraphErrors.h"
#include "TStyle.h"
#include "TH1F.h"
#include "TMath.h"
#include "TLatex.h"
#include "TMultiGraph.h"
#include "TAttFill.h"
#include "TVirtualPad.h"

double massimo(std::vector<double> v_campione){
  double max = v_campione.at(0);
  for (int i = 0; i < v_campione.size(); i++) {
    if (v_campione.at(i) > max) {
      max = v_campione.at(i);
    }
  }
  return ceil(max);
}

int posizione_max(double massimo, std::vector<double> v_dati){
  for (int i = 0; i < v_dati.size(); i++) {
      if (v_dati.at(i)==massimo) {
        return i;
      }
  }
}


int main(int argc, char **argv) {

  TApplication theApp("theApp", &argc, argv);
  gStyle->SetOptFit(1112);

// Apertura CH1
std::ifstream dati1;
dati1.open("RC100_ch1.txt", std::ios::in);
std::vector<double> v_secondi1, v_corrente1;
while (true) {
    double secondi1, corrente1;
    dati1 >> secondi1 >> corrente1;
    if(dati1.eof()==true) break;
    v_secondi1.push_back(secondi1);
    v_corrente1.push_back(corrente1);
}
dati1.close();

// Apertura CH2
std::ifstream dati2;
dati2.open("RC100_ch2.txt", std::ios::in);
std::vector<double> v_secondi2, v_corrente2;
while (true) {
    double secondi2, corrente2;
    dati2 >> secondi2 >> corrente2;
    if(dati2.eof()==true) break;
    v_secondi2.push_back(secondi2);
    v_corrente2.push_back(corrente2);
}
dati2.close();

// Apertura MATH
std::ifstream dati_math;
dati_math.open("RC100_math.txt", std::ios::in);
std::vector<double> v_secondi_math, v_corrente_math;
while (true) {
    double secondi_math, corrente_math;
    dati_math >> secondi_math >> corrente_math;
    if(dati_math.eof()==true) break;
    v_secondi_math.push_back(secondi_math);
    v_corrente_math.push_back(corrente_math);
}
dati_math.close();

  double ampiezza_massima_corrente_math = massimo(v_corrente_math);
  int posizione_massimo_corrente_math = posizione_max(ampiezza_massima_corrente_math, v_corrente_math);

  double ampiezza_massima_corrente1 = massimo(v_corrente1);
  int posizione_massimo_corrente1 = posizione_max(ampiezza_massima_corrente1, v_corrente1);

  double ampiezza_massima_corrente2 = massimo(v_corrente2);
  int posizione_massimo_corrente2 = posizione_max(ampiezza_massima_corrente2, v_corrente2);

  double differenza_fase_primata = v_secondi1.at(posizione_massimo_corrente_math)-v_secondi2.at(posizione_massimo_corrente1);

  double differenza_fase_doppioprimata = v_secondi1.at(posizione_massimo_corrente1)-v_secondi2.at(posizione_massimo_corrente2);





  return 0;
}
