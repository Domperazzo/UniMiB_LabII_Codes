/*
	c++ -o main primo_punto.cpp `root-config --glibs --cflags`
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


int main(int argc, char **argv) {

  TApplication theApp("theApp", &argc, argv);
  gStyle->SetOptFit(1112);

  std::ifstream dati;
	dati.open("resistenza2.txt", std::ios::in);
  //dati.open("resistenza1.txt", std::ios::in);
  std::vector<double> v_volt, v_corrente;
  while (true) {
      double volt, corrente;
	    dati >> volt >> corrente;
      if(dati.eof()==true) break;
      v_volt.push_back(volt);
      v_corrente.push_back(corrente);
  }
	dati.close();

  TGraphErrors grafico;
    for (int i = 0; i < v_corrente.size(); i++) {
      grafico.SetPoint(i, 0.000001*v_corrente.at(i), v_volt.at(i));
      grafico.SetPointError(i, 0, 0.00001);
        }
    grafico.SetTitle("Legge di Ohm");
    grafico.GetHistogram()-> GetXaxis()->SetTitle("corrente (A)");
    grafico.GetHistogram()-> GetYaxis()->SetTitle("d.d.p. (V)");


  TF1 modello ("modello", "[0]*x");
  TFitResultPtr fit_result = grafico.Fit(&modello, "S");
  modello.SetParName(0, "A");

  TCanvas c1;
  grafico.SetMarkerStyle(20);
  grafico.SetMarkerSize(0.5);
  grafico.Draw("AP");
  theApp.Run();
  c1.Print("legge di ohm.png","png");


  return 0;
}
