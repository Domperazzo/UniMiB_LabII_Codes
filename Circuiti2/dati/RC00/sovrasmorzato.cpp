/*
  c++ -o main3 sovrasmorzato.cpp `root-config --glibs --cflags`
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


double fgaus (double x, double mean, double sigma)
  {
    double esponente = (-0.5 * (x - mean) * (x - mean) / (sigma * sigma)) ;
    double termine_proporzionalita = 1 / (sqrt(2 * M_PI) * sigma);
    return termine_proporzionalita * exp(esponente);
  }

double rand_range (double min, double max)
  {
    return min + (max - min) * rand () / static_cast<double> (RAND_MAX) ;
  }

double rand_TAC_gaus (double mean, double sigma)
  {
    double x = 0. ;
    double y = 0. ;
    do {
      x = rand_range (mean - 4 * sigma, mean + 4 * sigma) ;
      y = rand_range (0, fgaus (mean, mean, sigma)) ;
    } while (y > fgaus (x, mean, sigma)) ;
    return x ;
  }

int main(int argc, char **argv) {
  TApplication theApp("theApp", &argc, argv);
  //gStyle->SetOptFit(1112);

  std::ifstream dati;
  dati.open("sovrasmorzamento.txt", std::ios::in);
  std::vector<double> v_secondi, v_Corrente;
  while (true) {
      double secondi, Corrente;
      dati >> secondi >> Corrente;
      if(dati.eof()==true) break;
      v_secondi.push_back(secondi);
      v_Corrente.push_back(Corrente);
  }
  dati.close();
  std::vector<double> v_err;
  for (int i = 0; i < v_secondi.size(); i++) {
    v_err.push_back(rand_TAC_gaus(1, 0.004));

  }


  TGraphErrors grafico;
  for (int i = 0; i < v_secondi.size(); i++) {
    grafico.SetPoint(i, v_secondi.at(i), v_Corrente.at(i));
    grafico.SetPointError(i, 0., 0.08/sqrt(12));
  }

  //grafico.SetFillStyle(3005);
  //grafico.SetFillColor(kCyan+3);
  //grafico.SetMarkerStyle(5);

  TF1 modello ("modello", "[0]*20000*([1]/(2*[2]))*(exp(-([3]-[2])*x)-exp(-([3]+[2])*x))");
  modello.SetParameter(0, 4.31);
  modello.SetParameter(1, 7.03);
  modello.SetParameter(2, 153000);
  modello.SetParameter(3, 157000);
  TFitResultPtr fit_result = grafico.Fit(&modello, "SQ+");
  TCanvas c1;
  grafico.SetTitle("Cicuito RLC sovrasmorzato ; Tempo [s] ; Tensione [V]");
  grafico.Draw("AP");
  theApp.Run();






  return 0;
}
