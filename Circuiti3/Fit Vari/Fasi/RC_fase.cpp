/*
	c++ -o RC_fase RC_fase.cpp `root-config --glibs --cflags`
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
  dati.open("RCdelta_phi2.txt", std::ios::in);
  std::vector<double> v_phi1, v_phi2, v_x;
  while (true) {
      double phi1, phi2, x ;
      dati >> phi1 >> phi2 >> x ;
      if(dati.eof()==true) break;
      v_phi1.push_back(phi1);
      v_phi2.push_back(phi2);
      v_x.push_back(x);
  }
	dati.close();



//Grafico e fit con y1

TGraphErrors grafico;

for (int i = 0; i < v_phi1.size(); i++) {
  grafico.SetPoint(i, v_x.at(i), v_phi1.at(i));
  grafico.SetPointError(i, 0., 0. );//0.03064);
}

TF1 modello("modello", "-atan(x*[0]*10.1)");
//modello.SetParName(0, "C");

TFitResultPtr fit_result = grafico.Fit(&modello, "S");


grafico.SetMarkerStyle(20);
grafico.SetMarkerSize(0.5);

grafico.SetTitle("Funzione di trasferimento circuito RC fase");
grafico.GetHistogram()-> GetXaxis()->SetTitle("Pulsazione #omega");
grafico.GetHistogram()-> GetYaxis()->SetTitle("|H(#omega)|");

/*
//Grafico e fit con y2

TGraphErrors grafico2;

for (int i = 0; i < v_x.size(); i++) {
  grafico2.SetPoint(i, v_x.at(i), v_y2.at(i));
  grafico2.SetPointError(i, 0., 0.03064);
}

TF1 modello2("modello2", "1/(sqrt(1+x^2*[0]^2*102.01))");
modello2.SetParName(0, "C");


TFitResultPtr fit_result2 = grafico2.Fit(&modello2, "S");


grafico2.SetMarkerStyle(20);
grafico2.SetMarkerSize(0.5);

grafico2.SetTitle("Funzione di trasferimento circuito RC ");
grafico2.GetHistogram()-> GetXaxis()->SetTitle("Pulsazione #omega");
grafico2.GetHistogram()-> GetYaxis()->SetTitle("|H(#omega)|");
*/




TCanvas c1;
grafico.Draw("AP");
theApp.Run();




  return 0;
}
