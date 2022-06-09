/*
	c++ -o main rc_circuito.cpp `root-config --glibs --cflags`
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


int main(int argc, char **argv) {

  TApplication theApp("theApp", &argc, argv);
  gStyle->SetOptFit(1112);

  std::ifstream dati;
  dati.open("RC_Canale2_copia.txt", std::ios::in);
  std::vector<double> v_secondi, v_Corrente;
  while (true) {
      double secondi, Corrente;
	    dati >> secondi >> Corrente;
      if(dati.eof()==true) break;
      v_secondi.push_back(secondi);
      v_Corrente.push_back(Corrente);
  }
	dati.close();

TGraphErrors grafico;
for (int i = 0; i < v_secondi.size(); i++) {
  grafico.SetPoint(i, v_secondi.at(i), v_Corrente.at(i));
  grafico.SetPointError(i, 0., 0.0088);
}

TF1 modello("modello", "[0]*exp(-x/(10.01*[1]))");
TFitResultPtr fit_result = grafico.Fit(&modello, "S");


TCanvas c1;
grafico.Draw("AP");
grafico.SetMarkerStyle(7);
grafico.SetTitle("Cicuito RC ; Tempo [s] ; Tensione [V]");
theApp.Run();
c1.Print("diodo.pdf", "pdf");





  return 0;
}
