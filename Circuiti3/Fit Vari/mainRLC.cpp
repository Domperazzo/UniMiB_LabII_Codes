/*
	c++ -o mainRLC mainRLC.cpp `root-config --glibs --cflags`
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
  dati.open("RLC_h.txt", std::ios::in);
  std::vector<double> v_y1, v_y2, v_x;
  while (true) {
      double y1, y2, x ;
      dati >> y1 >> y2 >> x;
      if(dati.eof()==true) break;
      v_y1.push_back(y1);
      v_y2.push_back(y2);
      v_x.push_back(x);
  }
	dati.close();
/*
//Grafico e fit con y1

TGraphErrors grafico;

for (int i = 0; i < v_x.size(); i++) {
  grafico.SetPoint(i, v_x.at(i), v_y1.at(i));
  grafico.SetPointError(i, 0., 0.040);
}

TF1 modello("modello", "(10.1)/(sqrt(102.01+(x*[0]-(1/(x*[1])))^2))");
modello.SetParName(0, "L");
modello.SetParName(1, "C");
modello.SetParameter(0, 0.0395);
modello.SetParameter(1, 3610000);

TFitResultPtr fit_result = grafico.Fit(&modello, "S");



grafico.SetMarkerStyle(20);
grafico.SetMarkerSize(0.5);

grafico.SetTitle("Funzione di trasferimento circuito RLC");
grafico.GetHistogram()-> GetXaxis()->SetTitle("Pulsazione #omega");
grafico.GetHistogram()-> GetYaxis()->SetTitle("|H(#omega)|");
*/

//Grafico e fit con y2

TGraphErrors grafico2;

for (int i = 0; i < v_x.size(); i++) {
  grafico2.SetPoint(i, v_x.at(i), v_y2.at(i));
  grafico2.SetPointError(i, 0., 0.06);
}

TF1 modello2("modello2", "(x*[0]-1/(x*[1]))/(sqrt(102.01+(x*[0]-(1/(x*[1])))^2))");
modello2.SetParName(0, "L");
modello2.SetParName(1, "C");
modello2.SetParameter(0, 0.0395);
modello2.SetParameter(1, 2030000000);



TFitResultPtr fit_result2 = grafico2.Fit(&modello2, "S");


grafico2.SetMarkerStyle(20);
grafico2.SetMarkerSize(0.5);

grafico2.SetTitle("Funzione di trasferimento circuito RLC ");
grafico2.GetHistogram()-> GetXaxis()->SetTitle("Pulsazione #omega");
grafico2.GetHistogram()-> GetYaxis()->SetTitle("|H(#omega)|");





TCanvas c1;
grafico2.Draw("AP");
theApp.Run();




  return 0;
}
