/*
  c++ -o fondo fondo.cpp `root-config --glibs --cflags`
*/
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include "TGraph.h"
#include "TGraphPainter.h"
#include "TApplication.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TFitResultPtr.h"
#include "TFitResult.h"
#include "TGraphErrors.h"
#include "TStyle.h"
#include "TPad.h"
#include "TH1F.h"
#include "TMath.h"
#include "TLatex.h"
#include "TMultiGraph.h"
#include "TAttFill.h"
#include "TAttMarker.h"
#include "TVirtualPad.h"

#define _USE_MATH_DEFINES


int main(int argc, char **argv) {
  TApplication theApp("theApp", &argc, argv);
  gStyle->SetOptFit(1112);

  std::ifstream dati;
  dati.open("fondo.txt", std::ios::in);
  std::vector<double> v_secondi, v_Corrente;
  while (true) {
      double secondi, Corrente;
      dati >> secondi >> Corrente;
      if(dati.eof()==true) break;
      v_secondi.push_back(secondi);
      v_Corrente.push_back(Corrente);
  }



  /*
  TGraphErrors grafico;
  for (int i = 0; i < v_secondi.size(); i++) {
    grafico.SetPoint(i, v_secondi.at(i), v_Corrente.at(i));
  }
  TCanvas c1;
  grafico.SetTitle("Rumure di fondo ; Tempo [s] ; Tensione [V]");
  grafico.Draw("ALP");
  theApp.Run();
*/

  int zero_otto=0;
  int zero=0;
  int altro=0;

for (int i = 0; i < v_Corrente.size(); i++) {
  if (v_Corrente.at(i)==0.08) {
    zero_otto+=1;
  } else if (v_Corrente.at(i)==0.) {
    zero+=1;
  } else {
    altro+=1;
  }
}

std::cout << "0,08:\t" << zero_otto << '\n';
std::cout << "0:\t" << zero << '\n';
std::cout << "Altro \t" << altro << '\n';





  return 0;
}
