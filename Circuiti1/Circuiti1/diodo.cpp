/*
	c++ -o diodo diodo.cpp `root-config --glibs --cflags`
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


int main(int argc, char **argv) {

  TApplication theApp("theApp", &argc, argv);
  gStyle->SetOptFit(1112);

  std::ifstream dati;
  dati.open("diodo.txt", std::ios::in);
  std::vector<double> v_volt, v_corrente;
  while (true) {
      double volt, corrente;
	    dati >> volt >> corrente;
      if(dati.eof()==true) break;
      v_volt.push_back(volt);
      v_corrente.push_back(0.001*corrente);
  }
	dati.close();
  std::ifstream err_dati;
  err_dati.open("err_diodo.txt", std::ios::in);
  std::vector<double> v_err;
  while (true) {
      double err;
      err_dati >> err;
      if(err_dati.eof()==true) break;
      v_err.push_back(err);
  }
  err_dati.close();

  TGraphErrors grafico, grafico_soglia;
    for (int i = 0; i < v_corrente.size(); i++) {
      grafico.SetPoint(i, v_volt.at(i),v_corrente.at(i));
      grafico.SetPointError(i, 0, v_err.at(i));
        }
    grafico.SetTitle("Legge di Shockley");
    grafico.GetHistogram()-> GetXaxis()->SetTitle("Tensione (V)");
    grafico.GetHistogram()-> GetYaxis()->SetTitle("Corrente (A)");


  //il diodo è al silicio quindi g=2

  TF1 modello ("modello", "[0]*(exp([1]*(38.6*x))-1)");
  modello.SetParameter(1, 0.5);
  TFitResultPtr fit_result = grafico.Fit(&modello, "S");
/*
  std::vector<double> v_corrente_reverse, v_volt_reverse;
  for (int i = v_corrente.size(); i == 0; i--) {
    v_corrente_reverse.push_back(v_corrente.at(i));
    v_volt_reverse.push_back(v_volt.at(i));
  }

  for (int i = 0; i < v_corrente_reverse.size(); i++) {
    grafico_soglia.SetPoint(i, v_volt_reverse.at(i),v_corrente_reverse.at(i));
    grafico_soglia.SetPointError(i, 0, 0.015);
  }
  grafico_soglia.SetTitle("Corrente di soglia");

  TF1 modello_retta("retta", "[0]*x+[1]");
  TFitResultPtr fit_result2 = grafico_soglia.Fit(&modello_retta, "S");

*/

	grafico.SetMarkerStyle(20);
	grafico.SetMarkerSize(0.5);
	//grafico.SetMarkerColor(kBlue);
/*
	grafico_soglia.SetMarkerStyle(20) ;
	grafico_soglia.SetMarkerSize(0.5);
	grafico_soglia.SetMarkerColor(kRed);
	grafico_soglia.SetTitle("Spettro atteso");

  TMultiGraph confronto;
  confronto.Add(&grafico);
  confronto.Add(&grafico_soglia);
*/
  TCanvas c2;
  c2.SetGrid(1,1);
  grafico.Draw("AP");
  //confronto.SetTitle("Tensione di soglia ; Tensione (V) ; Corrente (A)");
  //c2.BuildLegend();
  theApp.Run();
  c2.Print("confronto tra lambda 4.pdf", "pdf");




  return 0;
}
