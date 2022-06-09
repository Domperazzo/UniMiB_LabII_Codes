/*
	c++ -o main main.cpp `root-config --glibs --cflags`
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
    double esponente = (-0.5 * (x - mean) * (x - mean) / (sigma * sigma));
    double termine_proporzionalita = 1 / (sqrt(2 * M_PI) * sigma);
    return termine_proporzionalita * exp(esponente);
  }

double rand_range (double min, double max)
  {
    return min + (max - min) * rand () / static_cast<double> (RAND_MAX);
  }

double rand_TAC_gaus (double mean, double sigma)
  {
    double x = 0. ;
    double y = 0. ;
    do {
      x = rand_range (mean - 4 * sigma, mean + 4 * sigma);
      y = rand_range (0, fgaus (mean, mean, sigma));
    } while (y > fgaus (x, mean, sigma));
    return x;
  }


int main(int argc, char **argv) {

  TApplication theApp("theApp", &argc, argv);
  gStyle->SetOptFit(1112);

  std::ifstream dati;
  dati.open("ch2.txt", std::ios::in);
  //dati.open("RLch2.txt", std::ios::in);
  //dati.open("RLC_ch2.txt", std::ios::in);
  std::vector<double> v_secondi, v_Corrente;
  while (true) {
      double secondi, Corrente;
	    dati >> secondi >> Corrente;
      if(dati.eof()==true) break;
      v_secondi.push_back(secondi);
      //v_Corrente.push_back(Corrente);
      v_Corrente.push_back(Corrente);
  }
	dati.close();
std::vector<double> v_err;
  for (int i = 0; i < v_secondi.size(); i++) {
    v_err.push_back(rand_TAC_gaus(0.008555555555555, 0.004000000001000020300));
  }


TGraphErrors grafico;

//RC
for (int i = 338; i < 634; i++) {
  grafico.SetPoint(i, v_secondi.at(i), v_Corrente.at(i));
  grafico.SetPointError(i, 0., v_err.at(i));
}
/*

//RL
for (int i = 0; i < v_secondi.size(); i++) {
  grafico.SetPoint(i, v_secondi.at(i), v_Corrente.at(i));
  grafico.SetPointError(i, 0., v_err.at(i));
}*/
/*
//RLC
for (int i = 0; i < v_secondi.size(); i++) {
  grafico.SetPoint(i, v_secondi.at(i), v_Corrente.at(i));
  //grafico.SetPointError(i, 0., 2.1*v_err.at(i));
  grafico.SetPointError(i, 0., 0.08/sqrt(12));
}
*/


//grafico.SetFillStyle(3005);
//grafico.SetFillColor(kCyan+3);
//--------RC log SCALE----------------------
//TF1 modello("modello", "[0]-[1]*x");
//TF1 modello("modello", "[0]*exp(-[1]*x)");
//TF1 modello("modello", "[0]*(1 - exp(-(x*10.1)/[1]))");
//TF1 modello("modello", "[0]*exp(-x/[1])");
//TF1 modello("modello", "([0]*[2]*[1]^2)*x*exp(-[1]*x)");

//TF1 modello ("modello", "10.1*[0]*exp(-[1]*x)*(exp([2]*x)-exp(-[2]*x))");
//RLC
/*
modello.SetParameter(0, 0.468);
modello.SetParameter(1, 55100);
modello.SetParameter(2, 2200);
*/
TF1 modello("modello", "[0]*exp(-x*[1])");
//RC
modello.SetParameter(0, 10.);
modello.SetParameter(1, 0.000000005);
TFitResultPtr fit_result = grafico.Fit(&modello, "SQ+");
std::cout << "fit result is valid?\t"<< fit_result->IsValid() << '\n';




TCanvas c1;
//grafico.Draw("AP");
grafico.SetMarkerStyle(1);

grafico.SetTitle("Cicuito RC ; Tempo [s] ; Tensione [V]");
grafico.Draw("AP");
theApp.Run();
c1.Print("diodo.pdf", "pdf");

//---------------ROBA SOLO RLC SCARTI----------------------
/*
std::vector<double> v_expected, v_scarti;
for (int i = 0; i < v_secondi.size(); i++) {
  v_expected.push_back(modello.Eval(v_secondi.at(i)));
}
for (int i = 0; i < v_expected.size(); i++) {
  v_scarti.push_back(v_Corrente.at(i)-v_expected.at(i));
}

TH1F h_scarti("Istogramma scarti RLC", "Istogramma scarti RLC", v_secondi.size()/7, -0.05, 0.05);
for (int i = 0; i < v_scarti.size(); i++) {
  h_scarti.Fill(v_scarti.at(i));
}
h_scarti.SetFillColor(kCyan-8);

TCanvas c2;
h_scarti.Draw();
h_scarti.Fit("gaus");
h_scarti.SetTitle("Istogramma scarti RLC; Scarto; Numero conteggi");
theApp.Run();
c2.Print("h_scartiRLC.png", "png");
*/
  return 0;
}
